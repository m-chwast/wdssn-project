#include "program.h"
#include "usart.h"
#include "tim.h"
#include "adc.h"
#include "console.h"
#include "acquisition.h"


class Program {
private:

	std::vector<Module*> _modules;

	Console _console;
	Acquisition _acquisition;

public:

	Program(void);

	//to be used by code called from C
	void Init(void);
	void Loop(void);

} program;


Program::Program(void)
	: _console{Console(huart2)},
	  _acquisition{Acquisition(_console, htim6, hadc)} {

    _modules.push_back(&_acquisition);
	//console has to be pushed back last to allow for blocking sending during modules init
	_modules.push_back(&_console);
}

void Program::Init(void) {
	uint32_t timeStart = HAL_GetTick();
	_console << "Initializing...\r\n";
	for(Module* m : _modules) {
		m->Init();
	}
	uint32_t timeElapsed = HAL_GetTick() - timeStart;
	_console << "Initialized in " << timeElapsed << " ms\r\n";
}

void Program::Loop(void) {
	for(Module* m : _modules) {
		m->Manage();
	}
}


// functions to be called from C main

void Program_Init(void) {
	program.Init();
}

void Program_Loop(void) {
	program.Loop();
}
