#include "RdCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

RdCommand::RdCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

RdCommand::~RdCommand()
{

}

void RdCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElementVec = _cmdAnalysis->getCmdElementVec();
	if (0 == cmdElementVec.size())
	{
		cout << "无效的命令" << endl;
		return;
	}

	int tag = 0; //只删除空目录
	vector<string> strVec = _cmdAnalysis->paraAnalysis();
	if (strVec.size() > 1 || (strVec.size() == 1 && strVec[0].compare("/s") != 0))//参数多于1个或者参数为/y
	{
		cout << "无效的参数" << endl;
		return;
	}
	else if (strVec.size() != 0)
	{
		tag = 1;   //删除目录下的所有文件
	}

	vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	for (int i = 0; i < nodeVec.size(); ++i)
	{
		Folder* folder = dynamic_cast<Folder*>(nodeVec[i]);
		if (folder == nullptr)
		{
			SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[i]);
			if (nullptr != link && link->getType() == 2)  //直接移除
			{
				Folder* temp = dynamic_cast<Folder*>(link->getParent());
				temp->removeSubFile(link);
				link->deleteNode(false);
			}
			else
			{
				cout << "系统找不到该文件" << endl;
			}

			continue;
		}
		else
		{
			if (cmdElementVec[i].getIsWildCard())
			{
				cout << "系统找不到该文件" << endl;
				folder->setWildCardStr("*");
				continue;
			}

			if (!folder->isEmpty() && 0 == tag)
			{
				cout << "目录不是空的" << endl;
				folder->setWildCardStr("*");
				return;
			}
			//直接删除
			Folder* parent = dynamic_cast<Folder*>(folder->getParent());
			if (nullptr == parent)
			{
				cout << "不能删除盘符" << endl;
			}
			else
			{
				parent->removeSubFile(folder);
			}

			folder->deleteFolder();
		}
	}
}
