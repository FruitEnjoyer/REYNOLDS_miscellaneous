#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

# Количество точек
N = 6

def clarke(a, b, c):
    A = np.array([[2/3, -1/3, -1/3], [0, np.sqrt(3)/3, -np.sqrt(3)/3]])
    return A @ np.array([a, b, c])

def clarke_inv(point):
    x, y = point
    A = np.array([[1, 0], [-1/2, np.sqrt(3)/2], [-1/2, -np.sqrt(3)/2]])
    return A @ np.array([x, y])

angles = [2 * np.pi * i / N for i in range(N)]
points = [[np.cos(i), np.sin(i)] for i in angles]

phases = [clarke_inv(p) for p in points]

for i in range(len(phases)):
    print(f"{{{phases[i][0]:9.6f}, {phases[i][2]:9.6f}, {phases[i][1]:9.6f}}},   // {round(360 * angles[i] / 2 / np.pi)} deg")