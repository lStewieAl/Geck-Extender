#pragma once

/*
editable fields
dialogs edited

136
137
138
140
144
145
147
156
165
196
262
264
280
285
286
287
289
302
314
329
342
3219
3227
3229
3237
3239
3244
3246
3247
3251
3262
3266
3267
3268
3271
3272
3277
3278
3282
3286
3288
3290
3327
3352
3353
3354
3368
3369
3374
41512

buggy package dialog 3300, SysListView32 2644

*/

//432A80 LoadESPESMCallback
//44192A set callback

/*
List1 1056
Set as Active File 1121 -
Details 1185 -
"Created by" Edit 1025
"Summary" Edit 1024
"Parent Master" Edit 1057
OK Button 1 -
Cancel Button 2 -
TES Files 1101
Created By 1102
Summary 1103
Parent Masters 1104
Created On 1105
Last Modified 1106
"Created On" Date 1026 -
"Created On" Time 1027 -
File Version 2406
Current 2407
"File Version" 1681
"Current" 1682
"Last Modified" Date 1028 -
"Last Modified" Time 1029 -
*/

void ResizeFormListWindow(HWND hWnd, WORD newWidth, WORD newHeight)
{
	static const WORD RIGHT_PADDING = 50;
	static const WORD BOTTOM_PADDING = 100;
	static const WORD BUTTON_BOTTOM_PADDING = 30;

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	RECT idRect;
	HWND idTextField = GetDlgItem(hWnd, 5500);
	GetWindowRect(idTextField, &idRect);
	LONG height = idRect.bottom - idRect.top;
	SetWindowPos(idTextField, NULL, NULL, NULL, newWidth - RIGHT_PADDING, height, SWP_NOMOVE);

	HWND listView = GetDlgItem(hWnd, 2445);
	SetWindowPos(listView, NULL, NULL, NULL, newWidth - RIGHT_PADDING + 20, newHeight - BOTTOM_PADDING, SWP_NOMOVE);

	// move the bottom buttons
	HWND OkButton = GetDlgItem(hWnd, 1);
	HWND CancelButton = GetDlgItem(hWnd, 2);
	HWND LeftArrowButton = GetDlgItem(hWnd, 4008);
	HWND RightArrowButton = GetDlgItem(hWnd, 4009);

	POINT pos;
	for (HWND button : {OkButton, CancelButton, LeftArrowButton, RightArrowButton})
	{
		if (button == OkButton)
		{
			pos.x = newWidth / 2 - 90;
			pos.y = newHeight - BUTTON_BOTTOM_PADDING;
		}
		else if (button == CancelButton)
		{
			pos.x = newWidth / 2 + 20;
			pos.y = newHeight - BUTTON_BOTTOM_PADDING;
		}
		else if (button == LeftArrowButton)
		{
			pos.x = newWidth / 2 - 30;
			pos.y = newHeight - BUTTON_BOTTOM_PADDING - 25;
		}
		else if (button == RightArrowButton)
		{
			pos.x = newWidth / 2 + 10;
			pos.y = newHeight - BUTTON_BOTTOM_PADDING - 25;
		}

		SetWindowPos(button, NULL, pos.x, pos.y, NULL, NULL, SWP_NOSIZE);
	}

	InvalidateRect(hWnd, &clientRect, true);
}

// FormList dialog (3274)
BOOL __stdcall FormListCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SIZE)
	{
		// the callback is shared, only filter for the FormList dialog (classname is AlchemyClass)
		char className[256];
		if (GetClassNameA(hDlg, className, ARRAYSIZE(className)) > 0)
		{
			if (!strcmp(className, "AlchemyClass"))
			{
				WORD width = LOWORD(lParam);
				WORD height = HIWORD(lParam);
				ResizeFormListWindow(hDlg, width, height);
			}
		}
		else
		{
			return true;
		}
	}
	return ((WNDPROC)(0x480110))(hDlg, msg, wParam, lParam);
}

