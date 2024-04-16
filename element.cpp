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
	printf("Element %d started\n", this->ix);

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
	if(posX == 0) wc->send(HistoData());
	if(posY == 0) nc->send(HistoData());

	HistoData rcvHD;

	// Dir LR >
	ec->send(myHD);
	wc->fetch(rcvHD);
	myHD.mergeWith(rcvHD);
	printf("Element (%d,%d): Propagation LR complete!\n", posX, posY);

	// Dir UD v
	sc->send(myHD);
	nc->fetch(rcvHD);
	myHD.mergeWith(rcvHD);
	printf("Element (%d,%d): Propagation UD complete!\n", posX, posY);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	// Dir RL <
	wc->send(myHD);
	// printf("Element (%d,%d): Sent to west\n", posX, posY);
	ec->fetch(rcvHD);
	myHD.mergeWith(rcvHD);
	// printf("Element (%d,%d): Received from east\n", posX, posY);
	printf("Element (%d,%d): Propagation RL complete!\n", posX, posY);

	// Dir BU ^
	nc->send(myHD);
	// printf("Element (%d,%d): Sent to west\n", posX, posY);
	sc->fetch(rcvHD);
	// printf("Element (%d,%d): Received from east\n", posX, posY);
	myHD.mergeWith(rcvHD);

	printf("Element (%d,%d): Propagation complete!\n", posX, posY);
}
