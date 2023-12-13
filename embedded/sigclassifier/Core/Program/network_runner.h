#pragma once

#include "module.h"
#include "console.h"


class NetworkRunner : public Module {
private:

	Console& _console;

public:

	NetworkRunner(Console& console);

	virtual void Init(void) override;
	virtual void Manage(void) override;
};
