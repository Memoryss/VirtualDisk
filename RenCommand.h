#pragma once

#include "Command.h"

class RenCommand : public Command
{
public:
	RenCommand(CommandAnalysis* cmdAnasis);
	~RenCommand();

	virtual void execute(std::string str);

private:
	void rename(std::string targetName);
	
	bool matchString(char* desStr, char* srcStr);

private:
	std::vector<Node*> _renNode;

};