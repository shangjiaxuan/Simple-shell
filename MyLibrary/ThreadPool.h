//A custom thread pool
//Capabilities: syncing and deploying tasks to threads
//Actively checks the state of execution without blocking
//Thread by default do not exit on task finish and waits for new tasks
//Threads have separate pools of tasks, and the pool itself has a
//pool of tasks that can be assigned to available threads on a PollTasks call.
//
//Note that a thread will go to wait when the queue gets empty, and waking up from
//such a sleep can be costly at times. It is thus recommended to keep things busy



#pragma once

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <iostream>

extern std::atomic<int64_t> all_available;


//unsigned(-1) for main thread and threads not managed by a pool
unsigned GetThreadIndex(unsigned init_index = -1);
size_t GetPoolIndex(size_t init_index = -1);

class ThreadPool {
public:
	//A naive approach for a spin-locked task queue.
	//A better approach may be a lock-free linked list of fixed length arrays or something similar
	//(A pure linked list is not memory-efficient and has too many allocations and deallocations).
	//Or maybe a linked list of stack for "deallocated" nodes from pop could be used to reduce memory
	//allocation time count, this apprach can also add features by putting additional information
	//in the struct alongside the function pointer.
	//
	//Another thought is to implement the queue as a pseudo-fixed-length array queue.
	//When a reallocation of the queue needs to be done, a flag is set to block normal
	//pushing and popping, and start reallocating after all other worker has finished 
	//as indicated by a usage counter. The push and pop also needs a flag to indicate
	//the difference between push or pop being finished and memory being indicated as 
	//under-use by the start and end indicator (all needs to be atomic).
	//(Decided to give up since signalling a overflow of buffer can easily break since
	//the signal time cannot be properly synced in the most general case).
	class TaskQueue {
	private:
		//queue always ends with a nullptr
		//functions return true for rescheduling 
		//if dependent data is not ready after timeout.
		std::atomic_flag in_use = ATOMIC_FLAG_INIT;
		std::function<bool()>* queue = nullptr;
		size_t queue_start = 0;
		//end is the position of the nullptr
		size_t queue_end = 0;
		//size of queue structure - 1, same as number of available slots
		size_t queue_cache = 0;
	public:
		~TaskQueue();
		void clear();
		//Currently the lock is implemented with a atomic bool.
		//If exception happens, the boolean is not unlocked and may result in a deadlock
		//that eats up system resource.
		//Non-Exception-Safe!
		void push(const std::function<bool()>& function);
		std::function<bool()> pop();
		void append(const TaskQueue& src);
		//These functions do not use the in_use for spin lock
		//Thus, are prone to error if race condition occurs 
		//Especially so if the queue is nearly full for empty() function.
		//The idea for error is that false negative is more acceptible 
		//than false positive since pushing and popping has a lock and can
		//never drop a task when pushing, but may pop a nullptr if a
		//false positive occurs, and checking for nullptrs will be bad.
		bool empty();
		//use this to double-check before ending loops.
		bool empty_strict();
		//waiting() function has to be strict when used for evaluation.
		size_t waiting();
	};
	ThreadPool();
	ThreadPool(unsigned num);
	//ThreadPools can only be destroyed on the creation thread.
	~ThreadPool();
	const unsigned available;
	unsigned GetUsed() const;
	unsigned AddThread();
	//This function currently have the bug of a deadlock when
	//two threads are executing it to remove each other.
	bool RemThread(unsigned ID);

	//No mutex involved in Poll functions and Schedule functions!
	void PollAllTasks(float timeout = 0);
	void PollAllTasks_WaitingOnly(float timeout = 0);
	void PollTasks();
	void PollTasks_WaitingOnly();
	void PushTask(const std::function<bool()>& added);
	void ScheduleTasks(const TaskQueue& added);
	void ClearSchedule();
	void ClearAllTasks();

	bool ThreadTasks(const TaskQueue& added, unsigned thread);
	bool ThreadPush(const std::function<bool()>& added, unsigned thread);
	bool ThreadAvailable(unsigned thread);
	size_t QuerieSchedule();
	size_t QuerieThread(unsigned thread);
	unsigned WaitAll(float timeout = 0);
	bool WaitThread(unsigned ID, float timeout = 0);

	void auto_work(unsigned thread, bool work = true);

private:
//	std::mutex mtx;
	std::atomic<unsigned> used;
	TaskQueue MasterQueue;
	struct passed_vals {
		//The structure is exclusive to the executing thread and values 
		//are indicators for current execution state,
		//with the exception of the pool adding things from other thread.
		//setting this can allow a spin-lock when doing light-weight job.
		//Lock both this and the mutex for long-running dispatchs
		//Light-weight and fast changes should use this indicator instead.
		std::atomic_flag modifying = ATOMIC_FLAG_INIT;
		std::atomic<bool> waiting = false;
		std::mutex mtx{};
		std::condition_variable cv{};
		TaskQueue queue{};
		TaskQueue* Masterqueue;
		bool active = true;
		bool finished = true;
		size_t num_done = 0;
		bool ready_exit = false;
		unsigned thread_index;
		size_t pool_index;
		bool auto_get_task = true;
	};
	passed_vals* const states;
	std::thread* const threads;
	static void deploy_func(passed_vals* vals);
	const std::thread::id creation_thread;
	const size_t pool_index;
};

