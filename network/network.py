import numpy as np
from keras import Sequential
from keras.layers import Dense, Input
from keras.optimizers import SGD
import dataproc


def create_model() -> Sequential:
    model = Sequential()
    model.add(Input(shape=(100,)))
    model.add(Dense(units=32, activation="tanh"))
    model.add(Dense(units=32, activation="tanh"))
    model.add(Dense(units=6, activation="softmax"))

    model.compile(loss="categorical_crossentropy", optimizer=SGD(learning_rate=0.01), metrics=["accuracy"])
    return model


def model_train(
        model : Sequential, 
        data : ((np.ndarray, np.ndarray), (np.ndarray, np.ndarray))):
    train_data, train_labels = np.array(data[0][0]), np.array(data[0][1])
    valid_data, valid_labels = np.array(data[1][0]), np.array(data[1][1])
    
    validation=(valid_data, valid_labels)
    model.fit(x=train_data, y=train_labels, batch_size=8, epochs=10, verbose=1, validation_data=validation)


if __name__ == '__main__':
    data, labels_txt = dataproc.read_data()

    prepared_data = dataproc.prepare_data(data, labels_txt)

    model = create_model()
    model.summary()

    train_data, train_labels = prepared_data[0][0], prepared_data[0][1]
    #data.test_predictions(model, train_data, train_labels)

    model_train(model, prepared_data)

    dataproc.test_predictions(model, train_data, train_labels)
