#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"
#include "ebu2ebf.h"

char* ebc2ebu(char* inFile, char* outFile)
{   
    // Make input and output file pointers and validate them
    // Open inputfile in read binary mode
    FILE* inputFile= fopen(inFile, "rb");
     if(inputFile==NULL)
     {
      return NULL;
     }

    // Open outputfile in write binary mode
    FILE* ebuFile = fopen(outFile, "wb");
     if(ebuFile==NULL)
     {
      return NULL;
     }

    // Print the magic number into the output file
    fprintf(ebuFile, "%c%c\n", getc(inputFile), getc(inputFile));
   

    // Scan the height and width from the inputfile
    // Write the height and width out to output file 
    int width = 0, height = 0;
    fscanf(inputFile, "%d %d", &height, &width);
    fprintf(ebuFile, "%d %d\n", height, width);

    // Set file pointer to the start of the data
    fseek(inputFile, 1, SEEK_CUR);

    // Initialize pixel to store the packed pixel
    // Buffer reads in 8 bits at a time from the input file
    unsigned char buffer;
    unsigned int pixel = 0;
    int count = 0;

    while (fread(&buffer, sizeof(unsigned char), 1, inputFile) == 1) {
        
        // Loop runs untill 8 bits in buffer are processed
        for (int i = 0; i < 8; i++) 
        {
            // Using bit masking and shifting to store and count one bit at time
            // Untill a pixel is formed
            pixel |= ((buffer >> (7 - i)) & 1) << (4 - count);
            count++;

            if (count == 5) {
                // Write the pixel to the output file onces pixel has 5 bits
                fwrite(&pixel, sizeof(unsigned char), 1, ebuFile);
                // Reset pixel and count for next pixel
                pixel= 0;
                count = 0;
            }
        }
    }

    // Close files after usage
    fclose(inputFile);
    fclose(ebuFile);

    return outFile;
}

