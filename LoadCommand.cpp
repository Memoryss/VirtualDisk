#include "LoadCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "File.h"
#include "SymbolicLink.h"

#include <stdlib.h>
#include "tinyxml.h"

using namespace std;

LoadCommand::LoadCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

LoadCommand::~LoadCommand()
{

}

void LoadCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (cmdElement.size() != 1)
	{
		cout << "命令格式不正确" << endl;
		return;
	}

	string loadName = cmdElement[0].getElementStr();
	TiXmlDocument doc;
	if (!doc.LoadFile(loadName.c_str()))
	{
		cout << "不能解析指定的文件" << endl;
		return;
	}

	destoryDisk();  //销毁之前的虚拟磁盘

	TiXmlElement* rootElment = doc.RootElement();  //"virtualdisk"
	TiXmlAttribute* vdAttribute = rootElment->FirstAttribute();
	int tag = atoi(vdAttribute->Value());  //"tag"

	TiXmlElement* folderElement = rootElment->FirstChildElement();  //"c:"
	_rootFolder = dynamic_cast<Folder*>(readXML(folderElement, nullptr));
	_curFolder = _rootFolder;

	rebuildLink();
}

Node* LoadCommand::readXML( TiXmlElement* element, Folder* parent)
{
	Node* node;

	if (strcmp(element->Value(), "Folder") == 0)
	{
		node = new Folder;
	}
	else if (strcmp(element->Value(), "File") == 0)
	{
		node = new File;
	}
	else //对于符号链接 特殊处理
	{
		string name, pathName;
		TiXmlAttribute* attribute = element->FirstAttribute();
		for (; attribute != nullptr; attribute = attribute->Next())  //处理属性
		{
			if (strcmp(attribute->Name(), "name") == 0)
			{
				const char* str = attribute->Value();
				while(*str != '\0')
				{
					name.push_back(*str);
					str++;
				}
			}
			else if (strcmp(attribute->Name(), "LinkPath") == 0)
			{
				const char* str = attribute->Value();
				while(*str != '\0')
				{
					pathName.push_back(*str);
					str++;
				}
			}
		}

		_links.push_back(Link(parent->getDate(), name, pathName, parent));
		return nullptr;
	}

	TiXmlAttribute* attribute = element->FirstAttribute();
	for (; attribute != nullptr; attribute = attribute->Next())  //处理属性
	{
		if (strcmp(attribute->Name(), "name") == 0)
		{
			node->setName(attribute->Value());
		}
		
		node->setParent(parent);
	}

	TiXmlElement* childElemet = element->FirstChildElement();
	for (; childElemet != nullptr; childElemet = childElemet->NextSiblingElement())  //处理子节点
	{
		if (strcmp(childElemet->Value(), "Date") == 0)
		{
			vector<int> nums;
			TiXmlAttribute* dateAttribute = childElemet->FirstAttribute();
			for (; dateAttribute != nullptr; dateAttribute = dateAttribute->Next())
			{
				nums.push_back(atoi(dateAttribute->Value()));
			}

			Date date(nums[0], nums[1], nums[2], nums[3], nums[4]);
			node->setDate(date);
		}
		else if (strcmp(childElemet->Value(), "Children") == 0)
		{
			TiXmlElement* tempElement = childElemet->FirstChildElement(); 
			for (; tempElement != nullptr; tempElement = tempElement->NextSiblingElement())
			{
				Folder* tempFolder = dynamic_cast<Folder*>(node);
				readXML(tempElement, tempFolder);
			}
		}
		else if (strcmp(childElemet->Value(), "Content") == 0)
		{
			vector<char> strVec;
			const char* str = childElemet->FirstAttribute()->Value();
			while(*str != '\0')
			{
				strVec.push_back(*str);
				str++;
			}

			File* file = dynamic_cast<File*>(node);
			file->setContent(strVec);
		}
	}

	return node;
}

void LoadCommand::destoryDisk()
{
	_rootFolder->setWildCardStr("*");
	_rootFolder->deleteFolder();
}

void LoadCommand::rebuildLink()
{
	for (int i = 0; i < _links.size(); ++i)
	{
		string pathName = _links[i].pathName;
		_cmdAnalysis->splitPath(pathName, _rootFolder, _curFolder);
		Node* node = _cmdAnalysis->pathMatch();

		SymbolicLink* link = new SymbolicLink(_links[i].name, pathName, node);
		link->setDate(_links[i].date);
		link->setParent(_links[i].parent);

		Subject* subject = dynamic_cast<Subject*>(node);
		subject->addObserver(link);
	}
}
