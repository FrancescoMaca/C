
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>

#include "bmp.h"

#pragma warning(disable : 4997)

#define OFFSET_PIXEL 0x0A
#define OFFSET_HEADER 0x0E
#define OFFSET_CDEPTH 0x1C
#define OFFSET_DATALEN 0x22
#define OFFSET_PALETTE 0x2E

#define PIXEL_CHANNELS 3

#define BMPFILETYPE 0x4D42

bitmap ReadBitmap(FILE *fp) {
   FILEHEADER fileheader;
   BMPHEADER bmpheader;
   COLORTRIPLE triple;
   bitmap bitmap = { 0 };

   int nstep;
   unsigned char fillbyte;
   int i,j, k, fill;

   int s1 = sizeof(FILEHEADER);
   int s2 = sizeof(BMPHEADER);

   /* read the headers */
   fread (&fileheader, sizeof(FILEHEADER), 1, fp);
   fread (&bmpheader, sizeof(BMPHEADER), 1, fp);

#ifdef BMPSHOWALL
   printf ("Following numbers are in hexadecimal representation\n");
   printf ("fileheader.ImageFileType = %x\n", fileheader.ImageFileType);
   printf ("fileheader.FileSize = %lx\n", fileheader.FileSize);
   printf ("fileheader.Reserved1 = %x\n", fileheader.Reserved1);
   printf ("fileheader.Reserved2 = %x\n", fileheader.Reserved2);
   printf ("fileheader.ImageDataOffset = %lx\n", fileheader.ImageDataOffset);

   printf ("bmpheader.HeaderSize = %lx\n", bmpheader.HeaderSize);
   printf ("bmpheader.ImageWidth = %lx\n", bmpheader.ImageWidth);
   printf ("bmpheader.ImageHeight = %lx\n", bmpheader.ImageHeight);
   printf ("bmpheader.NumberOfImagePlanes = %x\n",
           bmpheader.NumberOfImagePlanes);
   printf ("bmpheader.BitsPerPixel = %x\n", bmpheader.BitsPerPixel);
   printf ("bmpheader.CompressionMethod = %lx\n",
           bmpheader.CompressionMethod);
   printf ("bmpheader.SizeOfBitmap = %lx\n", bmpheader.SizeOfBitmap);
   printf ("bmpheader.HorizonalResolution = %lx\n",
           bmpheader.HorizonalResolution);
   printf ("bmpheader.VerticalResolution = %lx\n",
           bmpheader.VerticalResolution);
   printf ("bmpheader.NumberOfColorsUsed = %lx\n",
           bmpheader.NumberOfColorsUsed);
   printf ("bmpheader.NumberOfSignificantColors = %lx\n",
           bmpheader.NumberOfSignificantColors);
#endif

   if (fileheader.ImageFileType != BMPFILETYPE) {
     fclose (fp);
     MessageBox(NULL, L"Not a Windows bitmap file\n", L"Fatal Error", MB_OK | MB_ICONERROR);
     exit(EXIT_FAILURE);
   }

   if (bmpheader.CompressionMethod != 0) {
     fclose (fp);
	 MessageBox(NULL, L"Compressed images not supported\n", L"Fatal Error", MB_OK | MB_ICONERROR);
     exit(EXIT_FAILURE);
   }

   switch (bmpheader.BitsPerPixel)
   {
       case 8: {
           fclose(fp);
		   MessageBox(NULL, L"Color palette  not supported\n", L"Fatal Error", MB_OK | MB_ICONERROR);
           exit(EXIT_FAILURE);
           break;
       }
      case 16: {
          fclose(fp);
		  MessageBox(NULL, L"Color palette  not supported\n", L"Fatal Error", MB_OK | MB_ICONERROR);
          exit(EXIT_FAILURE);
          break;
      }

      case 24: {
          bitmap.pixel = (COLORTRIPLE*)
				  malloc(sizeof(COLORTRIPLE) *
                  bmpheader.ImageWidth * bmpheader.ImageHeight);

          if (bitmap.pixel == NULL) {
			  MessageBox(NULL, L"Memory allocation error\n", L"Fatal Error", MB_OK | MB_ICONERROR);
              exit(EXIT_FAILURE);
          }

          bitmap.width = bmpheader.ImageWidth;
          bitmap.height = bmpheader.ImageHeight;

          /* number of bytes is forced to be multiple of 4 */
          fill = bitmap.width % 4;

          for (i = 0; i < bitmap.height; i++) {
              for (j = 0; j < bitmap.width; j++) {
                  fread(&triple, sizeof(COLORTRIPLE), 1, fp);
                  nstep = j + (i * bitmap.width);
                  bitmap.pixel[nstep] = triple;
              }

              for (k = 0; k < fill; k++)
                  fread(&fillbyte, sizeof(unsigned char), 1, fp);
          }

#ifdef BMPSHOWALL
          printf("%d pixels loaded\n", nstep + 1);
#endif
          break;
      }

      default: {
		  MessageBox(NULL, L"Unsupported Palette format\n", L"Fatal Error", MB_OK | MB_ICONERROR);
          exit(EXIT_FAILURE);
      }
   }

   /* save the headers read from the .bmp file */
   bitmap.fileheader = fileheader;
   bitmap.bmpheader = bmpheader;

   return bitmap;
}

