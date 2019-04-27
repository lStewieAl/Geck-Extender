#pragma once
#pragma comment(lib, "comctl32.lib")

#define UI_CMD_ADDLOGTEXT	(WM_APP + 1)
#define UI_CMD_CLEARLOGTEXT (WM_APP + 2)
#define UI_CMD_AUTOSCROLL	(WM_APP + 3)

#define UI_EXTMENU_ID			51001
#define UI_EXTMENU_SHOWLOG		51002
#define UI_EXTMENU_CLEARLOG		51003
#define UI_EXTMENU_AUTOSCROLL	51004
#define UI_EXTMENU_SPACER		51005
#define UI_EXTMENU_SPELLCHECK	51006
#define UI_EXTMENU_RENDER		51007
#define UI_EXTMENU_PREVIEW		51008

HWND g_MainHwnd;
HWND g_ConsoleHwnd;
HMENU g_ExtensionMenu;
WNDPROC OldEditorUI_WndProc;

bool EditorUI_CreateExtensionMenu(HWND MainWindow, HMENU MainMenu)
{
	//	Create extended menu options
	g_ExtensionMenu = CreateMenu();

	BOOL result = TRUE;
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_SHOWLOG, "Show Log");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_CLEARLOG, "Clear Log");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_AUTOSCROLL, "Autoscroll Log");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_SEPARATOR, (UINT_PTR)UI_EXTMENU_SPACER, "");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_SPELLCHECK, "Enable Spell Check");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_RENDER, "Render Window Uncap (requires restart)");
	result = result && InsertMenu(g_ExtensionMenu, -1, MF_BYPOSITION | MF_STRING, (UINT_PTR)UI_EXTMENU_PREVIEW, "Preview Window Uncap (requires restart)");

	MENUITEMINFO menuInfo;
	memset(&menuInfo, 0, sizeof(MENUITEMINFO));
	menuInfo.cbSize = sizeof(MENUITEMINFO);
	menuInfo.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_STRING;
	menuInfo.hSubMenu = g_ExtensionMenu;
	menuInfo.wID = UI_EXTMENU_ID;
	menuInfo.dwTypeData = "Extensions";
	menuInfo.cch = (uint32_t)strlen(menuInfo.dwTypeData);
	result = result && InsertMenuItem(MainMenu, -1, TRUE, &menuInfo);

	return result ? true : false;
}

void EditorUI_LogVa(const char *Format, va_list Va)
{
	char buffer[2048];
	int len = _vsnprintf_s(buffer, _TRUNCATE, Format, Va);

	if (len <= 0)
		return;

	if (len >= 2 && buffer[len - 1] != '\n')
		strcat_s(buffer, "\n");

	if (g_ConsoleHwnd)
		PostMessageA(g_ConsoleHwnd, UI_CMD_ADDLOGTEXT, 0, (LPARAM)_strdup(buffer));
		_MESSAGE(buffer);
}

void EditorUI_Log(const char *Format, ...)
{
	va_list va;
	va_start(va, Format);
	EditorUI_LogVa(Format, va);
	va_end(va);
}

int __cdecl EditorUI_Log2(const char *Format, ...)
{
	va_list va;
	va_start(va, Format);
	EditorUI_LogVa(Format, va);
	va_end(va);
	return 0;
}

LRESULT CALLBACK EditorUI_WndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_CREATE)
	{
		static bool editorUIInit = false;

		if (!editorUIInit)
		{
			const CREATESTRUCT *createInfo = (CREATESTRUCT *)lParam;

			if (_stricmp(createInfo->lpszName, "Garden of Eden Creation Kit") != 0 || _stricmp(createInfo->lpszName, "Garden of Eden Creation Kit") != 0)
			{
				ExitProcess(0);
			}

			editorUIInit = true;
			g_MainHwnd = Hwnd;
			EditorUI_CreateExtensionMenu(Hwnd, createInfo->hMenu);

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
					WritePrivateProfileString("General", "bEnableSpellChecker" , _itoa(bEnableSpellChecker,buffer,2), filename);
				}
				else
				{
					//	Enable Spell Check
					menuInfo.fState = MFS_CHECKED;
					SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_SPELLCHECK, FALSE, &menuInfo);
					bEnableSpellChecker = 1;
					char buffer[8];
					WritePrivateProfileString("General", "bEnableSpellChecker" , _itoa(bEnableSpellChecker,buffer,2), filename);
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
					WritePrivateProfileString("General", "bRenderWindowUncap" , _itoa(bRenderWindowUncap,buffer,2), filename);
				}
				else
				{
					//	Enable Render Uncap
					menuInfo.fState = MFS_CHECKED;
					SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_RENDER, FALSE, &menuInfo);
					bRenderWindowUncap = 1;
					char buffer[8];
					WritePrivateProfileString("General", "bRenderWindowUncap" , _itoa(bRenderWindowUncap,buffer,2), filename);
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
					WritePrivateProfileString("General", "bPreviewWindowUncap" , _itoa(bPreviewWindowUncap,buffer,2), filename);
				}
				else
				{
					//	Enable Preview Uncap
					menuInfo.fState = MFS_CHECKED;
					SetMenuItemInfo(g_ExtensionMenu, UI_EXTMENU_PREVIEW, FALSE, &menuInfo);
					bPreviewWindowUncap = 1;
					char buffer[8];
					WritePrivateProfileString("General", "bPreviewWindowUncap" , _itoa(bPreviewWindowUncap,buffer,2), filename);
				}
			}
			return 0;
		}
	}
	else if (Message == WM_SETTEXT && Hwnd == g_MainHwnd)
	{
		//	Continue normal execution but with a custom string
		char customTitle[1024];
		sprintf_s(customTitle, "%s -= GECK Extender Rev. 0.12s =-", (const char *)lParam);

		return CallWindowProc(OldEditorUI_WndProc, Hwnd, Message, wParam, (LPARAM)customTitle);
	}

	return CallWindowProc(OldEditorUI_WndProc, Hwnd, Message, wParam, lParam);
}

