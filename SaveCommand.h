#pragma once

#include "Command.h"

class Folder;
class TiXmlElement;
class SaveCommand : public Command
{
public:
	SaveCommand(CommandAnalysis* cmdAnalysis);
	~SaveCommand();

	virtual void execute(std::string str);

private:
	TiXmlElement* writeXml(Folder* folder, Folder* parent); 
};