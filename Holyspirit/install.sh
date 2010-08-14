#!/bin/bash
mkdir /usr/lib/sfml
cp -R sfml /usr/lib/
cd /usr/lib/sfml
ln -s libsfml-audio.so.2.0 libsfml-audio.so
ln -s libsfml-graphics.so.2.0 libsfml-graphics.so
ln -s libsfml-network.so.2.0 libsfml-network.so
ln -s libsfml-system.so.2.0 libsfml-system.so
ln -s libsfml-window.so.2.0 libsfml-window.so
echo "/usr/lib/sfml" > /etc/ld.so.conf.d/sfml2.0.conf
ldconfig

