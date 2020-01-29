#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h> //for bool type

#define QTY 512 //number of bytes per chunk in FAT storage system. for quantity parameter in fread.
#define SIZE 1 //number of bytes for size paremeter in fread.
typedef uint8_t BYTE;  //define new type byte to store a byte of information as unsigned integer

//declare functions
void use_fgetc(char *argv[], FILE *fp_card);
bool jpg_header(BYTE *buffer);

int main(int argc, char *argv[])
{
    //check there are correct number of command line inputs
    if (argc != 2)
    {
        printf("Program not run correctly. Should be recover name_of_file.\n");
        return 1;
    }

    //open filename in argv[1] and check for null
    FILE *fp_card = fopen(argv[1], "r");

    if (fp_card == NULL)
    {
        printf("File could not be opened. Please enter valid filename.\n");
        return 1;
    }


    //initialise variables
    //buffer is temporary storage area. holds QTY bytes of information.
    BYTE *buffer = calloc(QTY, SIZE);

    //counts number of jpegs found. starts on -1 because we increment by 1 each time we find jpg header
    int jpg_counter = -1;

    //file pointer for jpg files to be created
    FILE *fp_jpg = NULL;

    //string to store name of jpg files
    char jpg_name[7];


    //while loop until we cannot read another QTY worth of data
    while (fread(buffer, SIZE, QTY, fp_card) == QTY)
    {
        //if we don't detect jpg header and we have previously seen header file, write to current jpg file
        if (!(jpg_header(buffer)) && jpg_counter > -1)
        {
            fwrite(buffer, SIZE, QTY, fp_jpg);
        }

        //if we do detect jpg header and we have previously seen header file, close current jpg file and create new jpg file
        else if (jpg_header(buffer) && jpg_counter > -1)
        {
            //close previous file
            fclose(fp_jpg);

            //increment counter, create new name, open new file, check for null
            jpg_counter++;
            sprintf(jpg_name, "%03i.jpg", jpg_counter);
            fp_jpg = fopen(jpg_name, "w");

            if (fp_jpg == NULL)
            {
                printf("Attempting to open/create new file %s failed\n", jpg_name);
                return 1;
            }

            //write to file
            fwrite(buffer, SIZE, QTY, fp_jpg);
        }

        //if we do detect jpg header and this is first header to be detected, create first jpg file
        else if (jpg_header(buffer) && jpg_counter == -1)
        {
            //increment counter, create new name, open new file, check for null
            jpg_counter++;
            sprintf(jpg_name, "%03i.jpg", jpg_counter);
            fp_jpg = fopen(jpg_name, "a");

            if (fp_jpg == NULL)
            {
                printf("Attempting to open/create new file %s failed\n", jpg_name);
                return 1;
            }

            //write to file
            fwrite(buffer, SIZE, QTY, fp_jpg);
        }
    }

    fclose(fp_jpg);
    fclose(fp_card);
    free(buffer);


    return 0;
}



//check if header of new block has jpg header
bool jpg_header(BYTE *buffer)
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[4] <= 0xef;
}