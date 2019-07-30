#!/bin/bash

./pre_op_before_run
cp ./db_FireWall /bin/
cp ./config/db_FireWall /etc/init.d/
mkdir -p /etc/db_FireWall
cp ./config/db_FireWall.json /etc/db_FireWall/

