#include <cmath>
#include "common.h"

histogram getHistogram(const cv::Mat& img, int channel){
	histogram histo{};

	for(int row = 0; row < img.rows; ++row){
		for(int col = 0; col < img.cols; ++col){
			cv::Vec3b pix = img.at<cv::Vec3b>(row, col);
			histo[0][pix[0]]++;
			histo[1][pix[1]]++;
			histo[2][pix[2]]++;
		}
	}
	return histo;
}

void getHistoMinMax(const size_t& pixelCount, const std::array<int,255>& histo, int& min, int& max, float percent){
	if( (percent <= 0) || (percent >= 1) ) percent = 0.01;
	// Get pixel count
	for(min = 0; min < histo.size(); ++min){
		if( histo[min] >= (percent*pixelCount) )
			break;
	}
	for(max = histo.size(); max >= 0; --max){
		if( histo[max] >= (percent*pixelCount) )
			break;
	}
}

void adjustImage(cv::Mat& img, const float& alpha, const float& beta, int channel){
	for(int row = 0; row < img.rows; ++row){
		for(int col = 0; col < img.cols; ++col){
			auto pixel = img.at<cv::Vec3b>(row, col);
			int v = alpha * pixel[channel] + beta;
			if(v < 0) v = 0;
			if(v > 255) v = 255;
			pixel[channel] = v;
			img.at<cv::Vec3b>(row, col) = pixel;
		}
	}
}
