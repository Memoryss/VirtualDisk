#include "File.h"

#include <iostream>
#include <iomanip>

using namespace std;

File::File()
{
	char str[12];
	sprintf(str, "new file%d", _tag++);
	string name(str);

	setName(str);
}

File::File(string name):
Node(name)
{
	
}

int File::getSize()
{
	return _content.size();
}

const vector<char>& File::getContent()
{
	return _content;
}

void File::setContent(vector<char> content)
{
	_content.clear();
	_content = content;
}

void File::copy( File* file )
{
	if (_content.size() != 0)
	{
		_content.clear();
	}

	const vector<char>& content = file->getContent();

	int i = 0;
	while (content.size() > i)
	{
		_content.push_back(content[i]);
		i++;
	}
}

void File::write( File* file )
{
	const vector<char>& content = file->getContent();

	int i = 0;
	while (content.size() > i)
	{
		_content.push_back(content[i]);
		i++;
	}
}

void File::deleteNode(bool recursion)
{
	_content.clear();
	notify();

	delete this;
}

void File::show(bool isFolder)
{
	if (isFolder)
	{
		return;
	}

	Date date = getDate();
	cout << date._year << "/" << date._month << "/" << date._day << "  " << setw(2) << date._hour << ":" << setw(2) << date._minute << "         " << setw(10)
		<< _content.size() << "  " << getName() << endl;
}
