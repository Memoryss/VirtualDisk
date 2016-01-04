#include "MoveCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

MoveCommand::MoveCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::execute( std::string str )
{
	_cmdStr = str;

	_tag = 0;//同名文件或文件夹不覆盖
	vector<string> strVec = _cmdAnalysis->paraAnalysis();
	if (2 <= strVec.size() || 1 == strVec.size() && strVec[0].compare("/y") != 0)
	{
		cout << "命令格式不正确" << endl;
		return;
	}
	else if (1 == strVec.size() && strVec[0].compare("/y") == 0)
	{
		_tag = 1; //同名文件覆盖
	}

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (2 != cmdElement.size() || cmdElement[1].getIsWildCard())
	{
		cout << "命令格式不正确" << endl;
		return;
	}

	vector<Node*> dealNodeVec;
	vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	if (2 != nodeVec.size())
	{
		return;
	}

	if (cmdElement[0].getIsWildCard())
	{
		Folder* folder = dynamic_cast<Folder*>(nodeVec[0]);
		dealNodeVec = folder->matchNode();
	}
	else
	{
		dealNodeVec.push_back(nodeVec[0]);
	}

	Folder* tarFolder = dynamic_cast<Folder*>(nodeVec[1]);
	if (dealNodeVec.size() == 0)
	{
		cout << "找不到文件" << endl;
		return;
	}
	else if (nullptr == tarFolder && dealNodeVec.size() > 1)  //多个文件不能移动到单个文件中
	{
		SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[1]);
		if (link != nullptr && 2 == link->getType())
		{
			Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
			while (nullptr == temp)
			{
				SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
				temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
				link = linkTemp;
			}
			
			for (int i = 0; i < dealNodeVec.size(); ++i)
			{
				moveNode(dealNodeVec[i], temp, true);
			}
			return;
		}

		cout << "无法将多个文件移动到单个文件中" << endl;
		return;
	}
	else if (nullptr == tarFolder && dealNodeVec.size() == 1)  //单个文件移动到单个文件中 （即 覆盖源文件）
	{
		SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[1]);
		if (link != nullptr && 2 == link->getType())
		{
			Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
			while (nullptr == temp)
			{
				SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
				temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
				link = linkTemp;
			}

			if (cmdElement[0].getIsWildCard())
			{
				moveNode(dealNodeVec[0], temp, true);
			}
			else
			{
				moveNode(dealNodeVec[0], temp, false);
			}
		}
		else if (link == nullptr)
		{
			Folder* temp = dynamic_cast<Folder*>(nodeVec[1]->getParent());
			if (cmdElement[0].getIsWildCard())
			{
				moveNode(dealNodeVec[0], temp, true);
			}
			else
			{
				moveNode(dealNodeVec[0], temp, false);
			}
		}
		else
		{
			cout << "无法移动该文件" << endl;
		}		
	}
	else if(nullptr != tarFolder)  //移动到文件夹中（只能支持文件通配符， 文件夹通配符不支持）
	{
		if (cmdElement[0].getIsWildCard())
		{
			for (int i = 0; i < dealNodeVec.size(); ++i)
			{
				moveNode(dealNodeVec[i], tarFolder, true);
			}
		}
		else
		{
			for (int i = 0; i < dealNodeVec.size(); ++i)
			{
				moveNode(dealNodeVec[i], tarFolder, false);
			}
		}
	}
}

void MoveCommand::moveNode( Node* node, Folder* folder, bool isWildCard )
{
	if (true == isWildCard)
	{
		if (dynamic_cast<Folder*>(node) != nullptr)
		{
			return;
		}
		SymbolicLink* link = dynamic_cast<SymbolicLink*>(node);
		if (nullptr != link && 2 == link->getType())
		{
			return;
		}
	}
	
	Node* temp = folder->containNode(node->getName());
	if (nullptr == temp)
	{
		node->setParent(folder);
	}
	else
	{
		if (_tag == 0)
		{
			cout << "不能移动该文件" << endl;
			return;
		}

		folder->setWildCardStr(node->getName());
		folder->deleteNode(false);

		node->setParent(folder);
	}
}
