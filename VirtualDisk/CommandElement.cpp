#include "CommandElement.h"

using namespace std;

CommandElement::CommandElement()
{
	_isWildCard = false;
}

std::string CommandElement::getElementStr()
{
	return _elementStr;
}

void CommandElement::setElementStr( std::string str )
{
	_elementStr = str;
}

ElementType CommandElement::getElementType()
{
	return _elementType;
}

void CommandElement::setElementType( ElementType type )
{
	_elementType = type;
}

void CommandElement::setIsWildCard( bool flag )
{
	_isWildCard = flag;
}

bool CommandElement::getIsWildCard()
{
	return _isWildCard;
}
