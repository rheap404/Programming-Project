
typedef struct image
{
  int width;
  int height;
  unsigned int *imageData;
  unsigned int **imgDataArr;
  unsigned int *data;
  unsigned int *ebcData;
  unsigned int **ebcUnblockArr;
}Img;

int Args(int argc);
FILE* readFile(char *fileName);
void raiseError(int errorcode, char* fileName);
int MagicNumber(FILE* inputFile);
int badDimensions(FILE* inputFile, Img* image);
int badMalloc(FILE* inputFile, Img* image);
int badData(FILE* inputFile, Img* image);
FILE* writeFile(char *fileName);
int badOutput(FILE* inputFile, FILE* outputFile, Img* image);
void close( Img* image);

