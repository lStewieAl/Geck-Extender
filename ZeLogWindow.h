#pragma once
#pragma comment(lib, "comctl32.lib")
#include "GECKUtility.h"

#define UI_CMD_ADDLOGTEXT	(WM_APP + 1)
#define UI_CMD_CLEARLOGTEXT (WM_APP + 2)
#define UI_CMD_AUTOSCROLL	(WM_APP + 3)
#define UI_CMD_ADDLOGTEXT_NOFREE (WM_APP + 4)

#define UI_EXTMENU_ID			51001
#define UI_EXTMENU_SHOWLOG		51002
#define UI_EXTMENU_CLEARLOG		51003
#define UI_EXTMENU_AUTOSCROLL	51004
#define UI_EXTMENU_SPACER		51005
#define UI_EXTMENU_SPELLCHECK	51006
#define UI_EXTMENU_RENDER		51007
#define UI_EXTMENU_PREVIEW		51008
#define UI_EXTMENU_LAUNCHGAME   51009
#define UI_EXTMENU_SAVEPOSITION 51010
#define UI_EXTMENU_LOADPOSITION 51011
#define UI_EXTMENU_TOGGLENAVEMESHBISECT 51018
#define ID_TRACKBAR				51012
#define ID_TIMEOFDAYTEXT		51013
#define ID_RENDERWINDOWCELLLOADS_CHECKBOX 51014
#define ID_RENDERWINDOW_SHOWWATER_CHECKBOX	51015
#define ID_RENDERWINDOW_SHOWPORTALS_CHECKBOX	51016
#define UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED	51017
#define MAIN_WINDOW_CALLBACK 0xFEED

// unused button in vanilla menu
#define MENUOPTION_RENDER_WINDOW 0x9D06
#define MENUOPTION_RECOMPILE_ALL 0x9ECE

HWND g_MainHwnd;
HWND g_ConsoleHwnd;
HMENU g_ExtensionMenu;
HMENU g_MainMenu;
WNDPROC OldEditorUI_WndProc;

NiPoint3 savedRenderPos;
NiMatrix33 savedRenderDirection;

HWND g_trackBarHwnd;
HWND g_timeOfDayTextHwnd;
HWND g_allowCellWindowLoadsButtonHwnd;
HWND g_renderWindowShowWaterButtonHwnd;
HWND g_renderWindowShowPortalsButtonHwnd;

void ToggleNavmeshPlaceAboveOthers(bool isAllowed)
{
	//	Patch Navmesh editing to allow placing vertices over existing navmesh (disables line bisection)
	SafeWrite16(0x0045C590, isAllowed ? 0x9090 : 0x4675);
	bNavmeshAllowPlaceAboveOthers = isAllowed;
}

void ToggleObjectWindowFilterUnedited(bool show)
{
	if (show)
	{
		SafeWriteBuf(0x439739, "\x8B\x44\x24\x1C\x85\xC0", 6);
	}
	else
	{
		WriteRelJump(0x439739, UInt32(ObjectWindowListFilterUneditedHook));
	}
}


bool EditorUI_CreateExtensionMenu(HWND MainWindow, HMENU MainMenu)
{
	//	Create extended menu options
	g_ExtensionMenu = CreateMenu();

	BOOL result = TRUE;
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_SAVEPOSITION, "Save Position");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_LOADPOSITION, "Load Position");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_SHOWLOG, "Show Log");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_CLEARLOG, "Clear Log");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_AUTOSCROLL, "Autoscroll Log");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_SEPARATOR, (UINT_PTR)UI_EXTMENU_SPACER, "");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_SPELLCHECK, "Enable Spell Check");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_RENDER, "Render Window Uncap (requires restart)");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_PREVIEW, "Preview Window Uncap (requires restart)");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED, "Object Window - Only Show Edited");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_TOGGLENAVEMESHBISECT, "Allow Placement Of Navmesh Node Above Others");

	MENUITEMINFO menuInfo;
	memset(&menuInfo, 0, sizeof(MENUITEMINFO));
	menuInfo.cbSize = sizeof(MENUITEMINFO);
	menuInfo.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_STRING;
	menuInfo.hSubMenu = g_ExtensionMenu;
	menuInfo.wID = UI_EXTMENU_ID;
	menuInfo.dwTypeData = (char*)"Extensions";
	menuInfo.cch = (uint32_t)strlen(menuInfo.dwTypeData);
	result = result && InsertMenuItem(MainMenu, -1, TRUE, &menuInfo);

	return result ? true : false;
}

