#pragma once

#include "Command.h"
#include "Date.h"
#include <map>

struct Link
{
	Link(Date da, std::string na, std::string pa, Folder* par):date(da), name(na), pathName(pa), parent(par){}

	Date date;
	std::string name;
	std::string pathName;
	Folder* parent;
};

class TiXmlElement;
class Folder;
class LoadCommand : public Command
{
public:
	LoadCommand(CommandAnalysis* cmdAnalysis);
	~LoadCommand();

	virtual void execute(std::string str);

private:
	Node* readXML(TiXmlElement* element, Folder* parent);

	void destoryDisk();

	void rebuildLink();

private:
	std::vector<Link> _links;
};