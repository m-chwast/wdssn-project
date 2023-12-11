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


	Start();
	_console << "Acquisition init ok\r\n";
}

void Acquisition::Start(void) {
	HAL_StatusTypeDef adcStatus = HAL_ADC_Start_DMA(&_hadc, reinterpret_cast<uint32_t*>(_samples.data()), _sampleNo);
	HAL_StatusTypeDef timStatus = HAL_TIM_Base_Start(&_samplingHtim);

	if(adcStatus != HAL_OK || timStatus != HAL_OK) {
		_console << "Acquisition error: not started\r\n";
	}
	else {
		_console << "Acquisition started\r\n";
	}
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

void Acquisition::ADCConvCpltCb(void) {
	HAL_ADC_Stop_DMA(&_hadc);
	HAL_TIM_Base_Stop(&_samplingHtim);
}

void Acquisition::GeneralADCConvCpltCb(ADC_HandleTypeDef* hadc) {
	for(Acquisition* a : _acquisitions) {
		if(&a->_hadc == hadc) {
			a->ADCConvCpltCb();
			return;
		}
	}
}

