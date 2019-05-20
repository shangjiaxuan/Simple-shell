#include "ThreadPool.h"

#include <stdexcept>

#include <intrin.h>

std::atomic<int64_t> all_available = std::thread::hardware_concurrency() - 1;

unsigned GetThreadIndex(unsigned init_index) {
	thread_local static const unsigned index = init_index;
	return index;
}

size_t GetPoolIndex(size_t init_index) {
	thread_local static const size_t index = init_index;
	return index;
}

inline static size_t MakePoolIndex() {
	static std::atomic<unsigned> index = 0;
	return std::atomic_fetch_add_explicit(&index, 1, std::memory_order_seq_cst);
}

ThreadPool::ThreadPool():
	available([](){
	int64_t num; 
	do {
		num = all_available.exchange(0, std::memory_order_seq_cst);
		if (num == -1) {
			all_available.exchange(num, std::memory_order_seq_cst);
			throw
				std::runtime_error("Cannot detect Hardware Concurrency! Falied to auto initialize thread pool!");
		}
	} while (num < -1);
	return unsigned(num);}()),
	states(new passed_vals[available]{}),
	threads(new std::thread[available]{}),
	creation_thread(std::this_thread::get_id()),
	used(available),
	pool_index(GetPoolIndex(MakePoolIndex())) {
	for (unsigned i = 0; i < available; i++) {
		states[i].thread_index = i;
		states[i].pool_index=pool_index;
		states[i].Masterqueue = &MasterQueue;
		std::thread t(deploy_func, &states[i]);
		threads[i] = std::move(t);
//		threads[i].detach();
	}
}

ThreadPool::ThreadPool(unsigned num):
	available([num]() {
	//Checks for overflow and remove the used
	if (num > std::atomic_fetch_sub_explicit(&all_available, num, std::memory_order_seq_cst)) {
		std::atomic_fetch_add_explicit(&all_available, num, std::memory_order_seq_cst);
		throw std::runtime_error("Not Enough hardware for thread pool creation!");
	}
	return unsigned(num);}()),
	states(new passed_vals[available]{}),
	threads(new std::thread[available]{}),
	creation_thread(std::this_thread::get_id()),
	used(available),
	pool_index(GetPoolIndex(MakePoolIndex())) {
		for (unsigned i = 0; i < available; i++) {
			states[i].thread_index = i;
			states[i].pool_index = pool_index;
			states[i].Masterqueue = &MasterQueue;
			std::thread t(deploy_func, &states[i]);
			threads[i] = std::move(t);
		}
}

ThreadPool::~ThreadPool() {
	unsigned ThreadIndex = GetThreadIndex();
	if (ThreadIndex != unsigned(-1) && threads[ThreadIndex].get_id() != std::this_thread::get_id()) ThreadIndex = -1;
	for (unsigned i = 0; i < available; i++) {
		states[i].active = false;
		states[i].queue.clear();
		if (i != ThreadIndex) {
			while(states[i].modifying.test_and_set(std::memory_order_acquire)) _mm_pause();
			while (!states[i].waiting.load(std::memory_order_acquire)) _mm_pause();
			std::unique_lock<std::mutex> lck(states[i].mtx);
			states[i].cv.notify_all();
		}
	}
	for (unsigned i = 0; i < available; i++) {
		if (i != ThreadIndex) {
			threads[i].join();
//			std::unique_lock<std::mutex> lck(states[i].mtx);
//			states[i].cv.wait(lck, [this, i]() {return bool(this->states[i].ready_exit); });
		}
	}
	delete[] threads;
	delete[] states;
}

void ThreadPool::PollTasks() {
	unsigned ThreadIndex = GetThreadIndex();
	if (ThreadIndex != unsigned(-1) && threads[ThreadIndex].get_id() != std::this_thread::get_id()) ThreadIndex = -1;
	for (unsigned i = 0; !MasterQueue.empty() && i < available ; i++) {
		if (states[i].active && states[i].queue.empty() && !states[i].modifying.test_and_set(std::memory_order_acquire)) {
			try{
				states[i].queue.push(MasterQueue.pop());
				states[i].modifying.clear(std::memory_order_release);
			} catch (std::exception& e) {
				states[i].modifying.clear(std::memory_order_release);
				throw e;
			} catch (...) {
				states[i].modifying.clear(std::memory_order_release);
				throw;
			}
		}
	}
	for (unsigned i = 0; !MasterQueue.empty() && i < available; i++) {
		if (i != ThreadIndex && states[i].active && states[i].finished && !states[i].queue.empty() && !states[i].modifying.test_and_set(std::memory_order_acquire)) {
			while (!states[i].waiting.load(std::memory_order_acquire))  _mm_pause();
			states[i].cv.notify_all();
			states[i].modifying.clear(std::memory_order_release);
		}
	}
}

