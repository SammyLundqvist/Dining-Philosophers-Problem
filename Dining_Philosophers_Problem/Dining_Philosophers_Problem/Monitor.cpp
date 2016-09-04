#include "Monitor.h"
#include "Philosopher.h"

#include <sstream>
#include <iomanip>


Monitor::Monitor(std::vector<std::string>psNames, std::string logfilename, int it, int ps) :_psNames(psNames), _it(it), _nrOfPs(ps), _forks(ps), _states(THINK, ps)
{
	highestVal = 0;

	logfile.exceptions(std::ofstream::failbit);

	try {
		logfile.open(logfilename, std::ios_base::app);
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to open file::append mode: " << e.what() << std::endl;
		exit(0);
	}
}

Monitor::~Monitor()
{
	for (std::thread &i : _philosopherThreads)
	{
		try
		{
			if (i.joinable())
				i.join();  // Wait for the thread to finish
		}
		catch (std::system_error &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	std::ostringstream msg;

	msg << std::endl << "Nr of philosophers: " << _nrOfPs << std::endl;
	msg << "Nr of meals: " << _it << std::endl;
	msg << "Highest value of simultaneously eating philosophers: " << highestVal << std::endl << std::endl;
	std::cout << msg.str();

	log(msg.str());
	logfile.close();
}

void Monitor::start(){
	for (int i = 0; i < _nrOfPs; ++i)
		_philosopherThreads.push_back(std::thread(Philosopher(_psNames[i], i, _it, this)));
}
int Monitor::getRandomNr(int from, int too){
	std::unique_lock<std::mutex> lock(randmutex);
	return rand.rand<int>(from, too);
}
// Print and set states under lock. 
// cout stream is not used anywhere else, and states are set here.

void Monitor::printAndSetState(int id, int op){

	std::unique_lock<std::mutex> lock(printmutex);
	_states[id] = op; // assign new statechange

	//{LEFT, RIGHT, THINK, HUNGRY, EAT, FINISH, LEAVE};

	std::ostringstream msg;
	msg << std::setw(3) << std::left << id << std::setfill('.') << std::setw(30) << std::left << _psNames[id] << std::setfill(' ') << std::setw(30);

	switch (op)
	{
	case SIT:   msg << "> takes a seat at the table.";
		break;
	case LEFT:   msg << "> picks up his left fork.";
		break;
	case RIGHT:  msg << "> picks up his right fork.";
		break;
	case THINK:  msg << "> starts thinking.";
		break;
	case HUNGRY: msg << "> is hungry.";
		break;
	case EAT:    msg << "> begins to eat.";
		break;
	case FINISH: msg << "> have finished eating.";
		break;
	case LEAVE:  msg << "> leaves the table.";
		break;
	default:     msg << "> does some strange impossible stuff.";
	}

	// prints number of eating philosophers for every statechange.
	int count = 0;
	for (int i = 0; i < _nrOfPs; ++i)
		if (_states[i] == EAT)
			++count;

	msg << "(eating: " << count << ")" << std::endl;
	std::cout << msg.str();

	//log it
	log(msg.str());

	// save highest value of simultaneously eating philosophers
	if (count > highestVal)
		highestVal = count;
}

void Monitor::log(std::string log){
	std::unique_lock<std::mutex> lock(logmutex);
	logfile << log;
}
// I was thinking for a while if there was a natural way of preventing deadlocks and the answer was quite obvious
// each philosopher picks up his left and right fork in order
// this approach seems to be one of many solutions to this problem after reading wikipedia

void Monitor::getForks(int id)
{
	// even - left, odd - right (forks)

	int left = (id + 1) % _nrOfPs; //circular entry to forkvector
	int right = id;

	// after some philosopher/thread releases his lock on forks this thread set new lock to each fork
	// the monitor forces each philosopher to pick up left and right fork in an ordered manner
	// depending if idnr is even or odd

	if (id % 2 == 0)
	{
		_forks[left].wait(); // picks up left fork (has internal locking)
		printAndSetState(id, LEFT); // print and log this statechange
		_forks[right].wait();
		printAndSetState(id, RIGHT);
	}
	else
	{
		// reverese order
		_forks[right].wait();
		printAndSetState(id, RIGHT);
		_forks[left].wait();
		printAndSetState(id, LEFT);
	}
}

void Monitor::releaseForks(int id)
{
	// it should not matter what order forks get released
	_forks[id].signal();
	_forks[(id + 1) % _nrOfPs].signal();
}