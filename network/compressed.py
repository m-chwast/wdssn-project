import numpy as np
import tempfile
import os
from keras import Sequential
from keras.layers import Dense, Input
from keras.optimizers import SGD
import tensorflow as tf
import tensorflow_model_optimization as tfmot
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
        data : ((np.ndarray, np.ndarray), (np.ndarray, np.ndarray)),
        epochs = 10):
    train_data, train_labels = np.array(data[0][0]), np.array(data[0][1])
    valid_data, valid_labels = np.array(data[1][0]), np.array(data[1][1])
    
    validation=(valid_data, valid_labels)
    model.fit(x=train_data, y=train_labels, batch_size=16, epochs=epochs, verbose=1, validation_data=validation)

def create_model_quant_aware(model : Sequential) -> Sequential:
    quant_model = tfmot.quantization.keras.quantize_model(model)
    quant_model.compile(loss="categorical_crossentropy", optimizer=SGD(learning_rate=0.01), metrics=["accuracy"])
    quant_model.summary()
    return quant_model

def get_quantized_model(q_aware_model : Sequential):
    converter = tf.lite.TFLiteConverter.from_keras_model(q_aware_model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    quantized_tflite_model = converter.convert()
    return quantized_tflite_model

def compare_model_sizes(original_model, quantized_tflite_model):
    # Create float TFLite model.
    float_converter = tf.lite.TFLiteConverter.from_keras_model(original_model)
    float_tflite_model = float_converter.convert()

    # Measure sizes of models.
    _, float_file = tempfile.mkstemp('.tflite')
    _, quant_file = tempfile.mkstemp('.tflite')

    with open(quant_file, 'wb') as f:
        f.write(quantized_tflite_model)

    with open(float_file, 'wb') as f:
        f.write(float_tflite_model)

    print("Float model in kb:", os.path.getsize(float_file) / float(2**10))
    print("Quantized model in kb:", os.path.getsize(quant_file) / float(2**10))


def evaluate_one(interpreter, test_data):
    input_index = interpreter.get_input_details()[0]["index"]
    output_index = interpreter.get_output_details()[0]["index"]
    # Pre-processing: add batch dimension and convert to float32 to match with
    # the model's input data format.
    test_data = np.expand_dims(test_data, axis=0).astype(np.float32)
    interpreter.set_tensor(input_index, test_data)

    # Run inference.
    interpreter.invoke()

    # Post-processing: remove batch dimension and find the digit with highest
    # probability.
    output = interpreter.tensor(output_index)
    digit = np.argmax(output()[0])
    return digit

def evaluate_tflite_model(interpreter, test_data, test_labels):
    input_index = interpreter.get_input_details()[0]["index"]
    output_index = interpreter.get_output_details()[0]["index"]

    # Run predictions on every sample in the provided dataset.
    predicted = []
    for i, test_data in enumerate(test_data):
        if i % 5000 == 0:
            print('Evaluated on {n} results so far.'.format(n=i))
        
        digit = evaluate_one(interpreter, test_data)
         
        predicted.append(digit)

    # Compare prediction results with ground truth labels to calculate accuracy.
    predicted = np.array(predicted)

    correct = []
    for label in test_labels:
        correct.append(np.argmax(label))
    
    return predicted, correct


def test_quantized_model(tflite_model, test_data, test_labels):
    interpreter = tf.lite.Interpreter(model_content=tflite_model)
    interpreter.allocate_tensors()

    predicted, correct = evaluate_tflite_model(interpreter, test_data, test_labels)

    test_accuracy = (predicted == correct).mean()
    print('Quant TFLite test_accuracy:', test_accuracy)
    dataproc.show_confusion_matrix(actual=correct, predicted=predicted)

def main():
    data, labels_txt = dataproc.read_data()
    prepared_data = dataproc.prepare_data(data, labels_txt)

    model = create_model()
    model.summary()

    model_train(model, prepared_data)

    model_quant_aware = create_model_quant_aware(model)
    # train model with quantization-aware training
    model_train(model_quant_aware, prepared_data, epochs=2)

    model_tflite_quantized = get_quantized_model(model_quant_aware)

    train_data, train_labels = prepared_data[0][0], prepared_data[0][1]
    # dataproc.test_predictions(model, train_data, train_labels, 10000)
    
    compare_model_sizes(original_model=model, quantized_tflite_model=model_tflite_quantized)

    test_quantized_model(model_tflite_quantized, train_data, train_labels)

main()
