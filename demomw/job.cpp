#include "job.h"

Job::Job(cv::Mat img, const JobType& type):
	img(img), type(type){}


cv::Mat& Job::getImg(){
	return img;
}

JobType Job::getType(){
	return type;
}


HistoData& Job::getHistoData(){
	return hd;
}


void Job::setHistoData(const HistoData& hd){
	this->hd = hd;
}

