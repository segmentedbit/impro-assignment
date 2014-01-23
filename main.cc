#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "includes/stockpile.h"
#include "includes/statistics.h"
#include "includes/filters.h"
#include "includes/morphology.h"
#include "includes/images.h"

#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace cv;

int program(int argc, char** argv);

typedef chrono::high_resolution_clock highc;

int main(int argc, char** argv) {
	int returnValue = 0;

	// starting the program //
	returnValue = program(argc, argv);

 	return returnValue;
}

int program(int argc, char** argv) {

	// BASIC CODE
	if (argc < 2) {
		std::cout << "No image data, or not enough parameters";
		return -1;
	}

	char *imageName = argv[1];
	string input = imageName;
	Mat image;
	image = imread(imageName, 1);

	if (!image.data) {
		std::cout << "Failed to load image " << imageName;
		return -1;
	}

	///// String list
	string balloons = "images/assignmentPictures/balloons.png";
	string balls = "images/assignmentPictures/balls.png";
	string cheese ="images/assignmentPictures/cheese.png";
	string xray = "images/assignmentPictures/xray.png";
	string boltsnuts ="images/assignmentPictures/boltsnuts.png";
	string road = "images/assignmentPictures/road.png";

	///// checks if standard picture method is applicable to image name
	 if (!input.compare(balloons)) {
		solve::balloons(image);
	}
	 else if (!input.compare(cheese)) {
		solve::cheese(image);
	 }
	else if (!input.compare(balls)) {
		solve::balls(image);
	}
	else if (!input.compare(xray)) {
		solve::xray(image);
	}
	else if (!input.compare(boltsnuts)) {
		solve::boltsnuts(image);
	}
	else if(!input.compare(road)) {
		solve::road(image);
	}

	/////////////// EO BASIC OpenCV CODE
	else{
		/*
		 *  TESTING SPACE
		 *  You can manually write code here, this will be executed as long as
		 *  the input image isn't one of the images mentioned in the string
		 *  list above.
		 */

	}
	waitKey(0);
	return 0;
}
