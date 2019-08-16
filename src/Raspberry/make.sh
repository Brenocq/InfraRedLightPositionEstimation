#!/bin/bash

TARGET_BUILD_FOLDER=./build

mkdir $TARGET_BUILD_FOLDER
mkdir $TARGET_BUILD_FOLDER/hello_pixy
mkdir $TARGET_BUILD_FOLDER/genDataNN

cd $TARGET_BUILD_FOLDER/hello_pixy
cmake ../../src/hello_pixy
make
mv hello_pixy ../../bin/

cd ../../

cd $TARGET_BUILD_FOLDER/genDataNN
cmake ../../src/genDataNN
make
mv genDataNN ../../bin/
