#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "includes/stockpile.h"
#include "includes/statistics.h"
#include "includes/filters.h"
#include "includes/morphology.h"

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
	//String imageName = "images/Gray_image.png";
	String imageName = "images/lena.jpg";
	image = imread(imageName, 1);
	if (!image.data) {
		std::cout << "Failed to load image " << imageName;
		exit(1);
	}

	Mat gray = imread("images/orca.png", 1);
	gray = im::grayscale(gray);

	Mat quant = im::quantization(gray, 2);
	namedWindow("quant", CV_WINDOW_NORMAL);
	imshow("quant", quant);

	/*
	////////////////////////// Test morphSkeleton()
	Mat gray = imread("images/white.png", 1);
	gray = im::grayscale(gray);

	namedWindow("original", CV_WINDOW_NORMAL);
	namedWindow("skeleton", CV_WINDOW_NORMAL);
	namedWindow("segmentized", CV_WINDOW_NORMAL);

	Mat segmentized = im::segmentize(gray, 30);

	Mat skeleton = im::morphSkeleton(segmentized);
	im::displayPixels(segmentized, false, false, im::DISPLAY_MATRIX);
	im::displayPixels(skeleton, false, false, im::DISPLAY_MATRIX);
	imshow("original", gray);
	imshow("skeleton", skeleton);
	imshow("segmentized", segmentized);
	waitKey(0);
	*/

	/*
	////////////////////////// Test morphGeodesicDilate()
	Mat element = Mat::ones(7, 7, CV_8UC1);
	Mat defaultElement = im::defaultElement();

	Mat gray = imread("images/Gray_image.png", 1);
	gray = im::grayscale(gray);
	Mat grayEroded = im::morphErode(gray);
	Mat grayGeoDil = im::morphGeodesicDilate(grayEroded, gray);

	namedWindow("original", CV_WINDOW_NORMAL);
	namedWindow("grayEroded", CV_WINDOW_NORMAL);
	namedWindow("grayGeoDil", CV_WINDOW_NORMAL);

	imshow("original", gray);
	imshow("grayEroded", grayEroded);
	imshow("grayGeoDil", grayGeoDil);

	cout << "images are: " << ((im::equal(grayGeoDil, gray)) ? "equal"
			: "unequal") << endl;

	waitKey(0);
	 */

	/*
	 ////////////////////////// Test morphErode()
	 Mat own_gray_image = im::grayscale(image);
	 Mat element = Mat::ones(7,7, CV_8UC1);
	 Mat defaultElement = im::defaultElement();

	 namedWindow("original", CV_WINDOW_NORMAL);
	 namedWindow("opened", CV_WINDOW_NORMAL);
	 namedWindow("closed", CV_WINDOW_NORMAL);

	 Mat opened = im::morphOpen(own_gray_image, element, element);
	 Mat closed = im::morphClose(own_gray_image, element, element);

	 imshow("original", own_gray_image);
	 imshow("opened", opened);
	 imshow("closed", closed);

	 waitKey(0);
	 */

	/*
	 ////////////////////////// Test morphErode()
	 Mat own_gray_image = im::grayscale(image);
	 Mat element = Mat::ones(7,7, CV_8UC1);
	 Mat defaultElement = im::defaultElement();

	 namedWindow("structuring element", CV_WINDOW_NORMAL);
	 namedWindow("original", CV_WINDOW_NORMAL);
	 namedWindow("eroded", CV_WINDOW_NORMAL);

	 Mat eroded = im::morphErode(own_gray_image, element);

	 imshow("structuring element", defaultElement);
	 imshow("original", own_gray_image);
	 imshow("eroded", eroded);

	 im::displayPixels(own_gray_image, false, false, im::DISPLAY_MATRIX);
	 im::displayPixels(defaultElement, false, false, im::DISPLAY_MATRIX);
	 im::displayPixels(eroded, false, false, im::DISPLAY_MATRIX);
	 waitKey(0);
	 */

	/*
	 ////////////////////////// Test for morphDilate()
	 Mat element = Mat::ones(5,5, CV_8UC1);

	 Mat defaultElement = im::defaultElement();
	 namedWindow("structuring element", CV_WINDOW_NORMAL);
	 imshow("structuring element", defaultElement);
	 im::displayPixels(defaultElement, false, false, im::DISPLAY_MATRIX);

	 Mat own_gray_image = im::grayscale(image);

	 namedWindow("original", CV_WINDOW_NORMAL);
	 namedWindow("dilated", CV_WINDOW_NORMAL);

	 Mat dilated = im::morphDilate(own_gray_image, element);
	 imshow("original", own_gray_image);
	 imshow("dilated", dilated);
	 im::displayPixels(dilated, false, false, im::DISPLAY_MATRIX);
	 waitKey(0);
	 */

	////////////////////////// Test correctness refactored averageFilter function
	/*
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
	 */

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

	return 0;
}

