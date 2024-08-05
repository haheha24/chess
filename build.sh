#!/bin/bash

libs=-luser32
warnings=-Wno-writable-strings

g++ -g src/main.cpp -o chess.exe $libs $warnings