void EditorUI_AddSliderToToolbar(HWND MainWindow, HINSTANCE hInstance) {
	g_trackBarHwnd = CreateWindowEx(
		NULL,
		TRACKBAR_CLASSA,
		"Trackbar Control",
		(WS_CHILD | WS_VISIBLE | TBS_NOTICKS),
		898, 2, // x, y
		200, 24, // width, height
		MainWindow,
		(HMENU)ID_TRACKBAR,
		hInstance,
		NULL
	);
	SendMessageA(g_trackBarHwnd, TBM_SETRANGE, TRUE, MAKELONG(0, 24 * 4));
	SendMessageA(g_trackBarHwnd, TBM_SETTICFREQ, 10, 0);
	SendMessageA(g_trackBarHwnd, TBM_SETPOS, TRUE, 10 * 4);

}

void EditorUI_AddTimeOfDayTextToToolbar(HWND MainWindow, HINSTANCE hInstance) {
	g_timeOfDayTextHwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"Edit",
		"10.00",
		(WS_CHILD | WS_VISIBLE | ES_LEFT),
		1105, 5, // x, y
		45, 20, // width, height
		MainWindow,
		(HMENU)ID_TIMEOFDAYTEXT,
		hInstance,
		NULL
	);

	// try something nice, otherwise fall back on SYSTEM_FIXED_FONT
	fontHandle = CreateFontIndirect(&editorFont);
	if (fontHandle)
	{
		fontInfo = editorFont;
	}
	else
	{
		fontHandle = GetStockObject(SYSTEM_FIXED_FONT);
		GetObject(fontHandle, sizeof(fontInfo), &fontInfo);
	}
	SendMessageA(g_timeOfDayTextHwnd, WM_SETFONT, (WPARAM)fontHandle, 1);
	SendMessageA(g_timeOfDayTextHwnd, EM_LIMITTEXT, 8, 0);
}

void EditorUI_AddAllowRenderWindowCellLoadsCheckbox(HWND MainWindow, HINSTANCE hInstance) {
	g_allowCellWindowLoadsButtonHwnd = CreateWindowEx(
		NULL,
		"BUTTON",
		"Allow Render Window Cell Loads",
		BS_PUSHLIKE | BS_CHECKBOX | WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_BITMAP,
		1160, 4, // x, y
		24, 21, // width, height
		MainWindow,
		(HMENU)ID_RENDERWINDOWCELLLOADS_CHECKBOX,
		hInstance,
		NULL
	);
	SendMessageA(g_allowCellWindowLoadsButtonHwnd, BM_SETCHECK, GetIsRenderWindowAllowCellLoads(), NULL);

	HBITMAP hBitmap = (HBITMAP)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_ALLOW_CELL_LOADS_ICON), IMAGE_BITMAP, NULL, NULL, NULL);
	SendMessageA(g_allowCellWindowLoadsButtonHwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

void EditorUI_AddRenderWindowShowPortalsCheckbox(HWND MainWindow, HINSTANCE hInstance) {
	g_renderWindowShowPortalsButtonHwnd = CreateWindowEx(
		NULL,
		"BUTTON",
		"Render Window Show Portals",
		BS_PUSHLIKE | BS_CHECKBOX | WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_BITMAP,
		1185, 4, // x, y
		24, 21, // width, height
		MainWindow,
		(HMENU)ID_RENDERWINDOW_SHOWPORTALS_CHECKBOX,
		hInstance,
		NULL
	);
	SendMessageA(g_renderWindowShowPortalsButtonHwnd, BM_SETCHECK, GetIsShowPortalsAndRooms(), NULL);

	HBITMAP hBitmap = (HBITMAP)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_SHOW_PORTALS_ICON), IMAGE_BITMAP, NULL, NULL, NULL);
	SendMessageA(g_renderWindowShowPortalsButtonHwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}

