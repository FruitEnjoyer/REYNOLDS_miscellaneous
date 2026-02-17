#!/usr/bin/python3.12

import matplotlib.pyplot as plt


# Данные взяты с сайта https://kipia.xyz/thermocouples_calculate/table/type_k/100 (обращение от 17.02.2026)
filepath = "result.log"

input_ = []
rect = []
black = []

with open(filepath, 'r') as f:
    data = str(f.read())
    data = data.split('\n')
    for i in data:
        if len(i) > 0:
            temp = [float(x) for x in i.split(' ')]
            input_.append(temp[0])
            rect.append(temp[1])
            black.append(temp[2])
    f.close()
    plt.figure(1)
    plt.plot(input_, 'o-', linewidth=2)
    plt.plot(rect, 'o-', linewidth=2)
    plt.plot(black, 'o-', linewidth=2)
    plt.title('Impulse response')
    plt.grid(True)
    plt.show()
