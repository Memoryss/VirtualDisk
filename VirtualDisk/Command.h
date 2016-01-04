#pragma once
//#include "vld.h"
#include "CommandElement.h"
#include <vector>

class Node;
class CommandAnalysis;
class Folder;
class Command
{
public:
	Command(CommandAnalysis* cmdAnalysis);

	virtual ~Command();

	virtual void execute(std::string str) = 0;

	void setCurFolder(Folder* folder);
	Folder* getCurFolder();

	void setRootFolder(Folder* folder);
	Folder* getRootFolder();

protected:
	Folder* _curFolder;
	Folder* _rootFolder;

	std::string _cmdStr;

	CommandAnalysis* _cmdAnalysis;
};