LRESULT CALLBACK EditorUI_LogWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	static HWND richEditHwnd;

	switch (Message)
	{
	case WM_CREATE:
	{
		const CREATESTRUCT *info = (CREATESTRUCT *)lParam;

		//	Create the rich edit control (https://docs.microsoft.com/en-us/windows/desktop/Controls/rich-edit-controls)
		uint32_t style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_LEFT | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_READONLY;
		richEditHwnd = CreateWindowW(MSFTEDIT_CLASS, L"", style, 0, 0, info->cx, info->cy, Hwnd, nullptr, info->hInstance, nullptr);

		if (!richEditHwnd)
			return -1;

		// Set default position
//		int winW = g_INI.GetInteger("CreationKit_Log", "Width", info->cx);
//		int winH = g_INI.GetInteger("CreationKit_Log", "Height", info->cy);

//		MoveWindow(Hwnd, info->x, info->y, winW, winH, FALSE);

		// Set a better font
		CHARFORMAT2W format;
		memset(&format, 0, sizeof(format));

		// Convert Twips to points (1 point = 20 Twips)
//		int pointSize = g_INI.GetInteger("CreationKit_Log", "FontSize", 10) * 20;

		format.cbSize = sizeof(format);
		format.dwMask = CFM_FACE | CFM_SIZE | CFM_WEIGHT;

//		format.yHeight = pointSize;
//		strcpy_s(format.szFaceName, g_INI.Get("CreationKit_Log", "Font", "Consolas").c_str());
//		format.wWeight = (WORD)g_INI.GetInteger("CreationKit_Log", "FontWeight", FW_NORMAL);

		format.yHeight = 10 * 20;// Convert twips to points (1 point = 20 twips)
		format.wWeight = FW_MEDIUM;
		wcscpy_s(format.szFaceName, L"Consolas");
        SendMessageA(richEditHwnd, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);

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
			auto msgFilter = (MSGFILTER *)notification;

			if (msgFilter->msg == WM_LBUTTONDBLCLK)
				lastDoubleClickTime = GetTickCount();
		}
		else if (notification->code == EN_SELCHANGE)
		{
			auto selChange = (SELCHANGE *)lParam;

			//	Mouse double click with a valid selection -> try to parse form id
			if ((GetTickCount() - lastDoubleClickTime > 1000) || abs(selChange->chrg.cpMax - selChange->chrg.cpMin) <= 0)
				break;

			if (selChange->chrg.cpMin == 0 && selChange->chrg.cpMax == -1)
				break;

			//	Get the line number from the selected range
			LRESULT lineIndex = SendMessageA(richEditHwnd, EM_LINEFROMCHAR, selChange->chrg.cpMin, 0);

			char lineData[4096];
			*(uint16_t *)&lineData[0] = ARRAYSIZE(lineData);

			LRESULT charCount = SendMessageA(richEditHwnd, EM_GETLINE, lineIndex, (LPARAM)&lineData);

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

					TESForm* (*LookupFormByID)(UInt32 formID) = (TESForm* (*)(UInt32))0x4F9620;
					__int32 form = (__int32)LookupFormByID(id);

					if (form)
						(*(void(__thiscall **)(__int32, HWND, __int32, __int32))(*(__int32 *)form + 0x164))(form, g_MainHwnd, 0, 1);
				}
			}
		}
	}
	break;
	
	case UI_CMD_ADDLOGTEXT:
	{
		void *textData = (void *)lParam;

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

		free(textData);
	}
	return 0;

	case UI_CMD_CLEARLOGTEXT:
	{
		char emptyString[1];
		emptyString[0] = '\0';

		SendMessageA(richEditHwnd, WM_SETTEXT, 0, (LPARAM)&emptyString);
	}
	return 0;

	case UI_CMD_AUTOSCROLL:
	{
		bAutoScroll = (bool)wParam;
		char buffer[8];
		WritePrivateProfileString("General", "bAutoScroll" , _itoa(bAutoScroll,buffer,2), filename);
	}
	return 0;

  }
  return DefWindowProc(Hwnd, Message, wParam, lParam);
}

bool EditorUI_CreateLogWindow()
{
	const uint32_t width = 1024;
	const uint32_t height = 768;
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

	g_ConsoleHwnd = CreateWindowExA(0, "RTEDITLOG", "Message Log", WS_OVERLAPPEDWINDOW, 64, 64, width, height, nullptr, nullptr, instance, nullptr);

	if (!g_ConsoleHwnd)
		return false;

	ShowWindow(g_ConsoleHwnd, SW_SHOW);
	UpdateWindow(g_ConsoleHwnd);
	return true;
}

