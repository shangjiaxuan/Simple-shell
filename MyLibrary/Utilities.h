#pragma once

#include <chrono>

template<typename type>
static constexpr size_t find_MAX() {
	size_t start = 0x01;
	for (int i = 0; i < sizeof(type); i++) {
		start <<= 8;
	}
	return start;
}

template<typename type>
static constexpr size_t find_SQRT_OF_MAX() {
	return size_t(sqrt(find_MAX<type>()));
}


enum TimerType {
	normal,
	fast,
	slow
};
//A simple to use timer with three ranges and returns a double for duration.
//unit is seconds for normal clock
//microseconds for fast clock
//hours for slow clock
template<TimerType type>
class Timer {
public:
	void reset();
	Timer();
	double elapsed() const;
};

template<>
class Timer<normal>{
	std::chrono::time_point<std::chrono::steady_clock> start;
public:
	void reset() {
		start = std::chrono::steady_clock::now();
	}
	Timer() {
		reset();
	}
	double elapsed() const {
		return std::chrono::duration<double>
		(std::chrono::steady_clock::now() - start).count();
	}
};

template<> 
class Timer<fast>{
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
	void reset() {
		start = std::chrono::high_resolution_clock::now();
	}
	Timer() {
		reset();
	}
	double elapsed() {
		return std::chrono::duration<double,std::micro>
		(std::chrono::high_resolution_clock::now() - start).count();
	}
};

template<> 
class Timer<slow>{
	std::chrono::time_point<std::chrono::steady_clock> start;
public:
	void reset() {
		start = std::chrono::steady_clock::now();
	}
	Timer() {
		reset();
	}
	double elapsed() {
		return std::chrono::duration<double,std::ratio<3600,1>>
		(std::chrono::steady_clock::now() - start).count();
	}
};

