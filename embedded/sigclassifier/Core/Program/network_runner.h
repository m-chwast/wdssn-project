#pragma once

#include "module.h"
#include "console.h"
#include "acquisition.h"

#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"


class NetworkRunner : public Module {
private:

	Console& _console;
	Acquisition& _acquisition;

	tflite::ErrorReporter* const _errorReporter = new tflite::MicroErrorReporter();

	const tflite::Model* _model = nullptr;

	tflite::ops::micro::AllOpsResolver* _resolver = nullptr;
	tflite::MicroInterpreter* _interpreter = nullptr;

	constexpr static uint32_t _kTensorArenaSize = 4 * 1024;
	uint8_t _tensorArena[_kTensorArenaSize] __attribute__ ((aligned (16)));

	constexpr static size_t _networkInputSize = 100;
	static_assert(_networkInputSize == Acquisition::sampleNo);

	constexpr static size_t _netOutputClassesCnt = 6;

	typedef std::array<float, _netOutputClassesCnt> Output;

	void PrintOutput(const Output& out);
	std::string FloatToPercent(float f);

	void RunNetwork(const Acquisition::Samples& samples);

public:

	NetworkRunner(Console& console, Acquisition& acquisition);

	virtual void Init(void) override;
	virtual void Manage(void) override;

	~NetworkRunner() {
		delete _errorReporter;
		delete _resolver;
		delete _interpreter;
	}
};
