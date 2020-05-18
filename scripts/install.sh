#!/bin/bash
cp ../build/db_FireWall /bin
mkdir /etc/db_FireWall
chmod a+wx /etc/db_FireWall -R
cp ../config/db_FireWall.json /etc/db_FireWall
cp ../config/db_FireWall /etc/init.d/
#apt install rinetd
#mv /etc/rinetd.conf /etc/rinetd.conf.bak
#echo 'logfile /var/log/rinetd.log' >> /etc/rinetd.conf
#echo 'allow *.*.*.*' >> /etc/rinetd.conf
#echo '0.0.0.0 6666 0.0.0.0 3306' >> /etc/rinetd.conf
