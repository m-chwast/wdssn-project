from signals import Sine, Square
import matplotlib.pyplot as plt


for i in range(4):
    sine = Sine().generate_random(samples=100)
    plt.subplot(4, 2, i * 2 + 1)  
    plt.plot(sine, '.')

    square = Square().generate_random(samples=100)
    plt.subplot(4, 2, i * 2 + 2)
    plt.plot(square, '.')

plt.show()
