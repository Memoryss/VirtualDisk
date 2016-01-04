#include "CdCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

CdCommand::CdCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{
	_cmdAnalysis = cmdAnalysis;
}

CdCommand::~CdCommand()
{

}

void CdCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElementVec = _cmdAnalysis->getCmdElementVec();
	if (1 < cmdElementVec.size())
	{
		cout << "系统找不到指定路径。" << endl;
	}
	else if (cmdElementVec.size() == 0) //无参数显示当前路径
	{
		cout << _curFolder->getCurPath() << endl;
	}
	else if (cmdElementVec[0].getElementStr().compare("/") == 0 || cmdElementVec[0].getElementStr().compare("\\") == 0) //返回根路径
	{
		while(_curFolder->getParent() != nullptr)
		{
			_curFolder = dynamic_cast<Folder*>(_curFolder->getParent());
		}
	}
	else if (cmdElementVec[0].getElementStr().compare("..") == 0)
	{
		if (_curFolder->getParent() != nullptr)
		{
			_curFolder = dynamic_cast<Folder*>(_curFolder->getParent());
		}
	}
	else
	{
		vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
		if (nodeVec.size() != 1)  //cd 只能有一个参数
		{
			//cout << "系统找不到该文件夹" << endl;
			return;
		}

		Folder* folder = dynamic_cast<Folder*>(nodeVec[0]);
		if (nullptr == folder)
		{
			SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[0]);
			if (nullptr != link && 2 == link->getType())
			{
				Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
				while (nullptr == temp)
				{
					SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
					temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
					link = linkTemp;
				}

				_curFolder = temp;
			}
			else
			{
				cout << "无效的参数" << endl;
			}
			return;
		}

		if (cmdElementVec[0].getIsWildCard()) //是否需要通配
		{
			vector<Node*>vec = folder->matchNode();  //查找所有符合通配的node节点
			for (int i = 0; i < vec.size(); ++i)
			{
				Folder* folder = dynamic_cast<Folder*>(vec[i]);
				if (nullptr != folder)
				{
					_curFolder = folder;  //找到一个就ok
					return;
				}
			}

			cout << "系统找不到指定路径" << endl;
		}
		else
		{
			_curFolder = folder;
		}

	}
}
