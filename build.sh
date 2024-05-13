#!/bin/bash

FILE=main

cc -I./raylib/include -o $FILE main.c -L./raylib/lib -l:libraylib.a -l:libraylib.a -lm

if [[ -f "$FILE" ]]; then
    ./$FILE
fi
