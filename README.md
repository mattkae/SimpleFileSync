# SimpleFileSync

## Overview
The goal of this project is to reacquint myself with modern C++ by solving some of my organization problems. Essentially, I have a bunch of org files scattered throughout my computers and accounts that I would like to sync with a centralized server. Of course, there are many services that would easily do this for me, but I think that it would be more fun to do it by myself and, in the process, discover all of the new things that I've been missing in C++.

## Table of Contents
- [Bulding](#building)
- [Configuration](#configuration)
- [Running](#running)
- [Directories](#directories)
- [Architecture](#architecture)
- [Future](#future)

## Building

### Prerequisites
You must install the following in order before building the server and client:

#### Cmake
**Version 3.23**.

#### spdlog (for logging)
```
git submodule init
git submodule update
cd spdlog && mkdir -p build && cd build && cmake .. && make
```

#### Shared (glue between client and server)
```
cd ./shared && mkdir -p build && cd build && cmake .. && make
```

### Server
```sh
cd ./server && mkdir build && cd build && cmake .. && make
```

### Client
```sh
cd ./client && mkdir build && cd build && cmake .. && make
```

## Configuration

To get setup quickly, you can edit the configurations found in the `sample` directory and then use:
```sh
cd tools && ./make-data-dirs.sh
```

This will propagate your configurations to `~/.config/simplefilesync` if you are NOT super user, otherwise they will be saved in `/etc/simplefilesync`.

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

## Running

### Server
```sh
cd ./server/build && ./sfs_server
```

### Client
```sh
cd ./client/build && ./sfs_client
```

## Directories
When NOT `sudo` user:
- Configuration: `~/.config/simplefilesync`
- Data: `~/.local/share/simplefilesync`

When `sudo` user:
- Configuration: `/etc/simplefilesync`
- Data: `/var/lib/simplefilesync`

## Architecture
The project will feature a single centralized server talking to multiple clients (daemons). The server will know the latest hash of the data. At a set interval, the client will try and post any changes to the server. In this moment, one of three things will happen:
1. The client is on the same version as the server and nothing happens.
2. The client has made changes and is now on a new version of the server, so the server upgrades.
3. The client is on an old version of the data, so the client upgrades.

## Future
- Handle the client and server diverging (i.e. the server has new changes *and* the client has new changes). This will involve us doing some merge resolution if the client and server modify the same file. We will not handle this for version 1.
- Add a GUI frontend to the client daemon, so that the GUI speaks to the daemon and the client speaks to the server. This will be good for the future case of "merge issues". This GUI could even be a command-line gui, but we'll see what happens.