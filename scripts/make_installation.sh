#!/bin/bash
mkdir installation
cp ../build/db_FireWall installation
cp ../config installation -r
cp install.sh installation

tar -jcv -f installation.tar.bz2 installation/

rm -rf installation

