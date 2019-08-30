# Dining_Philosophers_Problem

A solution to the Dining Philosophers Problem using C++ and threads. Its based on the Arbitrator solution (with a waiter system). The downside of using a waiter system is that reduces the effectivness of parallelism to some extent. 

The project is a multithreaded application using sempahore flags and mutex locks to prevent deadlocks and starvation.
The cout stream is handled threadsafe. A general random number generator class using template methods is also included .

The project was created using Visual Studio 2015.

