#pragma once
#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <mutex>
#include <condition_variable>

#include "job.h"
#include "syncq.h"
#include "histodata.h"

class Manager{
public:
	Manager(cv::Mat& image, const size_t& hparts, const size_t& vparts);
	Manager(const Manager&) = delete;
	Manager(Manager&&) = delete;

public:
	JobPtr requestJob();
	void updateHistoData(const HistoData hd);
	void run();

private:
	cv::Mat& img;
	HistoData hd;
	size_t hparts;
	size_t vparts;
	SyncQ<JobPtr> q;
	size_t hdCount;

	std::mutex mutex;
	std::condition_variable cv;
};

#endif // __MANAGER_H__
