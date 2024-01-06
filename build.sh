#!/bin/sh

set -e

CC="gcc"
CFLAGS="-Wall -Wextra"
CFLAGS_DEBUG="-ggdb3 -g -O0"
CFLAGS_RELEASE="-O3"

LIBS="-lm"
SRC="src/*.c"
BIN_PATH="bin"
BIN="${BIN_PATH}/selectron"

compile () {
    set -x
    mkdir -p $BIN_PATH
    $CC $CFLAGS -o $BIN $SRC $LIBS
}

if [ $# -eq 0 ]; then
    CFLAGS="${CFLAGS} ${CFLAGS_RELEASE}"
    compile
fi

for arg in "$@"
do
    case $arg in
        debug)
            CFLAGS="${CFLAGS} ${CFLAGS_DEBUG}"
            compile
            ;;
        release)
            CFLAGS="${CFLAGS} ${CFLAGS_RELEASE}"
            compile 
            ;;
        clean)
            ( set -x; rm -rf ${BIN_PATH:?}/* )
            ;;
    esac
done