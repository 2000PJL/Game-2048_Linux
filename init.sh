sudo cp ./libs/libjpeg.so  /lib/
sudo ln -s /lib/libjpeg.so /lib/libjpeg.so.8
sudo ln -s /lib/libjpeg.so /lib/libjpeg.so.8.0
sudo ln -s /lib/libjpeg.so /lib/libjpeg.so.8.0.1
sudo chmod 777 /lib/*