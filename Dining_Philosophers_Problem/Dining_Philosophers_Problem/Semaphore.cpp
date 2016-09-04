#include "Semaphore.h"

semaphore::semaphore(int count) : count(count) {
}

void semaphore::wait() {
	std::unique_lock<std::mutex> semlock{ lock };
	--count;
	if (count < 0) cond.wait(semlock);
}

void semaphore::signal() {
	std::unique_lock<std::mutex> semlock{ lock };
	if (count < 0) cond.notify_one();
	++count;
}