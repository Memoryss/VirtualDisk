#pragma once

#include "Command.h"

class RdCommand : public Command
{
public:
	RdCommand(CommandAnalysis* cmdAnalysis);
	~RdCommand();

	virtual void execute(std::string str);
};