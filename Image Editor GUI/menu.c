#include "menu.h"

void AddMenu(HWND hWnd) {

	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hCreateSubMenu = CreateMenu();

	/* Default menu style */
	MENUINFO defaultMenu = {
		sizeof(MENUINFO),
		MIM_STYLE | MIM_APPLYTOSUBMENUS,
		MNS_AUTODISMISS,
		0,
		NULL,
		GetMenuContextHelpId(hMenu),
		NULL
	};

	SetMenuInfo(hMenu, &defaultMenu);

	/* Creating "Create"'s  submenu */
	AppendMenuW(hCreateSubMenu, MF_STRING, SHOW_CREATEBMP, L"New .bmp");

	AppendMenuW(hFileMenu, MF_POPUP, hCreateSubMenu, L"Create");
	AppendMenuW(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenuW(hFileMenu, MF_STRING, APP_EXIT, L"Exit");

	AppendMenuW(hMenu, MF_POPUP, hFileMenu, L"File");

	AppendMenuW(hMenu, MF_STRING, SHOW_HELP, L"Help");

	SetMenu(hWnd, hMenu);
}