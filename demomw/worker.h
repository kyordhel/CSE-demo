#pragma once
#ifndef __WORKER_H__
#define __WORKER_H__

#include <atomic>
#include <thread>

#include "job.h"
#include "manager.h"

class Worker{
public:
	Worker(Manager& manager, bool autostart=true);
	Worker(const Worker& other) = delete;

public:
	void startWork();
	void waitDone();

private:
	void workerThreadTask();
	void autoBalanceTask(const JobPtr& job);
	void histogramTask(const JobPtr& job);

private:
	JobPtr job;
	Manager& mgr;
	bool autostart;
	std::thread workerThread;

	int wid;
	static int wcount;
};

#endif // __WORKER_H__
