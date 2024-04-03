#!/bin/sh

make clean
make TARGET=funkey

mkdir -p opk
cp blockdude opk/blockdude
cp -r ./blockdudefs opk/blockdudefs
cp Blockdude.funkey-s.desktop opk/Blockdude.funkey-s.desktop

mksquashfs ./opk Blockdude.opk -all-root -noappend -no-exports -no-xattrs

rm -r opk