#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include <array>
#include <opencv2/core.hpp>

template <class T, unsigned I, unsigned J>
using Matrix = std::array<std::array<T, J>, I>;
using histogram = Matrix<int, 3, 255>;

histogram getHistogram(const cv::Mat& img, int channel = 0);
void getHistoMinMax(const size_t& pixelCount, const std::array<int,255>& histo, int& min, int& max, float percent=0.01);
void adjustImage(cv::Mat& img, const float& alpha, const float& beta, int channel = 0);

#endif // __COMMON_H__