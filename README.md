# SimpleFileSync

## Overview
The goal of this project is to reacquint myself with modern C++ by solving some of my organization problems. Essentially, I have a bunch of org files scattered throughout my computers and accounts that I would like to sync with a centralized server. Of course, there are many services that would easily do this for me, but I think that it would be more fun to do it by myself and, in the process, discover all of the new things that I've been missing in C++.

## How to build

### Prerequisites
You must install the following in order before building the server and client:

#### Cmake
We want **version 3.23**.

#### spdlog (for logging)
```
git submodule init
git submodule update
cd spdlog && mkdir -p build && cd build && cmake .. && make
```

#### boost (for TCP srever/client and program options)
You must have boost installed on your system. As an example, you can install on Ubuntu via:
```
sudo apt-get install libboost-all-dev
```

Obviously this will be different on your repository, so please check!

#### shared (glue between client and server)
```
cd ./shared && mkdir -p build && cd build && cmake .. && make
```

### Server
```
cd ./server && mkdir build && cd build && cmake .. && make
```

### Client
```
cd ./client && mkdir build && cd build && cmake .. && make
```

### What does it mean to use modern C++?
- No raw pointers
- No raw strings
- Cmake
- Modern C++ features
- clang-tidy

## Architecture
The project will feature a single centralized server talking to multiple clients (daemons). The server will know the latest hash of the data. At a set interval, the client will try and post any changes to the server. In this moment, one of three things will happen:
1. The client is on the same version as the server and nothing happens.
2. The client has made changes and is now on a new version of the server, so the server upgrades.
3. The client is on an old version of the data, so the client upgrades.

## Future
- Handle the client and server diverging (i.e. the server has new changes *and* the client has new changes). This will involve us doing some merge resolution if the client and server modify the same file. We will not handle this for version 1.
- Add a GUI frontend to the client daemon, so that the GUI speaks to the daemon and the client speaks to the server. This will be good for the future case of "merge issues". This GUI could even be a command-line gui, but we'll see what happens.