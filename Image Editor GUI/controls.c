#include "macros.h"

#include "controls.h"

/// <summary>
/// Adds control to the window.
/// </summary>
/// <param name="hWnd">The parent window.</param>
void AddControl(HWND hWnd) {

	/* Containers */
	containerHide = CreateWindowW(L"static", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
		15, 25, 525, 230,
		hWnd, NULL, NULL, NULL);
	containerFind = CreateWindowW(L"static", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
		15, 275, 525, 150,
		hWnd, NULL, NULL, NULL);

	/* Comboboxes */
	hWndComboBoxHide = CreateWindowW(L"combobox", L"", CBS_DROPDOWNLIST | WS_VSCROLL | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		430, 70, 100, 100,
		hWnd, NULL, NULL, NULL);
	hWndComboBoxFind = CreateWindowW(L"combobox", L"", CBS_DROPDOWNLIST | WS_VSCROLL | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		430, 315, 100, 100,
		hWnd, NULL, NULL, NULL);

	wchar_t item[5][7] = { L"Select", L"1", L"2", L"4", L"8" };

	for (int k = 0; k < 5; k++) {
		SendMessage(hWndComboBoxFind, CB_ADDSTRING, 0, (LPARAM)item[k]);
		SendMessage(hWndComboBoxHide, CB_ADDSTRING, 0, (LPARAM)item[k]);
	}

	SendMessage(hWndComboBoxFind, CB_SETCURSEL, (WPARAM)0, (LPARAM)item);
	SendMessage(hWndComboBoxHide, CB_SETCURSEL, (WPARAM)0, (LPARAM)item);


	/* Textboxes */
	tbSourcePath_h = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_LEFT | ES_AUTOHSCROLL,
		TEXT_ROFFSET, 70, 360, 25,
		hWnd, NULL, NULL, NULL);
	tbDestinationPath = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_LEFT | ES_AUTOHSCROLL,
		TEXT_ROFFSET, 190, 364, 25,
		hWnd, NULL, NULL, NULL);
	tbSourcePath_f = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_LEFT | ES_AUTOHSCROLL,
		TEXT_ROFFSET, 315, 364, 25,
		hWnd, NULL, NULL, NULL);


	/* Labels */
	CreateWindowW(L"static", L"Hide section", WS_VISIBLE | WS_CHILD | BS_CENTER,
		TEXT_ROFFSET, 15, 100, TEXT_H,
		hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Find section", WS_VISIBLE | WS_CHILD | BS_CENTER,
		TEXT_ROFFSET, 265, 100, TEXT_H,
		hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Destination image:", WS_VISIBLE | WS_CHILD | BS_CENTER,
		TEXT_ROFFSET, 170, 165, TEXT_H,
		hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Source image:", WS_VISIBLE | WS_CHILD | BS_CENTER,
		TEXT_ROFFSET, 45, 120, TEXT_H,
		hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Source image:", WS_VISIBLE | WS_CHILD | BS_CENTER,
		TEXT_ROFFSET, 290, 120, TEXT_H,
		hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Bit depth:", WS_VISIBLE | WS_CHILD | BS_CENTER,
		430, 45, 75, TEXT_H,
		hWnd, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Bit depth:", WS_VISIBLE | WS_CHILD | BS_CENTER,
		430, 290, 75, TEXT_H,
		hWnd, NULL, NULL, NULL);

	/* Buttons */
	CreateWindowW(L"button", L"Select file!", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER,
		TEXT_ROFFSET, 100, 100, 25,
		hWnd, (HMENU)APP_SEARCH_IN_F_HIDE, NULL, NULL);
	CreateWindowW(L"button", L"Select file!", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER,
		TEXT_ROFFSET, 220, 100, 25,
		hWnd, (HMENU)APP_SEARCH_OUT_F_HIDE, NULL, NULL);
	CreateWindowW(L"button", L"Select file!", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER,
		TEXT_ROFFSET, 345, 100, 25,
		hWnd, (HMENU)APP_SEARCH_IN_F_FIND, NULL, NULL);

	btHide = CreateWindowW(L"button", L"Hide", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER,
		430, 200, 100, 45,
		hWnd, (HMENU)APP_HIDEBMP, NULL, NULL);
	btFind = CreateWindowW(L"button", L"Find image!", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_CENTER,
		430, 370, 100, 45,
		hWnd, (HMENU)APP_FINDBMP, NULL, NULL);
}