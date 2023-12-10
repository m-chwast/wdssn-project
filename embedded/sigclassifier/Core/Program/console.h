#pragma once

#include <vector>
#include <string>
#include "main.h"


class Console {
private:

	UART_HandleTypeDef& _consoleHuart;

	std::vector<std::string> _messages;

public:

	Console(UART_HandleTypeDef& consoleHuart);

	//operators

	Console& operator<<(const char* msg);
};
