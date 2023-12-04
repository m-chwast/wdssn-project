import numpy as np
import csv
from matplotlib import pyplot as plt
import keras
from keras import utils
from keras.layers import Dense
from keras.optimizers import SGD


def read_data() -> tuple[np.ndarray, np.ndarray]:
    with open("data.csv", "r") as file:
        csv_reader = csv.reader(file)

        data_with_labels = [row for row in csv_reader]
        # convert the list into a np array
        data_array = np.array(data_with_labels)

        # separate the array back into data and labels
        loaded_data_str = data_array[:, :-1]  # all columns except the last one
        loaded_data = np.array(loaded_data_str, dtype=np.float64)
        loaded_labels = data_array[:, -1]  # the last column

        return loaded_data, loaded_labels
    
def enumarate_labels(labels_txt : list[str]) -> np.ndarray:
    string_to_numeric = {string: i for i, string in enumerate(set(labels_txt))}
    labels_numeric = [string_to_numeric[string] for string in labels_txt]
    return np.array(labels_numeric)

def prepare_data(
        data : np.ndarray, 
        labels_txt: np.ndarray,
        validation_data_rate : float = 0.1
        ) -> ((np.ndarray, np.ndarray), (np.ndarray, np.ndarray)):
    labels_num = enumarate_labels(labels_txt)
    labels_cat = utils.to_categorical(labels_num)

    split_index = int((1.0 - validation_data_rate) * len(data))
    train_data, train_labels = data[:split_index], labels_cat[:split_index]
    valid_data, valid_labels = data[split_index:], labels_cat[split_index:]

    return ((train_data, train_labels), (valid_data, valid_labels))

def create_model():
    model = keras.Sequential()
    model.add(Dense(units=100, activation="sigmoid", input_shape=(100,)))
    model.add(Dense(units=100, activation="sigmoid"))
    model.add(Dense(units=4, activation="softmax"))

    model.compile(loss="categorical_crossentropy", optimizer=SGD(learning_rate=0.01), metrics=["accuracy"])
    return model

def model_train(
        model, 
        data : ((np.ndarray, np.ndarray), (np.ndarray, np.ndarray))):
    train_data, train_labels = np.array(data[0][0]), np.array(data[0][1])
    valid_data, valid_labels = np.array(data[1][0]), np.array(data[1][1])

    validation=(valid_data, valid_labels)
    model.fit(x=train_data, y=train_labels, batch_size=128, epochs=10, verbose=1, validation_data=validation)

def plot_signals(signals : np.ndarray, labels : np.ndarray):
    cnt = len(signals)
    for i in range(0, cnt):
        signal = signals[i]
        label = labels[i]
        plt.subplot(cnt, 1, i + 1)
        plt.plot(signal)
        plt.title(label)
    plt.show()


def main():
    data, labels_txt = read_data()
    #plot_signals(data[:6], labels_txt[:6])

    prepared_data = prepare_data(data, labels_txt)

    model = create_model()
    #model.summary()

    model_train(model, prepared_data)


main()