int ardillo(int argc, char** argv) {
	/*
	 * Ardillo's testing code
	 */

	// BASIC CODE
	if (argc < 2) { //argc is het aantal parameters wat meegegeven wordt aan het programma
		std::cout << "No image data, or not enough parameters";
		return -1;
	}

	char *imageName = argv[1];
	Mat image;
	image = imread(imageName, 1);

	if (!image.data) {
		std::cout << "Failed to load image " << imageName;
		return -1;
	}

	// EO BASIC CODE

	//  TESTING SPACE
	namedWindow(imageName, CV_WINDOW_NORMAL);
	imshow(imageName, image);

	Mat own_gray_image = im::grayscale(image);
	namedWindow("own Gray_image", CV_WINDOW_NORMAL);
	imshow("own Gray_image", own_gray_image);

	Mat histOwnGrayImage = im::showHist(own_gray_image);
	namedWindow("histogram: Own grayImage", CV_WINDOW_NORMAL);
	imshow("histogram: Own grayImage", histOwnGrayImage);

	Mat equalized = im::equalize(own_gray_image);
	namedWindow("equalized", CV_WINDOW_NORMAL);
	imshow("equalized", equalized);


	//Mat histOwnGrayImage2 = im::showHist(equalized);
	//namedWindow("histogram: equalized", CV_WINDOW_NORMAL);
	//imshow("histogram: equalized", histOwnGrayImage2);


	/*

	///////////// x-Derivative ////////////////////
	Mat kernel = ( Mat_<float>(1,5) <<
		1, -8, 0, 8, -1);
		float divide_fact = 12;

	///////////// y-Derivative ////////////////////
	Mat kernel = ( Mat_<float>(5,1) <<
		1, -8, 0, 8, -1);
		float divide_fact = 12;

	//////////////// sharpen //////////////////////
	Mat kernel = ( Mat_<float>(3,3) <<
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0);
		float divide_fact = 12;

	////////////// Gaussian kernel /////////////////
	Mat kernel = ( Mat_<float>(5,5) <<
		1, 4, 7, 4, 1,
		4, 16, 26, 16, 4,
		7, 26, 41, 26, 7,
		4, 16, 26, 16, 4,
		1, 4, 7, 4, 1);
		float divide_fact = 273;

	*/

	auto t1 = highc::now();
	Mat quant = im::quantization(own_gray_image, 8);
	auto t2 = highc::now();
	namedWindow("Quantization", CV_WINDOW_NORMAL);
	imshow("Quantization", quant);

	auto timetaken1 = t2 - t1;
	cout << "time taken: " << timetaken1.count() << endl;

	Mat histOwnGrayImage2 = im::showHist(quant);
	namedWindow("histogram: quantized", CV_WINDOW_NORMAL);
	imshow("histogram: quantized", histOwnGrayImage2);



	waitKey(0);

	return 0;
}
