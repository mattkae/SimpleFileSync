cp ../client/build/simplefilesync_c /usr/bin/
cp example.client_systemd.service /etc/systemd/system/simplefilesync_c.service
systemctl enable simplefilesync_c.service
systemctl start simplefilesync_c.service