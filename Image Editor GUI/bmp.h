
/*****************************************************************

 Copyright 2001   PIER LUCA MONTESSORO

 University of Udine
 ITALY

 montessoro@uniud.it
 www.montessoro.it

 This file is part of a freeware open source software package.
 It can be freely used (as it is or modified) as long as this
 copyright note is not removed.

******************************************************************/
#pragma once

#include <stdio.h>
#include <stdbool.h>

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

/* Windows bitmap files (.BMP) version 3 */

typedef  unsigned char       byte;
typedef  unsigned short int  word;
typedef  unsigned long int   dword;

#define BYTE_SIZE 8

/* header structure definitions */

PACK(typedef struct mytagFILEHEADER
{
	word  ImageFileType;
	dword FileSize;
	word  Reserved1;
	word  Reserved2;
	dword ImageDataOffset;
} FILEHEADER);

PACK(typedef struct mytagBMPHEADER
{
   dword HeaderSize;
   dword ImageWidth;
   dword ImageHeight;
   word  NumberOfImagePlanes;
   word  BitsPerPixel;
   dword CompressionMethod;
   dword SizeOfBitmap;
   dword HorizonalResolution;
   dword VerticalResolution;
   dword NumberOfColorsUsed;
   dword NumberOfSignificantColors;
} BMPHEADER);

typedef struct mytagCOLORTRIPLE
{
   byte blue;
   byte green;
   byte red;
} COLORTRIPLE;

/* IMPORTANT NOTES:

   The number of bytes in one row of the file must always be adjusted to
   fit into the border of a multiple of four. Bytes set to zero must be
   appended if necessary.

   The image is stored upside down.
*/

typedef struct mytagBITMAP {
   dword width;
   dword height;
   COLORTRIPLE *pixel;
   FILEHEADER fileheader;
   BMPHEADER bmpheader;
} bitmap;

/* functions prototipes */

/// <summary>
/// Returns a bitmap object from a file pointer.
/// </summary>
/// <param name="fp">The file you want the bitmap object from.</param>
/// <returns>Returns a bitmap object of the input file.</returns>
bitmap ReadBitmap (FILE *fp);

/// <summary>
/// Writes the content of the bitmap to the file pointer.
/// </summary>
/// <param name="bitmap">The bitmap file to copy.</param>
/// <param name="fp">The destination file pointer.</param>
void WriteBitmap (bitmap bitmap, FILE *fp);

/// <summary>
/// 
/// </summary>
/// <param name="height"></param>
/// <param name="width"></param>
/// <returns></returns>
bitmap CreateEmptyBitmap (dword height, dword width);

/// <summary>
/// Releases memory allocated by the bitmap object.
/// </summary>
/// <param name="bitmap">The bitmap you want to free.</param>
void ReleaseBitmapData (bitmap* bitmap);

/// <summary>
/// Checks if a file is in a bmp file format.
/// </summary>
/// <param name="file">The file to check.</param>
/// <returns>Returns true if the file is a .bmp file, otherwise it returns false.</returns>
bool isBmpFile(FILE* file);

/// <summary>
/// Gets the size of the header in a bmp file.
/// </summary>
/// <param name="file">The file you want to check for.</param>
/// <returns>Returns the total size of the header in bytes.</returns>
int bmpGetHeaderSize(FILE* file);

/// <summary>
/// Gets the bmp's color depth. It can vary between 1, 4, 8, 16, 24 and 32.
/// </summary>
/// <param name="file">The input .bmp file.</param>
/// <returns>Returns the file depth.</returns>
int bmpGetColorDepth(FILE* file);

/// <summary>
/// Gets the offset from the start of the file to the raw pixel data.
/// </summary>
/// <param name="file">The input .bmp file.</param>
/// <returns>Retrns the offset in bytes.</returns>
int bmpGetPixelOffset(FILE* file);