void EditorUI_AddRenderWindowShowWaterCheckbox(HWND MainWindow, HINSTANCE hInstance) {
	g_renderWindowShowWaterButtonHwnd = CreateWindowEx(
		NULL,
		"BUTTON",
		"Render Window Show Water",
		BS_PUSHLIKE | BS_CHECKBOX | WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_BITMAP,
		1210, 4, // x, y
		24, 21, // width, height
		MainWindow,
		(HMENU)ID_RENDERWINDOW_SHOWWATER_CHECKBOX,
		hInstance,
		NULL
	);
	SendMessageA(g_renderWindowShowWaterButtonHwnd, BM_SETCHECK, GetIsShowWater(), NULL);

	HBITMAP hBitmap = (HBITMAP)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_SHOW_WATER_ICON), IMAGE_BITMAP, NULL, NULL, NULL);
	SendMessageA(g_renderWindowShowWaterButtonHwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
}


void EditorUI_LogVa(const char* Format, va_list Va)
{
	char buffer[2048];
	int len = _vsnprintf_s(buffer, _TRUNCATE, Format, Va);

	if (len <= 0)
		return;

	if (len >= 2 && buffer[len - 1] != '\n')
		strcat_s(buffer, "\n");

	if (g_ConsoleHwnd)
		PostMessageA(g_ConsoleHwnd, UI_CMD_ADDLOGTEXT, 0, (LPARAM)_strdup(buffer));

	_MESSAGE("%s", buffer);
}

void EditorUI_Log(const char* Format, ...)
{
	va_list va;
	va_start(va, Format);
	EditorUI_LogVa(Format, va);
	va_end(va);
}

int __cdecl EditorUI_Log2(const char* Format, ...)
{
	va_list va;
	va_start(va, Format);
	EditorUI_LogVa(Format, va);
	va_end(va);
	return 0;
}

