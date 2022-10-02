# SimpleFileSync


Testing something!

## Overview
The goal of this project is to reacquint myself with modern C++ by solving some of my organization problems. I have a bunch of `org` files scattered throughout my computers that I would like to sync with a centralized server. Of course, there are many services that would easily do this for me, but I think that it would be more fun to do it by myself and, in the process, discover all of the new things that I've been missing in C++.

## Table of Contents
- [Build](#build)
- [Configuration](#configuration)
- [Directories](#directories)

## Build

### Prerequisites
You must install the following in order before building the server and client:

#### CMake
**Version 3.1**.

#### pkg-config
```sh
sudo apt-get install -y pkg-config
```

#### OpenSSL dev
```sh
sudo apt-get install libssl-dev
```

#### Shared (glue between client and server)
```sh
cd ./shared
mkdir -p build
cd build
cmake ..
make
```

### Server
```sh
cd ./server
mkdir build
cd build
cmake ..
sudo make install
```

The server will be available as `simplefilesync_s`

### Client
```sh
cd ./client
mkdir build
cd build cmake .. 
sudo make install
```

The client will be available as `simplefilesync_c`


## Configuration

To get setup quickly, you can edit the configurations found in the `sample` directory and then use:
```sh
cd tools
./make-data-dirs.sh
```

This will propagate your configurations to `~/.config/simplefilesync` if you are NOT super user, otherwise they will be saved in `/etc/simplefilesync`.

❗**It is recommended that you do NOT run as super user, but it will work. The security vulnerabilities of this program are unknown at the moment, so use at your own risk.** Additionally, we provide no security at the moment (i.e. no password protection or such other security), so running as root would be incredibly dangerous.

### Server
```js
directory ./files           // Directory being synced with the clients (default: ./files)
port 3490                   // Port to use (default: 3490)
useSsl false                // Enable if using ssl (default: false)

ssl_publicKey server.crt    // Public key file, if SSL is enabled (default: server.crt)
ssl_privateKey server.key   // Private key file, if SSL is enabled (default: server.key)
```

### Client
```js
directory ./files           // Directory being synced with the clients (default: ./files)
port 3490                   // Port to connect to (default: 3490)
ip 127.0.0.1                // IP to connect to (default: 127.0.0.1)
useSsl false                // Enable if server uses ssl (default : false)
updateIntervalSeconds 10    // Interval at which we will check for file changes (default: 10 seconds)
```

## Directories
When NOT `sudo` user:
- Configuration: `~/.config/simplefilesync`
- Data: `~/.local/share/simplefilesync`

When `sudo` user:
- Configuration: `/etc/simplefilesync`
- Data: `/var/lib/simplefilesync`