/// <summary>
/// Gets the size of all the pixels in bytes.
/// </summary>
/// <param name="file">The input .bmp file.</param>
/// <returns>Returns the size of the raw pixel data. This shouldn't be confused with the file size.</returns>
int bmpGetDataSize(FILE* file);

/// <summary>
/// Gets the size of the color palette.
/// </summary>
/// <param name="file">The input .bmp file.</param>
/// <returns>Returns the color palette size. If the return is 0 then its a generic palette.</returns>
int bmpGetColorPaletteSize(FILE* file);

/// <summary>
/// Gets the total hidden size of pixel that can be used to store data from an input file.
/// </summary>
/// <param name="file">The input file.</param>
/// <param name="lsbDepth">The bit depth to calculate the hidden size.</param>
/// <returns>Returns the total size of bytes that can store data.</returns>
int bmpGetHiddenSizeInBytes(bitmap* file, int lsbDepth);

/// <summary>
/// Converts all the pixel channels values into a byte array.
/// </summary>
/// <param name="bmp">The input file.</param>
/// <param name="arr">The output byte array.</param>
/// <param name="size">The size of the output byte array.</param>
void bmpBitmapToByteArray(bitmap* bmp, byte* arr, int size);

/// <summary>
/// Copies all the byte array into the bitmap's pixels.
/// </summary>
/// <param name="bmp">The output bitmap with the values in it.</param>
/// <param name="pixels">The input byte array with the pixels to write in bmp.</param>
/// <param name="size">The size of the input byte array.</param>
void bmpByteArrayToBitmap(bitmap* bmp, byte* pixels, int size);

/// <summary>
/// Hides the input byte array into the output byte array.
/// </summary>
/// <param name="output">Output byte array.</param>
/// <param name="outputSize">The size of the output byte array.</param>
/// <param name="input">Input byte array.</param>
/// <param name="inputWidth">The width of the image.</param>
/// <param name="inputHeight">The height of the image.</param>
/// <param name="depth">The bit depth to overwrite the output byte array.</param>
/// <returns>Returns 1 if the operation succeed, otherwise returns 0.</returns>
int writeData(byte* output, int outputSize, byte* input, int inputWidth, int inputHeight, const int depth);

/// <summary>
/// Reads the data from the source byte array and places it into the destination byte array.
/// </summary>
/// <param name="source">The source byte array that stores the information to be read.</param>
/// <param name="sourceSize">The size of the source byte array.</param>
/// <param name="destination">A byte array, it needs to be freed once you're done with it.</param>
/// <param name="newFilename">The name of the file that will be filled with the output.</param>
/// <param name="depth">The bit depth to overwrite the output byte array.</param>
/// <returns>Returns 1 if the operation succeed, otherwise returns 0.</returns>
int readData(byte* source, int sourceSize, byte* destination, char* newFilename, const int depth);

/// <summary>
/// Hides the input image in a bitmap format into the destination image. A new file will be created with the given file name.
/// </summary>
/// <param name="inputImage">The fail in a bitmap format that will be hidden into the other image.</param>
/// <param name="destinationImage">The image that will be saved the other image in.</param>
/// <param name="newFilename">The new file name.</param>
/// <param name="depth">The depth to write with.</param>
void hideImage(bitmap* inputImage, bitmap* destinationImage, char* newFilename, int depth);

/// <summary>
/// Looks for a image into the given file, it will read the file and create a new one with the read data in it.
/// </summary>
/// <param name="inputFile">The file to be read in a bitmap format.</param>
/// <param name="newFilename">The name of the file that will be created.</param>
/// <param name="depth">The depth to read with.</param>
void lookForImage(bitmap* inputFile, char* newFilename, int depth);

/// <summary>
/// Converts a number to a binary string.
/// </summary>
/// <param name="n">The input number.</param>
/// <param name="len">The number of bits to be rapresented with.</param>
/// <returns>Returns the string.</returns>
char* toBinary(int n, int len);

