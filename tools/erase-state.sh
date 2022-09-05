#!/usr/bin/env bash
if [[ $EUID = 0 ]]; then
    rm -rf /var/lib/simplefilesync/.client_saved.data 
    rm -rf /var/lib/simplefilesync/.server_saved.data 
    rm -rf /var/lib/simplefilesync/.client_events
    rm -rf /var/lib/simplefilesync/.server_events
else
    rm -rf ~/.local/share/simplefilesync/.client_saved.data 
    rm -rf ~/.local/share/simplefilesync/.server_saved.data 
    rm -rf ~/.local/share/simplefilesync/.client_events
    rm -rf ~/.local/share/simplefilesync/.server_events
fi
