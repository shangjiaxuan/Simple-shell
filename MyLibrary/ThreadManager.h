/*
*	Header for managing multiple threads from one main thread.
*/

#pragma once

#include <vector>
#include <thread>

template<typename input, typename output>
class Thread_Manager {
public:
	//functions for multithreaded same calculation of a given array of input to an array of outputs
	static std::vector<output> vector_thread(std::vector<input>& source, output(*const function)(input&));
	static std::vector<output> vector_thread_copy(std::vector<input>& source, output(*const function)(input));
private:
	static void launch_function(bool* lock, output(*const function)(input&), input* data, output* out);
	static void launch_function_copy(bool* lock, output(*const function)(input), input data, output* out);
};

#include "ThreadManager.ipp"

