#include "Subject.h"
#include "Observer.h"
#include "SymbolicLink.h"

using namespace std;

Subject::~Subject()
{
	
}

void Subject::addObserver( Observer* observer )
{
	_links.insert(observer);
}

void Subject::removeObserver( Observer* observer )
{
	_links.erase(observer);
}

void Subject::notify()
{
	set<Observer*>::iterator iter;
	for (iter = _links.begin(); iter != _links.end();)
	{
		(*iter)->removeLink();

		iter = _links.begin();
	}
}
