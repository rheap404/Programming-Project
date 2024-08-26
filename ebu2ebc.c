#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"
#include "ebu2ebc.h"


char* ebu2ebc(char* inFile, char* outFile)
{
    // Make input and output file pointers and validate them
    // Open inputfile in read binary mode
    FILE* inputFile= fopen(inFile, "rb");
     if(inputFile==NULL)
     {
      return NULL;
     }

    // Open outputfile in write binary mode
    FILE* ebcFile = fopen(outFile, "wb");
     if(ebcFile==NULL)
     {
      return NULL;
     }

    // Print the magic number into the output file
     fprintf(ebcFile, "%c%c\n", getc(inputFile), getc(inputFile));
  

    // Scan the height and width from the inputfile
    // Write the height and width out to output file 
    int width = 0, height = 0;
    fscanf(inputFile, "%d %d", &height, &width);
    fprintf(ebcFile, "%d %d\n", height, width);

    // Set the file pointer to start of data
    fseek(inputFile, 1, SEEK_CUR);

    // Initialize buffer to store the packed pixel
    // Pixel reads in 8 bits at a time from the input file
    unsigned char buffer = 0;
    unsigned char pixel;
    int counter=8;

    while (fread(&pixel, sizeof(unsigned char), 1, inputFile) == 1) {

        // Loop runs untill the last 5 pixels from the 8 bits read in are extracted
        for(int i=4; i>=0; i--)
        {   
            // Pack the pixel one bit at a time into buffer using bitwise operations
            buffer |= ((pixel>> i) & 1) << (--counter);
            
            // Print buffer to output file when 8 bits are packed into buffer 
            if(counter == 0)
            {
                fwrite(&buffer, sizeof(unsigned char), 1, ebcFile);
                // Reset the counter and buffer for the next pixel
                counter=8;
                buffer=0;
            }
         }
    }

    // Close files after usage
    fclose(inputFile);
    fclose(ebcFile);

    return outFile;
}

