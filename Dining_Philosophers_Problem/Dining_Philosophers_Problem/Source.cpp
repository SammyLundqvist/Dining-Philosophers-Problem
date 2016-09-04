
#include <vector>
#include <fstream>
#include <algorithm> 
#include <stdexcept>

#include "Monitor.h"

#define FILENAME "philosofers353.txt" // skrivskyddad
#define DEFAULT_LOG "philo.log"

//void getChopsticks(void);

bool getPhilosophersNameFromFile(std::vector<std::string>&);
Monitor* getDefaultMonitor(std::vector<std::string>&philosopherNameList);

int main(int argc, char* argv[]) {

	srand((unsigned)time(nullptr)); // for random_shuffle

	// read namelist from file
	std::vector<std::string>philosopherNameList;
	if (!getPhilosophersNameFromFile(philosopherNameList))return 1;
	// shuffle names
	std::random_shuffle(philosopherNameList.begin(), philosopherNameList.end());

	Monitor* mon = nullptr;
	int nrPs = 0;
	int it = 0;
	
	std::string filename;

	if (argc == 1)
		mon = getDefaultMonitor(philosopherNameList); //mini factory
	else
	{
		try{
			if (argc != 4)
				throw std::invalid_argument("Argument count is wrong, expected (0) or (3). 1 = amount of philosophers, 2 = meals = 3 filename.");

			nrPs = std::stoi(argv[1]);
			it = std::stoi(argv[2]);

			filename =argv[3];

			if (nrPs < 4 || nrPs > 14)
				throw std::invalid_argument("Number of philosophers must be in range 4-14.");

			if (it < 4 || it > 10)
				throw std::invalid_argument("Number of meals must be in range 4-10.");

			mon = new Monitor(philosopherNameList, filename, it, nrPs); // list, filename, plates, philosophers

		}
		catch (std::invalid_argument & e){
			std::cerr << e.what() << std::endl;
			return 1;
		}
		catch (std::out_of_range & e){
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	mon->start();
	delete mon;

	return 0;
}

bool getPhilosophersNameFromFile(std::vector<std::string>&philosopherNameList){

	std::ifstream file(FILENAME);
	std::string str;

	if (!file.is_open())
	{
		std::cout << "File error!" << std::endl;
		return false;
	}

	while (std::getline(file, str))
		philosopherNameList.push_back(str);
	
	return true;
}

Monitor* getDefaultMonitor(std::vector<std::string>&philosopherNameList){
	int plates = 5; // nr of plates in use 
	int phil = 6;   // nr of philosophers eating
	return new Monitor(philosopherNameList, DEFAULT_LOG, plates, phil); 
}