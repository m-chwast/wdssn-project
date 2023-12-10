#pragma once

#include "main.h"


class Console {
private:

	UART_HandleTypeDef& _consoleHuart;

public:

	Console(UART_HandleTypeDef& consoleHuart);
};
