from signals import Sine, Square, Sawtooth, Triangle
import matplotlib.pyplot as plt
import numpy as np
import csv


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

    print("generating...")
    for gen in generators:
        for i in range(200):
            signal, label = gen.generate_random(samples=100)
            signals.append(signal)
            labels.append(label)

    data_with_labels = np.column_stack((signals, labels))

    # shuffle the data
    print("shuffling...")
    rng = np.random.default_rng()
    shuffled_data = rng.permutation(data_with_labels)
    

    print("saving...")
    with open("data.csv", "w", newline="") as file:
        csv_writer = csv.writer(file)
        csv_writer.writerows(shuffled_data)

    print("done!")

def main():
    #plot_sample()
    generate()

main()
