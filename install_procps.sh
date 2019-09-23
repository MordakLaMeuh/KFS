#!/bin/bash
export TARGET="i686-turbofish"
export PATH="/toolchain_turbofish/cross/bin:$PATH"
export TARGET_DIR="../../../system_disk/"

mkdir -pv build_procps
cp patch-procps-config-sub build_procps
cd build_procps
wget -c "https://fossies.org/linux/misc/procps-ng-3.3.15.tar.xz"
tar -xf 'procps-ng-3.3.15.tar.xz'
patch -p0 < patch-procps-config-sub
cd procps-ng-3.3.15
# cp ../../patch-procps-configure .
mkdir -pv build
cd build
CFLAGS="-g -O0 -fno-omit-frame-pointer" ../configure --without-ncurses --disable-modern-top  --disable-pidof --disable-kill   --disable-nls --disable-rpath --disable-numa --build="`gcc -dumpmachine`" --host=$TARGET
make
