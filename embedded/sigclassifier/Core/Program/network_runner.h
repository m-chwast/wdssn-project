#pragma once

#include "module.h"
#include "console.h"

#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"


class NetworkRunner : public Module {
private:

	Console& _console;

	tflite::ErrorReporter* const _errorReporter = new tflite::MicroErrorReporter();
	const tflite::Model* _model = nullptr;
	tflite::MicroInterpreter* _interpreter = nullptr;
	TfLiteTensor* _modelInput = nullptr;
	TfLiteTensor* _modelOutput = nullptr;

	constexpr static uint32_t _kTensorArenaSize = 2 * 1024;
	uint8_t _tensorArena[_kTensorArenaSize];

public:

	NetworkRunner(Console& console);

	virtual void Init(void) override;
	virtual void Manage(void) override;

	~NetworkRunner() {
		delete _errorReporter;
	}
};
