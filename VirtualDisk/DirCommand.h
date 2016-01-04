#pragma once


#include "Command.h"

class DirCommand : public Command
{
public:
	DirCommand(CommandAnalysis* cmdAnalysis);
	~DirCommand();

	virtual void execute(std::string str);
};