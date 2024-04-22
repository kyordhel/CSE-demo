#include "worker.h"
#include "common.h"

int Worker::wcount = 0;

Worker::Worker(Manager& manager, bool autostart):
	mgr(manager), autostart(autostart), wid(++wcount){

	if(autostart) startWork();
	// printf("Worker %d created\n", wid);
}

void Worker::startWork(){
	workerThread = std::thread(&Worker::workerThreadTask, this);
}


void Worker::waitDone(){
	if( workerThread.joinable() )
		workerThread.join();
}


void Worker::workerThreadTask(){
	// printf("Worker %d task started\n", wid);
	while(true){
		JobPtr job = mgr.requestJob();
		// printf("Worker %d got job %d\n", wid, (int)job->getType());
		switch( job->getType() ){
			case JobType::AutoBalance:
				autoBalanceTask(job);
				break;

			case JobType::Histogram:
				histogramTask(job);
				break;

			case JobType::Quit:
				return;
		}
	}
}


void Worker::autoBalanceTask(const JobPtr& job){
	cv::Mat& img = job->getImg();
	HistoData& hd = job->getHistoData();

	// Step 1: Adjust image
	for(int ch = 0; ch < 3; ++ch){
		float alpha = 255.0 / (hd.getMax(ch) - hd.getMin(ch));
		float beta  = -hd.getMin(ch) * alpha;
		adjustImage(img, alpha, beta, ch);
	}

	// Nothing else needs to be done.
}


void Worker::histogramTask(const JobPtr& job){
	cv::Mat& img = job->getImg();
	HistoData& hd = job->getHistoData();
	// Step 1: Calculate histogram of image segment
	histogram histo = getHistogram(img);

	// Step 2: Calculate minimum and maximum
	int min, max;
	for(int ch = 0; ch < 3; ++ch){
		getHistoMinMax(img.rows * img.cols, histo[ch], min, max, 0.001);
		hd.setMin(ch, min); hd.setMax(ch, max);
	}

	// Step 3: Send data to Manager
	mgr.updateHistoData(hd);
}

