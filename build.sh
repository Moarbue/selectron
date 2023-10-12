#!/bin/sh

set -xe

mkdir -p bin

gcc -ggdb -Wall -Wextra src/main.c src/tokenizer.c src/stack.c src/queue.c src/parser.c src/error.c src/operator.c -o bin/selectron -lm