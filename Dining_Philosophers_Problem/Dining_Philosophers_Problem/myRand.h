#pragma once

#include <random>
#include <functional>

class myRand{
private:

	std::random_device rd;

	template<typename T>
	std::uniform_int_distribution<>distribution(std::true_type, T min_range, T max_range){
		return  std::uniform_int_distribution<>(min_range, max_range);
	}

	template<typename T>
	std::uniform_real_distribution<>distribution(std::false_type, T min_range, T max_range){
		return  std::uniform_real_distribution<>(min_range, max_range);
	}

public:
	template<typename T>
	T rand(T min_rage, T max_range){
		std::mt19937 mt(rd());
		auto dis = distribution(typename std::is_integral<T>::type(), min_rage, max_range);
		return dis(mt);
	}
};


