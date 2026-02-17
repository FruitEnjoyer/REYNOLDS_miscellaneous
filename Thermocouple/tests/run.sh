#!/usr/bin/bash

gcc -I.. -shared -fPIC -o thermocouple.so ../thermocouple.c

pytest