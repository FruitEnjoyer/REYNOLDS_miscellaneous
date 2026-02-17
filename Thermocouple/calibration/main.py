#!/usr/bin/python3

import matplotlib.pyplot as plt

with open("TypeK.dat", 'r') as f:
    data = [float(x) for x in str(f.read()).replace('\t', '').replace('\n', ' ').split(' ')]
    data5 = data[0:-1:5]
    t = []
    v = []
    t5 = []
    v5 = []
    for i in range(len(data)):
        t.append(-270 + 10 * i)
        v.append(data[i])
    for i in range(len(data5)):
        t5.append(-270 + 50 * i)
        v5.append(data5[i])
    t5.append(-270 + 10 * (len(data) - 1))
    v5.append(data[-1])
    f.close()

    plt.figure(figsize=(20, 5))
    plt.plot(t, v, 'o-', linewidth=2)
    plt.plot(t5, v5, 'o-', linewidth=2)
    plt.title('ThermoEMF')
    plt.xlabel('Temperature, [°C]')
    plt.ylabel('Voltage, [mV]')
    plt.grid(True)
    plt.show()

    outputv5 = [str(x) for x in v5]
    outputv5 = ", ".join(outputv5)
    print(outputv5)
    print('\n')
    outputt5 = [str(x) for x in t5]
    outputt5 = ", ".join(outputt5)
    print(outputt5)