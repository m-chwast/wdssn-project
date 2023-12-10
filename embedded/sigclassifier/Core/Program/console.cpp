#include "console.h"


std::vector<Console*> Console::_consoles;


Console::Console(UART_HandleTypeDef& consoleHuart)
	: _consoleHuart{consoleHuart} {

	_consoles.push_back(this);
	HAL_UART_RegisterCallback(&consoleHuart, HAL_UART_TX_COMPLETE_CB_ID, &GeneralHuartTxCpltCallback);
}

void Console::Manage(void) {
	if(_messages.size() == 0) {
		return;
	}

	const std::string& msg = _messages[0];
	const uint8_t* data = reinterpret_cast<const uint8_t*>(msg.c_str());
	HAL_UART_Transmit(&_consoleHuart, data, msg.size(), HAL_MAX_DELAY);

	_messages.pop_front();
}

void Console::HuartTxCpltCallback(void) {

}


Console& Console::operator<<(const char* msg) {
	_messages.push_back(std::string(msg));
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
