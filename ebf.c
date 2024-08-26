#include <stdio.h>
#include <stdlib.h>
#include "ebf.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAGIC_NUMBER 0x6265
#define MAGIC_NUMBER_EU  0x7565
#define MAGIC_NUMBER_EC  0x6365
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1


int Args(int argc)
{   
  // Validate that user has enter 2 arguments (plus the executable name)
    if (argc == 1)
      {
       return 0;
      } 

    if (argc != 3)
        { // check arg count
         return 1;
        } // check arg count

    return 10;
}

FILE* readFile(char *fileName)
{  
   FILE *inputFile = fopen(fileName, "r");
   if(inputFile == NULL)
     {
      return NULL;
     }
    return inputFile;
}

int MagicNumber(FILE* inputFile)
{
    // create a char array to hold magic number
    // and cast to short
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue = (unsigned short *)magicNumber;
    // get first 2 characters which should be magic number

    magicNumber[0] = getc(inputFile);
    magicNumber[1] = getc(inputFile);
    
    // checking against the casted value due to endienness.
    if (*magicNumberValue != MAGIC_NUMBER && *magicNumberValue != MAGIC_NUMBER_EU && *magicNumberValue != MAGIC_NUMBER_EC)
        { // check magic number
          return BAD_MAGIC_NUMBER;
        } //check magic number

    return 0;
}

int badDimensions(FILE* inputFile, Img* image)
{   
    int width = 0, height = 0;
    long numBytes=0;

    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    int check = fscanf(inputFile, "%d %d", &height, &width);
    // printf("%i %i", width, height);
    
     if (check != 2 || height < MIN_DIMENSION || width < MIN_DIMENSION || height > MAX_DIMENSION || width > MAX_DIMENSION)
        { // check dimensions
          return BAD_DIM;
        } // check dimensions
      
     //initialize stuct values and allocate memory if check is passed
     numBytes=height*width;
     image->imageData = (unsigned int *)malloc(numBytes * sizeof(unsigned int));
     image->height=height;
     image->width=width;
     image->imgDataArr = (unsigned int **)malloc(height * sizeof(int *));
     image->data = (unsigned int *)malloc(width* height * sizeof(int));

     for (int i = 0; i < image->height; i++, image->data+=width) 
         {
          image->imgDataArr[i] = image->data;
         }
    
     return 0;

}

int badMalloc(FILE* inputFile, Img* image)
{     
     // if malloc is unsuccessful, it will return a BAD_MALLOC
    if (image->imageData== NULL)
       { // check malloc
          return BAD_MALLOC;
       } // check malloc 
     return 0;

}

int badData(FILE* inputFile, Img* image)
{   
    int current = 0,check=0;

     // using a nested for loop to check for valid data
     // and store it in a 2D array 
     for(int i=0; i<image->height; i++)
        { 
         for(int j=0; j<image->width; j++,current++)
            {
             if(current >= (image->height*image->width))
               {
                 break;
               }
             // read in each grey value from the file
             check = fscanf(inputFile, "%u", &image->imageData[current]);

             // validate that we have captured 1 pixel value
             if (check != 1 || image->imageData[current] > 31 )
                { // check inputted data
                  return BAD_DATA;
                } // check inputted data
        
             //store each pixel value in the 2D array
             image->imgDataArr[i][j]= image->imageData[current];
        
            }
            
        }
    
     // check whether the file contains too much data
     check = fscanf(inputFile, "%u", &image->imageData[current]);
     if(check == 1)
       {
        return BAD_DATA;
       }

     return 0;
}


FILE* writeFile(char *fileName)
{ // validate that the file has been opened correctly
   FILE *outputFile = fopen(fileName, "w");

   if(outputFile == NULL)
     { // validate output file
      return NULL;
     } // validate output file

   return outputFile;
}

int badOutput(FILE* inputFile, FILE* outputFile, Img* image)
{   
    // Write the header data in one block
   fseek(inputFile, 0, SEEK_SET);
   int check = fprintf(outputFile, "%c%c\n", getc(inputFile), getc(inputFile));
   
    if (check == 0) 
        { // check write
        return BAD_OUTPUT;
        } // check write

    check = fprintf(outputFile, "%d %d\n", image->height, image->width);
    if (check == 0) 
        { // check write
        return BAD_OUTPUT;
        } 

     // Using a nested loop to print into the file
     // Validate that the data is being written into the file.
     for(int i=0; i<image->height; i++)
        {
          for(int j=0; j<image->width; j++)
           {
             if(image->imgDataArr[i][j] < 10 ){

              check = fprintf(outputFile, "%u", image->imgDataArr[i][j]);
    
             }
             else {
              check = fprintf(outputFile, "%u", image->imgDataArr[i][j]);
             }
    
             if(j!=image->width-1)
               {
                 check = fprintf(outputFile, "%c", ' ');
               }

             if (check == 0)
                { // check write
                  return BAD_OUTPUT;
                } // check write
            }
           
          if(i!=image->height-1)
            {
             fprintf(outputFile, "%c", '\n');
            }
        }
       
     return 0;
}


void close(Img* image) {

  // Free the 1D array if it isn't free'd
  if(image->imageData != NULL)
  {
    free(image->imageData);
  }

  // Free the 2D array if it isn't free'd
  if(image->imgDataArr != NULL)
  {
    free(*image->imgDataArr);
    free(image->imgDataArr);
  }

}

void raiseError(int errorcode, char* fileName)
{  
  // This method is called whenever there is an error
  // and exits the program with appropriate error messages.
    if(errorcode==0)
      {
       exit(SUCCESS);
      }
    if(errorcode==1)
      {
        printf("ERROR: Bad Arguments\n");
        exit(BAD_ARGS);
      }
    if(errorcode==2)
      {
        printf("ERROR: Bad File Name (%s)\n", fileName );
        exit(BAD_FILE);
      }
    if(errorcode==3)
      {
        printf("ERROR: Bad Magic Number (%s)\n", fileName);
        exit(BAD_MAGIC_NUMBER);
      }
    if(errorcode==4)
      {
        printf("ERROR: Bad Dimensions (%s)\n", fileName);
        exit(BAD_DIM);
      }
    if(errorcode==5)
      {
        printf("ERROR: Image Malloc Failed\n");
        exit(BAD_MALLOC);
      }
    if(errorcode==6)
      {
        printf("ERROR: Bad Data (%s)\n", fileName);
        exit(BAD_DATA);
      }
    if(errorcode==7)
      {
        printf("ERROR: Bad Output\n");
        exit(BAD_OUTPUT);
      }
}