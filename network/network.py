import numpy as np
import csv


def read_data() -> tuple[np.ndarray, np.ndarray]:
    with open("data.csv", "r") as file:
        csv_reader = csv.reader(file)

        data_with_labels = [row for row in csv_reader]
        # convert the list into a np array
        data_array = np.array(data_with_labels)

        # separate the array back into data and labels
        loaded_data = data_array[:, :-1]  # all columns except the last one
        loaded_labels = data_array[:, -1]  # the last column

        return loaded_data, loaded_labels

def main():
    data, labels = read_data()

main()
