#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"
#include "ebu2ebf.h"
#include "math.h"

char* ebu2ebf(char* inFile, char* outFile)
{   
    // Make input and output file pointers and validate them
    // Open inputfile in read binary mode
    FILE* inputFile= fopen(inFile, "rb");
    if(inputFile==NULL)
     {
      return NULL;
     }

    // Open outputfile in write mode
    FILE* outputFile= fopen(outFile, "w");
    if(outputFile == NULL)
    {
      return NULL;
    }
    
    // Store the size of the file
    fseek(inputFile, 0L, SEEK_END);
    int size= ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    // Print the magic number into the output file
    fprintf(outputFile, "%c%c\n", getc(inputFile), getc(inputFile));
   
    // Scan the height and width from the inputfile
    // Write the height and width out to output file 
    int width = 0, height = 0;
    fscanf(inputFile, "%d %d", &height, &width);
    fprintf(outputFile, "%d %d\n", height, width);


    // Create an array buffer of of size file size
    unsigned char buffer[size];
    fseek(inputFile, 1, SEEK_CUR);
    fread(&buffer, size, 1, inputFile); 

    // If conversion file is being used for ebcBlock 
    // Height and width is adjusted accordingly
    if(size<height*width) {
      height= (int)round(height/3);
      width= (int)round(width/3);
    }
    
    // Print the contents of the buffer to output file
    int data=0;
    for (int i = 0; i < height; i++)
        {
         for (int j = 0; j < width; j++) 
             {
               if(data >= (height*width))
               {
                 break;
               }
              fprintf(outputFile, "%u", buffer[data]);
              if(j!=width-1)
               {
                fprintf(outputFile, "%c", ' ');
               }
              data++;
             }
         
          if(i!=height-1)
            {
             fprintf(outputFile, "%c", '\n');
            }
        }

   // Close files after useage  
   fclose(inputFile);
   fclose(outputFile);
  
   return outFile;
 
}


