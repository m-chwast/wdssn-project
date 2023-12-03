from signals import Sine, Square, Sawtooth, Triangle
import matplotlib.pyplot as plt


def get_generators():
    return [Sine(), Square(), Triangle(), Sawtooth()]

def plot_sample():
    generators = get_generators()
    gen_cnt = len(generators)

    for i in range(gen_cnt ** 2):
        signal, label = generators[int(i % gen_cnt)].generate_random(samples=100)
        plt.subplot(gen_cnt, gen_cnt, i + 1)
        plt.plot(signal, '.')
    plt.show()

def generate():
    generators = get_generators()
    signals = []
    labels = []

    for i in range(100):
        signal, label = generators[int(i % len(generators))].generate_random(samples=100)
        signals.append(signal)
        labels.append(label)


def main():
    plot_sample()
    generate()

main()
