
echo "Building shared library..."
cd shared && mkdir -p build && cd build && cmake .. && make && cd ../../

echo "Building client..."
cd client && mkdir -p build && cd build && cmake .. && make install && cd ../../

systemctl stop simplefilesync_c.service
systemctl disable simplefilesync_c.service
cp ./sample/example.client_systemd.service /etc/systemd/system/simplefilesync_c.service
systemctl enable simplefilesync_c.service
systemctl start simplefilesync_c.service
