#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "includes/stockpile.h"
#include "includes/statistics.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	// Means of creating a white image
	Mat imgWhite(512,512,CV_8UC1,Scalar(255));
	imgWhite = 255; // Assign the value 255 to the whole image

	// Means of creating a black image
	Mat imgBlack(512,512,CV_8UC1);
	// MatimgBlack(512,512,CV_8UC1,Scalar(0)); Same
	// imgWhite = 0; // Doesn't work for 0. Does work for 1 and above though

	imshow("imgWhite", imgWhite);
	imshow("imgBlack", imgBlack);
	waitKey();

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
	waitKey();

	/*
	 * Ardillo's testing code
	 */
	char  *imageName = argv[1];

	Mat image;
	image = imread(imageName, 1);

	if(argc !=2 || !image.data ){ //argc is het aantal parameters wat meegegeven wordt aan het programma
		std::cout << "No image data, or not enough parameters" ;
		return -1;
	}

	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

		//self constructed method to loop through image, while looping output is shown in console
	   std::cout << " before ptr : " << &image << std::endl;

	    bool debug = false;
		im::displayPixels(image, true, debug);
		im::displayPixels(gray_image, false, debug);


	namedWindow(imageName, CV_WINDOW_NORMAL);
	namedWindow("Gray_image", CV_WINDOW_NORMAL );

	imshow (imageName , image);
	imshow( "Gray_image", gray_image);

	waitKey(0);


}
