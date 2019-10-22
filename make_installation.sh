mkdir installation
cp build/db_FireWall installation
cp config installation -r
cp scripts installation -r

tar -jcv -f installation.tar.bz2 installation/

rm -rf installation

#rm -rf install.tar.bz2
#tar -jcv -f install.tar.bz2 install/
