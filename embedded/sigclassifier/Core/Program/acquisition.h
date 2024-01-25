#pragma once

#include <array>
#include <vector>
#include "main.h"
#include "module.h"
#include "console.h"


class Acquisition : public Module {
public:
	constexpr static uint32_t sampleNo = 100;

	typedef std::array<uint8_t, sampleNo> Samples;

private:

	constexpr static uint32_t _managePeriodMs = 1000;
	uint32_t _lastManageRunTime;

	constexpr static uint32_t _initSamplingFreq = 100000;

	constexpr static bool _logEvents = false;

	Console& _console;
	TIM_HandleTypeDef& _samplingHtim;
	ADC_HandleTypeDef& _hadc;

	Samples _samples;
	volatile bool _acqInProgress;
	bool _samplesReady = false;
	bool _canStartAcq = false;

	static std::vector<Acquisition*> _acquisitions;

	void ProcessSamples(void);

	static void GeneralADCConvCpltCb(ADC_HandleTypeDef* hdma);
	void ADCConvCpltCb(void);

public:

	Acquisition(Console& console, TIM_HandleTypeDef& samplingHtim, ADC_HandleTypeDef& hadc);

	void Init(void) override;
	void Manage(void) override;

	bool Start(void);
	bool IsAcquisitionInProgress(void) const { return _acqInProgress; }
	bool AreSamplesReady(void) const { return _samplesReady && (_canStartAcq == false); }
	const Samples GetSamples(void) { _canStartAcq = true; return _samples; }

	uint32_t SetSamplingFreq(uint32_t freqHz);
};
