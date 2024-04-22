#include "manager.h"

Manager::Manager(cv::Mat& image,
	const size_t& hparts, const size_t& vparts):
	img(image), hparts(hparts), vparts(vparts),
	hdCount(hparts*vparts)
{}

JobPtr Manager::requestJob(){
	// printf("[Manager]: Job requested\n");
	return q.dequeue();
}

void Manager::updateHistoData(const HistoData hd){
	std::lock_guard<std::mutex> lock(mutex);
	this->hd.mergeWith(hd);
	--hdCount;
	cv.notify_one();
}

void Manager::run(){
	int w = img.cols / hparts;
	int h = img.rows / vparts;

	// 1. Generate histogram jobs
	for(int row = 0; row < vparts; ++row){
		for(int col = 0; col < hparts; ++col){
			cv::Mat pimg = img(cv::Rect(col*w, row*h, w, h));
			JobPtr job( new Job(pimg, JobType::Histogram));
			q.enqueue(job);
		}
	}
	printf("[Manager]: Histogram jobs generated\n");

	// 2. Wait until all histograms have been calculated
	std::unique_lock<std::mutex> lock(mutex);
	cv.wait(lock, [this]{ return this->hdCount == 0; });
	lock.unlock();
	printf("[Manager]: Histogram calculated and merged\n");

	// 3. Generate autobalance jobs
	for(int row = 0; row < vparts; ++row){
		for(int col = 0; col < hparts; ++col){
			cv::Mat pimg = img(cv::Rect(col*w, row*h, w, h));
			JobPtr job( new Job(std::move(pimg), JobType::AutoBalance) );
			job->setHistoData(this->hd);
			q.enqueue(job);
		}
	}
	printf("[Manager]: Autobalance jobs generated\n");

	 // 4. Generate stop jobs
	for(size_t i = 0; i < (hparts * vparts); ++i){
		cv::Mat noimg;
		JobPtr job( new Job(std::move(noimg), JobType::Quit) );
		q.enqueue(job);
	}
}
