#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra"
LIBS="-lraylib -lGL -lm -lpthread -ldl"
gcc $CFLAGS -o mviz main.cpp $LIBS