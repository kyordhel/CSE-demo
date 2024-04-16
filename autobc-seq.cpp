#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "common.h"

int main(){
	cv::Mat input = cv::imread("lenna.png", cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

	auto start = std::chrono::high_resolution_clock::now();

	int min, max;
	cv::Mat output = input.clone();
	histogram histo = getHistogram(output);
	for(int ch = 0; ch < 3; ++ch){
		getHistoMinMax(output.rows * output.cols, histo[ch], min, max, 0.001);
		float alpha = 255.0 / (max - min);
		float beta  = -min * alpha;
		adjustImage(output, alpha, beta, ch);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Image brightnes/contrast autobalance took " <<
	std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

	cv::imshow("Original image", input);
	cv::imshow("Corrected image", output);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

