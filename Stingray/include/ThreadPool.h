#pragma once

// cstdlib
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include <condition_variable>
#include <queue>

// external libs

// program
#include "Logger.h"

// TODO: Doxygen


namespace Threads
{
	class ThreadPool
	{
	public:
		using Task = std::function<void()>;

		ThreadPool(const int _numThreads);
		~ThreadPool();

		template<class T>
		auto enqueue(T _task)->std::future<decltype(_task())>
		{
			// contain a shared pointer to an abstract function
			auto wrapper = std::make_shared<std::packaged_task<decltype(_task()) ()>>(std::move(_task));
			// scope for mutex lock
			{
				std::unique_lock<std::mutex> lock(m_eventMutex);
				m_tasks.emplace([=]
				{
					(*wrapper)();
				});
			}
			m_cond_event.notify_all();
			return wrapper->get_future();
		}

	protected:
		void start(const int _numThreads);
		void stop();

		std::queue<Task> m_tasks;

		std::vector<std::thread> m_threads;

		std::condition_variable m_cond_event;
		std::mutex m_eventMutex;
		bool m_stopping = false;					// under the protection of m_eventMutex

	};
}
