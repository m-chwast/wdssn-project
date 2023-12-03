from signals import Sine, Square, Sawtooth, Triangle
import matplotlib.pyplot as plt


for i in range(4):
    sine = Sine().generate_random(samples=100)
    plt.subplot(4, 4, i * 4 + 1)  
    plt.plot(sine, '.')

    square = Square().generate_random(samples=100)
    plt.subplot(4, 4, i * 4 + 2)
    plt.plot(square, '.')

    sawtooth = Sawtooth().generate_random(samples=100)
    plt.subplot(4, 4, i * 4 + 3)
    plt.plot(sawtooth, '.')

    triangle = Triangle().generate_random(samples=100)
    plt.subplot(4, 4, i * 4 + 4)
    plt.plot(triangle, '.')

plt.show()