void ResizeObjectPalette(HWND hWnd, WORD newWidth, WORD newHeight)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	HWND listView = GetDlgItem(hWnd, 1018);
	SetWindowPos(listView, NULL, NULL, NULL, 200, newHeight - 90, SWP_NOMOVE);

	// move the bottom buttons
	HWND NewButton = GetDlgItem(hWnd, 1124);
	HWND LoadButton = GetDlgItem(hWnd, 5097);
	HWND SaveButton = GetDlgItem(hWnd, 1000);
	HWND SaveAsButton = GetDlgItem(hWnd, 1986);
	HWND MergeButton = GetDlgItem(hWnd, 1988);

	SetWindowPos(NewButton, NULL, 8, newHeight - 42, NULL, NULL, SWP_NOSIZE);
	SetWindowPos(LoadButton, NULL, 78, newHeight - 30, NULL, NULL, SWP_NOSIZE);
	SetWindowPos(SaveButton, NULL, 78, newHeight - 55, NULL, NULL, SWP_NOSIZE);
	SetWindowPos(SaveAsButton, NULL, 148, newHeight - 55, NULL, NULL, SWP_NOSIZE);
	SetWindowPos(MergeButton, NULL, 148, newHeight - 30, NULL, NULL, SWP_NOSIZE);

	InvalidateRect(hWnd, &clientRect, true);
}

// objects palette window (375)
BOOL __stdcall ObjectPaletteCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SIZE)
	{
		WORD width = LOWORD(lParam);
		WORD height = HIWORD(lParam);
		ResizeObjectPalette(hDlg, width, height);
	}
	return ((WNDPROC)(0x40D190))(hDlg, msg, wParam, lParam);
}

#define USE_REPORT_USED_IN_THESE_CELLS_TEXT_ID 1102
void ResizeUseReportWindow(HWND hWnd, WORD newWidth, WORD newHeight)
{
	static const WORD LEFT_PADDING = 10;
	static const WORD RIGHT_PADDING = 40;
	static const WORD BOTTOM_PADDING = 100;
	static const WORD BUTTON_BOTTOM_PADDING = 30;
	static const WORD INTER_LIST_PADDING = 40;
	static const WORD BUTTON_SIZE = 39;

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	int List1Height = newHeight / 2 - INTER_LIST_PADDING;
	int List1Width = newWidth - RIGHT_PADDING + 20;
	HWND listView1 = GetDlgItem(hWnd, 1637);
	SetWindowPos(listView1, NULL, NULL, NULL, List1Width, List1Height, SWP_NOMOVE);

	HWND UsedInTheseCellsText = GetDlgItem(hWnd, USE_REPORT_USED_IN_THESE_CELLS_TEXT_ID);
	SetWindowPos(UsedInTheseCellsText, NULL, LEFT_PADDING, newHeight / 2, NULL, NULL, SWP_NOSIZE);

	HWND listView2 = GetDlgItem(hWnd, 1638);
	SetWindowPos(listView2, NULL, LEFT_PADDING, List1Height + 1.5 * INTER_LIST_PADDING, newWidth - RIGHT_PADDING + 20, (newHeight / 2) - 60, NULL);

	// move the bottom buttons
	HWND OkButton = GetDlgItem(hWnd, 1);
	POINT newButtonPos;
	newButtonPos.x = newWidth / 2 - BUTTON_SIZE;
	newButtonPos.y = newHeight - BUTTON_BOTTOM_PADDING;

	SetWindowPos(OkButton, NULL, newButtonPos.x, newButtonPos.y, NULL, NULL, SWP_NOSIZE);

	InvalidateRect(hWnd, &clientRect, true);
}

// Use Report window (220)
BOOL __stdcall UseReportCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SIZE)
	{
		WORD width = LOWORD(lParam);
		WORD height = HIWORD(lParam);
		ResizeUseReportWindow(hDlg, width, height);
	}
	return ((WNDPROC)(0x468860))(hDlg, msg, wParam, lParam);
}