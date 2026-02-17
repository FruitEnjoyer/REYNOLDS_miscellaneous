#!/usr/bin/python3

# Loading shared library into python tutorial:
# https://docs-python.ru/standart-library/modul-ctypes-python/povedenie-vneshnih-funktsij-modulja-ctypes

from ctypes import *
import pytest
import math as m

# Load shared library
libpath = "/home/user/work/projects/REYNOLDS_miscellaneous/Thermocouple/tests/thermocouple.so"
tc = cdll.LoadLibrary(libpath)
tc.TC_Volts2Temp.argtypes = [c_float, c_float]
tc.TC_Volts2Temp.restype = c_float


def isclose(a, b, tol=1):
    return abs(a - b) <= tol