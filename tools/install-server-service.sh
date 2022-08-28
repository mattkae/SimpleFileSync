cp ../server/build/simplefilesync_s /usr/bin/
cp example.server_systemd.service /etc/systemd/system/simplefilesync_s.service
systemctl enable simplefilesync_s.service
systemctl start simplefilesync_s.service