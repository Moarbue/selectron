#!/bin/sh

set -xe

mkdir -p bin

gcc -ggdb -Wall -Wextra src/main.c src/tokenizer.c -o bin/selectron