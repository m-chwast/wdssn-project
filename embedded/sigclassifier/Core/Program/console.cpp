#include "console.h"

Console::Console(UART_HandleTypeDef& consoleHuart)
	: _consoleHuart{consoleHuart} {

}

Console& Console::operator<<(const char* msg) {
	_messages.push_back(std::string(msg));
	return *this;
}
