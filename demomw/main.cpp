#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "worker.h"
#include "manager.h"

#define V_PARTS 3
#define H_PARTS 3
#define N_WORKERS 4

int main(int argc, char** argv){
	const char* imgpath = (argc > 1) ? argv[1] : "img/lenna.png";
	cv::Mat input = cv::imread(imgpath, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
	auto start = std::chrono::high_resolution_clock::now();

	cv::Mat output = input.clone();

	// 1. Allocate vector of workers
	std::vector<std::shared_ptr<Worker>> workers;
	workers.reserve(N_WORKERS);


	// 2. Create manager and workers
	Manager manager(output, H_PARTS, V_PARTS);
	printf("Manager created\n");
	for(size_t i = 0; i < N_WORKERS; ++i){
		std::shared_ptr<Worker> wptr( new Worker(manager) );
		workers.push_back(wptr);
	}
	std::cout << "Workers created: " << workers.size() << std::endl;

	// 3. Run
	manager.run();

	// 4. Wait for workers to finish
	for(auto& wptr: workers)
	// 	wptr.waitDone();
		wptr->waitDone();


	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Image brightnes/contrast autobalance took " <<
	std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

	float scaleFactor = std::min(720.0 / input.rows, 1.0);
	cv::resize(input, input, cv::Size(input.cols*scaleFactor, input.rows*scaleFactor));
	cv::resize(output, output, cv::Size(output.cols*scaleFactor, output.rows*scaleFactor));
	cv::imshow("Original image", input);
	cv::imshow("Corrected image", output);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}