#include <windows.h>
#include <math.h>
#include <commdlg.h>

#include "wndCreateBmp.h"
#include "wndHelp.h"
#include "macros.h"
#include "controls.h"
#include "menu.h"
#include "bmp.h"

LRESULT _stdcall WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND btHide, tbSourcePath_h, tbDestinationPath, hWndComboBoxHide;
HWND btFind, tbSourcePath_f, hWndComboBoxFind;
HWND containerHide, containerFind;

/// <summary>
/// Entry point.
/// </summary>
int _stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	// Creating new class
	WNDCLASS wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"hwndSteganography";
	wc.lpfnWndProc = WindowProc;

	// Register the class
	if (!RegisterClassW(&wc)) {
		return -1;
	}

	// Create the main window
	CreateWindowW(L"hwndSteganography", L"Image Editor", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		960 - (W_HWND / 2), 540 - (H_HWND / 2), W_HWND, H_HWND,
		NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	// Message loop
	while(GetMessageW(&msg, (HWND)NULL, (UINT)NULL, (UINT)NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

/// <summary>
/// Main window procedures.
/// </summary>
LRESULT _stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg) {
		// Event to listen to the GUI
		case WM_COMMAND: {

			// Handles varius custom events
			switch (wp) {
			
				// Exits the application
			case APP_EXIT: {
				DestroyWindow(hWnd);
				break;
			}
				// Shows the help window
			case SHOW_HELP: {
				CreateHelpWnd(hWnd);
				break;
			}
				// Creates a new bitmap file of predefines size
			case SHOW_CREATEBMP: {
				wchar_t* path = CreateBmpFile(800, 600, hWnd);
			
				if (lstrlenW(path) == 0) {
					MessageBox(hWnd, L"Something went wrong with the file creation...", L"File not created", MB_OK | MB_ICONERROR);
					break;
				}

				wchar_t* text = L"A new bitmap file has been created in you local folder.\n ";
				wprintf(text, L"A new bitmap file has been created in you local folder.\n %s", path);

				MessageBox(hWnd, text, L"File created!", MB_OK);
				break;
			}
				// These three APP_SEARCH are for the find file dialog
			case APP_SEARCH_IN_F_FIND: {
					
				OPENFILENAME ofn;

				wchar_t file_name[255];

				memset(&ofn, 0, sizeof(OPENFILENAME));

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = file_name;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = 255;
				ofn.lpstrFilter = L"*.bmp\0*.BMP*\0";
				ofn.nFilterIndex = 1;

				if (GetOpenFileNameW(&ofn)) {
					SetWindowText(tbSourcePath_f, ofn.lpstrFile);
				}
					
				break;
			}
			case APP_SEARCH_IN_F_HIDE: {

				OPENFILENAME ofn;

				wchar_t file_name[255];

				memset(&ofn, 0, sizeof(OPENFILENAME));

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = file_name;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = 255;
				ofn.lpstrFilter = L"*.bmp\0*.BMP*\0";
				ofn.nFilterIndex = 1;

				if (GetOpenFileNameW(&ofn)) {
					SetWindowText(tbSourcePath_h, ofn.lpstrFile);
				}
				break;
			}
			case APP_SEARCH_OUT_F_HIDE: {

				OPENFILENAME ofn;

				wchar_t file_name[255];

				memset(&ofn, 0, sizeof(OPENFILENAME));

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = file_name;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = 255;
				ofn.lpstrFilter = L"*.bmp\0*.BMP*\0";
				ofn.nFilterIndex = 1;

				if (GetOpenFileNameW(&ofn)) {
					SetWindowText(tbDestinationPath, ofn.lpstrFile);
				}
				break;
			}
				// Hides the selected image
			case APP_HIDEBMP: {
				// Temporary buffer
				wchar_t wbuff[255];

				// gets the textbox index
				int currIndex = SendMessage(hWndComboBoxHide, CB_GETCURSEL, 0, 0);

				// Asks to the window to get the selected index's value
				SendMessage(hWndComboBoxHide, CB_GETLBTEXT, currIndex, (LPARAM)wbuff);

				// Gets the value in the textboxes and the one selected in the combobox
				if (GetWindowTextW(tbDestinationPath, wbuff, 255) && GetWindowTextW(tbDestinationPath, wbuff, 255) && (currIndex > 0 && currIndex < 5)) {
					
					// Calculates the depth based on the index
					int depth = (int)powl(2, (double)currIndex - 1);
					
					// Defines main files
					FILE* fpSrc;
					FILE* fpDest;
					
					// Reads the textboxes
					GetWindowText(tbSourcePath_h, wbuff, 255);
					_wfopen_s(&fpSrc, wbuff, L"rb");

					GetWindowText(tbDestinationPath, wbuff, 255);
					_wfopen_s(&fpDest, wbuff, L"rb");

					// If the files are null then it throws an error
					if (fpSrc == 0) {
						MessageBeep(0);
						MessageBox(hWnd, L"There has been a problem opening the file", L"Error", MB_OK | MB_ICONERROR);
						break;
					}

					if (fpDest == 0) {
						MessageBeep(0);
						MessageBox(hWnd, L"There has been a problem opening the file", L"Error", MB_OK | MB_ICONERROR);
						break;
					}

					// If they're not then ccreates the bitmap objects
					bitmap src = ReadBitmap(fpSrc);
					bitmap dest = ReadBitmap(fpDest);

					// Defines a defualt file name
					char* defaultFilename = "output_hide.bmp";

					// Hides the image
					hideImage(&src, &dest, defaultFilename, depth);

					// If the name has been changed to empty then the logic has faulted
					if (strcmp(defaultFilename, " ") == 0) {
						MessageBox(hWnd, L"An error occurred!", L"Task not complete", MB_OK | MB_ICONERROR);
						break;
					}

					// Frees all the allocated data
					ReleaseBitmapData(&src);
					ReleaseBitmapData(&dest);
					fclose(fpSrc);
					fclose(fpDest);

					// Send back a response message that everything has gone right
					MessageBox(hWnd, L"The image has been hidden!", L"Task complete", NULL);
				}
				else {
					// If not all the fields are selected then it throws a message error
					MessageBeep(0);
					MessageBox(hWnd, L"Insufficient data!", L"Error", MB_OK | MB_ICONERROR);
				}

				break;
			}
				// Handles the finding-image logic
			case APP_FINDBMP: {

				// Temporary buffer
				wchar_t wbuff[255];

				// gets the textbox index
				int currIndex = SendMessage(hWndComboBoxFind, CB_GETCURSEL, 0, 0);

				// Asks to the window to get the selected index's value
				SendMessage(hWndComboBoxFind, CB_GETLBTEXT, currIndex, (LPARAM)wbuff);

				// If the user has entered all necessary data then it proceeds
				if (GetWindowTextW(tbSourcePath_f, wbuff, 255) != 0 && (currIndex > 0 && currIndex < 5)) {

					// Calculates the depth with the selected index
					int depth = (int)powl(2, (double)currIndex - 1);

					// Defines the files
					FILE* fpSrc;

					// Gets the textbox's text and saves that into the temporary buffer
					GetWindowText(tbSourcePath_f, wbuff, 255);
					_wfopen_s(&fpSrc, wbuff, L"rb");

					// If the file is nulls it throws an error and exits
					if (fpSrc == 0) {
						MessageBeep(0);
						MessageBox(hWnd, L"There has been a problem opening the file", L"Error", MB_OK | MB_ICONERROR);
						break;
					}

					// Creates a bitmap object
					bitmap src = ReadBitmap(fpSrc);

					// Defines the output file name
					char* output_filename = "output_find.bmp";

					// The logic behind the read of an image
					lookForImage(&src, output_filename, depth);

					// Frees all the recources
					ReleaseBitmapData(&src);
					fclose(fpSrc);

					// Send back a response message that everything has gone right
					MessageBox(hWnd, L"The image has been read!", L"Task complete", NULL);
				}
				else {
					// Throws a message error if just one field is empty
					MessageBeep(0);
					MessageBox(hWnd, L"Insufficient data!", L"Error", MB_OK | MB_ICONERROR);
				}

				break;
			}
			}

			break;
		}
			// Windows creation
		case WM_CREATE: {
			AddMenu(hWnd);
			AddControl(hWnd);
			break;
		}
			// Windows destruction
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
	}

	// It default handles all the events that we didn't take care of
	return DefWindowProcW(hWnd, msg, wp, lp);
}