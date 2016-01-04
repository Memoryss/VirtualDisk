#include "CopyCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "File.h"
#include "SymbolicLink.h"
#include "ZBase64.h"

#include <Windows.h>
#include <fstream>
using namespace std;

CopyCommand::CopyCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{
	_diskFile = nullptr;
}

CopyCommand::~CopyCommand()
{
	if (nullptr != _diskFile)
	{
		delete _diskFile;
	}
}

void CopyCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (cmdElement.size() != 2)
	{
		cout << "命令格式不正确" << endl;
		return;
	}

	CommandElement srcElement = cmdElement[0];
	CommandElement desElement = cmdElement[1];

	if (srcElement.getElementStr().find('@') == 0)   //真正磁盘
	{
		string path = srcElement.getElementStr();
		path.erase(path.begin());

		_cmdAnalysis->splitPath(path, _rootFolder, _curFolder);
		vector<string> strVec = _cmdAnalysis->getFileNameVec();

		_diskFile = loadFile(path);
		if (nullptr == _diskFile)
		{
			cout << "打不开指定的磁盘文件" << endl;
			return;
		}

		_diskFile->setName(strVec[strVec.size() - 1]);
		_copyNode.push_back(_diskFile);
	}
	else
	{
		cmdElement.clear();
		cmdElement.push_back(srcElement);
		vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
		if (0 == nodeVec.size())
		{
			cout << "系统找不到指定文件" << endl;
			return;
		}

		Folder* copyFolder = nullptr;
		Folder* folder = dynamic_cast<Folder*>(nodeVec[0]);
		if (srcElement.getIsWildCard())  //通配符
		{
			vector<Node*> nodes = folder->matchNode();
			_copyNode.insert(_copyNode.begin(), nodes.begin(), nodes.end());
		}
		else
		{
			if (nullptr == folder) 
			{
				SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[0]);   //复制符号链接
				if (nullptr != link && 2 == link->getType())
				{
					Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
					while (nullptr == temp)
					{
						SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
						temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
						link = linkTemp;
					}

					copyFolder = temp;
				}
				else
				{
					_copyNode.push_back(nodeVec[0]);
				}
				
			}
			else  //复制文件夹
			{
				copyFolder = folder;
			}
		}

		if (nullptr != copyFolder)  //把复制的文件夹转化为复制的文件
		{
			copyFolder->setWildCardStr("*");
			vector<Node*> nodes = copyFolder->matchNode();
			_copyNode.insert(_copyNode.begin(), nodes.begin(), nodes.end());
		}
	}

	Node* targetNode;
	string name;
	cmdElement.clear();
	cmdElement.push_back(desElement);
	vector<Node*> desNodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	vector<string> fileName = _cmdAnalysis->getFileNameVec();
	if (0 == desNodeVec.size() && fileName.size() > 1)
	{
		cout << "系统找不到指定路径" << endl;
		return;
	}
	else
	{
		if (desElement.getIsWildCard())  //通配符
		{
			targetNode = desNodeVec[0];
			name = desElement.getElementStr();
		}
		else
		{
			if (desNodeVec.size() == 0)  //不存在的文件  需要新建文件
			{
				targetNode = nullptr;
				name = desElement.getElementStr();
			}
			else
			{
				targetNode = desNodeVec[0];  //文件夹
			}
		}
	}

	copyFile(targetNode, name);
}

