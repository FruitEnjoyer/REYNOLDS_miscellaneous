#!/usr/bin/bash

clear && gcc-13 -I.. main.c ../lowpassfilter.c -lm -Werror -Wpedantic && ./a.out && ./main2.py &