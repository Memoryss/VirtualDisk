#pragma once

#include <iostream>
#include "Node.h"
#include "Observer.h"

class SymbolicLink : public Node, public Observer, public Subject
{
public:
	SymbolicLink(std::string name, std::string linkPath, Node*link);

	Node* getLinkNode();

	virtual void removeLink();  //链接的节点 移除自己

	virtual void deleteNode(bool recursion);

	virtual void show(bool isFolder);

	virtual Node* containNode(std::string nodeName);

	virtual void showSubFile(bool recursion, bool isFolder);

	std::string getLinkPath();

	int getType();

private:
	Node* _link;
	std::string _linkName;
	Subject* _subject;

	std::string _linkPath;
};