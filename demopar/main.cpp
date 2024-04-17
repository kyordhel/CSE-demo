#include <chrono>
#include <thread>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "common.h"

#define H_ELEMENTS 3
#define V_ELEMENTS 3
// C C C
// C C C
// C C C

void thread_work(cv::Mat& img, int row, int col);

int main(int argc, char** argv){
	const char* imgpath = (argc > 1) ? argv[1] : "img/lenna.png";
	cv::Mat input = cv::imread(imgpath, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

	auto start = std::chrono::high_resolution_clock::now();

	cv::Mat output = input.clone();

	// Generate worker threads
	const int w = input.cols / H_ELEMENTS;
	const int h = input.rows / V_ELEMENTS;
	cv::Mat parts[V_ELEMENTS][H_ELEMENTS];
	std::vector<std::thread> threads;
	threads.reserve(V_ELEMENTS*H_ELEMENTS);
	for(int row = 0; row < V_ELEMENTS; ++row){
		for(int col = 0; col < H_ELEMENTS; ++col){
			parts[row][col] = output(cv::Rect(col*w, row*h, w, h));
			std::thread t(thread_work, std::ref(parts[row][col]), row, col);
			threads.push_back( std::move(t) );
		}
	}

	printf("%lu threads launched\n", threads.size());

	for(auto& thread: threads)
		if(thread.joinable())thread.join();

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


void thread_work(cv::Mat& img, int row, int col){
	int min, max;
	histogram histo = getHistogram(img);
	for(int ch = 0; ch < 3; ++ch){
		getHistoMinMax(img.rows * img.cols, histo[ch], min, max, 0.001);
		float alpha = 255.0 / (max - min);
		float beta  = -min * alpha;
		adjustImage(img, alpha, beta, ch);
	}
	printf("Thread (%d,%d) done!\n", row, col);

}
