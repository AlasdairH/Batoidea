#include "ThreadPool.h"

namespace Threads
{

	ThreadPool::ThreadPool(const int _numThreads)
	{
		start(_numThreads);
	}

	ThreadPool::~ThreadPool()
	{
		stop();
	}

	void ThreadPool::start(const int _numThreads)
	{
		for (int i = 0; i < _numThreads; ++i)
		{
			// create a new thread with a lambda
			m_threads.emplace_back([=]
			{
				while (true)
				{
					Task task;

					// scope for mutex. We do not want the mutex locked while the task is running. It could be a while.
					{
						std::unique_lock<std::mutex> lock(m_eventMutex);
						// this thread should wait for either stopping or wait if there are no tasks to complete
						m_cond_event.wait(lock, [=] { return m_stopping || !m_tasks.empty(); });

						if (m_stopping && m_tasks.empty())
							break;

						task = std::move(m_tasks.front());
						m_tasks.pop();
					}

					task();

				}
			});
		}

		LOG_MESSAGE("Created thread pool with " << _numThreads << " threads");
	}

	void ThreadPool::stop()
	{
		LOG_MESSAGE("Stopping Thread Pool...");

		// scope for mutex lock
		{
			std::unique_lock<std::mutex> lock(m_eventMutex);
			m_stopping = true;
		}

		// notify all threads
		m_cond_event.notify_all();
		// loop through all the treads and wait for them to stop execution
		for (auto &thread : m_threads)
			thread.join();

		LOG_MESSAGE("Stopped Thead Pool");
	}

}
