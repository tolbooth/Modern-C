#ifndef IMAGE_SEGMENTATION_H
#define IMAGE_SEGMENTATION_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "image_processing.h"

/*
The goal of this challenge is to perform a segmentation of such an image: to group the pixels (the unsigned char elements of the matrix)
into connected regions that are “similar” in some sense or another. Such a segmentation forms a partition of the set of pixels, much as
we saw in challenge 4. Therefore, you should use a Union-Find structure to represent regions, one per pixel at the start:

	- Can you implement a statistics function that computes a statistic for all regions? This should
	be another array (the third array in the game) that for each root holds the number of pixels and
	the sum of all values.

	- Can you implement a merge criterion for regions? Test whether the mean values of two regions
	are not too far apart: say, no more than five gray values.
	
	- Can you implement a line-by-line merge strategy that, for each pixel on a line of the image,
	tests whether its region should be merged to the left and/or to the top? Can you iterate line
	by line until there are no more changes: that is, such that the resulting regions/sets all test
	negatively with their respective neighboring regions? Now that you have a complete function for
	image segmentation, try it on images with assorted subjects and sizes, and also vary your merge
	criterion with different values for the mean distance instead of five. 
*/

#endif