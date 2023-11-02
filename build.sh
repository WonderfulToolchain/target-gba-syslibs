#!/bin/sh
set -e

if [ -d build ]; then
	rm -r build
fi
mkdir build
cp -aRv target build/
cd crts
make WF_TARGET_DIR=../build/target/gba/rom
DESTDIR=../build make install
cd ..

build_library () {
	cd "$1"
	make -j1 clean WF_TARGET_DIR=../build/target/"$2" TARGET="$2"
	make WF_TARGET_DIR=../build/target/"$2" TARGET="$2"
	make -j1 install WF_TARGET_DIR=../build/target/"$2" TARGET="$2"
	cd ..
}

for i in gba/rom gba/multiboot; do
	build_library libgba $i
done

build_library libgbafat gba/rom
