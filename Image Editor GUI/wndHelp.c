#include "wndHelp.h"

LRESULT _stdcall WindowHelpProc(HWND, UINT, WPARAM, LPARAM);

/// <summary>
/// Prints the help message.
/// </summary>
/// <param name="hWnd">The parent window.</param>
void CreateHelpWnd(HWND hWnd) {
	MessageBox(hWnd, L"This application allows you to hide or look for images in other images. The \"Hide\" section is used when you need to hide one image (source image) into another one(destination image). Just type the source path and the destination path. Then click the button. The \"Find image\" section will ask you to insert just the image in which you want to look an image for.", L"Help", MB_OK | MB_ICONINFORMATION);
}