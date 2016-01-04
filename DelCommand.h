#pragma once

#include "Command.h"

class Folder;
class DelCommand : public Command
{
public:
	DelCommand(CommandAnalysis* cmdAnalysis);
	~DelCommand();

	virtual void execute(std::string str);

private:
	void deleteNode(Node* file);
	void deleteFolder(Folder* folder);

	int _tag;
};