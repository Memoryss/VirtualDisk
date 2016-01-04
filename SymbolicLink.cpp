#include "SymbolicLink.h"

#include "Folder.h"
#include "File.h"
#include <iomanip>
using namespace std;

SymbolicLink::SymbolicLink( std::string name, std::string linkPath, Node* link):
Node(name)
{
	_link = link;
	_linkName = _link->getName();
	_linkPath = linkPath;

	_subject = dynamic_cast<Subject*>(link);
}

void SymbolicLink::removeLink()
{
	if (nullptr != _subject)
	{
		_subject->removeObserver(this);

		_link = nullptr;
		_subject = nullptr;
	}
}

void SymbolicLink::deleteNode(bool recursion)
{
	removeLink();
	notify();

	delete this;
}

Node* SymbolicLink::getLinkNode()
{
	return _link;
}

void SymbolicLink::show(bool isFolder)
{
	Date date = getDate();

	if (isFolder)
	{
		if (dynamic_cast<Folder*>(_link) != nullptr)
		{
			cout << date._year << "/" << date._month << "/" << date._day << "  " << setw(2) << date._hour << ":" << setw(2) << date._minute << "    " 
				<< "<SYMLINKD>" << "       " << getName() << "  [" << _linkName << "]" << endl;

			return;
		}
	}

	cout << date._year << "/" << date._month << "/" << date._day << "  " << setw(2) << date._hour << ":" << setw(2) << date._minute << "    " 
		<< "<SYMLINKD>" << "       " << getName() << "  [" << _linkName << "]" << endl;
}

Node* SymbolicLink::containNode( std::string nodeName )
{
	if (nullptr == _link)
	{
		return nullptr;
	}

	Node* node = _link->containNode(nodeName);

	return node;
}

void SymbolicLink::showSubFile(bool recursion, bool isFolder)
{
	if (nullptr != _link)
	{
		Folder* folder = dynamic_cast<Folder*>(_link);
		if (nullptr != folder)
		{
			string str = folder->getCurPath();
			while (str[str.size() - 1] != '\\')
			{
				str.pop_back();
			}
			str.append(getName());

			cout << "  " << folder->getCurPath() << "的目录" << endl;
		}
		_link->showSubFile(recursion, isFolder);
	}
}

std::string SymbolicLink::getLinkPath()
{
	return _linkPath;
}

int SymbolicLink::getType()
{
	Node* link = _link;
	while (nullptr != link)
	{
		File* file = dynamic_cast<File*>(link);
		Folder* folder = dynamic_cast<Folder*>(link);
		if (nullptr != file)
		{
			return 1; //表是链接的对象是文件
		}
		else if (nullptr != folder)
		{
			return 2; //表示链接的对象是文件夹
		}
		else
		{
			SymbolicLink* temp = dynamic_cast<SymbolicLink*>(link);
			link = temp->getLinkNode();
		}
	}

	return 0;
}