void WriteBitmap(bitmap bitmap, FILE *fp) {


   COLORTRIPLE triple;
   unsigned char fillbyte = 0;
   int nstep;
   int i, j, k, fill;

   fwrite (&bitmap.fileheader, sizeof (FILEHEADER), 1, fp);
   fwrite (&bitmap.bmpheader, sizeof (BMPHEADER), 1, fp);

   /* number of bytes in a row must be multiple of 4 */
   fill = bitmap.width % 4;

   // Itera attraverso ogni pixel della bitmap
   for (i = 0; i < bitmap.height; i++) {
      for (j = 0; j < bitmap.width; j++) {
         // Per ogni pixel calcola l'offset dell'array
         nstep = j + (i * bitmap.width);

         // Salva il pixel in triple
         triple = bitmap.pixel[nstep];

         // Scrive triple nel file fp
         fwrite(&triple, sizeof(COLORTRIPLE), 1, fp);
      }

      for (k = 0; k < fill; k++)
         fwrite (&fillbyte, sizeof(unsigned char), 1, fp);
   }

#ifdef BMPSHOWALL
   printf ("%d pixels written\n", nstep + 1);
#endif
   return;
}

void ReleaseBitmapData(bitmap *bitmap) {

   free(bitmap->pixel);
   bitmap->bmpheader.ImageHeight = bitmap->height = 0;
   bitmap->bmpheader.ImageWidth = bitmap->width = 0;
   bitmap->pixel = NULL;

   return;
}

bitmap CreateEmptyBitmap(dword height, dword width) {
	bitmap bitmap = { 0 };

#ifdef BMPSHOWALL
   printf ("Creating empty bitmap %d x %d pixels\n", height, width);
#endif

   /* bitmap header */
   bitmap.fileheader.ImageFileType = BMPFILETYPE;   /* magic number! */
   bitmap.fileheader.FileSize = 14 + 40 + (height * width * 3);
//   bitmap.fileheader.Reserved1 = 0;
//   bitmap.fileheader.Reserved2 = 0;
   bitmap.fileheader.ImageDataOffset = 14 + 40;

   /* bmp header */
   bitmap.bmpheader.HeaderSize = 40;
   bitmap.bmpheader.ImageWidth = bitmap.width = width;
   bitmap.bmpheader.ImageHeight = bitmap.height = height;
   bitmap.bmpheader.NumberOfImagePlanes = 1;
   bitmap.bmpheader.BitsPerPixel = 24;             /* the only supported format */
   bitmap.bmpheader.CompressionMethod = 0;         /* compression is not supported */
   bitmap.bmpheader.SizeOfBitmap = 0;              /* conventional value for uncompressed images */
   bitmap.bmpheader.HorizonalResolution = 0;       /* currently unused */
   bitmap.bmpheader.VerticalResolution = 0;        /* currently unused */
   bitmap.bmpheader.NumberOfColorsUsed = 0;        /* dummy value */
   bitmap.bmpheader.NumberOfSignificantColors = 0; /* every color is important */
   
   bitmap.pixel = (COLORTRIPLE*)calloc(width * height, sizeof(COLORTRIPLE));

   if (bitmap.pixel == NULL) {
	   MessageBox(NULL, L"Memory allocation error.\n", L"Fatal Error", MB_OK | MB_ICONERROR);

      exit(EXIT_FAILURE);
   }

   return bitmap;
}

