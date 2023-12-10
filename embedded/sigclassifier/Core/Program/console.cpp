#include <cstring>
#include "console.h"


std::vector<Console*> Console::_consoles;


Console::Console(UART_HandleTypeDef& consoleHuart)
	: _consoleHuart{consoleHuart} {

	_consoles.push_back(this);
}

void Console::Init(void) {
	HAL_StatusTypeDef status = HAL_UART_RegisterCallback(&_consoleHuart, HAL_UART_TX_COMPLETE_CB_ID, &GeneralHuartTxCpltCallback);
	(*this) << "Console init " << (status == HAL_OK ? "ok" : "error") << "\r\n";
	_nonBlockingMode = true;
}

void Console::Manage(void) {
	if(_messages.size() == 0) {
		return;
	}

	if(_messageSendComplete == false) {
		return;
	}

	_messageSendComplete = false;

	const std::string& msg = _messages[0];
	const uint8_t* data = reinterpret_cast<const uint8_t*>(msg.c_str());
	HAL_StatusTypeDef status = HAL_UART_Transmit_IT(&_consoleHuart, data, msg.size());

	if(status != HAL_OK) {
		//allow for next try
		_messageSendComplete = true;
		return;
	}

	_messages.pop_front();
}

void Console::HuartTxCpltCallback(void) {
	_messageSendComplete = true;
}


Console& Console::operator<<(const char* msg) {
	if(_nonBlockingMode == true) {
		_messages.push_back(std::string(msg));
	}
	else {
		HAL_UART_Transmit(&_consoleHuart, reinterpret_cast<const uint8_t*>(msg), strlen(msg), HAL_MAX_DELAY);
	}
	return *this;
}


//static methods:

void Console::GeneralHuartTxCpltCallback(UART_HandleTypeDef* huart) {
	for(Console* c : _consoles) {
		if(&c->_consoleHuart == huart) {
			c->HuartTxCpltCallback();
			break;
		}
	}
}