void ThreadPool::PollTasks_WaitingOnly() {
	for (unsigned i = 0; !MasterQueue.empty() && i < available; i++) {
		if (states[i].active && states[i].finished  && states[i].queue.empty() && !states[i].modifying.test_and_set(std::memory_order_acquire)) {
			try {
				states[i].queue.push(MasterQueue.pop());
				states[i].modifying.clear(std::memory_order_release);
			} catch (std::exception& e) {
				states[i].modifying.clear(std::memory_order_release);
				throw e;
			} catch (...) {
				states[i].modifying.clear(std::memory_order_release);
				throw;
			}
		}
	}
	unsigned ThreadIndex = GetThreadIndex();
	if (ThreadIndex != unsigned(-1) && threads[ThreadIndex].get_id() != std::this_thread::get_id()) ThreadIndex = -1;
	for (unsigned i = 0; !MasterQueue.empty() && i < available; i++) {
		if (i != ThreadIndex && states[i].finished && states[i].active && !states[i].queue.empty() && !states[i].modifying.test_and_set(std::memory_order_acquire)) {
			while (!states[i].waiting.load(std::memory_order_acquire))  _mm_pause();
			states[i].cv.notify_all();
			states[i].modifying.clear(std::memory_order_release);
		}
	}
}

void ThreadPool::PushTask(const std::function<bool()>& added) {
	MasterQueue.push(added);
}

void ThreadPool::PollAllTasks(float timeout) {
	if (used == 0) return;
	if (!timeout) {
		do {
			while (!MasterQueue.empty()) {
				PollTasks();
			}
		} while (!MasterQueue.empty_strict());
	} else {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		do{
			while (!MasterQueue.empty() && timeout < std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now() - start ).count()) {
				PollTasks();
			}
		}while(timeout < std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now() - start ).count() && !MasterQueue.empty_strict());
	}
}

void ThreadPool::PollAllTasks_WaitingOnly(float timeout) {
	if (!timeout) {
		do{
			while (!MasterQueue.empty()) {
				PollTasks_WaitingOnly();
			}
		}while(!MasterQueue.empty_strict());
	} else {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		do{
			while (!MasterQueue.empty() && timeout < std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now() - start ).count()) {
				PollTasks_WaitingOnly();
			}
		}while(timeout < std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now() - start ).count() && !MasterQueue.empty_strict());
	}
}

unsigned ThreadPool::GetUsed() const {
	return used.load(std::memory_order_acquire);
}

unsigned ThreadPool::AddThread() {
	if (std::this_thread::get_id() != creation_thread) return -1;
//	std::lock_guard<std::mutex> lck(mtx);
	if(used.load(std::memory_order_acquire)>=available) return -1;
	unsigned rtn;
	for (unsigned i = 0; i < available; i++) {
		if (!states[i].active && states[i].ready_exit) { //&& states[i].modifying.exchange(true,std::memory_order_acq_rel)) {
			std::lock_guard<std::mutex> lck(states[i].mtx);
			states[i].waiting.store(false, std::memory_order_release);
			states[i].num_done = 0;
			states[i].active = true;
			std::thread t(deploy_func, &states[i]);
			threads[i] = std::move(t);
			states[i].ready_exit = false;
			states[i].modifying.clear(std::memory_order_release);
			std::atomic_fetch_add_explicit(&used,1,std::memory_order_acq_rel);
			rtn = i;
			break;
		}
	}
	return rtn;
}

bool ThreadPool::RemThread(unsigned ID) {
	if (ID >= available) return false;
	if (!states[ID].active) return true;
	if(std::this_thread::get_id()!=creation_thread) return false;
//	std::lock_guard<std::mutex> lck(mtx);
	/*
	if(ID==GetThreadIndex()){ 
		states[ID].active = false;
		states[ID].queue.clear();
		return true;
	}
	*/
	while(states[ID].modifying.test_and_set(std::memory_order_acquire))  _mm_pause();
	std::unique_lock<std::mutex> thread_lck(states[ID].mtx);
	states[ID].active = false;
	states[ID].queue.clear();
	states[ID].cv.notify_all();
	threads[ID].join();
//	states[ID].cv.wait(thread_lck, [this, ID]() {return bool(this->states[ID].ready_exit);});
//	states[ID].queue.clear();
//	std::thread t;
//	threads[ID] = std::move(t);
	std::atomic_fetch_sub_explicit(&used, 1, std::memory_order_acq_rel);
	states[ID].modifying.clear(std::memory_order_release);
	return true;
}

