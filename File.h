#pragma once

#include <vector>
#include "Node.h"
#include "Subject.h"

class File : public Node, public Subject
{
public:
	File();
	File(std::string name);

	void copy(File* file);  //虚拟磁盘中 copy使用
	void write(File* file); //多个文件copy到一个文件中使用（合并）

	const std::vector<char>& getContent();
	void setContent(std::vector<char> content); //从真正磁盘路径中copy使用

	int getSize();

	virtual void deleteNode(bool recursion);

	virtual void show(bool isFolder);

private:
	File(const File&);
	File& operator=(const File&);

private:
	std::vector<char> _content;
};