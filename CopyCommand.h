#pragma once

#include "Command.h"

class SymbolicLink;
class File;
class CopyCommand : public Command
{
public:
	CopyCommand(CommandAnalysis* cmdAnalysis);
	~CopyCommand();

	virtual void execute(std::string str);

private:
	void copyFile(Node* targetNode, std::string name);
	void copy( File* srcNode, Node* tarNode, std::string name );

	File* loadFile(std::string path);
	File* switchFile( Node* node );

	bool matchString( char* pat, char* str );

private:
	std::vector<Node*> _copyNode;
	std::vector<File*> _copyFile;

	File* _diskFile;
};