bool ThreadPool::ThreadTasks(const TaskQueue& added, unsigned thread) {
	if(thread>=available) return false;
	if(states[thread].ready_exit || !states[thread].active) return false;
	if (!states[thread].finished) {
		states[thread].queue.append(added);
		return true;
	}
	while(states[thread].modifying.test_and_set(std::memory_order_acquire))  _mm_pause();
	try{
		states[thread].queue.append(added);
		states[thread].cv.notify_all();
		states[thread].modifying.clear(std::memory_order_release);
		return true;
	} catch (std::exception& e) {
		states[thread].modifying.clear(std::memory_order_release);
		throw e;
	} catch (...) {
		states[thread].modifying.clear(std::memory_order_release);
		throw;
	}
}

bool ThreadPool::ThreadPush(const std::function<bool()>& added, unsigned thread) {
	if (thread >= available) return false;
	if (states[thread].ready_exit || !states[thread].active) return false;
	if (!states[thread].finished) {
		states[thread].queue.push(added);
		return true;
	}
	while (states[thread].modifying.test_and_set(std::memory_order_acquire))  _mm_pause();
	try {
		states[thread].queue.push(added);
		states[thread].cv.notify_all();
		states[thread].modifying.clear(std::memory_order_release);
		return true;
	} catch (std::exception& e) {
		states[thread].modifying.clear(std::memory_order_release);
		throw e;
	} catch (...) {
		states[thread].modifying.clear(std::memory_order_release);
		throw;
	}
}

void ThreadPool::auto_work(unsigned thread, bool work) {
	states[thread].auto_get_task = work;
	states[thread].cv.notify_all();
}

void ThreadPool::ScheduleTasks(const TaskQueue & added) {
	MasterQueue.append(added);
}

void ThreadPool::ClearSchedule() {
	MasterQueue.clear();
}

void ThreadPool::ClearAllTasks() {
	unsigned ThreadIndex = GetThreadIndex();
	if (ThreadIndex != unsigned(-1) && threads[ThreadIndex].get_id() != std::this_thread::get_id()) ThreadIndex = -1;
	for (unsigned i = 0; i < available; i++) {
		if (i != ThreadIndex) {
			while (states[i].modifying.test_and_set(std::memory_order_acquire)) _mm_pause();
				std::unique_lock<std::mutex> lck(states[i].mtx);
		}
		states[i].queue.clear();
		states[i].cv.notify_all();
		states[i].modifying.clear(std::memory_order_release);
	}
}

bool ThreadPool::ThreadAvailable(unsigned thread) {
	if (thread >= available) return false;
	if (states[thread].ready_exit || !states[thread].active) return false;
	return states[thread].active && states[thread].waiting.load(std::memory_order_acquire);
}

size_t ThreadPool::QuerieThread(unsigned thread) {
	if (thread >= available) return -1;
	if (states[thread].ready_exit || !states[thread].active) return -1;
	return states[thread].queue.waiting();
}

size_t ThreadPool::QuerieSchedule() {
	return MasterQueue.waiting();
}

bool ThreadPool::WaitThread(unsigned ID, float timeout) {
	if (ID >= available) return false;
	if (!states[ID].active) return false;
	if (states[ID].ready_exit || states[ID].queue.empty()) return true;
	if(ID==GetThreadIndex() && std::this_thread::get_id()==threads[ID].get_id()) return false;
	while (states[ID].modifying.test_and_set(std::memory_order_acquire))  _mm_pause();
	std::unique_lock<std::mutex> lck(states[ID].mtx);
	if (!timeout) {
		try {
			states[ID].cv.wait(lck, [this, ID]() {return bool(states[ID].queue.empty()); });
		} catch (std::exception& e) {
			states[ID].modifying.clear(std::memory_order_release);
			throw e;
		} catch (...) {
			states[ID].modifying.clear(std::memory_order_release);
			throw;
		}
		states[ID].modifying.clear(std::memory_order_release);
		return true;
	} else {
		bool rtn;
		try {
			rtn = states[ID].cv.wait_for(lck, std::chrono::duration<float>(timeout), [this, ID]() {return bool(states[ID].queue.empty()); });
		} catch (std::exception& e) {
			states[ID].modifying.clear(std::memory_order_release);
			throw e;
		} catch (...) {
			states[ID].modifying.clear(std::memory_order_release);
			throw;
		}
		states[ID].modifying.clear(std::memory_order_release);
		return rtn;
	}
}

