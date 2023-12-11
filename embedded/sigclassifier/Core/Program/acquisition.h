#pragma once

#include "module.h"
#include "console.h"

class Acquisition : public Module {
private:

	Console& _console;

public:

	Acquisition(Console& console)
		: _console{console} {}

	void Init(void) override;
	void Manage(void) override {};
};
