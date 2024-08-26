#include <stdio.h>
#include <stdlib.h>
#include "ebu2ebf.h"
#include "ebf.h"

#define SUCCESS 0


int main(int argc, char **argv)
{   
    // Check for number of arguments
    if( Args(argc)==0 )
    { 
      printf("Usage: ebuComp file1 file2");
      raiseError(0, argv[1]);
    }
     if( Args(argc)==1)
    {
      raiseError(1, argv[1]);
    }


    // Open the input file in read mode
    // Convert file to ebf format
    char* gotFile = ebu2ebf(argv[1],"ebuComp1.ebu");
    FILE* inputFile1= readFile(gotFile);

    if(inputFile1 == NULL)
    {
      fclose(inputFile1);
      raiseError(2, argv[1]);
    }

    if(MagicNumber(inputFile1)==3)
    {
      fclose(inputFile1);
      raiseError(3, argv[1]);
    }

    Img *image1 =(Img *)malloc((sizeof(Img)));

    // Create and initialise variables used within code
    unsigned char magicNumber1[2];
    unsigned short *magicNumberValue1 = (unsigned short *)magicNumber1;

    if(badDimensions(inputFile1, image1)==4)
    {
      // print appropriate error message and return  
      fclose(inputFile1);
      close(image1);
      raiseError(4, argv[1]);
    }
    
    if(badMalloc(inputFile1, image1)==5)
    {   
      fclose(inputFile1);
      close(image1);
      raiseError(5, argv[1]);
    }

   if(badData(inputFile1, image1)==6)
   {
      fclose(inputFile1);
      close(image1);
      raiseError(6, argv[1]);
   }

    // Now we have finished using the inputFile1 we should close it
    fclose(inputFile1);
    

    // Open the input file 2 in read mode
    // Convert the file to ebf format

    char* gotFile2 = ebu2ebf(argv[2], "ebuComp2.ebu");
    FILE* inputFile2= readFile(gotFile2);

    if(inputFile2 == NULL)
    {
      fclose(inputFile2);
      close(image1);
      raiseError(2, argv[2]);
    }

    if(MagicNumber(inputFile2)==3)
    {
      fclose(inputFile2);
      close(image1);
      raiseError(3, argv[2]);
    }

    Img *image2 =(Img *)malloc((sizeof(Img)));
    
    // Create a char array to hold magic number
    // and cast to short
    unsigned char magicNumber2[2];
    unsigned short *magicNumberValue2 = (unsigned short *)magicNumber2;

    if(badDimensions(inputFile2, image2)==4)
    {  
      fclose(inputFile2);
      close(image2);
      raiseError(4, argv[2]);
    }
    
    if(badMalloc(inputFile2, image2)==5)
    {   
      fclose(inputFile2);
      close(image2);
      raiseError(5, argv[2]);
    }

    if(badData(inputFile2, image2)==6)
   {
      fclose(inputFile2);
      close(image2);
      raiseError(6, argv[2]);
   }

    // Now we have finished using the inputFile 2 we should close it
    fclose(inputFile2);

    // Compare the data from the two files:
    
    // Start with magic number values
    if (*magicNumberValue1 != *magicNumberValue2)
        { // free and exit
          fclose(inputFile2);
          close(image1);
          close(image2);
          printf("DIFFERENT\n");
         return SUCCESS;
        } // free and exit

    // Check dimensions
    if ((image1->height != image2->height) || (image1->width != image2->width))
        { // free and exit
          fclose(inputFile2);
          close(image1);
          close(image2);
          printf("DIFFERENT\n");
          return SUCCESS;
        } // free and exit

    // and check the pixel values
    for (int n=0; n<(image1->height*image1->width); n++)
        {
         if(image1->imageData[n] != image2->imageData[n])
           { // free and exit
              fclose(inputFile2);
              close(image1);
              close(image2);
              printf("DIFFERENT\n");
              return SUCCESS;
           } // free and exit
        }

    // Free allocated memory before exit
    fclose(inputFile2);
    close(image1);
    close(image2);
  
    // If we have not exited on different data, must be identical
    printf("IDENTICAL\n");
    return SUCCESS;

 } 
