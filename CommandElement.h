#pragma once

#include <iostream>

enum ElementType
{
	CMDTYPE = 0,
	PATHTYPE,
	PARATYPE
};

class CommandElement
{
public:
	CommandElement();

	std::string getElementStr();
	void setElementStr(std::string str);

	ElementType getElementType();
	void setElementType(ElementType type);

	void setIsWildCard(bool flag);
	bool getIsWildCard();

private:
	std::string	_elementStr;
	ElementType _elementType;

	bool _isWildCard; //Í¨Åä·û
};