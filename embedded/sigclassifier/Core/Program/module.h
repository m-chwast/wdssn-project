#pragma once

class Module {
public:

	virtual void Init(void) {};
	virtual void Manage(void) = 0;
};
