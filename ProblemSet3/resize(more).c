// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    float f = atof(argv[1]);
//    scanf("%f",&f);
    // ensure proper usage
//    if (argc != 3)
//    {
//        fprintf(stderr, "Usage: copy infile outfile\n");
//        return 1;
//    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);


    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    int padding_in = (4 - (bi.biWidth*3)%4)%4;
    int og_width = bi.biWidth, og_height = bi.biHeight;
    bi.biWidth *= f;
    bi.biHeight *= f;
    int padding_out = (4 - (bi.biWidth*3)%4)%4;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) *bi.biWidth) + padding_out) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    const int in_row_size = bi.biWidth;
    RGBTRIPLE full_row[bi.biWidth];

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);;
    RGBTRIPLE row_of_new[bi.biWidth], temp;
    for (int i=0 ; i<abs(og_height); i++)
    {
        if(f>1)
        {
            for(int j=0; j<bi.biWidth;)
            {
                fread(&temp, sizeof(RGBTRIPLE), 1, inptr);
                for(int k = 0; k<f;k++,j++)
                {
                    row_of_new[j]=temp;
                    printf("%d,%ld ",temp.rgbtRed%10,ftell(outptr));

                }

            }

            fseek(inptr, padding_in, SEEK_CUR);

            printf("\n");

            for(int j=0; j<f; j++)
            {
                fwrite(&row_of_new, sizeof(row_of_new), 1, outptr);
                for(int q = 0; q < padding_out; q++)
                {
                    fputc(0x00, outptr);
                }

            }


        }
        if(f<1)
        {
            int n = (int)(1.0 / f);
            for(int j=0; j<og_width; j++)
            {
                fread(&temp, sizeof(RGBTRIPLE), 1, inptr);
                printf("%d,",temp.rgbtRed%10);
                for(int hori_move = 0; hori_move<n-1 ; hori_move++,j++ )
                    fseek(inptr, sizeof(RGBTRIPLE), SEEK_CUR);
                fwrite(&temp, sizeof(RGBTRIPLE), 1, outptr);
                printf("%ld ", ftell(outptr));
            }
            for (int k = 0; k < padding_out; k++)
            {
                fputc(0x00, outptr);
            }
            fseek(inptr, padding_in, SEEK_CUR);
//            printf("%ld ",ftell(inptr));

            i+=n-1;
            for(int q = 0; q < n-1 ; q++)
            {
                fseek(inptr, (sizeof(RGBTRIPLE)*og_width + padding_in), SEEK_CUR);
//                printf("%ld ",ftell(inptr));
            }
            printf("\n");
        }

    }


    if(f==1)
    {
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            // skip over padding, if any
            fseek(inptr, padding_in, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding_out; k++)
            {
                fputc(0x00, outptr);
            }
        }

    }







    // close infile
    fclose(inptr);
//    printf(" %d %d ", bi.biWidth, bi.biHeight);
    // close outfile
    fclose(outptr);

    // success
    return 0;
}
