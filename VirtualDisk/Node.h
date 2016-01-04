#pragma once
//#include "vld.h"
#include "Date.h"
#include <string>

class File;
class Node
{
public:
	Node();
	Node(std::string name);

	virtual ~Node();

	void setDate(Date date);
	Date getDate();

	void setName(std::string name);
	std::string getName();

	void setParent(Node* node);
	Node* getParent();

	virtual void addSubFile(Node* node);
	virtual void removeSubFile(Node* node);

	virtual Node* containNode(std::string nodeName);

	virtual void deleteNode(bool recursion) = 0;//删除文件 不能删除文件夹

	virtual void show(bool isFolder) = 0;
	virtual void showSubFile(bool recursion, bool isFolder);

	virtual void write(File* file);
	virtual void copy(File* file);
	//virtual void addLinkNode() = 0;

public:
	static int _tag;  //默认名字

protected:
	Date _date;
	std::string _name;
	Node* _parent;
};