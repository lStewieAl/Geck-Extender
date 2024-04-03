#pragma once
#pragma comment(lib, "comctl32.lib")

#define UI_CMD_ADDLOGTEXT	(WM_APP + 1)
#define UI_CMD_CLEARLOGTEXT (WM_APP + 2)
#define UI_CMD_AUTOSCROLL	(WM_APP + 3)
#define UI_CMD_ADDLOGTEXT_NOFREE (WM_APP + 4)

void EditorUI_LogVa(const char* Format, va_list Va);
void EditorUI_Log(const char* Format, ...);
int __cdecl EditorUI_Log2(const char* Format, ...);
void AddLogText(LPARAM lParam, HWND richEditHwnd, bool doFree);

LRESULT CALLBACK EditorUI_LogWndProc(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
bool EditorUI_CreateLogWindow();