void CopyCommand::copyFile( Node* targetNode, std::string name )
{
	vector<Node*>::iterator iter = _copyNode.begin();
	for (iter; iter != _copyNode.end(); ++iter)
	{
		File* temp = switchFile(*iter);
		if (nullptr != temp)
		{
			_copyFile.push_back(temp);
		}
	}
	_copyNode.clear();


	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	Date date(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

	if (nullptr == targetNode)
	{
		File* file = new File(name);
		file->setDate(date);
		file->setParent(_cmdAnalysis->getLastFolder());

		for (int i = 0; i < _copyFile.size(); ++i)
		{
			file->write(_copyFile[i]);
		}
	}
	else
	{
		for (int i = 0; i < _copyFile.size(); ++i)
		{
			copy(_copyFile[i], targetNode, name);
		}
	}
}

void CopyCommand::copy( File* srcFile, Node* tarNode, std::string name )
{
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	Date date(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

	Folder* tarFolder = dynamic_cast<Folder*>(tarNode);
	File* tarFile = dynamic_cast<File*>(tarNode);
	SymbolicLink* tarlink = dynamic_cast<SymbolicLink*>(tarNode);

	if (nullptr != tarFolder)  //如果是文件夹 表明两种情况 一种是通配符 另一种是拷贝到文件夹中
	{
		if (0 != name.size())  //通配符
		{
			string newName, lastName;

			char* des = const_cast<char*>(name.c_str());
			lastName = srcFile->getName();
			char* src = const_cast<char*>(lastName.c_str());
			matchString(des, src);
			newName = src;

			Node* node = tarFolder->containNode(newName);
			if (nullptr != node)
			{
				cout << "是否覆盖" << node->getName() << "?(y/n):" ;
				string answer;
				getline(cin, answer);
				if (answer.compare("y") == 0 ||answer.compare("Y") == 0)
				{
					node->copy(srcFile);
				}
			}
			else
			{
				File* file = new File(newName);
				file->setDate(date);
				file->setParent(tarFolder);
				file->copy(srcFile);
			}
		}
		else
		{
			Node* node = tarFolder->containNode(srcFile->getName());
			if (nullptr != node)
			{
				cout << "是否覆盖" << node->getName() << "?(y/n):";
				string answer;
				getline(cin, answer);
				if (answer.compare("y") == 0 ||answer.compare("Y") == 0)
				{
					node->copy(srcFile);
				}
			}
			else
			{
				File* file = new File(srcFile->getName());
				file->setDate(date);
				file->setParent(tarFolder);
				file->copy(srcFile);
			}
		}
		
		tarFolder->setWildCardStr("*");
	}
	else if (nullptr != tarFile)
	{
		cout << "是否覆盖" << tarFile->getName() << "?(y/n):";
		string answer;
		getline(cin, answer);
		if (answer.compare("y") == 0 ||answer.compare("Y") == 0)
		{
			tarFile->copy(srcFile);
		}
	}
	else
	{
		if (tarlink->getType() == 2)
		{
			Folder* temp = dynamic_cast<Folder*>(tarlink->getLinkNode());
			while (nullptr == temp)
			{
				SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(tarlink->getLinkNode());
				temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
				tarlink = linkTemp;
			}

			Node* node = temp->containNode(srcFile->getName());
			if (nullptr != node)
			{
				cout << "是否覆盖" << node->getName() << "?(y/n):";
				string answer;
				getline(cin, answer);
				if (answer.compare("y") == 0 ||answer.compare("Y") == 0)
				{
					node->copy(srcFile);
				}
			}
			else
			{
				File* file = new File(srcFile->getName());
				file->setDate(date);
				file->setParent(temp);
				file->copy(srcFile);
			}
			
		}
		else
		{
			cout << "是否覆盖" << tarlink->getName() << "?(y/n):";
			string answer;
			getline(cin, answer);
			if (answer.compare("y") == 0 ||answer.compare("Y") == 0)
			{
				File* file = dynamic_cast<File*>(tarlink->getLinkNode());
				file->copy(srcFile);
			}
		}
	}
}

File* CopyCommand::switchFile( Node* node )
{
	File* srcFile = dynamic_cast<File*>(node);
	if (nullptr != srcFile)
	{
		return srcFile;
	}
	else
	{
		SymbolicLink* srcFile = dynamic_cast<SymbolicLink*>(node);
		if (nullptr != srcFile && 1 == srcFile->getType())
		{
			File* temp = dynamic_cast<File*>(srcFile->getLinkNode());
			while (nullptr == temp)
			{
				SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(srcFile->getLinkNode());
				temp = dynamic_cast<File*>(linkTemp->getLinkNode());
				srcFile = linkTemp;
			}

			return temp;
		}
	}

	return nullptr;
}

File* CopyCommand::loadFile( std::string path )
{
	string srcStr;
	vector<char> vec;

	ifstream out(path.c_str(), ios::binary);

	if (out.is_open())
	{
		while(!out.eof())
		{
			srcStr.push_back(out.get());
		}
	}
	else
	{
		return nullptr;
	}

	string desStr = ZBase64::saveData(srcStr.c_str(), srcStr.size());
	for (int i = 0; i < desStr.size(); ++i)
	{
		vec.push_back(desStr[i]);
	}

	File* file = new File;
	file->setContent(vec);

	return file;
}

bool CopyCommand::matchString( char* pat, char* str )
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