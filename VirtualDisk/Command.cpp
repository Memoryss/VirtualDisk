#include "Command.h"
#include "CommandElement.h"
#include "Folder.h"
#include "CommandAnalysis.h"



using namespace std;

Command::Command(CommandAnalysis* cmdAnalysis)
{
	_cmdAnalysis = cmdAnalysis;
}


Command::~Command()
{

}


void Command::setCurFolder(Folder* folder)
{
	_curFolder = folder;
}

void Command::execute(string cmdstr)
{
	/*
	_cmdStr = cmdstr;
	int tag = _cmdAnalysis->cmdTypeAnalysis(_cmdStr);
	switch (tag)
	{
	case 0:
		dirCommand();
		break;
	case 1:
		cdCommand();
		break;
	case 2:
		mdCommand();
		break;
	case 3:
		break;
	case 4:
		delCommand();
		break;
	case 5: 
		rdCommand();
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		mkCommand();
		break;
	case 10:
		break;
	default:
		cout << "无效的命令" << endl;
		break;
	}*/
}

void Command::setRootFolder( Folder* folder )
{
	_rootFolder = folder;
}

Folder* Command::getCurFolder()
{
	return _curFolder;
}

Folder* Command::getRootFolder()
{
	return _rootFolder;
}

/*
void Command::renCommand()
{

	if (2 != _cmdElement.size())
	{
		cout << "命令格式不正确" << endl;
		return;
	}

	splitPath(_cmdElement[0].getElementStr());
	if (!pathAnalysis())
	{
		cout << "找不到指定文件" << endl;
		return;
	}
	else
	{
		Node* node = _folder->containNode(_fileNameVec[_fileNameVec.size() - 1]);
		if (_folder->containNode(_cmdElement[1].getElementStr()) != nullptr)
		{
			cout << "该目录下已有重名文件" << endl;
		}
		node->setName(_cmdElement[1].getElementStr());
	}
}

void Command::mkCommand()
{
	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (cmdElement.size() != 2)
	{
		cout << "命令语法不正确" << endl;
		return;
	}

	vector<string> symPathVec, srcPathVec;

	Folder* symStartFolder = _cmdAnalysis->splitPath(cmdElement[0].getElementStr(), _rootFolderm, _curFolder);
	Node* node = _cmdAnalysis->pathMatch();
	symPathVec = _cmdAnalysis->getFileNameVec();
	if (nullptr != node)
	{
		cout << "当文件已经存在时，无法创建该文件" << endl;
		return;
	}
	else
	{
		if (symPathVec !=)
		{
		}
	}

	Folder srcStartFolder = _cmdAnalysis->splitPath(cmdElement[1].getElementStr(), _rootFolder, _curFolder);
	srcPathVec = _cmdAnalysis->getFileNameVec();
}*/