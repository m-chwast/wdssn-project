#include "program.h"
#include "usart.h"
#include "console.h"


class Program {
private:

	Console _console;

public:

	Program(void);

} program;

Program::Program(void)
	: _console{Console(huart2)} {

}



// functions to be called from C main

void Program_Init(void) {

}

void Program_Loop(void) {

}
