#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = (int) (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (int) 3;
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

    //create new variable/pointer temp_3x3 which will store 3x3 sub arrays
    RGBTRIPLE(*temp_3x3)[3] = calloc(3, 3*sizeof(RGBTRIPLE));
    if (temp_3x3 == NULL)
    {
        printf("assigning memory for temp_3x3 failed\n");
        return;
    }

    //create variable kernel to multiply with temp_3x3
    int kernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    //variable size_of_sub_array
    int n = 9;

    //go through array and calculate new rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //create sub array centred on pixel i-j
            n = sub_array_3x3(height, width, temp, i, j, temp_3x3);

            //for each colour, multiply kernel with sub array and divide by n.
            image[i][j].rgbtBlue = mult_kernel_sub_array(kernel, temp_3x3, 'b') / n;
            image[i][j].rgbtGreen = mult_kernel_sub_array(kernel, temp_3x3, 'g') / n;
            image[i][j].rgbtRed = mult_kernel_sub_array(kernel, temp_3x3, 'r') / n;

            /*checks checks checks
            if (i == 0 && j == 5)
            {
            for (int k = 1; k < 3; k++)
            {
                for (int m = 0; m < 3; m++)
                {
                    printf("%i %i\n", temp_3x3[k][m].rgbtBlue, temp[i - 1 + k][j - 1 + m].rgbtBlue);
                }
            }

            printf("%i %i %i\n", image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed);*/

            /*if (image[i][j].rgbtBlue == 0 && image[i][j].rgbtGreen == 0 && image[i][j].rgbtRed == 0 && i == 0)
            {
                printf("i is %i, j is %i\n", i ,j);
            }

            if (image[i][j].rgbtBlue == 23 & image[i][j].rgbtGreen == 56 && image[i][j].rgbtRed == 38)
            {
                printf("special i is %i, j is %i\n", i ,j);
            }*/

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
