from signals import Sine, Square, Sawtooth, Triangle
import matplotlib.pyplot as plt


generators = [Sine(), Square(), Triangle(), Sawtooth()]
gen_cnt = len(generators)

signals = []
labels = []

for i in range(gen_cnt ** 2):
    wave, label = generators[int(i % gen_cnt)].generate_random(samples=100)
    
    signals.append(wave)
    labels.append(label)

    plt.subplot(gen_cnt, gen_cnt, i + 1)
    plt.plot(wave, '.')

plt.show()
