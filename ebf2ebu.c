#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"
#include "ebf2ebu.h"

char* ebf2ebu(char* inFile, char* outFile, Img* image)
{   
    // Make input and output file pointers and validate them
    // Open inputfile in read mode
    FILE *inputFile = fopen(inFile, "r");
    if(inputFile == NULL)       
    {
        return NULL;
    }
    
    // Open outputfile in write binary mode
    FILE *outputFile = fopen(outFile, "wb");
    if(outputFile == NULL)
     {
      return NULL;
     }

    // Print the magic number into the output file
    fprintf(outputFile, "%c%c\n", getc(inputFile), getc(inputFile));
 
    // Scan the height and width from the inputfile
    // Write the height and width out to output file 
    int width = 0, height = 0;
    fscanf(inputFile, "%d %d", &height, &width);
    fprintf(outputFile, "%d %d\n", height, width);

    // Set the file pointer to start of data
    fseek(inputFile, 1, SEEK_CUR);
    
    // Write out the data from inputfile to output file in binary
    for(int i=0; i<image->height; i++) {
        for( int j=0; j<image->width; j++) {
            
            fwrite(&image->imgDataArr[i][j], sizeof(unsigned char), 1, outputFile);
           
        }
        
    }

    // Close files after usage
    fclose(inputFile);
    fclose(outputFile);

    return outFile;
}

