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

    //go through array and calculate new rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //create sub array centred on pixel i-j
            sub_array_3x3(height, width, temp, i, j, temp_3x3);

            //for each colour, multiply kernel with sub array and divide by 9.
            image[i][j].rgbtBlue = mult_kernel_sub_array(kernel, temp_3x3, 'b') / 9;
            image[i][j].rgbtGreen = mult_kernel_sub_array(kernel, temp_3x3, 'g') / 9;
            image[i][j].rgbtRed = mult_kernel_sub_array(kernel, temp_3x3, 'r') / 9;

            //NEED TO FIX FACT THAT YOU SHOULDN:T ALWAYS DIVIDE BY 9!!!!
            //NEED TO FIX FACT THAT YOU SHOULDN:T ALWAYS DIVIDE BY 9!!!!
            //NEED TO FIX FACT THAT YOU SHOULDN:T ALWAYS DIVIDE BY 9!!!!

        }
    }

/*    //checks checks checks
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i %i\n", temp_3x3[i][j].rgbtBlue, temp[5 - 1 + i][5 - 1 + j].rgbtBlue);
        }
    }

    int test = mult_kernel_sub_array(kernel, temp_3x3, 'b');
    printf("%i\n", test); */

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
void sub_array_3x3(int height, int width, RGBTRIPLE temp[height][width], int row, int column, RGBTRIPLE temp_3x3[3][3])
{
    //case 0. centre of image
    if (0 < row && row < height - 1 && 0 < column && column < width - 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                temp_3x3[i][j].rgbtBlue = temp[row - 1 + i][column - 1 + j].rgbtBlue;
                temp_3x3[i][j].rgbtGreen = temp[row - 1 + i][column - 1 + j].rgbtGreen;
                temp_3x3[i][j].rgbtRed = temp[row - 1 + i][column - 1 + j].rgbtRed;
            }
        }
    }
    //....
    else if (0 == 1)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                temp_3x3[i][j].rgbtBlue = temp[row - 1 + i][column - 1 + j].rgbtBlue;
                temp_3x3[i][j].rgbtGreen = temp[row - 1 + i][column - 1 + j].rgbtGreen;
                temp_3x3[i][j].rgbtRed = temp[row - 1 + i][column - 1 + j].rgbtRed;
            }
        }
    }
    return;
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