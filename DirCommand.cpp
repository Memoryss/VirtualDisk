#include "DirCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

DirCommand::DirCommand(CommandAnalysis* cmdAnalysis):
Command(cmdAnalysis)
{
	
}


DirCommand::~DirCommand()
{
	
}

void DirCommand::execute( std::string str )
{
	_cmdStr = str;

	int tag = 0;
	vector<string> strVec = _cmdAnalysis->paraAnalysis();
	if (strVec.size() > 1 || (strVec.size() == 1 && strVec[0].compare("/y") == 0))//参数多于1个或者参数为/y
	{
		cout << "无效的参数" << endl;
		return;
	}
	else if (strVec.size() == 1 && strVec[0].compare("/ad") == 0)
	{
		tag = 1;
	}
	else if (strVec.size() == 1 && strVec[0].compare("/s") == 0)
	{
		tag = 2;
	}

	vector<Node*> dealNodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	for (int i = 0; i < dealNodeVec.size(); ++i)
	{
		Folder* folder = dynamic_cast<Folder*>(dealNodeVec[i]);
		if (nullptr == folder)
		{
			SymbolicLink* link = dynamic_cast<SymbolicLink*>(dealNodeVec[i]);
			if (nullptr != link && 2 == link->getType())
			{
				Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
				while (nullptr == temp)
				{
					SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
					temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
					link = linkTemp;
				}

				folder = temp;
			}
			else
			{
				cout << "无效的参数" << endl;
				continue;
			}
		}

		cout << "  " << folder->getCurPath() << "的目录" << endl;
		switch (tag)
		{
		case 0:
			folder->showSubFile(false, false);
			break;
		case 1:
			folder->showSubFile(false, true);
			break;
		case 2:
			folder->showSubFile(true, false);
			break;
		}
	}
}
