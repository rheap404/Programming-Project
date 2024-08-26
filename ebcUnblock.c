#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ebf.h"
#include "ebu2ebf.h"
#include "ebc2ebu.h"
#include "ebf2ebu.h"
#include "ebu2ebc.h"

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
   // Convert file to ebf format to run test checks, using conversion files
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
    
    // Change struct variables to hold the compressed height and width
    image->height= (int)round(image->height/3); 
    image->width= (int)round(image->width/3); 

   if(badData(inputFile, image)==6)
     {
        fclose(inputFile);
        close(image);;
        raiseError(6, argv[1]);
     }

    // Set file pointers to scan uncompressed height and width
    fseek(inputFile, 0, SEEK_SET);
    fseek(inputFile, 2, SEEK_CUR);
    
    fscanf(inputFile, "%d %d", &image->height, &image->width);

    // Close inputfile after useage
    fclose(inputFile);

    // Open a file in write mode
    FILE *outputFile = fopen("out1", "w");

    // Print the magic number and uncompressed height and width into the file
    fprintf(outputFile, "ec\n%d %d\n", image->height, image->width);

    // Allocate memory to a 2D array to store decompressed block data
    image->ebcUnblockArr = (unsigned int **)malloc(image->height * sizeof(int *));

     for (int i = 0; i < image->height; i++, image->data+=image->width) 
         {
          image->ebcUnblockArr[i] = image->data;
         }

    // Create variables to hold the compressed height and width
    int height= (int)round(image->height/3); 
    int width= (int)round(image->width/3);

    // Write nested loops to store the uncompressed data in blocks of 9 into the 2D array 
     for(int i=0; i<height; i++) 
        {
          for(int j=0; j<width; j++) 
              {
                for(int k = i*3, c1=0; c1<3; k++, c1++) 
                    {
                        for(int l = j*3, c2=0; c2<3; l++, c2++) 
                            {    
                                image->ebcUnblockArr[k][l]=image->imgDataArr[i][j];
                            }
                    }
                
              }
        }

   if(outputFile==NULL)
     {
       fclose(inputFile);
       close(image);
       raiseError(2, argv[2]);
     }

    // Print the uncompressed block data into the output file from the 2D array 
     for(int i=0; i<image->height; i++)
        {
          for(int j=0; j<image->width; j++)
           {
             if(image->ebcUnblockArr[i][j] < 10 ){

            fprintf(outputFile, "%2u", image->ebcUnblockArr[i][j]);
    
             }
             else {
               fprintf(outputFile, "%u", image->ebcUnblockArr[i][j]);
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

  
    // Convert the output file back to ebc format 
    ebf2ebu("out1", "out2", image);
    ebu2ebc("out2", argv[2]);

  
   // Free memory and close output file before exit
   fclose(outputFile);
   close(image);

   // Print final success message and return
   printf("ECHOED\n");

   return SUCCESS;





}