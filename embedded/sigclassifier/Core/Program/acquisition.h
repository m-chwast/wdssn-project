#pragma once

#include <array>
#include "main.h"
#include "module.h"
#include "console.h"


class Acquisition : public Module {
private:

	constexpr static uint32_t _initSamplingFreq = 100000;
	constexpr static uint32_t _sampleNo = 100;

	Console& _console;
	TIM_HandleTypeDef& _samplingHtim;
	ADC_HandleTypeDef& _hadc;

	std::array<uint8_t, _sampleNo> _samples;

public:

	Acquisition(Console& console, TIM_HandleTypeDef& samplingHtim, ADC_HandleTypeDef& hadc)
		: _console{console},
		  _samplingHtim{samplingHtim},
		  _hadc{hadc} {}

	void Init(void) override;
	void Manage(void) override {};

	void Start(void);

	uint32_t SetSamplingFreq(uint32_t freqHz);
};
