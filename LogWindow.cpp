#pragma once
#pragma comment(lib, "comctl32.lib")
#include "GameAPI.h"
#include "GameForms.h"
#include "LogWindow.h"
#include <Richedit.h>
#include <regex>
#include "Settings.h"
#include "GECKUtility.h"

HWND g_ConsoleHwnd;
extern HWND g_MainHwnd;
extern bool g_bPauseLogging = false;

void Console_PrintVa(const char* Format, va_list Va)
{
	char buffer[2048];
	int len = stbsp_vsnprintf(buffer, sizeof(buffer), Format, Va);

	if (len <= 0)
		return;

	if (len >= 2 && buffer[len - 1] != '\n')
		strcat_s(buffer, "\n");

	if (g_ConsoleHwnd)
		PostMessageA(g_ConsoleHwnd, UI_CMD_ADDLOGTEXT, 0, (LPARAM)_strdup(buffer));

	_MESSAGE("%s", buffer);
}

void Console_Print(const char* Format, ...)
{
	if (g_bPauseLogging)
		return;

	va_list va;
	va_start(va, Format);
	Console_PrintVa(Format, va);
	va_end(va);
}

int __cdecl Console_Print2(const char* Format, ...)
{
	if (g_bPauseLogging)
		return 0;

	va_list va;
	va_start(va, Format);
	Console_PrintVa(Format, va);
	va_end(va);
	return 0;
}

void AddLogText(LPARAM lParam, HWND richEditHwnd, bool doFree)
{
	void* textData = (void*)lParam;

	//	Save old position if not scrolling
	POINT scrollRange;

	if (!config.bAutoScroll)
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

	if (!config.bAutoScroll)
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
		int pointSize = GetOrCreateINIValue("Log", "FontSize", 10, IniPath) * 20;
		format.yHeight = pointSize;

		char fontNameBuf[32];
		GetPrivateProfileStringA("Log", "Font", "Consolas", fontNameBuf, 31, IniPath);
		mbstowcs(format.szFaceName, fontNameBuf, 31);

		format.wWeight = (WORD)GetOrCreateINIValue("Log", "FontWeight", FW_MEDIUM, IniPath);
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

		if (notification->code == EN_MSGFILTER) {
			auto msgFilter = (MSGFILTER*)notification;

			if (msgFilter->msg == WM_LBUTTONDBLCLK)
				lastDoubleClickTime = GetTickCount();
		}
		else if (notification->code == EN_SELCHANGE) {
			auto selChange = (SELCHANGE*)lParam;
			auto selLen = abs(selChange->chrg.cpMax - selChange->chrg.cpMin);

			if ((GetTickCount() - lastDoubleClickTime > 1000) || selLen == 0)
				break;

			if (selChange->chrg.cpMin == 0 && selChange->chrg.cpMax == -1)
				break;

			// expand the selection to include any brackets if double clicking on a number
			selChange->chrg.cpMin--;
			selChange->chrg.cpMax++;
			selLen += 2;

			// Get the selected text from the rich edit control
			char buf[0x400]; *buf = 0;

			if (2 * (selLen + 1) >= sizeof(buf))
				break;

			TEXTRANGEA textRange;
			textRange.chrg = selChange->chrg;
			textRange.lpstrText = buf;

			SendMessageA(richEditHwnd, EM_GETTEXTRANGE, 0, (LPARAM)&textRange);

			char selectedText[0x400];
			WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)buf, -1, selectedText, sizeof(selectedText), nullptr, nullptr);

			// Ensure null termination
			selectedText[selLen] = '\0';

			// Capture the first form id with regex "\(([0-9a-fA-F]*)\)"
			static const std::regex formIdRegex("\\(([0-9a-fA-F]{1,8})\\)");
			std::smatch sm;

			std::string selectedStr = selectedText;
			if (std::regex_search(selectedStr, sm, formIdRegex)) {
				uint32_t id = strtoul(sm[1].str().c_str(), nullptr, 16);
				if (auto form = LookupFormByID(id)) {
					OpenForm(form, g_MainHwnd);
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
		config.bAutoScroll = wParam;
		char buffer[8];
		WritePrivateProfileString("Log", "bAutoScroll", _itoa(config.bAutoScroll, buffer, 2), IniPath);
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
	int posX = GetPrivateProfileIntA("Log", "iWindowPosX", 64, IniPath);
	int posY = GetPrivateProfileIntA("Log", "iWindowPosY", 64, IniPath);
	int width = GetPrivateProfileIntA("Log", "iWindowPosDX", 1024, IniPath);
	int height = GetPrivateProfileIntA("Log", "iWindowPosDY", 768, IniPath);

	g_ConsoleHwnd = CreateWindowExA(0, "RTEDITLOG", "Message Log", WS_OVERLAPPEDWINDOW, posX, posY, width, height, nullptr, nullptr, instance, nullptr);

	if (!g_ConsoleHwnd)
		return false;

	ShowWindow(g_ConsoleHwnd, SW_SHOW);
	UpdateWindow(g_ConsoleHwnd);
	return true;
}