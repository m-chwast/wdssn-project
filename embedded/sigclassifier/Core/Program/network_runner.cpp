#include "network_runner.h"
#include "tflite_model.h"


NetworkRunner::NetworkRunner(Console& console)
	: _console{console} {

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

}

