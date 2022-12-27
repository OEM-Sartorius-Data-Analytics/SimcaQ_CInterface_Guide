#!/bin/bash

aclocal

autoconf

touch README AUTHORS NEWS ChangeLog

automake -a

./configure PKG_CONFIG_PATH=/usr/lib64/pkgconfig

make
