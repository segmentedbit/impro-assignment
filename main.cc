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
		std::cout << "\n\nNot enough parameters. Usage:\n\n" << myName << " <pipeline> <image> [-d]\nFor usage information, please see the supplied \"user manual\"\n"
				<< "The program needs at least two parameters in order to run. The third option '-d'\nis optional.\n\n"
				<< "\t<pipeline_name>\n\tThere are five different names, according with each image, these are:\n"
				<<"\t- cheese\n"
				<<"\t- balls\n"
				<<"\t- xray\n"
				<<"\t- boltsnuts\n"
				<<"\t- road\n\n"
				<<"\t<image>\n"
				<<"\tAccording to the chosen pipeline, you need to give the path of the\n"
				<<"\timage located in the 'images' directory, the possible options are:\n"
				<<"\t- images/cheese.png\n"
				<<"\t- images/balls.png\n"
				<<"\t- images/xray.png ; images/xray2.png ; images/xray3.png\n"
				<<"\t- images/boltsnuts.png\n"
				<<"\t- images/road.png\n\n"
				<<"\t[-d]\n"
				<<"\tConcluding the command with '-d' gives the debug output of the chosen\n"
				<<"\tpipeline. We have kept the default output to a minimum and made an\n"
				<<"\textra option to print some pipeline information if wanted.\n"
				<< endl;
		return -1;
	}

	// Turn debug on when requested
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
	if (!input.compare("cheese")) {
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
