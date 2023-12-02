from signals import Sine
import matplotlib.pyplot as plt


for i in range(4):
    sine = Sine().generate_random(100)
    plt.subplot(4, 1, i + 1)    
    plt.plot(sine, '.')

plt.show()
