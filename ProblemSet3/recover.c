#include <stdio.h>
#include <stdlib.h>

// bmp.h is pasted here:
// BMP-related data types based on Microsoft's own

#include <stdint.h>

// aliases for C/C++ primitive data types
// https://msdn.microsoft.com/en-us/library/cc230309.aspx
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

// information about the type, size, and layout of a file
// https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx
typedef struct
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

// information about the dimensions and color format
// https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx
typedef struct
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

// relative intensities of red, green, and blue
// https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx
typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;



int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "function recover must have exactly 1 argument");
        return 1;
    }

    char *image_file = argv[1];
    FILE *image_ptr = fopen(image_file, "r");

    if(image_ptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", image_file);
        return 2;
    }

    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;

    //getting size of a file
    fseek(image_ptr, 0, SEEK_END);
    long int size = ftell(image_ptr);
    fseek(image_ptr, 0, SEEK_SET);

    int image_number = 0;
    unsigned char block[512];
    int flag = 0;
    char filename[8];
    sprintf(filename, "%03d.jpg", image_number);
    FILE *outptr = fopen(filename, "w");

    for(int i = 0; i < size; i += 512)
    {
        fread(&block, 512, 1, image_ptr);

        if(flag == 1 && block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[3] <= 0xef)
        {
            fclose(outptr);
            image_number++;
            sprintf(filename, "%03d.jpg", image_number);
            outptr = fopen(filename, "w");
        }

        if(flag == 0 && block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[3] <= 0xef)
        {
            flag = 1;
        }

        if(flag == 1)
        {
            fwrite(&block, 512, 1, outptr);
        }

    }

    fclose(outptr);

    return 0;

}
