#pragma once
#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <atomic>
#include <thread>

#include "channel.h"
#include "histodata.h"

Channel<HistoData> typedef ChHD;
std::shared_ptr<ChHD> typedef ChHDptr;


class Element{
public:
	Element(const cv::Mat& srcImage,
		ChHDptr& north, ChHDptr& south,
		ChHDptr& east,  ChHDptr& west,
		const int& posX, const int& posY,
		const int& nElemX, const int& nElemY
	);
	Element(const Element& other) = delete;
	Element(Element&& other) = delete;

public:
	void startWork();
	void waitDone();

private:
	void propagate(HistoData& myHD);
	void workThreadTask();

private:
	cv::Mat img;
	int nElemX, nElemY;
	int posX, posY;
	int step;
	ChHDptr nc, ec, sc, wc;
	std::thread workingThread;
	std::atomic<bool> running;

	int ix;
	static int ecount;
};

#endif // __ELEMENT_H__
