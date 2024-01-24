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

	tflite::ops::micro::AllOpsResolver* _resolver = nullptr;
	tflite::MicroInterpreter* _interpreter = nullptr;

	constexpr static uint32_t _kTensorArenaSize = 4 * 1024;
	uint8_t _tensorArena[_kTensorArenaSize] __attribute__ ((aligned (16)));

public:

	NetworkRunner(Console& console);

	virtual void Init(void) override;
	virtual void Manage(void) override;

	~NetworkRunner() {
		delete _errorReporter;
		delete _resolver;
		delete _interpreter;
	}
};
