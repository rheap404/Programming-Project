#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"

#define SUCCESS 0


int main(int argc, char **argv)
{ 
  // Check for correct number of arguments  
  if( Args(argc)==0 )
    { 
      printf("Usage: ebfEcho file1 file2");
      raiseError(0, argv[1]);
    }

  if( Args(argc)==1 )
    {
      raiseError(1, argv[1]);
    }

  // Open the input file in read mode
  FILE *inputFile = readFile(argv[1]);
  
  // Run test checks
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

   // Open the output file in write mode
   FILE *outputFile = writeFile(argv[2]);

   if(outputFile==NULL)
     {
        fclose(inputFile);
        close(image);
        raiseError(2, argv[2]);
     }

    if(badOutput(inputFile, outputFile, image)==7)
      {
        fclose(inputFile);
        close(image);
        raiseError(7, argv[2]);
      }

    // Now we have finished using the inputFile we should close it
    fclose(inputFile);

    // free allocated memory before exit
    // close the output file before exit

     fclose(outputFile);
     close(image);

    // print final success message and return
    printf("ECHOED\n");
    return SUCCESS;

    } // main()