unsigned ThreadPool::WaitAll(float timeout) {
	unsigned rtn = 0;
	unsigned ThreadIndex = GetThreadIndex();
	if (ThreadIndex < available && threads[ThreadIndex].get_id() != std::this_thread::get_id()) ThreadIndex = -1;
	if (!timeout) {
		for (unsigned i = 0; i < available; i++) {
			if (i != ThreadIndex) {
				if (!states[i].active || states[i].ready_exit) continue;
				while (states[i].active && states[i].modifying.test_and_set(std::memory_order_acquire)) _mm_pause();
				if (states[i].finished) continue;
				std::unique_lock<std::mutex> lck(states[i].mtx);
				states[i].cv.wait(lck, [this, i]() { return bool(states[i].finished); });
				rtn++;
			}
		}
		for (unsigned i = 0; i < available; i++) {
			if (i != ThreadIndex) {
				if (!states[i].active || states[i].ready_exit) continue;
				states[i].modifying.clear(std::memory_order_release);
			}
		}
		return rtn;
	} else {
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		float time_passed = 0;
		bool last_set;
		for (unsigned i = 0; i < available && timeout > time_passed; i++) {
			if (i != ThreadIndex) {
				if (!states[i].active || states[i].ready_exit) continue;
				while (states[i].active && timeout > time_passed && states[i].modifying.test_and_set(std::memory_order_acquire)){
					time_passed = std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now() - start ).count();
				}
				if (timeout <= time_passed) continue;
				if (states[i].finished) continue;
				std::unique_lock<std::mutex> lck(states[i].mtx);
				last_set = states[i].cv.wait_for(lck, std::chrono::duration<float>(timeout - time_passed), [this, i]() { return bool(states[i].finished); });
				rtn += last_set;
			}
			time_passed = std::chrono::duration_cast<std::chrono::duration<float>>( std::chrono::steady_clock::now() - start ).count();
		}
		for (unsigned i = 0; i < rtn + !last_set; i++) {
			if (i != ThreadIndex) {
				if (!states[i].active || states[i].ready_exit) continue;
				states[i].modifying.clear(std::memory_order_release);
			}
		}
		return rtn;
	}
}

void ThreadPool::deploy_func(passed_vals* vals) {
	GetThreadIndex(vals->thread_index);
	GetPoolIndex(vals->pool_index);
	std::unique_lock<std::mutex> lck(vals->mtx);
	while (vals->active) {//adding a condition variable here may be  a good choice
						  //start execution of queue only after the calling thread sets the finished flag to false	
		vals->waiting.store(true, std::memory_order_release);
		while (vals->queue.empty() && vals->active) {
			//if a notification comes here, it may be missed!
			vals->cv.wait(lck);
//			while (vals->modifying.load(std::memory_order_acquire));
		}
		vals->finished = false;
		vals->waiting.store(false, std::memory_order_release);
		do {
			while (vals->active && !vals->queue.empty()) {
				do {
					while (vals->active && !vals->queue.empty()) {
						auto func = vals->queue.pop();
						if (func()) {
							vals->queue.push(func);
						}
						vals->num_done++;
					}
					//double check for emptiness
				} while (!vals->queue.empty_strict());
				if (vals->auto_get_task) {
					auto task = vals->Masterqueue->pop();
					if (task) vals->queue.push(task);
				}
			}
		} while (!vals->queue.empty_strict());
		vals->finished = true;
		if (vals->active) vals->cv.notify_all();
	}
	std::notify_all_at_thread_exit(vals->cv, std::move(lck));
	vals->ready_exit = true;
}

ThreadPool::TaskQueue::~TaskQueue() {
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	delete[] queue;
}

void ThreadPool::TaskQueue::clear() {
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	delete[] queue;
	queue = nullptr;
	queue_start = 0;
	queue_end = 0;
	queue_cache = 0;
	in_use.clear(std::memory_order_release);
}

