#include "network_runner.h"
#include "tflite_model.h"
#include <map>
#include <algorithm>


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
	TfLiteTensor* _modelInput = _interpreter->input(0);
	TfLiteTensor* _modelOutput = _interpreter->output(0);

	float inData[_networkInputSize];
	for(size_t i = 0; i < _networkInputSize; i++) {
		inData[i] = samples[i] / 255.0;
	}

	for(size_t i = 0; i < _networkInputSize; i++) {
		_modelInput->data.f[i] = inData[i];
	}

	TfLiteStatus invokeStatus = _interpreter->Invoke();
	if(invokeStatus != kTfLiteOk) {
		TF_LITE_REPORT_ERROR(_errorReporter, "Invoke failed\r\n");
		return;
	}

	Output out;
	for(size_t i = 0; i < _netOutputClassesCnt; i++) {
		out[i] = _modelOutput->data.f[i];
	}

	PrintOutput(out);
}

void NetworkRunner::PrintOutput(const Output& out) const {
	const std::map<uint8_t, const char*> classes {
			{0, "Sine"},
			{1, "Square"},
			{2, "Triangle"},
			{3, "Sawtooth"},
			{4, "Noise"},
			{5, "EKG"},
	};

	const auto maxElem = std::max_element(out.begin(), out.end());
	uint8_t index = std::distance(out.begin(), maxElem);

	std::string outStr = "Result: ";
	outStr += (*classes.find(index)).second;
	for(size_t i = outStr.length(); i < 30; i++) {
		outStr += " ";
	}

	outStr += "Percents: ";
	for(size_t i = 0; i < _netOutputClassesCnt; i++) {
		outStr += (*classes.find(i)).second;
		outStr += "=";
		outStr += FloatToPercent(out[i]);
		outStr += (i == _netOutputClassesCnt - 1 ? "\r\n" : ", ");
	}

	_console << outStr.c_str();
}

std::string NetworkRunner::FloatToPercent(float f) const {
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

