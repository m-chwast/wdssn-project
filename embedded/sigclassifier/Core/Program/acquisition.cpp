#include "acquisition.h"


std::vector<Acquisition*> Acquisition::_acquisitions;


Acquisition::Acquisition(Console& console, TIM_HandleTypeDef& samplingHtim, ADC_HandleTypeDef& hadc)
	: _console{console},
	  _samplingHtim{samplingHtim},
	  _hadc{hadc} {

	_acquisitions.push_back(this);
}

void Acquisition::Init(void) {
	SetSamplingFreq(_initSamplingFreq);

	HAL_ADC_RegisterCallback(&_hadc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, &GeneralADCConvCpltCb);

	_console << "Acquisition init ok\r\n";
}

void Acquisition::Manage(void) {
	if(HAL_GetTick() - _lastManageRunTime < _managePeriodMs) {
		return;
	}

	_lastManageRunTime = HAL_GetTick();

	if(IsAcquisitionInProgress()) {
		return;
	}

	ProcessSamples();

	bool startOk = Start();

	if(startOk == false) {
		_console << "Acquisition error: not started\r\n";
	}
	else {
		_console << "Acquisition started\r\n";
	}
}

bool Acquisition::Start(void) {
	HAL_StatusTypeDef adcStatus = HAL_ADC_Start_DMA(&_hadc, reinterpret_cast<uint32_t*>(_samples.data()), _sampleNo);
	HAL_StatusTypeDef timStatus = HAL_TIM_Base_Start(&_samplingHtim);

	if(adcStatus != HAL_OK || timStatus != HAL_OK) {
		return false;
	}

	_acqInProgress = true;
	return true;
}

uint32_t Acquisition::SetSamplingFreq(uint32_t freqHz) {
	TIM_TypeDef* tim = _samplingHtim.Instance;

	const uint32_t originalFreq = freqHz;
	const uint32_t timFreq = SystemCoreClock / (tim->PSC + 1);

	uint32_t maxFreq = timFreq;
	if(maxFreq > 1000000) {
		maxFreq = 1000000;
	}

	uint32_t minFreq = (timFreq / UINT16_MAX) + 1;

	if(freqHz > maxFreq) {
		freqHz = maxFreq;
	}
	else if(freqHz < minFreq) {
		freqHz = minFreq;
	}

	uint16_t timPulses = timFreq / freqHz;
	tim->ARR = timPulses;

	uint32_t freqObtained = timFreq / timPulses;

	_console << "Acquisition: requested " << originalFreq;
	_console << " Hz, obtained " << freqObtained << " Hz\r\n";

	return freqObtained;
}


void Acquisition::ProcessSamples(void) {
	if(IsAcquisitionInProgress()) {
		return;
	}

	uint16_t minIndex = 0, maxIndex = 0;
	for(uint16_t i = 0; i < _sampleNo; i++) {
		if(_samples[i] < _samples[minIndex]) {
			minIndex = i;
		}
		if(_samples[i] > _samples[maxIndex]) {
			maxIndex = i;
		}
	}

	uint8_t minVal = _samples[minIndex];
	uint8_t maxVal = _samples[maxIndex];
	uint8_t range = maxVal - minVal;

	if(range == 0) {
		return;
	}

	for(uint8_t& sample : _samples) {
		sample -= minVal;
		sample = (uint32_t)sample * 255 / range;
	}
}

void Acquisition::ADCConvCpltCb(void) {
	HAL_ADC_Stop_DMA(&_hadc);
	HAL_TIM_Base_Stop(&_samplingHtim);
	_acqInProgress = false;
}

void Acquisition::GeneralADCConvCpltCb(ADC_HandleTypeDef* hadc) {
	for(Acquisition* a : _acquisitions) {
		if(&a->_hadc == hadc) {
			a->ADCConvCpltCb();
			return;
		}
	}
}

