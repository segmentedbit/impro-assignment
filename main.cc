/*
 * images.c
 *
 *  Created on: Jan 20, 2014
 *      Author: Ardillo & segmentedbit
 */

#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "includes/stockpile.h"
#include "includes/statistics.h"
#include "includes/filters.h"
#include "includes/morphology.h"
#include "includes/images.h"
#include "includes/config.h"

using namespace std;
using namespace cv;

int program(int argc, char** argv);

typedef chrono::high_resolution_clock highc;

int main(int argc, char** argv) {

	// starting the program //
	int returnValue = program(argc, argv);

 	return returnValue;
}

int program(int argc, char** argv) {

	// BASIC CODE
	if (argc < 3) {
		string myName = argv[0];
		size_t loc = myName.find_last_of("/");
		if (loc != myName.npos) {
			myName = myName.substr(loc + 1);
		}
		std::cout << "Not enough parameters. Usage:\n" << myName << " <sequence> <image>\nFor usage information, please see the supplied \"Usage document\"" << endl;
		return -1;
	}

	// Turn debug on when requested
	cout << argc;
	if (argc == 4) {
		string debug = argv[3];
		if (!debug.compare("-d"))
			config::DEBUG = true;
	}

	string imageName = argv[2];
	string input = argv[1];
	Mat image;
	image = imread(imageName, 1);

	if (!image.data) {
		std::cout << "Failed to load image " << imageName;
		return -1;
	}

	///// checks what procedure should be run.
	 if (!input.compare("balloons")) {
		solve::balloons(image);
	}
	 else if (!input.compare("cheese")) {
		solve::cheese(image);
	 }
	else if (!input.compare("balls")) {
		solve::balls(image);
	}
	else if (!input.compare("xray")) {
		solve::xray(image);
	}
	else if (!input.compare("boltsnuts")) {
		solve::boltsnuts(image);
	}
	else if(!input.compare("road")) {
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
