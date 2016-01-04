#pragma once

#include "Command.h"

class CdCommand : public Command
{
public:
	CdCommand(CommandAnalysis* cmdAnalysis);
	~CdCommand();

	virtual void execute(std::string str);
};