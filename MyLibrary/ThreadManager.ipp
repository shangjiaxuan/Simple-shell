#pragma once

template<typename input, typename output>
std::vector<output> Thread_Manager<input, output>::vector_async(std::vector<input>& source, output(*const function)(input&)) {
	const unsigned subthreads = std::thread::hardware_concurrency() - 1;
	size_t size = source.size();
	std::vector<output> rtn(size);
	switch (int(subthreads)) {
	case -1:
		std::cout << "Cannot identify hardware concurrency, multithreading failed!\n";
	case 0:
		std::cout << "Only one thread supported!\n";
	case 1:
		std::cout << "Working as single thread!\n" << std::endl;
		for (size_t i = 0; i < size; i++) {
			rtn[i] = function(source[i]);
		}
		return rtn;
	default:
		std::vector<size_t> tasks(subthreads);
		std::vector<char> locks(subthreads);
		std::vector<output> rtn(size);
		for (unsigned i = 0; i < ((subthreads < size) ? subthreads : size); i++) {
			std::thread t(launch_function, &locks[i], function, &source[i], &rtn[i]);
			t.detach();
			tasks[i] = i;
		}
		for (size_t i = subthreads; i < size;) {
			for (unsigned j = 0; j < subthreads && i < size; j++) {
				if (!locks[j]) {
					std::thread t(launch_function, &locks[j], function, &source[i], &rtn[i]);
					t.detach();
					tasks[j] = i;
					i++;
				}
			}
		}
		bool not_finished;
		do {
			not_finished = false;
			for (unsigned i = 0; i < subthreads; i++) {
				not_finished = not_finished || locks[i];
			}
		} while (not_finished);
		return rtn;
	}
};

template<typename input, typename output>
void Thread_Manager<input, output>::launch_function(char* lock, output(*const function)(input&), input* data, output* out) {
	*lock = true;
	*out = function(*data);
	*lock = false;
};


template<typename input, typename output>
std::vector<output> Thread_Manager<input, output>::vector_async_copy(std::vector<input>& source, output(*const function)(input)) {
	const unsigned subthreads = std::thread::hardware_concurrency() - 1;
	size_t size = source.size();
	std::vector<output> rtn(size);
	switch (int(subthreads)) {
	case -1:
		std::cout << "Cannot identify hardware concurrency, multithreading failed!\n";
	case 0:
		std::cout << "Only one thread supported!\n";
	case 1:
		std::cout << "Working as single thread!\n" << std::endl;
		for (size_t i = 0; i < size; i++) {
			rtn[i] = function(source[i]);
		}
		return rtn;
	default:
		std::vector<size_t> tasks(subthreads);
		std::vector<char> locks(subthreads);
		std::vector<output> rtn(size);
		for (unsigned i = 0; i < ((subthreads < size) ? subthreads : size); i++) {
			std::thread t(launch_function_copy, &locks[i], function, source[i], &rtn[i]);
			t.detach();
			tasks[i] = i;
		}
		for (size_t i = subthreads; i < size;) {
			for (unsigned j = 0; j < subthreads && i < size; j++) {
				if (!locks[j]) {
					std::thread t(launch_function_copy, &locks[j], function, source[i], &rtn[i]);
					t.detach();
					tasks[j] = i;
					i++;
				}
			}
		}
		bool not_finished;
		do {
			not_finished = false;
			for (unsigned i = 0; i < subthreads; i++) {
				not_finished = not_finished || locks[i];
			}
		} while (not_finished);
		return rtn;
	}
};

template<typename input, typename output>
void Thread_Manager<input, output>::launch_function_copy(char* lock, output(*const function)(input), input data, output* out) {
	*lock = true;
	*out = function(data);
	*lock = false;
};


