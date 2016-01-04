#pragma once

#include <string>

class Command;
class CommandAnalysis;
class CommandFactory
{
public:
	CommandFactory();
	~CommandFactory();

	Command* createCmd(std::string cmdStr);

private:
	int _cmdType;

	CommandAnalysis* _cmdAnalysis;
};