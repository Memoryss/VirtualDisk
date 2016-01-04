#pragma once

#include "Command.h"

class MkCommand : public Command
{
public:
	MkCommand(CommandAnalysis* cmdAnalysis);
	~MkCommand();

	virtual void execute(std::string str);
};