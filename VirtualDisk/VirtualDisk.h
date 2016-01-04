#pragma once

#include <vector>

class CommandFactory;
class Folder;
class Command;
class VirtualDisk
{
public:
	VirtualDisk();
	~VirtualDisk();

	void loadFile();

	bool executeCmd(std::string str);

	std::string getCurPath();

private:
	Command* _cmd;
	CommandFactory* _cmdFactory;

	std::string _cmdStr;

	Folder* _curFolder;
	Folder* _rootFolder;
};