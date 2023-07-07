#!/bin/bash

# install command
install_command="sudo apt-get install -y"

sudo apt-get remove modemmanager

$install_command libtool libusb-1.0 libftdi-dev python3 python3-serial python3-empy

arch=$(uname -i)
if [[ $arch == x86_64* ]]; then
    $install_command  lib32ncurses5-dev
elif [[ $arch == aarch* ]]; then
    $install_command  libncurses5-dev

deb_arch=$(dpkg-architecture -q DEB_BUILD_ARCH)
if [[ $arch == amd* ]]; then
    $install_command  libpython2.7:i386
elif [[ $arch == arm* ]]; then
    $install_command  python2.7-dev

FILE=./gcc-arm-none-eabi-5_4-2016q2
if [ ! -d "$FILE" ]; then
    wget http://handsfree-mv.oss-cn-shenzhen.aliyuncs.com/handsfree_download/OpenRE_Development_Toolchain/gcc-arm-none-eabi-5_4-2016q2.tar.bz2
    tar -jxvf gcc-arm-none-eabi-5_4-2016q2.tar.bz2
    rm gcc-arm-none-eabi-5_4-2016q2.tar.bz2
fi


FILE=./openocd
if [ ! -d "$FILE" ]; then
    git clone https://github.com/openocd-org/openocd.git
#    wget http://handsfree-mv.oss-cn-shenzhen.aliyuncs.com/handsfree_download/OpenRE_Development_Toolchain/openocd.tar.bz2
#    tar -jxvf openocd.tar.bz2
#    rm openocd.tar.bz2
fi


FILE=./stlink
if [ ! -d "$FILE" ]; then
    wget http://handsfree-mv.oss-cn-shenzhen.aliyuncs.com/handsfree_download/OpenRE_Development_Toolchain/stlink.tar.bz2   
    tar -jxvf stlink.tar.bz2
    rm stlink.tar.bz2
fi

cd openocd/
./bootstrap
./configure --disable-werror
make clean
make
sudo make install
cd ../stlink/
make clean
make
sudo make install
cd ../

sudo usermod -a -G dialout $USER