void ThreadPool::TaskQueue::push(const std::function<bool()>& function) {
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	try{
		if (( queue_end > queue_start ) && ( queue_end - queue_start == queue_cache )
			|| ( queue_end < queue_start ) && ( queue_start - 1 == queue_end )) {
			size_t new_size = size_t(queue_cache*1.618) + 2;
			std::function<bool()>* new_buffer = new std::function<bool()>[new_size] {};
			for (size_t i = queue_start; i <= queue_end || ( queue_end < queue_start && i <= queue_cache ); i++) {
				new_buffer[i - queue_start] = queue[i];
			}
			if (queue_end < queue_start) {
				for (size_t i = 0; i <= queue_end; i++) {
					new_buffer[queue_cache - queue_start + 1 + i] = queue[i];
				}
			}
			std::swap(queue, new_buffer);
			size_t temp = queue_cache;
			queue_end = temp;
			queue_cache = new_size - 1;
			queue_start = 0;
			delete[] new_buffer;
		}
		if (queue_cache == 0) {
			queue = new std::function<bool()>[2]{};
			queue_cache = 1;
		}
		queue[queue_end] = function;
		if (queue_end == queue_cache) queue_end = 0;
		else queue_end++;
		queue[queue_end] = nullptr;
		in_use.clear(std::memory_order_release);
	} catch (std::exception& e) {
		in_use.clear(std::memory_order_release);
		throw e;
	} catch (...) {
		in_use.clear(std::memory_order_release);
		throw;
	}
}

std::function<bool()> ThreadPool::TaskQueue::pop() {
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	try{
		if (queue_start == queue_end) return nullptr;
		std::function<bool()> rtn = queue[queue_start];
		queue[queue_start] = nullptr;
		if (queue_start == queue_cache) queue_start = 0;
		else queue_start++;
		in_use.clear(std::memory_order_release);
		return rtn;
	} catch (std::exception& e) {
		in_use.clear(std::memory_order_release);
		throw e;
	} catch (...) {
		in_use.clear(std::memory_order_release);
		throw;
	}
}

bool ThreadPool::TaskQueue::empty() {
	return queue_end == queue_start;
}

bool ThreadPool::TaskQueue::empty_strict() {
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	bool rtn = ( queue_end == queue_start );
	in_use.clear(std::memory_order_release);
	return rtn;
}

size_t ThreadPool::TaskQueue::waiting() {
	size_t rtn;
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	if (queue_end >= queue_start) rtn = queue_end - queue_start;
	else rtn = queue_cache - queue_start + queue_end + 1;
	in_use.clear(std::memory_order_release);
	return rtn;
}

void ThreadPool::TaskQueue::append(const TaskQueue & src) {
	while (in_use.test_and_set(std::memory_order_acquire)) _mm_pause();
	try{
		size_t size1 = ( queue_end >= queue_start ) ? queue_end - queue_start : queue_cache - queue_start + queue_end + 1;
		size_t size2 = ( src.queue_end >= src.queue_start ) ? src.queue_end - src.queue_start : src.queue_cache - src.queue_start + src.queue_end + 1;
		if (size_t(( size1 + size2 ) * 1.618) < queue_cache) {
			for (size_t i = src.queue_start; i < src.queue_end || ( src.queue_end < src.queue_start&&i - src.queue_start <= src.queue_cache ); i++) {
				queue[queue_end] = src.queue[src.queue_start + i];
				if (queue_end == queue_cache) queue_end = 0;
				else queue_end++;
			}
			if (src.queue_end < src.queue_start) {
				for (size_t i = 0; i < src.queue_end; i++) {
					queue[queue_end] = src.queue[i];
					if (queue_end == queue_cache) queue_end = 0;
					else queue_end++;
				}
			}
			queue[queue_end] = nullptr;
			in_use.clear(std::memory_order_release);
			return;
		} else {
			size_t new_size = size_t(( size1 + size2 ) * 1.618) + 1;
			std::function<bool()>* new_queue = new std::function<bool()>[new_size + 1]{};
			for (size_t i = queue_start; i < queue_end || ( queue_end < queue_start && i <= queue_cache ); i++) {
				new_queue[i - queue_start] = queue[i];
			}
			if (queue_end < queue_start) {
				for (size_t i = 0; i < queue_end; i++) {
					new_queue[queue_cache - queue_start + 1 + i] = queue[i];
				}
			}
			for (size_t i = src.queue_start; i < src.queue_end || ( src.queue_end < src.queue_start&& i <= src.queue_cache ); i++) {
				new_queue[i - src.queue_start + size1] = src.queue[i];
			}
			if (src.queue_end < src.queue_start) {
				for (size_t i = 0; i < src.queue_end; i++) {
					new_queue[src.queue_cache - src.queue_start + 1 + i + size1] = src.queue[i];
				}
			}
			new_queue[size1 + size2] = nullptr;
			std::swap(new_queue, queue);
			delete[] new_queue;
			queue_cache = new_size;
			queue_start = 0;
			queue_end = size1 + size2;
			in_use.clear(std::memory_order_release);
			return;
		}
	} catch (std::exception& e) {
		in_use.clear(std::memory_order_release);
		throw e;
	} catch (...) {
		in_use.clear(std::memory_order_release);
		throw;
	}
}
