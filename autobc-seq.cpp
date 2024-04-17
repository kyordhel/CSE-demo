#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "common.h"

int main(int argc, char** argv){
	const char* imgpath = (argc > 1) ? argv[1] : "lenna.png";
	cv::Mat input = cv::imread(imgpath, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);

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

	float scaleFactor = std::min(720.0 / input.rows, 1.0);
	cv::resize(input, input, cv::Size(input.cols*scaleFactor, input.rows*scaleFactor));
	cv::resize(output, output, cv::Size(output.cols*scaleFactor, output.rows*scaleFactor));
	cv::imshow("Original image", input);
	cv::imshow("Corrected image", output);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

