
#include "wndCreateBmp.h"
#include "bmp.h"

/// <summary>
/// Creates a bmp file on user request.
/// </summary>
/// <param name="width">The width of the image.</param>
/// <param name="height">The height of the image</param>
/// <returns>Returns the path to the image.</returns>
wchar_t* CreateBmpFile(DWORD width, DWORD height) {

	wchar_t path[150];
	
	// Gets current directory file
	GetCurrentDirectoryW(150, path);

	// Creates the bitmap
	bitmap bmp = CreateEmptyBitmap(1000, 1000);
	FILE* fp;
	fopen_s(&fp, "new_image.bmp", "wb");
	
	// If the file is null then it throws error
	if (fp == 0) {
		return L"";
	}

	// Writes the bitmap into the file
	WriteBitmap(bmp, fp);

	// Releases all the recources
	fclose(fp);
	ReleaseBitmapData(&bmp);

	return path;
}