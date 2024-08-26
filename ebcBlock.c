#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"
#include "ebu2ebf.h"
#include "ebc2ebu.h"
#include "ebf2ebu.h"
#include "ebu2ebc.h"

#include <math.h>
#define SUCCESS 0


int main(int argc, char **argv)
{
  // Check for number of arguments
   if( Args(argc)==0 )
    { 
       printf("Usage: ebcEcho file1 file2");
       raiseError(0, argv[1]);
    } 

  if( Args(argc)==1)
    {
       raiseError(1, argv[1]);
    }

   // Open the input file in read mode
   // Convert file to ebf format to run test checks, using conversion
   char* gotFile = ebc2ebu(argv[1], "B1");
   char* getFile = ebu2ebf(gotFile, "B2");
   FILE* inputFile= readFile(getFile);
 
  if(inputFile == NULL)
    { 
      fclose(inputFile);
      raiseError(2, argv[1]);
    }
 
  if(MagicNumber(inputFile)==3)
   {
     fclose(inputFile);
     raiseError(3, argv[1]);
   }  
  
   Img *image =(Img *)malloc((sizeof(Img)));

   if(badDimensions(inputFile, image)==4)
     { 
       fclose(inputFile);
       raiseError(4, argv[1]);
      }
   
   if(badMalloc(inputFile, image)==5)
     {
       fclose(inputFile);
       close(image);
       raiseError(5, argv[1]);
     }
    
   if(badData(inputFile, image)==6)
     {
       fclose(inputFile);
       close(image);
       raiseError(6, argv[1]);
     }


    // Create variables to hold the compressed height and width
    int height= (int)round(image->height/3);
    int width= (int)round(image->width/3);

    // Reallocate the 1D array memory to hold the compressed block data 
    image->imageData = (unsigned int *)realloc(image->imageData, width * height * sizeof(unsigned int));

    // Initialise variables for the loop 
    double sum=0;
    int m=0;
    int count=0;

    // Create nested loops that sums data in the 2d array in blocks of 9
    // And takes their mean averages and stores them in the 1D array
     for(int i=0; i<image->height; i+=3) 
        {
          for(int j=0; j<image->width; j+=3) 
              {
                for(int k=0; k<3; k++, i++)
                   { 
                    if(i>image->height)
                      {
                        break;
                      }
                      for(int l=0; l<3; l++, j++) 
                        {
                          if(j>image->width)
                            {
                              break;
                            }
                          sum+=image->imgDataArr[i][j];
                          count++;
                        }
                    j-=3;
                  }
                  // Round the average using round function and store in the array
                  image->imageData[m]= (int)round(sum/count);
                  count=0;
                  m++;
                  sum=0;
                  i-=3;
              } 
         }

    // Reallocate image data array memory to store the new mean average pixel values
    image->imgDataArr = (unsigned int **)realloc(image->imgDataArr, height * sizeof(int *));
    image->ebcData = (unsigned int *)malloc( width * height * sizeof(int));

     for (int i = 0; i < height; i++, image->ebcData+=width) 
         {
          image->imgDataArr[i] = image->ebcData;
         }

    // Create a loop to store the new mean average pixel values in the 2D array
    m=0;
    for(int i=0; i<height; i++)
      {
        for(int j=0; j<width; j++)
        {
         image->imgDataArr[i][j]= image->imageData[m];
          m++;
        }
      }

    // Close inputfile after useage
    fclose(inputFile);

    // Open a file in write mode
    // Write into it in ebf format
    FILE *outputFile = fopen("Bout", "w");

    // Print the magic number and uncompressed height and width
    fprintf(outputFile, "ec\n%d %d\n", image->height, image->width);

    image->height=height;
    image->width=width;

   if(outputFile==NULL)
     {
       fclose(outputFile);
       close(image);
       raiseError(2, argv[2]);
     }
   
   // Print the mean pixel data into the output file using nested loops
   for(int i=0; i<image->height; i++)
        {
          for(int j=0; j<image->width; j++)
           {
             if( image->imgDataArr[i][j] < 10 ){

            fprintf(outputFile, "%2u",  image->imgDataArr[i][j]);
    
             }
             else {
               fprintf(outputFile, "%u",  image->imgDataArr[i][j]);
             }
    
             if(j!=image->width-1)
               {
                 fprintf(outputFile, "%c", ' ');
               }

            }
           
          if(i!=image->height-1)
            {
             fprintf(outputFile, "%c", '\n');
            }
        }

   // Convert from ebf into ebc 
   ebf2ebu("Bout", "Bfinal", image);
   ebu2ebc("Bfinal", argv[2]);

   // Free memory and close output file before exit
   fclose(outputFile);
   close(image);

   // Print final success message and return
   printf("ECHOED\n");

   return SUCCESS;

}