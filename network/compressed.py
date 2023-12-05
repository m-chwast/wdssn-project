import numpy as np
from keras import Sequential
from keras.layers import Dense, Input
from keras.optimizers import SGD
import tensorflow_model_optimization as tfmot
import dataproc


def create_model() -> Sequential:
    model = Sequential()
    model.add(Input(shape=(100,)))
    model.add(Dense(units=32, activation="tanh"))
    model.add(Dense(units=32, activation="tanh"))
    model.add(Dense(units=4, activation="softmax"))

    model.compile(loss="categorical_crossentropy", optimizer=SGD(learning_rate=0.01), metrics=["accuracy"])
    return model

def model_train(
        model : Sequential, 
        data : ((np.ndarray, np.ndarray), (np.ndarray, np.ndarray)),
        epochs = 10):
    train_data, train_labels = np.array(data[0][0]), np.array(data[0][1])
    valid_data, valid_labels = np.array(data[1][0]), np.array(data[1][1])
    
    validation=(valid_data, valid_labels)
    model.fit(x=train_data, y=train_labels, batch_size=16, epochs=epochs, verbose=1, validation_data=validation)

def create_model_quantized(model : Sequential) -> Sequential:
    quant_model = tfmot.quantization.keras.quantize_model(model)
    quant_model.compile(loss="categorical_crossentropy", optimizer=SGD(learning_rate=0.01), metrics=["accuracy"])
    quant_model.summary()
    return quant_model


def main():
    data, labels_txt = dataproc.read_data()
    prepared_data = dataproc.prepare_data(data, labels_txt)

    model = create_model()
    model.summary()

    model_train(model, prepared_data)

    model_quantized = create_model_quantized(model)
    # train quantized model with quantization-aware training
    model_train(model_quantized, prepared_data, epochs=2)

main()
