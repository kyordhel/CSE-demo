#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include "element.h"
#include "channel.h"
#include "histodata.h"


//   C   C   C
// C E C E C E C
//   C   C   C
// C E C E C E C
//   C   C   C
// C E C E C E C
//   C   C   C
// 24×C, 9×E
#define H_ELEMENTS 3
#define V_ELEMENTS 3

int main(int argc, char** argv){
	const char* imgpath = (argc > 1) ? argv[1] : "img/lenna.png";
	cv::Mat input = cv::imread(imgpath, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH);
	auto start = std::chrono::high_resolution_clock::now();

	cv::Mat output = input.clone();

	// 1. Allocate array of elements
	ChHDptr vchannels[V_ELEMENTS+1][H_ELEMENTS];
	ChHDptr hchannels[V_ELEMENTS][H_ELEMENTS+1];

	// 2. Create and connect elements
	std::shared_ptr<Element> elmntArray[V_ELEMENTS][H_ELEMENTS];
	std::vector<std::shared_ptr<Element>> elements;
	elements.reserve(V_ELEMENTS*H_ELEMENTS);
	for(int row = 0; row < V_ELEMENTS; ++row){
		for(int col = 0; col < H_ELEMENTS; ++col){
			// If the channel does not exist, it is created here
			if( !vchannels[ row ][ col ] ) vchannels[ row ][ col ] = ChHDptr(new ChHD());
			if( !vchannels[row+1][ col ] ) vchannels[row+1][ col ] = ChHDptr(new ChHD());
			if( !hchannels[ row ][ col ] ) hchannels[ row ][ col ] = ChHDptr(new ChHD());
			if( !hchannels[ row ][col+1] ) hchannels[ row ][col+1] = ChHDptr(new ChHD());

			std::shared_ptr<Element> eptr(new Element(output,
				vchannels[row][col],
				vchannels[row+1][col],
				hchannels[row][col],
				hchannels[row][col+1],
				col, row, V_ELEMENTS, H_ELEMENTS
			));
			elmntArray[row][col] = eptr;
			elements.push_back(eptr);
		}
	}
	std::cout << "Elements created: " << elements.size() << std::endl;

/*
???
01?
???
 */

	// 3. Run
	for(auto eptr: elements)
		eptr->startWork();

	// 4. Wait for elements to finish
	for(auto eptr: elements)
		eptr->waitDone();


	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Image brightnes/contrast autobalance took " <<
	std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

	// cv::namedWindow("Original image", cv::WINDOW_NORMAL);
	// cv::namedWindow("Corrected image", cv::WINDOW_NORMAL);
	float scaleFactor = std::min(720.0 / input.rows, 1.0);
	cv::resize(input, input, cv::Size(input.cols*scaleFactor, input.rows*scaleFactor));
	cv::resize(output, output, cv::Size(output.cols*scaleFactor, output.rows*scaleFactor));
	cv::imshow("Original image", input);
	cv::imshow("Corrected image", output);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}