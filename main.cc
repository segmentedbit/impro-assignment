#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "includes/stockpile.h"
#include "includes/statistics.h"
#include "includes/filters.h"

#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace cv;

int segmentedbit();
int ardillo(int argc, char** argv);

typedef chrono::high_resolution_clock highc;

int main(int argc, char** argv) {
	int returnValue = 0;

	///////// So far starting up code /////////

	returnValue = segmentedbit();
	//returnValue = ardillo(argc, argv);

	return returnValue;
}

int segmentedbit() {
	Mat image;
	String imageName = "images/Gray_image.jpg";
	image = imread("images/Gray_image.jpg", 1);
	if(!image.data ){
		std::cout << "Failed to load image " << imageName;
		exit(1);
	}

	Mat own_gray_image = im::grayscale(image);

	////////////////////////// Test correctness refactored averageFilter function

	namedWindow(imageName, CV_WINDOW_NORMAL);
	namedWindow("old", CV_WINDOW_NORMAL);
	namedWindow("new", CV_WINDOW_NORMAL);

	// Check is new addMatrix function does the same
	Mat oldIm = im::averageFilterDebug(own_gray_image, 3, 3, false);
	Mat newIm = im::averageFilter(own_gray_image, 3, 3, im::PZERO);
	cout << "Add functions are: " << ((im::equal(oldIm, newIm)) ? "equal" : "unequal") << endl;

	imshow(imageName,own_gray_image);
	imshow("old", oldIm);
	imshow("new", newIm);

	cout << "original" << endl;
	im::displayPixels(own_gray_image, false, false, im::DISPLAY_MATRIX);

	cout << "old version of averageFilter" << endl;
	im::displayPixels(oldIm, false, false, im::DISPLAY_MATRIX);

	cout << "new version of averageFilter" << endl;
	im::displayPixels(newIm, false, false, im::DISPLAY_MATRIX);

	//cout << "" << endl;
	//im::displayPixels(im::subtractMatrix(newIm,oldIm), false, false, im::DISPLAY_MATRIX);
	//im::displayPixels(im::subtractMatrix(oldIm,newIm), false, false, im::DISPLAY_MATRIX);

	waitKey(0);

	////////////////////////// Test correctness refactored add and subtract functions
	/*
	namedWindow(imageName, CV_WINDOW_NORMAL);
	namedWindow("added", CV_WINDOW_NORMAL);
	namedWindow("subtracted", CV_WINDOW_NORMAL);

	// Check is new addMatrix function does the same
	Mat added1 = im::addMatrix(own_gray_image, own_gray_image);
	Mat added2 = im::addMatrixOld(own_gray_image, own_gray_image);
	cout << "Add functions are: " << ((im::equal(added1, added2)) ? "equal" : "unequal") << endl;


	// Check is new subtractMatrix function does the same
	Mat subtracted1 = im::subtractMatrix(own_gray_image, own_gray_image);
	Mat subtracted2 = im::subtractMatrixOld(own_gray_image, own_gray_image);
	cout << "Subtract functions are: " << ((im::equal(subtracted1, subtracted2)) ? "equal" : "unequal") << endl;

	imshow(imageName,own_gray_image);
	imshow("added", added1);
	imshow("subtracted", subtracted1);
	 */

	////////////////////////// Speed test invert functions
	/*
		waitKey(0);
		Mat averaged = im::averageFilter(own_gray_image, 5, 5, im::PWHITE);

		namedWindow(imageName, CV_WINDOW_NORMAL);
		imshow(imageName,own_gray_image);

		namedWindow(imageName + " averaged", CV_WINDOW_NORMAL);
		imshow(imageName + " averaged",averaged);


		Mat invertedImage;
		auto s1 = highc::now();
		invertedImage = im::invert(own_gray_image);
		auto e1 = highc::now();

		auto s2 = highc::now();
		invertedImage = im::invertGray(own_gray_image);
		auto e2 = highc::now();


		auto timetaken1 = e1 - s1;
		auto timetaken2 = e2 - s2;
		cout << "Duration1: " << timetaken1.count() << endl <<
				"Duration2: " << timetaken2.count() << endl;

		// Check
		// http://stackoverflow.com/questions/14373934/iterator-loop-vs-index-loop
		// http://stackoverflow.com/questions/776624/whats-faster-iterating-an-stl-vector-with-vectoriterator-or-with-at
	*/



	/*
		// Means of creating a white image
		Mat imgWhite(512,512,CV_8UC1,Scalar(255));
		imgWhite = 255; // Assign the value 255 to the whole image

		// Means of creating a black image
		Mat imgBlack(512,512,CV_8UC1);
		// MatimgBlack(512,512,CV_8UC1,Scalar(0)); Same
		// imgWhite = 0; // Doesn't work for 0. Does work for 1 and above though
		// TODO edit Ardillo... above is true, but try to initialize with Mat::zeros(512,512,CV_8UC1);

		imshow("imgWhite", imgWhite);
		imshow("imgBlack", imgBlack);
		//waitKey();

		//Mat img;


		//img1.create(100,100,CV_8UC1);

		int offset = 25;
		imgBlack.row(offset) = 255; // Assigns the value 255 to the +offset row
		imgBlack.col(offset) = 255; // Assigns the value 255 to the +offset column
		imgBlack.row(imgBlack.size().width -1 -offset) = 255; // Assigns the value 255 to the -offset row (note the extra -1)
		imgBlack.col(imgBlack.size().height -1 -offset) = 255; // Assigns the value 255 to the -offset column (note the extra -1)

		imgBlack.col(1).copyTo(imgWhite.col(20));
		// imgBlack.col(1).copyTo(imgWhite.row(20)); Assertion failure. Don't try

		imshow("imgWhite", imgWhite);
		imshow("imgBlack", imgBlack);
		imshow("new", Mat (imgBlack, Rect(0, 0, 2*offset, 2*offset)));

		Mat bla = im::invert(imgWhite);
		imshow("new", bla);

		//Mat::clone
		//waitKey();
	*/
	return 0;
}

