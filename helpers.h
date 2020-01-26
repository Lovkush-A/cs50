#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

//Create copy of image
void copy(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE temp[height][width]);

//create 3x3 sub-array of array of image
void sub_array_3x3(int height, int width, RGBTRIPLE temp[height][width], int row, int column, RGBTRIPLE temp_3x3[3][3]);

//multiply kernel with 3x3 sub-array.
int mult_kernel_sub_array(int kernel[3][3], RGBTRIPLE temp_3x3[3][3], char colour);

