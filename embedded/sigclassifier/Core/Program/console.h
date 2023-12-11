#pragma once

#include <deque>
#include <vector>
#include <string>
#include "main.h"
#include "module.h"


class Console : public Module {
private:

	UART_HandleTypeDef& _consoleHuart;

	bool _nonBlockingMode = false;	//until initialized, send messages in blocking mode

	std::deque<std::string> _messages;
	bool _messageSendComplete = true;

	static std::vector<Console*> _consoles;

	static void GeneralHuartTxCpltCallback(UART_HandleTypeDef* huart);
	void HuartTxCpltCallback(void);

public:

	Console(UART_HandleTypeDef& consoleHuart);

	void Init(void) override;
	void Manage(void) override;

	//operators

	Console& operator<<(const char* msg);
	Console& operator<<(int32_t val);
};
