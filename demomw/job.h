#pragma once
#ifndef __JOB_H__
#define __JOB_H__

#include <opencv2/core.hpp>

#include "histodata.h"

enum class JobType{
	Histogram,
	AutoBalance,
	Quit
};

class Job{
public:
	Job(cv::Mat img, const JobType& type);
	Job(const Job& other) = delete;

public:
	JobType getType();
	cv::Mat& getImg();
	HistoData& getHistoData();
	void setHistoData(const HistoData& hd);

private:
	cv::Mat img;
	JobType type;
	HistoData hd;
};

typedef std::shared_ptr<Job> JobPtr;

#endif // __JOB_H__
