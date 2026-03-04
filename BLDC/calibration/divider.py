#!/usr/bin/python3

import matplotlib.pyplot as plt
import numpy as np
import cmath


Vbus = 27 # Volts

# Рабочие конфигурации
# Средняя точка: 100k, 0R, 39k, 0.1uF
# Фаза: 39k, 39k, 0.1uF

R1, R2, R3, C = 100000, 0, 10000, 0.0000001

def Zc(w):
    return 1 / (1j * C * w)

def divAbs(w):
    if abs(w - 0) <= 10e-6:
        return R3 / (R1 + R2 + R3)
    Zfull = R1 + R2 + R3 * Zc(w) / (R3 + Zc(w))
    return abs((R3 * Zc(w) / (R3 + Zc(w))) / Zfull)

def divPhase(w):
    if abs(w - 0) <= 10e-6:
        return 0
    Zfull = R1 + R2 + R3 * Zc(w) / (R3 + Zc(w))
    return cmath.phase((R3 * Zc(w) / (R3 + Zc(w))) / Zfull)

w = np.linspace(0, 10000, 10001)
v = [Vbus * divAbs(2 * np.pi * i) for i in w]

backemf = [60. / 550 * i * divAbs(i) for i in w]

print(f"Wrot / wpwm = {divAbs(2 * np.pi * 2000) / divAbs(2 * np.pi * 200000):.3}")
print(f"Vpwm = {Vbus * divAbs(2 * np.pi * 200000):.3}")


plt.plot(w, v)
plt.title(f"R1 = {R1}, R2 = {R2}, R3 = {R3}, C = {1000000 * C:.3} uF")
plt.grid()
plt.show()