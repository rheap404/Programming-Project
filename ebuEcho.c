#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"
#include "ebu2ebf.h"
#include "ebf2ebu.h"

#define SUCCESS 0

int main(int argc, char **argv)
{   
  // Check for corret argument count
   if( Args(argc)==0 )
    { 
       printf("Usage: ebuEcho file1 file2");
       raiseError(0, argv[1]);
    } 

  if( Args(argc)==1)
    {
       raiseError(1, argv[1]);
    }

  // Convert file to ebf, to run the test checks
  char* gotFile = ebu2ebf(argv[1], "ebfEcho.ebf");
  FILE* inputFile= readFile(gotFile);
 
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
       // print appropriate error message and return  
       fclose(inputFile);
       close(image);
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
   // Write data to an ebf file 
   FILE *outputFile = writeFile("ebuEcho.ebf");

   if(outputFile==NULL)
     {
       fclose(outputFile);
       close(image);
       raiseError(2, argv[2]);
     }

    if(badOutput(inputFile, outputFile, image)==7)
      {
        fclose(outputFile);
        close(image);
        raiseError(7, argv[2]);
      }
    
    // Now we have finished using the inputFile we should close it
    fclose(inputFile);
    // Convert the output file back to original ebu format
    if(ebf2ebu("ebuEcho.ebf", argv[2], image)==NULL)
    {
       fclose(inputFile);
       close(image);
       raiseError(2, argv[2]);
     }

   // free allocated memory before exit
   // close the output file before exit
    fclose(inputFile);
    close(image);

    // print final success message and return
    printf("ECHOED\n");

    return SUCCESS;
}