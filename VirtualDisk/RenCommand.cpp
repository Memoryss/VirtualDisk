#include "RenCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

#include <new>

using namespace std;

RenCommand::RenCommand( CommandAnalysis* cmdAnasis ):
Command(cmdAnasis)
{

}

RenCommand::~RenCommand()
{

}

void RenCommand::execute( std::string str )
{
	_cmdStr = str;
	
	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (2 != cmdElement.size())
	{
		cout << "命令语法不正确" << endl;
		return;
	}

	string targetName = cmdElement[1].getElementStr();
	cmdElement.erase(++(cmdElement.begin()));

	vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	if (0 == nodeVec.size())
	{
		cout << "系统找不到指定文件" << endl;
		return;
	}

	if (cmdElement[0].getIsWildCard()) // 通配符对文件夹不适用
	{
		Folder* folder = dynamic_cast<Folder*>(nodeVec[0]);
		vector<Node*> nodes = folder->matchNode();
		if (0 == nodes.size())
		{
			cout << "系统找不到指定文件" << endl;
			return;
		}

		for (int i = 0; i < nodes.size(); ++i)
		{
			Folder* file = dynamic_cast<Folder*>(nodes[i]);
			if (nullptr == file)
			{
				SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodes[i]);
				if (nullptr != link && link->getType() == 1)
				{
					_renNode.push_back(link);
				}
				else if(nullptr == link)
				{
					_renNode.push_back(nodes[i]);
				}
			}
		}
	}
	else
	{
		_renNode.push_back(nodeVec[0]);
	}

	rename(targetName);
}

bool RenCommand::matchString( char* pat, char* str )
{
	char *s = NULL;
	char *p = NULL;
	bool star = false;
	bool bBreak = false;
	do
	{
		bBreak = false;
		for (s = str, p = pat; *s; ++s, ++p)
		{
			switch (*p)
			{
			case '?':
				break;
			case '*':
				star = true;
				str = s;
				pat = p;
				if (!*++pat)
					return true;
				bBreak = true;
				break;
			default:
				if (*s != *p)
				{
					if (!star)
					{
						*s = *p;
						if (matchString(p, s))
						{
							return true;
						}
					}
					str++;
					bBreak = true;
				}
				break;
			}
			if (bBreak)
				break;
		}
		if (bBreak == false)
		{
			if (*p == '*')
				++p;
			return (!*p);
		}
	} while (true);
}

void RenCommand::rename( string targetName )
{
	char* des =  const_cast<char*>(targetName.c_str());
	targetName.copy(des, targetName.size(), 0);
	for (int i = 0; i < _renNode.size(); ++i)
	{

		string srcStr = _renNode[i]->getName();
		char* src = const_cast<char*>(srcStr.c_str());
		
		//srcStr.copy(src, srcStr.size(), 0);

		matchString(des, src);
		srcStr = src;

		Folder* folder = dynamic_cast<Folder*>(_renNode[i]->getParent());
		if (nullptr != folder)
		{
			if ( nullptr != folder->containNode(srcStr))
			{
				cout << "存在一个重名文件" << endl;
				continue;
			}

			_renNode[i]->setName(srcStr);
		}
	}
}
