#pragma once

#include <vector>
#include "CommandElement.h"

class Node;
class Folder;
class CommandAnalysis
{
public:
	CommandAnalysis();
	~CommandAnalysis();

	int cmdTypeAnalysis(std::string cmdStr);

	std::vector<Node*> pathAnalysis(Folder* rootFolder, Folder* curFolder);

	std::vector<std::string> paraAnalysis();

	Folder* splitPath(std::string path, Folder* rootFolder, Folder* curFolder);

	void splitCmd(std::string cmdStr);

	std::vector<CommandElement>& getCmdElementVec();

	std::vector<std::string> getFileNameVec();

	Node* pathMatch();

	Folder* getLastFolder();  //·µ»Ø_folder;

private:
	std::vector<CommandElement> _cmdElementVec;
	std::vector<std::string> _fileNameVec;
	Folder* _folder;

	int _cmdType;
};