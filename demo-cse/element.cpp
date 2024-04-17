#include <utility>

#include <chrono>

#include "element.h"
#include "common.h"

int Element::ecount = 0;

Element::Element(const cv::Mat& srcImage,
				ChHDptr& north, ChHDptr& south,
				ChHDptr& west,  ChHDptr& east,
				const int& posX, const int& posY,
				const int& nElemX, const int& nElemY):
	step(0), running(false),
	nc(north), sc(south),
	wc(west),  ec(east),
	posX(posX), posY(posY), nElemX(nElemX), nElemY(nElemY)
{
	nc = north;
	int w = srcImage.cols / nElemX;
	int h = srcImage.rows / nElemY;

	// img = srcImage(cv::Rect(posX*w, posY*h, w, h));
	img = srcImage(cv::Rect(posX*w, posY*h, w, h));
	ix = ++Element::ecount;
	// std::cout << "Created Element " << ix << std::endl;
	// printf("Created Element %d: %dx%d%+d%+d\n", ix, w, h, posX*w, posY*h );

}


void Element::startWork(){
	if(running) return;
	running = true;
	workingThread = std::thread(&Element::workThreadTask, this);
}

void Element::waitDone(){
	if(workingThread.joinable())
		workingThread.join();
}

void Element::workThreadTask(){
	// Step 1: Calculate histogram of image segment
	histogram histo = getHistogram(img);

	// Step 2: Fetch histogram data for the segment
	int min, max;
	HistoData myHD;
	for(int ch = 0; ch < 3; ++ch){
		getHistoMinMax(img.rows * img.cols, histo[ch], min, max, 0.001);
		myHD.setMin(ch, min); myHD.setMax(ch, max);
	}
	// Step 3: Request neighbors for their data
	propagate(myHD);
	propagate(myHD);

	// Step 4: Adjust image
	for(int ch = 0; ch < 3; ++ch){
		float alpha = 255.0 / (myHD.getMax(ch) - myHD.getMin(ch));
		float beta  = -myHD.getMin(ch) * alpha;
		adjustImage(img, alpha, beta, ch);
	}

	// printf("Element %d work completed\nSize: %dx%d, min: %d, max: %d\n",
	// 	this->ix, img.cols, img.rows, min, max);
	running = false;
}


void Element::propagate(HistoData& myHD){
	// Top and left elements inject dummy data to prevent deadlocks
	if(posX == 0) wc->sendOne(HistoData());
	if(posY == 0) nc->sendOne(HistoData());
	if(posX == (nElemX-1)) ec->sendTwo(HistoData());
	if(posY == (nElemY-1)) sc->sendTwo(HistoData());

	HistoData rcvHD;

	// Dir LR >
	ec->sendOne(myHD);
	wc->fetchOne(rcvHD);
	myHD.mergeWith(rcvHD);

	// Dir UD v
	sc->sendOne(myHD);
	nc->fetchOne(rcvHD);
	myHD.mergeWith(rcvHD);

	// Dir RL <
	wc->sendTwo(myHD);
	ec->fetchTwo(rcvHD);
	myHD.mergeWith(rcvHD);

	// Dir BU ^
	nc->sendTwo(myHD);
	sc->fetchTwo(rcvHD);
	myHD.mergeWith(rcvHD);
}