void MoveChildWindow(HWND hwndChild, LPPOINT offset)
{
	// Get the current position of the child window
	RECT rcChild;
	GetWindowRect(hwndChild, &rcChild);
	MapWindowPoints(HWND_DESKTOP, GetParent(hwndChild), (LPPOINT)&rcChild, 2); // Convert to parent-relative coordinates

	// Calculate the new position
	int newX = rcChild.left + offset->x;
	int newY = rcChild.top + offset->y;

	// Move the child window to the new position
	SetWindowPos(hwndChild, NULL, newX, newY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

LRESULT CALLBACK EditorUI_WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_CREATE)
	{
		static bool editorUIInit = false;

		if (!editorUIInit)
		{
			const CREATESTRUCT* createInfo = (CREATESTRUCT*)lParam;

			if (_stricmp(createInfo->lpszName, "Garden of Eden Creation Kit") != 0 || _stricmp(createInfo->lpszName, "Garden of Eden Creation Kit") != 0)
			{
				ExitProcess(0);
			}

			editorUIInit = true;
			g_MainHwnd = Hwnd;

			EditorUI_CreateExtensionMenu(Hwnd, createInfo->hMenu);

			if (bShowTimeOfDaySlider) {
				EditorUI_AddSliderToToolbar(Hwnd, createInfo->hInstance);
				EditorUI_AddTimeOfDayTextToToolbar(Hwnd, createInfo->hInstance);
			}

			if (bShowAdditionalToolbarButtons) {
				EditorUI_AddAllowRenderWindowCellLoadsCheckbox(Hwnd, createInfo->hInstance);
				EditorUI_AddRenderWindowShowWaterCheckbox(Hwnd, createInfo->hInstance);
				EditorUI_AddRenderWindowShowPortalsCheckbox(Hwnd, createInfo->hInstance);
			}

			// insert a Launch Game button if an executable was specified in the ini
			char buf[MAX_PATH];
			if (GetPrivateProfileStringA("Launch Game", "ExecutableName", "", buf, MAX_PATH, iniName))
			{
				InsertMenu(createInfo->hMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_LAUNCHGAME, "Launch Game");
			}

			g_MainMenu = createInfo->hMenu;
			EnableMenuItem(g_MainMenu, MENUOPTION_RENDER_WINDOW, MF_ENABLED);
			CheckMenuItem(g_MainMenu, MENUOPTION_RENDER_WINDOW, MF_CHECKED);

			if (!bAllowRecompileAll)
			{
				EnableMenuItem(g_MainMenu, MENUOPTION_RECOMPILE_ALL, MF_DISABLED | MF_GRAYED);
				// patch switch table offset for recompile all menu button
				SafeWrite8(0x4455E0, 0xE8);

				// patch switch table for script edit recompile all button
				SafeWrite16(0x5C4E3C, 0x4D6B);
			}

			if (bObjectWindowOnlyShowEditedByDefault)
			{
				ToggleObjectWindowFilterUnedited(false);

				MENUITEMINFO menuInfo;
				menuInfo.cbSize = sizeof(MENUITEMINFO);
				menuInfo.fMask = MIIM_STATE;
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED, FALSE, &menuInfo);
			}

			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;

			if (!bAutoScroll == 0)
			{
				//	Enable scroll
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_AUTOSCROLL, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_CMD_AUTOSCROLL, (WPARAM)true, 0);
			}

			if (!bEnableSpellChecker == 0)
			{
				//	Enable spellchecker
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_SPELLCHECK, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_EXTMENU_SPELLCHECK, (WPARAM)true, 0);
			}

			if (!bRenderWindowUncap == 0)
			{
				//	Enable Render Uncap
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_RENDER, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_EXTMENU_RENDER, (WPARAM)true, 0);
			}

			if (!bPreviewWindowUncap == 0)
			{
				//	Enable Preview Uncap
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_PREVIEW, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_EXTMENU_PREVIEW, (WPARAM)true, 0);
			}

			if (!bShowLoadFilesAtStartup == 0) {
				// open esm/esp selection window at startup
				if (*(BYTE*)(0xECFE16) != 1) { // is not NIF mode
					PostMessageA(g_MainHwnd, WM_COMMAND, 0x9CD1, 0);
				}
			}

			if (!bNavmeshAllowPlaceAboveOthers == 0)
			{
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_TOGGLENAVEMESHBISECT, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_EXTMENU_TOGGLENAVEMESHBISECT, (WPARAM)true, 0);
				ToggleNavmeshPlaceAboveOthers(true);
			}
		}
	}
	else if (Message == WM_COMMAND)
	{
		const uint32_t param = LOWORD(wParam);

		switch (param)
		{
		case UI_EXTMENU_SHOWLOG:
		{
			ShowWindow(g_ConsoleHwnd, SW_SHOW);
			SetForegroundWindow(g_ConsoleHwnd);
		}
		return 0;

		case UI_EXTMENU_CLEARLOG:
		{
			PostMessageA(g_ConsoleHwnd, UI_CMD_CLEARLOGTEXT, 0, 0);
		}
		return 0;

		case UI_EXTMENU_AUTOSCROLL:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_AUTOSCROLL, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				//	Disable scroll
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_AUTOSCROLL, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_CMD_AUTOSCROLL, (WPARAM)false, 0);
			}
			else
			{
				//	Enable scroll
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_AUTOSCROLL, FALSE, &menuInfo);
				PostMessageA(g_ConsoleHwnd, UI_CMD_AUTOSCROLL, (WPARAM)true, 0);
			}
		}
		return 0;

		case UI_EXTMENU_SPELLCHECK:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_SPELLCHECK, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				//	Disable Spell Check
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_SPELLCHECK, FALSE, &menuInfo);
				bEnableSpellChecker = 0;
				char buffer[8];
				WritePrivateProfileString("General", "bEnableSpellChecker", _itoa(bEnableSpellChecker, buffer, 2), iniName);
			}
			else
			{
				//	Enable Spell Check
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_SPELLCHECK, FALSE, &menuInfo);
				bEnableSpellChecker = 1;
				char buffer[8];
				WritePrivateProfileString("General", "bEnableSpellChecker", _itoa(bEnableSpellChecker, buffer, 2), iniName);
			}
		}
		return 0;

		case UI_EXTMENU_RENDER:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_RENDER, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				//	Disable Render Uncap
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_RENDER, FALSE, &menuInfo);
				bRenderWindowUncap = 0;
				char buffer[8];
				WritePrivateProfileString("General", "bRenderWindowUncap", _itoa(bRenderWindowUncap, buffer, 2), iniName);
			}
			else
			{
				//	Enable Render Uncap
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_RENDER, FALSE, &menuInfo);
				bRenderWindowUncap = 1;
				char buffer[8];
				WritePrivateProfileString("General", "bRenderWindowUncap", _itoa(bRenderWindowUncap, buffer, 2), iniName);
			}
		}
		return 0;

		case UI_EXTMENU_PREVIEW:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_PREVIEW, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				//	Disable Preview Uncap
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_PREVIEW, FALSE, &menuInfo);
				bPreviewWindowUncap = 0;
				char buffer[8];
				WritePrivateProfileString("General", "bPreviewWindowUncap", _itoa(bPreviewWindowUncap, buffer, 2), iniName);
			}
			else
			{
				//	Enable Preview Uncap
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_PREVIEW, FALSE, &menuInfo);
				bPreviewWindowUncap = 1;
				char buffer[8];
				WritePrivateProfileString("General", "bPreviewWindowUncap", _itoa(bPreviewWindowUncap, buffer, 2), iniName);
			}
		}
		return 0;

		case UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				//	Disable showing unedited
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED, FALSE, &menuInfo);
				ToggleObjectWindowFilterUnedited(true);
			}
			else
			{
				//	Show unedited only
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_OBJECTWINDOW_TOGGLESHOWUNEDITED, FALSE, &menuInfo);
				ToggleObjectWindowFilterUnedited(false);
			}

			ObjectsView::Refresh();
		}
		return 0;

		case UI_EXTMENU_LAUNCHGAME:
		{
			char falloutNVPath[MAX_PATH];
			strcpy(falloutNVPath, (const char*)0xECFE30); // current dir from command line args
			strcat(falloutNVPath, "\\");

			char nvExeName[MAX_PATH];
			GetPrivateProfileStringA("Launch Game", "ExecutableName", "FalloutNV.exe", nvExeName, MAX_PATH, iniName);
			strcat(falloutNVPath, nvExeName);

			ShellExecuteA(0, 0, falloutNVPath, 0, 0, 1);
		}
		return 0;

		case UI_EXTMENU_SAVEPOSITION:
		{
			GetCamera(&savedRenderPos, &savedRenderDirection);
		}
		return 0;

		case UI_EXTMENU_LOADPOSITION:
		{
			SetCamera(&savedRenderPos, &savedRenderDirection);
		}
		return 0;

		case UI_EXTMENU_TOGGLENAVEMESHBISECT:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_TOGGLENAVEMESHBISECT, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_TOGGLENAVEMESHBISECT, FALSE, &menuInfo);
				ToggleNavmeshPlaceAboveOthers(false);
			}
			else
			{
				//	Show unedited only
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_TOGGLENAVEMESHBISECT, FALSE, &menuInfo);
				ToggleNavmeshPlaceAboveOthers(true);
			}

			// save state to ini
			char buffer[8];
			WritePrivateProfileString("Render Window", "bNavmeshAllowPlaceAboveOthers", _itoa(bNavmeshAllowPlaceAboveOthers, buffer, 2), iniName);
		}
		return 0;

		case ID_TIMEOFDAYTEXT:
		{
			if (GetFocus() != (HWND)lParam) break;

			if (HIWORD(wParam) == EN_MAXTEXT) break;

			char text[16];
			int len = SendMessageA((HWND)lParam, EM_GETLINE, 0, (LPARAM)text);
			text[len] = '\0'; // null terminate the line since EM_GETLINE does not

			float time = atof(text);
			SetTimeOfDay(time);
			SendMessageA(g_trackBarHwnd, TBM_SETPOS, TRUE, time * 4); // update scrollbar
		}
		return 0;

		case ID_RENDERWINDOWCELLLOADS_CHECKBOX:
		{
			if (HIWORD(wParam) == BN_CLICKED && bShowAdditionalToolbarButtons)
			{
				bool newButtonState = !SendMessageA(g_allowCellWindowLoadsButtonHwnd, BM_GETCHECK, 0, 0);
				SendMessageA(g_allowCellWindowLoadsButtonHwnd, BM_SETCHECK, newButtonState, 0);
				ToggleRenderWindowAllowCellLoads(newButtonState);
			}
		}
		return 0;

		case ID_RENDERWINDOW_SHOWWATER_CHECKBOX:
		{
			if (HIWORD(wParam) == BN_CLICKED && bShowAdditionalToolbarButtons)
			{
				bool newButtonState = !SendMessageA(g_renderWindowShowWaterButtonHwnd, BM_GETCHECK, 0, 0);
				SendMessageA(g_renderWindowShowWaterButtonHwnd, BM_SETCHECK, newButtonState, 0);
				SetIsShowWater(newButtonState);
			}
		}
		return 0;

		case ID_RENDERWINDOW_SHOWPORTALS_CHECKBOX:
		{
			if (HIWORD(wParam) == BN_CLICKED && bShowAdditionalToolbarButtons)
			{
				bool newButtonState = !SendMessageA(g_renderWindowShowPortalsButtonHwnd, BM_GETCHECK, 0, 0);
				SendMessageA(g_renderWindowShowPortalsButtonHwnd, BM_SETCHECK, newButtonState, 0);
				SetIsShowPortalsAndRooms(newButtonState);
			}
		}
		return 0;

		case MENUOPTION_RENDER_WINDOW:
		{
			MENUITEMINFO menuInfo;
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;
			GetMenuItemInfo(g_MainMenu, MENUOPTION_RENDER_WINDOW, FALSE, &menuInfo);

			if (menuInfo.fState == MFS_CHECKED)
			{
				//	Hide window
				menuInfo.fState = MFS_UNCHECKED;
				SetMenuItemInfo(g_MainMenu, MENUOPTION_RENDER_WINDOW, FALSE, &menuInfo);
				ShowWindow(RenderWindow::GetWindow(), false);
			}
			else
			{
				//	Show window
				menuInfo.fState = MFS_CHECKED;
				SetMenuItemInfo(g_MainMenu, MENUOPTION_RENDER_WINDOW, FALSE, &menuInfo);
				ShowWindow(RenderWindow::GetWindow(), true);
			}
		}
		return 0;
			
		case MAIN_WINDOW_CALLBACK:
		{
			auto callback = reinterpret_cast<void(*)()>(lParam);
			callback();
		}
		return 0;
		}
		
		
	}
	else if (Message == WM_SETTEXT && Hwnd == g_MainHwnd)
	{
		//	Continue normal execution but with a custom string
		char customTitle[256];
		sprintf_s(customTitle, "%s -= GECK Extender Rev. 0.41 =-", (const char*)lParam);

		return CallWindowProc(OldEditorUI_WndProc, Hwnd, Message, wParam, (LPARAM)customTitle);
	}
	else if (Message == WM_HSCROLL && bShowTimeOfDaySlider)
	{
		if ((HWND)lParam == g_trackBarHwnd)
		{
			char timeBuf[100];
			if ((LOWORD(wParam) == SB_THUMBPOSITION || LOWORD(wParam) == SB_THUMBTRACK))
			{
				float time = HIWORD(wParam) * 0.25;
				SetTimeOfDay(time);

				sprintf(timeBuf, "%.2f", time);
				SetWindowTextA(g_timeOfDayTextHwnd, timeBuf);
			}
			else if (LOWORD(wParam) == SB_ENDSCROLL)
			{
				float time = SendMessageA(g_trackBarHwnd, TBM_GETPOS, 0, 0) * 0.25;
				SetTimeOfDay(time);

				sprintf(timeBuf, "%.2f", time);
				SetWindowTextA(g_timeOfDayTextHwnd, timeBuf);
			}
			return 0;
		}
	} 
	else if (Message == WM_MOVE)
	{
		static int prevX, prevY = 0;

		// Extract the new position from lParam
		int newX = (int)(short)LOWORD(lParam); // New x-coordinate
		int newY = (int)(short)HIWORD(lParam); // New y-coordinate

		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
		{
			// Calculate the offset based on the new and old positions
			POINT offset = { newX - prevX, newY - prevY };

			for (auto pWindow : { RenderWindow::GetWindow(), CellView::GetWindow(), ObjectsView::GetWindow()})
			{
				MoveChildWindow(pWindow, &offset);
			}
		}

		// Update the previous position for the next move
		prevX = newX;
		prevY = newY;
	}

	return CallWindowProc(OldEditorUI_WndProc, Hwnd, Message, wParam, lParam);
}

