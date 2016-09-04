#pragma once

#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm> 

class Monitor;

class Philosopher
{
private:
	std::string _name; // not in use as of now
	int _id;
	int _it;
	Monitor* _mon;
public:
	Philosopher(std::string,int,int,Monitor*);

	void operator()();

	void think();
	void eat();
	
	~Philosopher();
};
