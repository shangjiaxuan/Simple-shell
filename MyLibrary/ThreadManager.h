#pragma once

#include <vector>
#include <thread>

template<typename input, typename output>
class Thread_Manager {
public:
	static std::vector<output> vector_async(std::vector<input>& source, output(*const function)(input&));
	static std::vector<output> vector_async_copy(std::vector<input>& source, output(*const function)(input));
private:
	static void launch_function(char* lock, output(*const function)(input&), input* data, output* out);
	static void launch_function_copy(char* lock, output(*const function)(input), input data, output* out);
};

#include "ThreadManager.ipp"