void AddLogText(LPARAM lParam, HWND richEditHwnd, bool doFree)
{
	void* textData = (void*)lParam;

	//	Save old position if not scrolling
	POINT scrollRange;

	if (!bAutoScroll)
	{
		SendMessageA(richEditHwnd, WM_SETREDRAW, FALSE, 0);
		SendMessageA(richEditHwnd, EM_GETSCROLLPOS, 0, (WPARAM)&scrollRange);
	}

	//	Move caret to the end, then write
	CHARRANGE range;
	range.cpMin = LONG_MAX;
	range.cpMax = LONG_MAX;

	SendMessageA(richEditHwnd, EM_EXSETSEL, 0, (LPARAM)&range);
	SendMessageA(richEditHwnd, EM_REPLACESEL, FALSE, (LPARAM)textData);

	if (!bAutoScroll)
	{
		SendMessageA(richEditHwnd, EM_SETSCROLLPOS, 0, (WPARAM)&scrollRange);
		SendMessageA(richEditHwnd, WM_SETREDRAW, TRUE, 0);
		RedrawWindow(richEditHwnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
	}

	if (doFree)
		free(textData);
}

LRESULT CALLBACK EditorUI_LogWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static HWND richEditHwnd;

	switch (Message)
	{
	case WM_CREATE:
	{
		const CREATESTRUCT* info = (CREATESTRUCT*)lParam;

		//	Create the rich edit control (https://docs.microsoft.com/en-us/windows/desktop/Controls/rich-edit-controls)
		uint32_t style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_READONLY;
		richEditHwnd = CreateWindowW(MSFTEDIT_CLASS, L"", style, 0, 0, info->cx, info->cy, Hwnd, nullptr, info->hInstance, nullptr);

		if (!richEditHwnd)
			return -1;

		// Set a better font
		CHARFORMAT2W format;
		memset(&format, 0, sizeof(format));

		format.cbSize = sizeof(format);
		format.dwMask = CFM_FACE | CFM_SIZE | CFM_WEIGHT;

		// Convert Twips to points (1 point = 20 Twips)
		int pointSize = GetOrCreateINIInt("Log", "FontSize", 10, iniName) * 20;
		format.yHeight = pointSize;

		char fontNameBuf[32];
		GetPrivateProfileStringA("Log", "Font", "Consolas", fontNameBuf, 31, iniName);
		mbstowcs(format.szFaceName, fontNameBuf, 31);

		format.wWeight = (WORD)GetOrCreateINIInt("Log", "FontWeight", FW_MEDIUM, iniName);
		SendMessageA(richEditHwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)& format);

		//	Subscribe to EN_MSGFILTER and EN_SELCHANGE
		SendMessageW(richEditHwnd, EM_SETEVENTMASK, 0, ENM_MOUSEEVENTS | ENM_SELCHANGE);

	}
	return 0;

	case WM_DESTROY:
		DestroyWindow(richEditHwnd);
		return 0;

	case WM_SIZE:
	{
		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
		MoveWindow(richEditHwnd, 0, 0, w, h, TRUE);
	}
	break;

	case WM_ACTIVATE:
	{
		if (wParam != WA_INACTIVE)
			SetFocus(richEditHwnd);
	}
	return 0;

	case WM_CLOSE:
		ShowWindow(Hwnd, SW_HIDE);
		return 0;

	case WM_NOTIFY:
	{
		static uint64_t lastDoubleClickTime;
		LPNMHDR notification = (LPNMHDR)lParam;

		if (notification->code == EN_MSGFILTER)
		{
			auto msgFilter = (MSGFILTER*)notification;

			if (msgFilter->msg == WM_LBUTTONDBLCLK)
				lastDoubleClickTime = GetTickCount();
		}
		else if (notification->code == EN_SELCHANGE)
		{
			auto selChange = (SELCHANGE*)lParam;

			//	Mouse double click with a valid selection -> try to parse form id
			if ((GetTickCount() - lastDoubleClickTime > 1000) || abs(selChange->chrg.cpMax - selChange->chrg.cpMin) <= 0)
				break;

			if (selChange->chrg.cpMin == 0 && selChange->chrg.cpMax == -1)
				break;

			//	Get the line number from the selected range
			LRESULT lineIndex = SendMessageA(richEditHwnd, EM_LINEFROMCHAR, selChange->chrg.cpMin, 0);

			char lineData[4096];
			*(uint16_t*)& lineData[0] = ARRAYSIZE(lineData);

			LRESULT charCount = SendMessageA(richEditHwnd, EM_GETLINE, lineIndex, (LPARAM)& lineData);

			if (charCount > 0)
			{
				lineData[charCount - 1] = '\0';

				//	Capture each form id with regex "(XXXXXXXX)"
				static const std::regex formIdRegex("\\(([0-9a-fA-F]*)\\)");
				std::smatch sm;

				for (std::string line = lineData; std::regex_search(line, sm, formIdRegex); line = sm.suffix())
				{
					//	Parse to integer, then bring up the menu
					uint32_t id = strtoul(sm[1].str().c_str(), nullptr, 16);

					TESForm* (*LookupFormByID)(UInt32 formID) = (TESForm * (*)(UInt32))0x4F9620;
					__int32 form = (__int32)LookupFormByID(id);

					if (form)
					{
						(*(void(__thiscall * *)(__int32, HWND, __int32, __int32))(*(__int32*)form + 0x164))(form, g_MainHwnd, 0, 1);
					}
				}
			}
		}
	}
	break;

	case UI_CMD_ADDLOGTEXT:
	{
		AddLogText(lParam, richEditHwnd, true);
	}
	return 0;


	case UI_CMD_ADDLOGTEXT_NOFREE:
	{
		AddLogText(lParam, richEditHwnd, false);
	}
	return 0;

		
	case UI_CMD_CLEARLOGTEXT:
	{
		char emptyString[1] = { '\0' };

		SendMessageA(richEditHwnd, WM_SETTEXT, 0, (LPARAM)&emptyString);
	}
	return 0;

	case UI_CMD_AUTOSCROLL:
	{
		bAutoScroll = wParam;
		char buffer[8];
		WritePrivateProfileString("Log", "bAutoScroll", _itoa(bAutoScroll, buffer, 2), iniName);
	}
	return 0;

	}
	return DefWindowProc(Hwnd, Message, wParam, lParam);
}

