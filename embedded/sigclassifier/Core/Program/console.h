#pragma once

#include <deque>
#include <string>
#include "main.h"


class Console {
private:

	UART_HandleTypeDef& _consoleHuart;

	std::deque<std::string> _messages;

public:

	Console(UART_HandleTypeDef& consoleHuart);

	void Manage(void);

	//operators

	Console& operator<<(const char* msg);
};
