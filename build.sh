#!/bin/bash

FILE=main

if [[ ! -d "./bin" ]]; then
    mkdir bin
fi

cc -I./raylib/include -o ./bin/$FILE main.c -L./raylib/lib -l:libraylib.a -l:libraylib.a -lm

if [[ -f "./bin/$FILE" ]]; then
    ./bin/$FILE
fi
