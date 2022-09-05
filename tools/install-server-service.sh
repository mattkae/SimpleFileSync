
echo "Building shared library..."
cd shared && mkdir -p build && cd build && cmake .. && make && cd ../../

echo "Building server..."
cd server && mkdir -p build && cd build && cmake .. && make install && cd ../../

systemctl stop simplefilesync_s.service
systemctl disable simplefilesync_s.service
cp ./sample/example.server_systemd.service /etc/systemd/system/simplefilesync_s.service
systemctl enable simplefilesync_s.service
systemctl start simplefilesync_s.service
