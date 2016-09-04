#pragma once

#include <mutex>
#include <condition_variable>

class semaphore {
private:
	std::mutex lock;
	std::condition_variable cond;
	int count;
public:
	semaphore(int count = 1);
	void wait(); 
	void signal();  
};


