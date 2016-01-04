#pragma once

#include <vector>

#include "Node.h"
#include "Subject.h"

class Folder : public Node, public Subject
{
public:
	Folder();
	Folder(std::string name);

	virtual void addSubFile(Node* file);
	virtual void removeSubFile(Node* file);

	virtual Node* containNode(std::string nodeName);

	virtual void deleteNode(bool deleteAll);
	void deleteFolder();

	virtual void show(bool isFolder);
	virtual void showSubFile(bool recursion, bool isFolder);

	std::string getCurPath();

	void setWildCardStr(std::string str);

	std::vector<Node*> matchNode();

	bool isEmpty();

	std::string getWildCard();
private:
	Folder(const Folder&);
	Folder& operator=(const Folder&);

	bool matchWildCard(const char *pat, const char *str);

private:
	std::vector<Node*> _subFile;
	std::string _wildCardStr;
};