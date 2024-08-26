#include <stdio.h>
#include <stdlib.h>
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

  // Convert to ebf format to run test checks, using conversion files
   char* gotFile = ebc2ebu(argv[1], "U");
   char* getFile = ebu2ebf(gotFile, "F");
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

  // Open the output file in write mode
  // Write the output file in ebf
  
  FILE *outputFile = writeFile("ebcEcho.ebf");

    if(badOutput(inputFile, outputFile, image)==7)
      {
        fclose(outputFile);
        close(image);
        raiseError(7, argv[2]);
      }
  
  // Now we have finished using the inputFile we should close it
  fclose(inputFile);

   // Convert File back to original format
   ebf2ebu("ebcEcho.ebf", "U1", image);
   if(ebu2ebc("U1",argv[2])==NULL)
   {
      fclose(outputFile);
      close(image);
      raiseError(2, argv[2]);
   }
   

  // free allocated memory before exit
  // close the output file before exit
  fclose(outputFile);
  close(image);
  
  // print final success message and return
  printf("ECHOED\n");

  return SUCCESS;

}