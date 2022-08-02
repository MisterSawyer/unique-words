#ifndef THREAD_SCHEDULER_H
#define THREAD_SCHEDULER_H

#include <thread>
#include <list>
#include <filesystem>

#include "../concurent-set/concurent-set.h"
#include "../file-loader/file-loader.h"


class ThreadScheduler
{
public:
	void Start(ConcurentSet<std::string>& concurentSet, const FileLoader & loader);
	void Synchronize();
private:
	std::list<std::thread> m_threads;
};

#endif

