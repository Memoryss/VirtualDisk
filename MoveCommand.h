#pragma once

#include "Command.h"

class Folder;
class MoveCommand : public Command
{
public:
	MoveCommand(CommandAnalysis* cmdAnalysis);
	~MoveCommand();

	virtual void execute(std::string str);

private:
	void moveNode(Node* node, Folder* folder, bool isWildCard);

private:
	int _tag;
};