bool EditorUI_CreateLogWindow()
{
	HINSTANCE instance = (HINSTANCE)GetModuleHandle(nullptr);

	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hInstance = instance;
	wc.hIcon = LoadIconA(instance, MAKEINTRESOURCE(0x13E));
	wc.hIconSm = LoadIconA(instance, MAKEINTRESOURCE(0x13E));
	wc.lpfnWndProc = EditorUI_LogWndProc;
	wc.lpszClassName = TEXT("RTEDITLOG");
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	if (!RegisterClassEx(&wc))
		return false;

	// get previous stored position
	int posX = GetPrivateProfileIntA("Log", "iWindowPosX", 64, iniName);
	int posY = GetPrivateProfileIntA("Log", "iWindowPosY", 64, iniName);
	int width = GetPrivateProfileIntA("Log", "iWindowPosDX", 1024, iniName);
	int height = GetPrivateProfileIntA("Log", "iWindowPosDY", 768, iniName);

	g_ConsoleHwnd = CreateWindowExA(0, "RTEDITLOG", "Message Log", WS_OVERLAPPEDWINDOW, posX, posY, width, height, nullptr, nullptr, instance, nullptr);

	if (!g_ConsoleHwnd)
		return false;

	ShowWindow(g_ConsoleHwnd, SW_SHOW);
	UpdateWindow(g_ConsoleHwnd);
	return true;
}
