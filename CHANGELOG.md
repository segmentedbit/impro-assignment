###27-12-2013 Ardillo<br>
added 'display pixels' and 'greyscale' in statistics.cc also added the 'main()' to test it.

###28-12-2013 Ardillo<br>
added more functionality: thresholding, inverse, histogram, add, substract.<br>
added an auto-zoom thing (very ugly) for auto-scaling the histogram matrix output.<br>
Removed auto-zoom, final histogram adjustments, works fine now.

###29-12-213 Ardillo<br>
Found some info that calculating gray-values is a bit more than just adding RGB 
values and divide them by 3.<br>
There need te be some kind of factor:
- float B = 0.1140 * input.at<Vec3b>(i,j)[0];
- float G = 0.5870 * input.at<Vec3b>(i,j)[1];
- float R = 0.2989 * input.at<Vec3b>(i,j)[2];<br>
it has something to do with the frequency space in the color spectrum. 
Google CIE 1931 standard of colorspaces.
Yes.. it originates from 1931, back then already people figured this out.
Look for more info:
http://stackoverflow.com/questions/687261/converting-rgb-to-grayscale-intensity
https://en.wikipedia.org/wiki/Gamma_correction
http://www.cis.rit.edu/mcsl/research/broadbent/CIE1931_RGB.pdf
https://en.wikipedia.org/wiki/CIE_1931_color_space

Began with making a filter with variable kernel size and padding in temporary matrix

###31-12-2013 Ardillo<br>
Finished an average filter. It is not the fastest method to use on HD wallpapers,
but it works good enough for our project.<br>
Also used earlier made functions to substract two different averaged images, then
thresholded the result.
The result is a nice edge image.

###01-01-2014 Ardillo<br>
Started a general filter function with a manual kernel. Does not work yet, due 
to problems with different data-types of matrices. If kernel has negative numbers
some artifacts will occur.

###08-01-2014 segmentedbit<br>
Cleaned ip averageFilter. Introduced a C++11 way of measuring time, which may be
better. Introduced an header file that can be used to set global variables, 
like debugging yes/no. Also added a function 'copyWithPadding', which allows 
for copying and adding padding at the same time.
This function still needs some work though.

###08-01-2014 segmentedbit<br>
Checked all functions in stockpile.cc. Didn't check filter(), remains TODO for 
when we need it.<br>
Test code in main in separated now. Added a function 'equal' to check whether 
images are equal, which is useful for checking if refactorings don't break code.

###10-01-2014 segmentedbit<br>
Fixed minor rounding bug in averageFilter. Slightly changed displayPixels() to 
accept an extra, optional, parameter on how to print the pixels. Also added 
blueprints for some morphology functions.

###10-01-2014 segmentedbit<br>
Added morphDilate() and helper function defaultElement() which creates a 3x3 "+" 
shaped structuring element. Also added a validateKernel() function to streamline 
validating kernels. Changed and therefore improved the quality of the checkboard 
image into png format, which avoids the compression noise jpeg introduces.

###12-01-2014 Ardillo<br>
Made a histogram equalize function, found some information about matrix data 
types: http://docs.opencv.org/modules/core/doc/basic_structures.html#mat-depth<br>

###12-01-2014 segmentedbit<br>
Created morphErode(), morphOpen(), morphClose() and morphGeodesicDilate(). Also 
removed some old code

###14-01-2014 Ardillo<br>
Started a binaryLabel function. I'm not far with this because of the problem 
how to registrate the objects which I've already scanned. Maybe do some reading
first. I adjusted im::filter to the latest standard functions, I will rewrite 
this im::filter function to calculate the x / y derivative.

###15-01-2014 
####segmentedbit<br>
Enhanced morpGeodesicDilate(), issue 17. The function lost the parameter for the
structuring element. Also added error catching in copyWithPadding, to ease 
debugging.

####Ardillo<br>
Adjusted im::filter so it will work with custom kernels. -- added float divide_factor<br>
Made function im::matUcharToFloat <br>
Made function im::matFloatToUchar -- and fixed a bug <br>
Made some standard filter kernels <br>
Started with Quantization -- Work in progress still <br>

###15-01-2014 segmentedbit<br>
Added skeleton and several helper functions. Skeleton doesn't work yet, though.<br>

###16-01-2014 Ardillo<br>
Made function im::maxFloatValue to find max value in floating point matrix <br>
Made function im::minFloatValue to find min value in floating point matrix <br>
Finished Quantization function, found stable till +/- 40 levels of quantizing.<br>
Fixed thinning a bit, problem was recursive matrices weren't updated and some don't care issues 
the order of which kernel you take first does matter.<br>

###18-01-2014 segmentedbit<br>
Researched creating skeletons further and it simply takes too long to create a good
working one within the time we have. Taken three nice pieces of code, not sure yet 
which one performs best in our situation. Changed the createGolay function to take
a parameter so it can return different alphabets, cleaned our own skeleton function.
Also added pruning - almost the same as skeleton, for testing the influence of pruning
within skeletonizing cycles.<br>

###20-01-2014 Ardillo<br>
Made an .cc file to process the planned images for the exercise. 
Used some own developed functions and some OpenCV functions. Dit not come to a 
good result yet, seems hard to get a good result.<br>

###21-01-2014 Ardillo<br>
Made local minimum of local maximum filter to filter shading and shadows - Done<br>
added local maximum of minimum <br>
Added function divideMatrix pixelwise division. <br>

###21-01-2014 segmentedbit<br>
Added function gaussianKernel()<br>
Added some testing code in the segmentedbit main() flow, to compare how well the four
available skeleton operations perform compared to each other. Note that depending on the
image, you have to set the "flip" flag, which calls invertGray() on the input image.<br>
Created medianFilter. Gotta love border control.<br>
