#include "console.h"


Console::Console(UART_HandleTypeDef& consoleHuart)
	: _consoleHuart{consoleHuart} {

	HAL_UART_RegisterCallback(&consoleHuart, HAL_UART_TX_COMPLETE_CB_ID, &Console_GeneralHuartTxCpltCallback);
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


Console& Console::operator<<(const char* msg) {
	_messages.push_back(std::string(msg));
	return *this;
}


//static methods:

void Console::Console_GeneralHuartTxCpltCallback(UART_HandleTypeDef* huart) {

}