bool isBmpFile(FILE* file) {

	// Error check
	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}

	short code;

	fseek(file, 0, 0);
	fread(&code, sizeof(short), 1, file);

	// Checks for the correct BMP format value
	if (code == 0x4d42) {
		return true;
	}

	return false;
}

int bmpGetHeaderSize(FILE* file) {

	// Error check
	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}

	int hSize;

	fseek(file, OFFSET_HEADER, 0);
	fread(&hSize, sizeof(int), 1, file);

	return hSize;
}

int bmpGetColorDepth(FILE* file) {

	// Error check
	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}

	short cDepth;

	fseek(file, OFFSET_CDEPTH, 0);
	fread(&cDepth, sizeof(short), 1, file);

	return cDepth;
}

int bmpGetPixelOffset(FILE* file) {

	// Error check
	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}

	short pixelOffset;

	fseek(file, OFFSET_PIXEL, 0);
	fread(&pixelOffset, sizeof(short), 1, file);

	return pixelOffset;
}

int bmpGetDataSize(FILE* file) {

	// Error check
	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}

	int dSize;

	fseek(file, OFFSET_DATALEN, 0);
	fread(&dSize, sizeof(int), 1, file);

	return dSize;
}

int bmpGetColorPaletteSize(FILE* file) {

	// Error check
	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}
	int paletteSize;

	fseek(file, OFFSET_PALETTE, 0);
	fread(&paletteSize, sizeof(int), 1, file);

	return paletteSize;

}

