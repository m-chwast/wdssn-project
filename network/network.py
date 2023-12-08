import numpy as np
import csv
from matplotlib import pyplot as plt
import keras
from keras import utils
from keras.layers import Dense
from keras.optimizers import SGD
from sklearn.metrics import confusion_matrix
import sklearn.metrics as metr


SignalEnum = {
    "Sine": 0,
    "Square": 1,
    "Triangle": 2,
    "Sawtooth": 3,
    "WhiteNoise": 4,
    "EKG": 5
}
SignalEnumLabel = [key[0] for key in sorted([v for v in SignalEnum.items()], key=lambda v: v[1])]
def getMostProbableIndex(arrInd):
    return np.where(max(arrInd) == arrInd)[0][0]

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
    labels_numeric = [SignalEnum[string] for string in labels_txt]
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
    model.add(Dense(units=32, activation="tanh", input_shape=(100,)))
    model.add(Dense(units=32, activation="tanh"))
    model.add(Dense(units=6, activation="softmax"))

    model.compile(loss="categorical_crossentropy", optimizer=SGD(learning_rate=0.01), metrics=["accuracy"])
    return model

def model_train(
        model, 
        data : ((np.ndarray, np.ndarray), (np.ndarray, np.ndarray))):
    train_data, train_labels = np.array(data[0][0]), np.array(data[0][1])
    valid_data, valid_labels = np.array(data[1][0]), np.array(data[1][1])

    validation=(valid_data, valid_labels)
    model.fit(x=train_data, y=train_labels, batch_size=8, epochs=15, verbose=1, validation_data=validation)

def plot_signals(signals : np.ndarray, labels : np.ndarray):
    cnt = len(signals)
    for i in range(0, cnt):
        signal = signals[i]
        label = labels[i]
        plt.subplot(cnt, 1, i + 1)
        plt.plot(signal)
        plt.title(label)
    plt.tight_layout()
    plt.show()

def show_confusion_matrix(actual, predicted):
    number_of_predicted = 0
    for a, p in zip(actual, predicted):
        if a == p:
            number_of_predicted += 1
    cm = confusion_matrix(actual, predicted)
    labels = [list(SignalEnum.keys())[v] for v in sorted(set(actual) | set(predicted))]
    plotCM = metr.ConfusionMatrixDisplay(confusion_matrix = cm, display_labels = labels)
    plotCM.plot()
    print(f"Total Accuracy {number_of_predicted / len(actual) * 100}")
    plt.show()

def test_predictions(model : keras.Sequential, data, labels, cnt : int = 6):
    indexes = np.random.randint(low=0, high=len(data), size=cnt)
    pred_data = []
    pred_labels = []
    
    actual = []
    for i in indexes:
        pred_data.append(data[i])
        predictedTypeInd = getMostProbableIndex(labels[i])
        pred_labels.append(f"good: {labels[i]}({SignalEnumLabel[predictedTypeInd]})")
        actual.append(predictedTypeInd)

    predictions = model.predict(x=np.array(pred_data), batch_size=cnt)

    for pred in predictions:
        for i in range(0, len(pred)):
            if pred[i] < 0.001:
                pred[i] = 0
    
    predicted = []
    for i in range(len(predictions)):
        predictedTypeInd = getMostProbableIndex(predictions[i])
        pred_labels[i] += f", predicted: {predictions[i]}({SignalEnumLabel[predictedTypeInd]})"
        predicted.append(predictedTypeInd)
    
    show_confusion_matrix(actual, predicted)
    plot_signals(np.array(pred_data)[:cnt], np.array(pred_labels)[:cnt])

def main():
    data, labels_txt = read_data()
    plot_signals(data[:8], labels_txt[:8])

    prepared_data = prepare_data(data, labels_txt)

    model = create_model()
    model.summary()

    test_predictions(model, prepared_data[0][0], prepared_data[0][1])

    model_train(model, prepared_data)

    test_predictions(model, prepared_data[0][0], prepared_data[0][1])


if __name__ == '__main__':
    main()
