#!/bin/bash

if [ ! -e src/test ]; then
  mkdir src/test
  mkdir src/test/plot
fi

if [ ! -e src/lib ]; then
  mkdir src/lib
fi
cd src/lib
wget -O EasyBMP.zip http://prdownloads.sourceforge.net/easybmp/EasyBMP_1.06.zip?download
if [ ! -e bmp ]; then
  mkdir bmp
fi
unzip EasyBMP.zip -d bmp