int bmpGetHiddenSizeInBytes(bitmap* file, int lsbDepth) {

	// Error check
	if (lsbDepth > 8 || lsbDepth < 1) {
#ifdef DEBUG
		printf("%s() - The bit depth is not valid ( > 8 or < 1)\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return -1;
	}

	if (file == NULL) {
#ifdef DEBUG
		printf("%s() - The file is null\n", __func__);
#endif
		return -1;
	}

	return ((file->height * file->width * PIXEL_CHANNELS) / 8) * lsbDepth;
}

void bmpBitmapToByteArray(bitmap* bmp, byte* pixels, int size) {

	if (pixels == NULL) {
#ifdef DEBUG
		printf("%s() - Byte array is null\n", __func__);
		MessageBox(NULL, L"The byte array is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return;
	}

	for (int i = 0, j = 0; i < size; i += 3, j++) {
		pixels[i] = bmp->pixel[j].red;
		pixels[i + 1] = bmp->pixel[j].green;
		pixels[i + 2] = bmp->pixel[j].blue;
	}
}

void bmpByteArrayToBitmap(bitmap* bmp, byte* pixels, int size) {

	if (bmp == NULL) {
#ifdef DEBUG
		printf("%s() - Bitmap is null\n", __func__);
		MessageBox(NULL, L"The file is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return;
	}

	if (pixels == NULL) {
#ifdef DEBUG
		printf("%s() - Byte array is null\n", __func__);
		MessageBox(NULL, L"The byte array is null.\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return;
	}

	for (int i = 0, j = 0; i < size; i += 3, j++) {
		bmp->pixel[j].red = pixels[i];
		bmp->pixel[j].green = pixels[i + 1];
		bmp->pixel[j].blue = pixels[i + 2];
	}
}

void hideImage(bitmap* inputImage, bitmap* destinationImage, char* newFilename, int depth) {

	if (destinationImage == NULL) {
#ifdef DEBUG
		printf("Destination is not valid!\n");
		MessageBox(NULL, L"Destination is not valid!\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return;
	}

	if (inputImage == NULL) {
#ifdef DEBUG
		printf("Input file is not valid!\n");
		MessageBox(NULL, L"The input file is not valid!\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return;
	}

	int inputSize = sizeof(COLORTRIPLE) * inputImage->height * inputImage->width;
	int outputSize = sizeof(COLORTRIPLE) * destinationImage->height * destinationImage->width;

	/* Creates the buffers to store bmp data */
	byte* bIn = malloc(inputSize);
	byte* bOut = malloc(outputSize);

	/* Converts the bitmaps to byte array */
	bmpBitmapToByteArray(inputImage, bIn, inputSize);
	bmpBitmapToByteArray(destinationImage, bOut, outputSize);

	/* Hides the input data to the output array */
	if (writeData(bOut, outputSize, bIn, inputImage->width, inputImage->height, depth) == 0) {
		free(bOut);
		free(bIn);
		return;
	}

	/* Converts the modified output data to a bmp file */
	bmpByteArrayToBitmap(destinationImage, bOut, outputSize);

	/* Creating new file for the output */
	FILE* outFile;
	fopen_s(&outFile, newFilename, "wb");

	/* Checking if the file has been created */
	if (outFile == 0) {
		return;
	}

	/* Writes the new bitmap to the new file */
	WriteBitmap(*destinationImage, outFile);

	/* Closing and freeing all the memory */
	fclose(outFile);
	free(bOut);
	free(bIn);

	return;
}

void lookForImage(bitmap* inputFile, char* newFilename, int depth) {

	if (inputFile == NULL) {
#ifdef DEBUG
		printf("Destination is not valid!\n");
		MessageBox(NULL, L"Destination is not valid!\n", L"Fatal Error", MB_OK | MB_ICONERROR);
#endif
		return;
	}

	int inputSize = sizeof(COLORTRIPLE) * inputFile->height * inputFile->width;

	/* Creates the buffers to store bmp data */
	byte* bIn = malloc(inputSize);
	byte* bOut = { 0 };

	bmpBitmapToByteArray(inputFile, bIn, inputSize);

	if (readData(bIn, inputSize, bOut, newFilename, depth) == 0) {
		DeleteFile(newFilename);
	}

	free(bIn);
	free(bOut);

	return;
}

int readData(byte* source, int sourceSize, byte* destination, char* filename, const int depth) {

	/* Checking the bit depth */
	if (BYTE_SIZE % depth != 0 || depth == 0) {
#ifdef DEBUG
		printf("%s() - The bit depth is not valid.\n", __func__);
#endif
		MessageBox(NULL, L"The bit depth is not valid!\n", L"Fatal Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	int indexSource = 0, indexDestination = 0;
	int bitIndexDestination = 0;

	/* Counts how many cycles it takes to fill a byte with a given depth */
	int byteSlices = BYTE_SIZE / depth;

	/* Buffer to hold the slices of the byte that will recreate the data */
	byte* bufferOutput = malloc(byteSlices * sizeof(byte));

#ifdef DEBUG
	/* Input passed to function */
	printf("Source: %p (size: %d)\n", source, sourceSize);
	printf("Destination: %p (size: %d)\n", destination, destinationSize);
	printf("Used depth: %d\n", depth);
	printf("Number of reads to recreate a byte: %d\n", byteSlices);
#endif

	for (int i = 0; i < byteSlices; i++) {
		bufferOutput[i] = 0;
	}

	int dataLength = 0;
	int width = 0, height = 0;

	byte mask = ((int)pow(2, depth) - 1);

	/* Reads the header of the data */

	/* Reads the image width */
	for (int i = 0; i < (sizeof(int) * BYTE_SIZE) / depth; i++) {
		width |= (source[indexSource] & mask) << i * depth;
		indexSource++;
	}

	/* Reads the image height */
	for (int i = 0; i < (sizeof(int) * BYTE_SIZE) / depth; i++) {
		height |= (source[indexSource] & mask) << i * depth;
		indexSource++;
	}

	/* Incrementing once more the destination counter */
	indexSource++;

	/* Calculates the image size in pixels */
	dataLength = width * height * sizeof(COLORTRIPLE);

	/* Allocates space for the output byte array */
	destination = malloc(dataLength);

	while (indexSource < dataLength * byteSlices + (sizeof(int) * 2)) {

#ifdef DEBUG 
		printf("Current indexes: \n");
		printf("- input byte array's index:  %d | %s (source)\n", indexSource, toBinary(indexSource, BYTE_SIZE));
		printf("- output byte array's index: %d | %s (destination)\n", indexSource, toBinary(indexSource, BYTE_SIZE));

		printf("Current read buffer: \n");
		for (int i = 0; i < byteSlices; i++) {
			printf("buffer[%d] - %d \n", i, bufferOutput[i]);
		}

		_getch();
#endif
		/* Creating buffer */
		byte bufferSource = source[indexSource];

		/* Mask to read only the first [depth] bits */
		byte mask = ((int)pow(2, depth) - 1);

		/* Each time the buffer fills up it recreates a byte and resets itself */
		if (indexSource % byteSlices == 0 && indexSource != 0) {

			destination[indexDestination] = 0;

			/* Merging all reads together.
			* Description:
			* Since we need to rebuild data we read [x] bits, [x] = 8 / [depth]
			* so, let's say I have a depth of 2:
			* slice 1: 10
			* slice 2: 11
			* slice 3: 10
			* slice 4: 00
			*
			* output byte: slice 1 << [depth * 0] | slice 2 << [depth * 1]...
			* 01 + 1100 + 100000 + 00000000 = 00101101 (byte recreated)
			*/
#ifdef DEBUG
			printf("Merging read buffer...\n");
#endif

			for (int i = 0; i < byteSlices * sizeof(byte); i++) {
#ifdef DEBUG
				printf("Writing destination buffer %d...\n", i);
				printf("Adding %d (%s) to destination...\n", i, bufferOutput[i], toBinary(bufferOutput[i] << (i * depth), BYTE_SIZE));
#endif
				destination[indexDestination] |= (bufferOutput[i] << (i * depth));
			}

			/* Reset output buffer */
			for (int i = 0; i < byteSlices; i++) {
				bufferOutput[i] = 0;
			}

			indexDestination++;
		}
		/* Saves the last [depth] bit in the source byte array */
		bufferOutput[indexSource % byteSlices] = bufferSource & mask;

		indexSource++;
	}

	/* Save read to file */
	FILE* outFile;
	fopen_s(&outFile, filename, "wb");
	
	if (outFile == 0) {
#ifdef DEBUG
		printf("%s() - The file \"%s\" couldn't be created.\n", __func__, filename);
#endif
		return 0;
	}

	bitmap out = CreateEmptyBitmap(height, width);

	/* Converts the byte array to bitmap and then copies it into the new file*/
	bmpByteArrayToBitmap(&out, destination, dataLength);
	WriteBitmap(out, outFile);

	fclose(outFile);

	/* Frees the temporary buffer */
	free(bufferOutput);
	free(destination);

#ifdef DEBUG
	printf("Exiting %s()\n", __func__);
#endif

	return 1;
}

int writeData(byte* output, int outputSize, byte* input, int inputWidth, int inputHeight, const int depth) {

	const int inputSize = inputHeight * inputWidth * sizeof(COLORTRIPLE);

	/* Checking the bit depth */
	if (BYTE_SIZE % depth != 0 || depth == 0) {
#ifdef DEBUG
		printf("%s() - The bit depth is not valid.\n", __func__);
#endif
		MessageBox(NULL, L"The bit depth is not valid!\n", L"Fatal Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	/* Checking that the amount of data can fit in the output */
	if ((outputSize * depth) < inputSize * BYTE_SIZE) {
#ifdef DEBUG
		printf("%s() - Data can't fit in the output.\n", __func__);
#endif
		MessageBox(NULL, L"Data can't fit in the output!\n", L"Fatal Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	int indexInput = 0, indexOutput = 0;
	int bitIndexInput = 0;

#ifdef DEBUG
	/* Input passed to function */
	printf("Source: %p (size: %d)\n", output, outputSize);
	printf("Destination: %p (size: %d)\n", input, inputSize);
	printf("Used depth: %d\n", depth);
#endif

	/* Mask to remove last [depth] bits */
	byte depthRemover = (((int)pow(2, BYTE_SIZE) - 1) - ((int)pow(2, depth) - 1));

	/* Writes the header of the data to be read */
	for (int i = 0; i < (sizeof(int) * BYTE_SIZE) / depth; i++) {
		byte mask = (((1 << depth) - 1) & (inputWidth >> i * depth));
		output[indexOutput] &= depthRemover;
		output[indexOutput] |= mask;
		indexOutput++;
	}

	/* Writes the header of the data to be read */
	for (int i = 0; i < (sizeof(int) * BYTE_SIZE) / depth; i++) {
		byte mask = (((1 << depth) - 1) & (inputHeight >> i * depth));
		output[indexOutput] &= depthRemover;
		output[indexOutput] |= mask;
		indexOutput++;
	}

#ifdef DEBUG
	printf("Hidden size: %d | %s\n(this is hidden in the first %d byte of the output data)\n",
		inputSize,
		toBinary(inputSize, BYTESIZE * sizeof(int)),
		((sizeof(int) * BYTE_SIZE) / depth));

#endif

	while ((bitIndexInput + (indexInput * BYTE_SIZE)) < (inputSize * BYTE_SIZE)) {
#ifdef DEBUG
		printf("Current condition: %d < %d\n",
			bitIndexInput + (indexInput * BYTE_SIZE),
			inputSize * BYTE_SIZE);
#endif
		/* Creating buffers */
		byte bufferSource = output[indexOutput];
		byte bufferInput = input[indexInput];

		/* Creating mask for the output data */
		byte mask = (((1 << depth) - 1) & (bufferInput >> bitIndexInput));

#ifdef DEBUG
		printf("Buffers:\n");
		printf("\tSource: %d (char: \'%c\') | %s", bufferSource, bufferSource, toBinary(bufferSource, BYTE_SIZE));
		printf("\tInput:  %d (char: \'%c\') | %s", bufferInput, bufferInput, toBinary(bufferInput, BYTE_SIZE));
		printf("Value mask: %d (%d with length %d)\n", mask, bitIndexInput, depth);
		printf("Depth mask: %d | %s\n", depthRemover, toBinary(depthRemover, BYTE_SIZE));
#endif

		bufferSource = bufferSource & depthRemover;
		bufferSource = bufferSource | mask;
		output[indexOutput] = bufferSource;

#ifdef DEBUG
		printf("Indexes:\n");
		printf("\tOutput: %d\n", indexOutput);
		printf("\tInput: %d\n", indexInput);
		printf("output[%d]: %d | %s\n", indexOutput, output[indexOutput], toBinary(output[indexOutput], BYTE_SIZE));
		getch();
#endif

		bitIndexInput += depth;
		if (bitIndexInput % BYTE_SIZE == 0) {
			bitIndexInput = 0;
			indexInput++;
		}

		indexOutput++;
	}

#ifdef DEBUG
	printf("Exiting %s()\n", __func__);
#endif

	return 1;
}

char* toBinary(int n, int len) {

	if (len > 64) {
		return NULL;
	}

	// array to store binary number
	static char binaryNum[65] = { 0 };
	int i;
	for (i = 0; i < len; i++)
		binaryNum[i] = '0';

	binaryNum[len] = '\0';

	while (n > 0) {

		// storing remainder in binary array
		if (n % 2 == 0) {
			binaryNum[i] = '0';
		}
		else {
			binaryNum[i] = '1';
		}

		n = n / 2;
		i--;
	}

	return binaryNum;
}