#pragma once

#include "Command.h"

class ExitCommand : public Command
{
public:
	ExitCommand(CommandAnalysis* cmdAnalysis);
	~ExitCommand();

	virtual void execute(std::string str);
};