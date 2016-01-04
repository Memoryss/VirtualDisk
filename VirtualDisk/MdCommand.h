#pragma once

#include "Command.h"

class MdCommand : public Command
{
public:
	MdCommand(CommandAnalysis* cmdAnalysis);
	~MdCommand();

	virtual void execute(std::string str);
};