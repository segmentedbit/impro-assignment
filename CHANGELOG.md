###27-12-2013 Ardillo<br>
added 'display pixels' and 'greyscale' in statistics.cc also added the 'main()' to test it.

###28-12-2013 Ardillo<br>
added more functionality: thresholding, inverse, histogram, add, substract.<br>
added an auto-zoom thing (very ugly) for auto-scaling the histogram matrix output.<br>
Removed auto-zoom, final histogram adjustments, works fine now.

###29-12-213 Ardillo<br>
Found some info that calculating gray-values is a bit more than just adding RGB values and divide them by 3.<br>
There need te be some kind of factor:
- float B = 0.1140 * input.at<Vec3b>(i,j)[0];
- float G = 0.5870 * input.at<Vec3b>(i,j)[1];
- float R = 0.2989 * input.at<Vec3b>(i,j)[2];<br>
it has something to do with the frequency space in the color spectrum. Google CIE 1931 standard of colorspaces.
Yes.. it originates from 1931, back then already people figured this out.
Look for more info:
http://stackoverflow.com/questions/687261/converting-rgb-to-grayscale-intensity
https://en.wikipedia.org/wiki/Gamma_correction
http://www.cis.rit.edu/mcsl/research/broadbent/CIE1931_RGB.pdf
https://en.wikipedia.org/wiki/CIE_1931_color_space

Began with making a filter with variable kernel size and padding in temporary matrix

###31-12-2013 Ardillo<br>
Finished an average filter. It is not the fastest method to use on HD wallpapers, but it works good enough for our project.<br>
Also used earlier made functions to substract two different averaged images, then thresholded the result.
The result is a nice edge image.

###01-01-2014 Ardillo<br>
Started a general filter function with a manual kernel. Does not work yet, due to problems with different data-types
of metrices. If kernel has negative numbers some artifacts will occur.

###08-01-2014 segmentedbit<br>
Cleaned ip averageFilter. Introduced a C++11 way of measuring time, which may be better. Introduced an header file that can be used to
set global variables, like debugging yes/no. Also added a function 'copyWithPadding', which allows for copying and adding padding at the same time.
This function still needs some work though.

###08-01-2014 segmentedbit<br>
Checked all functions in stockpile.cc. Didn't check filter(), remains TODO for when we need it.<br>
Test code in main in separated now. Added a function 'equal' to check whether images are equal, which is useful for checking if refactorings don't break code.

###10-01-2014 segmentedbit<br>
Fixed minor rounding bug in averageFilter. Slightly changed displayPixels() to accept an extra, optional, parameter on how to print the pixels. Also added blueprints for some morphology functions.

###10-01-2014 segmentedbit<br>
Added morphDilate() and helper function defaultElement() which creates a 3x3 "+" shaped structuring element. Also added a validateKernel() function to streamline validating kernels. Changed and therefore improved the quality of the checkboard image into png format, which avoids the compression noise jpeg introduces.
