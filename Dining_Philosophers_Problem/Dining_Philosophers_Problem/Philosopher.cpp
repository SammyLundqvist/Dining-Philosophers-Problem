#include "Philosopher.h"
#include "Monitor.h"

#include <chrono>

Philosopher::Philosopher(std::string name, int id, int it, Monitor* mon) :_name(name), _id(id), _it(it), _mon(mon)
{
	_mon->printAndSetState(_id, _mon->SIT);
}

Philosopher::~Philosopher()
{}

void Philosopher::operator()(){

	for (int i = 0; i < _it; ++i)
	{
		think(); // just think
		eat(); // involves picking up forks, eating, and returning forks 
	}
	_mon->printAndSetState(_id, _mon->LEAVE); // print leave table state
}

void Philosopher::think()
{
	//print thinking
	_mon->printAndSetState(_id, _mon->THINK);
	//sleep for 1-5 sec
	int sleeptime = _mon->getRandomNr(1, 5); 
	std::this_thread::sleep_for(std::chrono::seconds(sleeptime));
}

void Philosopher::eat()
{
	//print hungry state
	_mon->printAndSetState(_id, _mon->HUNGRY);

	// signal pick up forks
	// prints additional states for picking up left fork and right fork
	_mon->getForks(_id);

	// print eating state
	_mon->printAndSetState(_id, _mon->EAT);
	// now sleep for 1-5 sec
	int sleeptime = _mon->getRandomNr(1, 5);
	std::this_thread::sleep_for(std::chrono::seconds(sleeptime));

	//signal to release lock on forks
	_mon->releaseForks(_id);
	// print finish statement for this meal
	_mon->printAndSetState(_id, _mon->FINISH);

	//... begin all over again
}