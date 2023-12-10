#include "program.h"
#include "usart.h"
#include "console.h"


class Program {
private:

	Console _console;

public:

	Program(void);

	//to be used by code called from C
	void Init(void);
	void Loop(void);

} program;


Program::Program(void)
	: _console{Console(huart2)} {}

void Program::Init(void) {
	_console.Init();
	_console << "Initializing...\r\n";

	_console << "Initialized\r\n";
}

void Program::Loop(void) {
	_console.Manage();
}


// functions to be called from C main

void Program_Init(void) {
	program.Init();
}

void Program_Loop(void) {
	program.Loop();
}
