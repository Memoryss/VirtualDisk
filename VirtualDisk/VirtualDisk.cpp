#include "VirtualDisk.h"
#include "CommandFactory.h"
#include "Command.h"
#include "ExitCommand.h"
#include "File.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

VirtualDisk::VirtualDisk()
{
	loadFile();

	_cmdFactory = new CommandFactory;
	_cmd = nullptr;
}


VirtualDisk::~VirtualDisk()
{
	delete _cmd;
	delete _cmdFactory;

	if (nullptr != _rootFolder)
	{
		_rootFolder->deleteFolder();
	}
}


void VirtualDisk::loadFile()
{
	Date date;
	date._year = 2015;
	date._month = 12;
	date._day = 15;
	date._hour = 15;
	date._minute = 34;

	vector<char> vec;
	vec.push_back('s');
	vec.push_back('s');
	vec.push_back('t');
	vec.push_back('l');

	_rootFolder = new Folder("C:");
	_rootFolder->setParent(nullptr);
	_rootFolder->setDate(date);

	Folder* folder11 = new Folder("b in");
	folder11->setParent(_rootFolder);
	folder11->setDate(date);

	Folder* folder12 = new Folder("baa");
	folder12->setParent(_rootFolder);
	folder12->setDate(date);
	
	File* file13 = new File("1.txt");
	file13->setParent(_rootFolder);
	file13->setDate(date);
	file13->setContent(vec);

	File* file14 = new File("11.txt");
	file14->setParent(_rootFolder);
	file14->setDate(date);
	file14->setContent(vec);

	File* file15 = new File("2.txt");
	file15->setParent(_rootFolder);
	file15->setDate(date);
	file15->setContent(vec);

	File* file21 = new File("1.txt");
	file21->setParent(folder11);
	file21->setDate(date);

	File* file22 = new File("11.txt");
	file22->setParent(folder11);
	file22->setDate(date);

	File* file23 = new File("2.txt");
	file23->setParent(folder11);
	file23->setDate(date);

	Folder* folder24 = new Folder("stl");
	folder24->setParent(folder11);
	folder24->setDate(date);

	File* file31 = new File("1.txt");
	file31->setParent(folder12);
	file31->setDate(date);

	File* file32 = new File("11.txt");
	file32->setParent(folder12);
	file32->setDate(date);

	File* file33 = new File("2.txt");
	file33->setParent(folder12);
	file33->setDate(date);

	SymbolicLink* symbolicLink = new SymbolicLink("sym", folder11->getCurPath(), folder11);
	symbolicLink->setParent(_rootFolder);
	symbolicLink->setDate(date);
	Subject* subject = dynamic_cast<Subject*>(folder11);
	subject->addObserver(symbolicLink);

	_curFolder = _rootFolder;
}

bool VirtualDisk::executeCmd(string str)
{
	if (_cmd != nullptr)
	{
		delete _cmd;
		_cmd = nullptr;
	}

	_cmd = _cmdFactory->createCmd(str);
	if (nullptr == _cmd)
	{
		cout << "ÎÞÐ§µÄÃüÁî" << endl;
		return true;
	}

	_cmd->setRootFolder(_rootFolder);
	_cmd->setCurFolder(_curFolder);
	_cmd->execute(str);

	if (nullptr != dynamic_cast<ExitCommand*>(_cmd))
	{
		return false;
	}

	return true;
}

string VirtualDisk::getCurPath()
{
	if (nullptr != _cmd)
	{
		_rootFolder = _cmd->getRootFolder();
		_curFolder = _cmd->getCurFolder();
	}
	
	return _curFolder->getCurPath();
}
