#pragma once

#include <deque>
#include <vector>
#include <string>
#include "main.h"


class Console {
private:

	UART_HandleTypeDef& _consoleHuart;

	std::deque<std::string> _messages;
	bool _messageSendComplete = true;

	static std::vector<Console*> _consoles;

	static void GeneralHuartTxCpltCallback(UART_HandleTypeDef* huart);
	void HuartTxCpltCallback(void);

public:

	Console(UART_HandleTypeDef& consoleHuart);

	void Manage(void);

	//operators

	Console& operator<<(const char* msg);
};
