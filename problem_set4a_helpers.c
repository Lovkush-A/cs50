#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include <math.h> //needed for square root


//declare functions
//Create copy of image
void copy(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE temp[height][width]);

//create 3x3 sub-array of array of image
//returns "size" of sub-array
//size is 9 for subarrays from centre of images, size of 4 for subarrays from corners, size of 6 for subarrays from sides.
int sub_array_3x3(int height, int width, RGBTRIPLE temp[height][width], int row, int column, RGBTRIPLE temp_3x3[3][3]);

//multiply kernel with 3x3 sub-array.
int mult_kernel_sub_array(int kernel[3][3], RGBTRIPLE temp_3x3[3][3], char colour);

//do sobel operation on 3x3 sub-array. returns new rgb value
int sobel(RGBTRIPLE temp_3x3[3][3], char colour);




// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = (int) round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float) 3);
            image[i][j].rgbtGreen = image[i][j].rgbtBlue;
            image[i][j].rgbtRed = image[i][j].rgbtBlue;
        }
    }
    return;
}



// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //create new variable temp which will store copy of image
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        printf("assigning memory for temporary copy failed\n");
        return;
    }

    //copy image into temp
    copy(height, width, image, temp);

    //do reflection
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][width-1-j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][width-1-j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][width-1-j].rgbtRed;
        }
    }

    //free memory
    free(temp);

    return;
}



// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //create new variable temp which will store copy of image
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        printf("assigning memory for temp failed\n");
        return;
    }

    //copy image into temp
    copy(height, width, image, temp);

    //create new variable temp_3x3 which will store 3x3 sub arrays
    RGBTRIPLE(*temp_3x3)[3] = calloc(3, 3*sizeof(RGBTRIPLE));
    if (temp_3x3 == NULL)
    {
        printf("assigning memory for temp_3x3 failed\n");
        return;
    }

    //create variable kernel to multiply with temp_3x3
    //this kernel is equivalent to calculating the sum of the values of sub-array
    int kernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    //variable n stores size of sub array.
    //either 9 or 6 or 4 depending on if pixel is on corner or edge or middle of image
    //required for determining average. we will divide total by n
    int n = 9;

    //go through array and calculate new rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //create sub array centred on pixel i-j
            n = sub_array_3x3(height, width, temp, i, j, temp_3x3);

            //for each colour, multiply kernel with sub array and divide by n.
            image[i][j].rgbtBlue = (int) round(mult_kernel_sub_array(kernel, temp_3x3, 'b') / (float) n);
            image[i][j].rgbtGreen = (int) round(mult_kernel_sub_array(kernel, temp_3x3, 'g') / (float) n);
            image[i][j].rgbtRed = (int) round(mult_kernel_sub_array(kernel, temp_3x3, 'r') / (float) n);
        }
    }

    //free memory
    free(temp);
    free(temp_3x3);

    return;
}



// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //create new variable temp which will store copy of image
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        printf("assigning memory for temp failed\n");
        return;
    }

    //copy image into temp
    copy(height, width, image, temp);

    //create new variable temp_3x3 which will store 3x3 sub arrays
    RGBTRIPLE(*temp_3x3)[3] = calloc(3, 3*sizeof(RGBTRIPLE));
    if (temp_3x3 == NULL)
    {
        printf("assigning memory for temp_3x3 failed\n");
        return;
    }


    //go through array and calculate new rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //create sub array centred on pixel i-j
            sub_array_3x3(height, width, temp, i, j, temp_3x3);

            //for each colour, multiply kernel with sub array and divide by n.
            image[i][j].rgbtBlue = sobel(temp_3x3, 'b');
            image[i][j].rgbtGreen = sobel(temp_3x3, 'g');
            image[i][j].rgbtRed = sobel(temp_3x3, 'r');
        }
    }

    //free memory
    free(temp);
    free(temp_3x3);

    return;
}



//Create temporary copy of image to temp
void copy(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE temp[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j].rgbtBlue = image[i][j].rgbtBlue;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
}



//create 3x3 sub-array of array of image
//returns "size" of sub-array
//size is 9 for subararys from centre of images, size of 4 for subarrays from corners, size of 6 for subarrays from sides.
int sub_array_3x3(int height, int width, RGBTRIPLE temp[height][width], int row, int column, RGBTRIPLE temp_3x3[3][3])
{
    //reset values of sub array to zero
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp_3x3[i][j].rgbtBlue = 0;
            temp_3x3[i][j].rgbtGreen = 0;
            temp_3x3[i][j].rgbtRed = 0;
        }
    }

    //create default bounds and value for n
    int imin = 0;
    int imax = 2;
    int jmin = 0;
    int jmax = 2;
    int n = 9;

    //adjust imin, imax, jmin, jmax and depending on which edge we're on, if any.
    if (0 < row && row < height - 1) //middle rows
    {
        //left column
        if (0 == column)
        {
            jmin = 1;
            n = 6;
        }
        //right column
        else if (column == width - 1)
        {
            jmax = 1;
            n = 6;
        }
    }
    //top row
    else if (0 == row)
    {
        imin = 1;

        //middle columns
        if (0 < column && column < width - 1)
        {
            n = 6;
        }
        //left column
        else if (0 == column)
        {
            jmin = 1;
            n = 4;
        }
        //right column
        else if (column == width - 1)
        {
            jmax = 1;
            n = 4;
        }
    }
    //bottom row
    else if (row == height - 1)
    {
        imax = 1;

        //middle columns
        if (0 < column && column < width - 1)
        {
            n = 6;
        }
        //left column
        else if (0 == column)
        {
            jmin = 1;
            n = 4;
        }
        //right column
        else if (column == width - 1)
        {
            jmax = 1;
            n = 4;
        }
    }

    //create sub array
    for (int i = imin; i <= imax; i++)
    {
        for (int j = jmin; j <= jmax; j++)
        {
            temp_3x3[i][j].rgbtBlue = temp[row - 1 + i][column - 1 + j].rgbtBlue;
            temp_3x3[i][j].rgbtGreen = temp[row - 1 + i][column - 1 + j].rgbtGreen;
            temp_3x3[i][j].rgbtRed = temp[row - 1 + i][column - 1 + j].rgbtRed;
        }
    }

    return n;
}



//multiply kernel with 3x3 sub-array.
int mult_kernel_sub_array(int kernel[3][3], RGBTRIPLE temp_3x3[3][3], char colour)
{
    int sum = 0;

    switch (colour)
    {
        case 'b':
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    sum += kernel[i][j] * temp_3x3[i][j].rgbtBlue;
                }
            }
            break;

        case 'g':
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    sum += kernel[i][j] * temp_3x3[i][j].rgbtGreen;
                }
            }
            break;

        case 'r':
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    sum += kernel[i][j] * temp_3x3[i][j].rgbtRed;
                }
            }
        break;
    }

    return sum;
}



//sobel operator.
int sobel(RGBTRIPLE temp_3x3[3][3], char colour)
{
    //create variable for the two kernels
    int kernelx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernely[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //calculate Gx and Gy
    float Gx = mult_kernel_sub_array(kernelx, temp_3x3, colour);
    float Gy = mult_kernel_sub_array(kernely, temp_3x3, colour);

    //calculate new rgb value
    int temp_value = (int) fmin(255, round(sqrt(Gx*Gx + Gy*Gy)));

    return temp_value;
}
