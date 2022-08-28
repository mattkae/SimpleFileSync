#!/usr/bin/env bash
if [[ $EUID = 0 ]]; then
    mkdir -p /etc/simplefilesync
    cp ../sample/server.conf /etc/simplefilesync/
    cp ../sample/client.conf /etc/simplefilesync/
else
    mkdir -p ~/.config/simplefilesync
    cp ../sample/server.conf ~/.config/simplefilesync/
    cp ../sample/client.conf ~/.config/simplefilesync/
fi
