#include "network_runner.h"
#include "tflite_model.h"


NetworkRunner::NetworkRunner(Console& console, Acquisition& acquisition)
	: _console{console}, _acquisition{acquisition} {

}

void NetworkRunner::Init(void) {

	_model = tflite::GetModel(TFLiteModel::model);

	if(_model->version() != TFLITE_SCHEMA_VERSION) {
		TF_LITE_REPORT_ERROR(_errorReporter,
				"Model provided is schema version %d not equal "
				"to supported version %d\r\n",
				_model->version(), TFLITE_SCHEMA_VERSION);
	}

	_resolver = new tflite::ops::micro::AllOpsResolver();
	_interpreter = new tflite::MicroInterpreter(
			_model, *_resolver, _tensorArena, _kTensorArenaSize, _errorReporter);

	TfLiteStatus allocateStatus = _interpreter->AllocateTensors();
	if(allocateStatus != kTfLiteOk) {
	    TF_LITE_REPORT_ERROR(_errorReporter, "AllocateTensors() failed");
	    return;
	}

	TfLiteTensor* _modelInput = _interpreter->input(0);
	TfLiteTensor* _modelOutput = _interpreter->output(0);

	float inData[100];
	for(int i = 0; i < 100; i++) {
		inData[i] = i > 25 ? 1 : 0;
	}

	for(int i = 0; i < 100; i++) {
		_modelInput->data.f[i] = inData[i];
	}

	TfLiteStatus invokeStatus = _interpreter->Invoke();
	if(invokeStatus != kTfLiteOk) {
		TF_LITE_REPORT_ERROR(_errorReporter, "Invoke failed\r\n");
		return;
	}

	float outData[6];
	for(int i = 0; i < 6; i++) {
		outData[i] = _modelOutput->data.f[i];
	}

	_console << "Result: ";

	std::string percent = "";

	for(int i = 0; i < 6; i++) {
		float result = outData[i];
		percent += FloatToPercent(result) + ", ";
	}

	_console << percent.c_str() << "\r\n";

	_console << "Network Runner init ok\r\n";
}

void NetworkRunner::Manage(void) {
	if(_acquisition.AreSamplesReady() == false) {
		return;
	}

	std::array<uint8_t, _acquisition.sampleNo> samples = _acquisition.GetSamples();
	RunNetwork(samples);
}

void NetworkRunner::RunNetwork(const Acquisition::Samples& samples) {

}

std::string NetworkRunner::FloatToPercent(float f) {
	std::string str;
	uint32_t i = f * 1000;

	uint32_t percent = i / 10;
	uint8_t decimal = i % 10;

	str += std::to_string(percent);
	str += '.';
	str += std::to_string(decimal);
	str += '%';

	return str;
}

