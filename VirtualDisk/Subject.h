#pragma once

#include <set>

class Observer;
class Subject
{
public:
	virtual ~Subject();

	virtual void addObserver(Observer* observer);

	virtual void removeObserver(Observer* observer);

	virtual void notify();

private:
	std::set<Observer*> _links;
};