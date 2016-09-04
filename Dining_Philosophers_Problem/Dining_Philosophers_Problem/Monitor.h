#pragma once // just testing this, it should be supported by almost all compilers

#include <iostream>
#include <string>
#include <mutex>
#include <vector>
#include <thread>
#include <fstream>  

#include "Rand.h"
#include "Semaphore.h"


/* // not really needed, because all operations on the vectors happens under locked funcions, but I keep it anyway
About the std::vector class concurrency abilities...
As I know, std::vector should be thread safe when reading from multiple threads and writing from max one thread.
So I made this little template class that wraps the vector and uses a unique lock to prevent multiple writes
*/

template <class T> 
class concurrency_vector
{
private:
	std::mutex m;
	std::vector<T>arr;
public:
	concurrency_vector(int SIZE = 0) :arr(SIZE){}
	concurrency_vector(T val,int SIZE = 0) :arr(SIZE, val){}

	T get(int idx){ // just a copy, no locking
		return arr[idx];
	}

	void push_back(T){
		std::unique_lock<std::mutex> lock(m);
		arr.push_back(T);
	}

	T &operator[](int i)
	{
		std::unique_lock<std::mutex> lock(m); // dunno if this helps because its a reference
		return arr[i];
	}

	size_t size(){
		std::unique_lock<std::mutex> lock(m);
		return arr.size();
	}
};

class Monitor
{
private:
	std::mutex printmutex;
	std::mutex logmutex;
	std::mutex randmutex;
	Rand rand;
	std::ofstream logfile;
	int _it;     // nr of times to eat
	int _nrOfPs; // nr of philosophers
	int highestVal; // highest amount of eating ps
	concurrency_vector<semaphore>_forks;
	concurrency_vector<int>_states; // the states
	std::vector<std::string>_psNames;
	std::vector<std::thread>_philosopherThreads;
public:
	Monitor(std::vector<std::string>, std::string,int, int);

	const enum { SIT, LEFT, RIGHT, THINK, HUNGRY, EAT, FINISH, LEAVE };
	void start();
	void printAndSetState(int,int);
	void log(std::string log);
	int getRandomNr(int, int);
	void getForks(int);
	void releaseForks(int);

	~Monitor();
};

