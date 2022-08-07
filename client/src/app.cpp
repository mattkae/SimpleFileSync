#include "app.hpp"
#include "client_message.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <cstddef>
#include <iostream>
#include <filesystem>
#include <iterator>
#include <string>
#include "event.hpp"
#include "event_ledger.hpp"
#include "file_watcher.hpp"
#include "save_area.hpp"
#include "serializer.hpp"
#include "server_message.hpp"
#include "hash_calculator.hpp"
#include <spdlog/spdlog.h>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
typedef boost::asio::ssl::stream<tcp::socket> SslSocket;

namespace client {
	App::App(const AppOptions& opts): 
		mConfig(shared::getSaveAreaPath("client.conf")),
		mAppData(shared::getSaveAreaPath(".client_saved.data"), opts.blankSlate),
		mLedger(shared::getSaveAreaPath(".client_events"), opts.blankSlate)
	{
		mAppData.load();
		mConfig.load();
		auto bso = shared::BinarySerializerOptions();
		mClientSerializer = shared::BinarySerializer(bso);
		mFw = client::FileWatcher([this](std::vector<shared::Event> eventList) {
			try {
				this->onDirectoryChange(eventList);
			}
			catch (const boost::wrapexcept<boost::system::system_error>& e) {
				spdlog::error("Failed to connect to socket: {0}", e.what());
			}
		}, mConfig.getDirectory());
	}

	App::~App() {
	}

	bool verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx)
	{
		// The verify callback can be used to check whether the certificate that is
		// being presented is valid for the peer. For example, RFC 2818 describes
		// the steps involved in doing this for HTTPS. Consult the OpenSSL
		// documentation for more details. Note that the callback is called once
		// for each certificate in the certificate chain, starting from the root
		// certificate authority.

		// In this example we will simply print the certificate's subject name.
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		std::cout << "Verifying " << subject_name << "\n";

		return preverified;
	}

	void App::onDirectoryChange(std::vector<shared::Event> eventList) {
		spdlog::info("Processing next client update...");
		client::Config globalConfig(shared::getSaveAreaPath("client.conf"));
		globalConfig.load();
		boost::asio::io_service ios;
		std::string host = globalConfig.getIp();
		int port = globalConfig.getPort();
		spdlog::info("Making connection to {0}:{1}", host, port);

		// See: https://github.com/miseri/asio-ssl-client/blob/master/main.cpp
		// https://stackoverflow.com/questions/33640084/boost-asio-encryption-in-a-loop-without-closing-connection
		boost::asio::ssl::context sslContext(boost::asio::ssl::context::sslv23);
		sslContext.load_verify_file("/home/matthewk/Projects/SimpleFileSync/server/build/server.crt");
		sslContext.set_verify_mode(boost::asio::ssl::verify_peer);
		SslSocket socket(ios, sslContext);
		tcp::resolver resolver(ios);
		boost::asio::connect(socket.lowest_layer(), resolver.resolve({boost::asio::ip::address::from_string(host), boost::asio::ip::port_type(port)}));
		socket.set_verify_callback(boost::bind(&verify_certificate, _1, _2));
		socket.handshake(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::client);

		// Begin communicaton with the server: Write exactly the amount of data that we need and expect the 
		// start communication in response
		shared::ClientMessage startMessage;
		startMessage.type = shared::ClientMessageType::RequestStartComm;
		startMessage.event.hash = mAppData.getHash();
		mClientSerializer.reset();
		mClientSerializer.writeObject(startMessage);
		boost::system::error_code error;
		socket.write_some(
			boost::asio::buffer(
				mClientSerializer.getData(), 
				mClientSerializer.getSize()
			),  error);

		// Receiving a start communicaton message from the server
		size_t len = socket.read_some(boost::asio::buffer(mResponseBuffer, 1024), error);
		shared::BinaryDeserializer mServerSerializer({ mResponseBuffer, len, 0 });
		shared::ServerMessage response = mServerSerializer.readObject<shared::ServerMessage>();

		switch (response.type) {
			case shared::ServerMessageType::ResponseStartComm:
				if (response.eventsForClient.size()) {
					spdlog::info("Client is behind by {0} events.", response.eventsForClient.size());
					for (auto event : response.eventsForClient) {
						shared::executeEvent(event, mConfig.getDirectory());
						addNewEvent(event);
					}
					spdlog::info("Client caught up.");
				}
				break;
			default:
				spdlog::error("Invalid initial response from server: type={0}", (int)response.type);
				socket.lowest_layer().close();
				return;
		}

		// Start sending the events
		for (auto event : eventList) {
			shared::ClientMessage fileUpdateMsg;
			fileUpdateMsg.type = shared::ClientMessageType::ChangeEvent;
			fileUpdateMsg.event = event;
			fileUpdateMsg.event.hash = shared::getHash(mAppData.getHash(), event);
			switch (event.type) {
			case shared::EventType::Created: {
				spdlog::info("Created file: {0}", event.path);
				break;
			}
			case shared::EventType::Modified: {
				spdlog::info("Modified file: {0}", event.path);
				break;
			}
			case shared::EventType::Deleted: {
				spdlog::info("Deleted file: {0}", event.path);
				break;
			}
			default:
				spdlog::error("Unknown event type, ignoring: {0}", (int)event.type);
				break;
			}

			addNewEvent(event);

			mClientSerializer.reset();
			mClientSerializer.writeObject(fileUpdateMsg);
			socket.write_some(
				boost::asio::buffer(
					mClientSerializer.getData(), 
					mClientSerializer.getSize()
				),  error);
		}

		// Tell the server that we won't be sending data anymore and close the connection
		mClientSerializer.reset();
		shared::ClientMessage endCommMsg;
		endCommMsg.type = shared::ClientMessageType::RequestEndComm;
		mClientSerializer.writeObject(endCommMsg);
		socket.write_some(
			boost::asio::buffer(
				mClientSerializer.getData(), 
				mClientSerializer.getSize()
			),  error);

		spdlog::info("Closing connection.");
		socket.lowest_layer().close();	
		return;
	}

	void App::addNewEvent(shared::Event& event) {
		mAppData.addHash(event.hash);
		mAppData.write();
		mLedger.record(event);
	}
};