int ardillo(int argc, char** argv) {
	/*
	 * Ardillo's testing code
	 */

	// BASIC CODE
	if(argc <2){ //argc is het aantal parameters wat meegegeven wordt aan het programma
		std::cout << "No image data, or not enough parameters" ;
		return -1;
	}

	char *imageName = argv[1];
	Mat image;
	image = imread(imageName, 1);

	if(!image.data ){ //Image kon niet geladen worden, oid.
		std::cout << "Failed to load image " << imageName;
		return -1;
	}

	// EO BASIC CODE

	// OpenCV checks
	//		Mat gray_image;
	//		cvtColor(image, gray_image, CV_BGR2GRAY);

	//  TESTING SPACE
			Mat own_gray_image = im::grayscale(image);
			Mat inverted_image = im::invertGray(own_gray_image);
			Mat threshold = im::threshold(own_gray_image, 150);
			Mat adding = im::addMatrix(own_gray_image, threshold);
			Mat substracting = im::subtractMatrix(own_gray_image, threshold);
	//		Mat histOpenCVGrayImage = im::showHist(gray_image);
			Mat histOwnGrayImage = im::showHist(own_gray_image);
			long int start = im::getTime();
			Mat filtered1 = im::averageFilter(own_gray_image, 9, 9, im::PWHITE);
			long int stop = im::getTime();
			Mat filtered2 = im::averageFilter(own_gray_image, 3, 3, im::PWHITE);
			Mat substractAverage = im::subtractMatrix(filtered1, filtered2);
			Mat thresholdedSubstractAverage  = im::threshold(substractAverage, 15);

			//testje
			Mat kernel (3, 3, CV_8UC1);

			kernel.at<schar>(0,0) = -1;
			kernel.at<schar>(0,1) = -1;
			kernel.at<schar>(0,2) = -1;
			kernel.at<schar>(1,0) = -1;
			kernel.at<schar>(1,1) = 9;
			kernel.at<schar>(1,2) = -1;
			kernel.at<schar>(2,0) = -1;
			kernel.at<schar>(2,1) = -1;
			kernel.at<schar>(2,2) = -1;

			/*
			 *
			kernel.at<schar>(0,0) = 0;
			kernel.at<schar>(0,1) = 0;
			kernel.at<schar>(0,2) = 0;
			kernel.at<schar>(1,0) = 0;
			kernel.at<schar>(1,1) = 9;
			kernel.at<schar>(1,2) = 0;
			kernel.at<schar>(2,0) = 0;
			kernel.at<schar>(2,1) = 0;
			kernel.at<schar>(2,2) = 0;


			std::cout << kernel << std::endl;

			Mat customfilter = im::filter(filtered1, kernel);


	//		im::displayPixels(image, true, false);
	//		im::displayPixels(gray_image, false, false);
	//		im::displayPixels(own_gray_image, false, false);

			std::cout << "time: " << (stop - start)/1000 << " Micro seconds" << std::endl;

			namedWindow(imageName, CV_WINDOW_NORMAL);
	//		namedWindow("Gray_image", CV_WINDOW_NORMAL );
			namedWindow("own Gray_image", CV_WINDOW_NORMAL);
			namedWindow("inverted_image", CV_WINDOW_NORMAL);
			namedWindow("thresholded_image", CV_WINDOW_NORMAL);
			namedWindow("added_image", CV_WINDOW_NORMAL);
			namedWindow("substracted_image", CV_WINDOW_NORMAL);
	//		namedWindow("histogram: OpenCV grayImage", CV_WINDOW_NORMAL);
			namedWindow("histogram: Own grayImage", CV_WINDOW_NORMAL);
			namedWindow("averageFilter", CV_WINDOW_NORMAL);
			namedWindow("difference of averages", CV_WINDOW_NORMAL);
			namedWindow("thresholded DO-average", CV_WINDOW_NORMAL);
			namedWindow("custom Filter", CV_WINDOW_NORMAL);


			imshow (imageName , image);
	//		imshow( "Gray_image", gray_image);
			imshow("own Gray_image", own_gray_image);
			imshow("inverted_image", inverted_image);
			imshow("thresholded_image", threshold);
			imshow("added_image", adding);
			imshow("substracted_image", substracting);
	//		imshow("histogram: OpenCV grayImage", histOpenCVGrayImage);
			imshow("histogram: Own grayImage", histOwnGrayImage);
    		imshow("averageFilter", filtered1);
    		imshow("difference of averages", substractAverage);
    		imshow("thresholded DO-average", thresholdedSubstractAverage);
    		imshow("custom Filter", customfilter);


	waitKey(0);
	*/
	return 0;
}
