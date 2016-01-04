#include "SaveCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "File.h"
#include "SymbolicLink.h"
#include <direct.h>

#include "tinyxml.h"

using namespace std;

SaveCommand::SaveCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

SaveCommand::~SaveCommand()
{

}

void SaveCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (cmdElement.size() != 1)
	{
		cout << "命令格式不正确" << endl;
		return;
	}

	string saveName = cmdElement[0].getElementStr();
	if (saveName.find(".") == string::npos)  //以xml文件形式保存
	{
		saveName.append(".xml");
	}
	else
	{
		string typeStr(saveName, saveName.find(".")+1);
		if (typeStr.compare("xml") != 0)
		{
			int a = saveName.find(".")+1;
			int b = saveName.size() - saveName.find(".") - 1;
			saveName.replace(a, b, string("xml"));
		}
	}

	FILE* file = fopen(saveName.c_str(),"w");
	if (nullptr == file)
	{
		cout << "无效的路径" << endl;
		return;
	}
	else
	{
		fclose(file);

		if (saveName.find(":") == string::npos)
		{
			char path[100];
			if (nullptr != _getcwd(path, 100))
			{
				cout << "文件保存到" << path << endl;
			}
		}
		else
		{
			cout << "文件保存到" << saveName << endl;
		}
	}

	TiXmlDocument doc;
	TiXmlDeclaration *declare = new TiXmlDeclaration("1.0", "","");
	TiXmlElement* virutualDisk = new TiXmlElement("VirtualDisk");
	virutualDisk->SetAttribute("tag", _rootFolder->_tag);

	TiXmlElement* disk = writeXml(_rootFolder, nullptr);
	
	virutualDisk->LinkEndChild(disk);
	doc.LinkEndChild(declare);
	doc.LinkEndChild(virutualDisk);
	doc.SaveFile(saveName.c_str());
	
}

TiXmlElement* SaveCommand::writeXml( Folder* folder, Folder* parent )
{
	folder->setWildCardStr("*");
	vector<Node*> nodeVec = folder->matchNode();

	TiXmlElement* rootElement = new TiXmlElement("Folder");
	rootElement->SetAttribute("name", folder->getName().c_str());
	if (nullptr != parent)
	{
		rootElement->SetAttribute("parent", parent->getCurPath().c_str());
	}
	else
	{
		rootElement->SetAttribute("parent", "nullptr");
	}

	TiXmlElement* date = new TiXmlElement("Date");
	date->SetAttribute("Year", folder->getDate()._year);
	date->SetAttribute("Month", folder->getDate()._month);
	date->SetAttribute("Day", folder->getDate()._day);
	date->SetAttribute("Hour", folder->getDate()._hour);
	date->SetAttribute("Minute", folder->getDate()._minute);
	rootElement->LinkEndChild(date);

	TiXmlElement* children = new TiXmlElement("Children");
	children->SetAttribute("nums", nodeVec.size());
	for (int i = 0; i < nodeVec.size(); ++i)
	{
		Folder* tempFolder = dynamic_cast<Folder*>(nodeVec[i]);
		File* tempFile = dynamic_cast<File*>(nodeVec[i]);
		SymbolicLink* tempLink = dynamic_cast<SymbolicLink*>(nodeVec[i]);

		TiXmlElement* tempElement;
		if (nullptr != tempFolder)
		{
			tempElement = writeXml(tempFolder, folder);
		}
		else if (nullptr != tempFile)
		{
			tempElement = new TiXmlElement("File");
			tempElement->SetAttribute("name", tempFile->getName().c_str());
			tempElement->SetAttribute("parent", folder->getCurPath().c_str());

			TiXmlElement* date = new TiXmlElement("Date");
			date->SetAttribute("Year", tempFile->getDate()._year);
			date->SetAttribute("Month", tempFile->getDate()._month);
			date->SetAttribute("Day", tempFile->getDate()._day);
			date->SetAttribute("Hour", tempFile->getDate()._hour);
			date->SetAttribute("Minute", tempFile->getDate()._minute);
			tempElement->LinkEndChild(date);

			TiXmlElement* content = new TiXmlElement("Content");
			string contentStr(tempFile->getContent().begin(), tempFile->getContent().end());
			content->SetAttribute("Content", contentStr.c_str());
			tempElement->LinkEndChild(content);
		}
		else
		{
			tempElement = new TiXmlElement("SymbolicLink");
			tempElement->SetAttribute("name", tempLink->getName().c_str());
			tempElement->SetAttribute("parent", folder->getCurPath().c_str());
			tempElement->SetAttribute("LinkPath", tempLink->getLinkPath().c_str());

			TiXmlElement* date = new TiXmlElement("Date");
			date->SetAttribute("Year", tempLink->getDate()._year);
			date->SetAttribute("Month", tempLink->getDate()._month);
			date->SetAttribute("Day", tempLink->getDate()._day);
			date->SetAttribute("Hour", tempLink->getDate()._hour);
			date->SetAttribute("Minute", tempLink->getDate()._minute);
			tempElement->LinkEndChild(date);
		}

		children->LinkEndChild(tempElement);
	}
	rootElement->LinkEndChild(children);

	return rootElement;
}
