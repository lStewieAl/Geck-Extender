#include "GeckUtility.h"

namespace CustomReferenceBatchAction
{
	/*
	* TODO: Initially Disabled, Set Encounter Zone and Set Level Modifier
	*/

	void AddCustomReferenceBatchActionCombobox(HWND hDlg)
	{
		RECT rcBtn = { 224, 162, 224 + 9, 162 + 9 };

		MapDialogRect(hDlg, &rcBtn);

		CreateWindowEx(
			0,
			"BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			rcBtn.left,
			rcBtn.top,
			rcBtn.right - rcBtn.left,
			rcBtn.bottom - rcBtn.top,
			hDlg,
			(HMENU)1036,
			GetModuleHandle(NULL),
			NULL
		);

		RECT rc = { 238, 162, 238 + 110, 162 + 99 };

		MapDialogRect(hDlg, &rc);

		HWND hCombo = CreateWindowEx(
			0,
			"COMBOBOX",
			"",
			WS_CHILD | WS_VISIBLE |
			CBS_DROPDOWNLIST |
			CBS_SORT |
			WS_VSCROLL |
			WS_TABSTOP,
			rc.left,
			rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			hDlg,
			(HMENU)1984,
			GetModuleHandle(NULL),
			NULL
		);
		HFONT hFont = (HFONT)SendMessage(hDlg, WM_GETFONT, 0, 0);
		SendMessage(hCombo, WM_SETFONT, (WPARAM)hFont, TRUE);

		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)0xD2C5A0); // ' NONE '
		SendMessage(hCombo, CB_SETCURSEL, 0, 0);

		HWND hGroup = FindWindowEx(hDlg, NULL, "Button", "Action");
		RECT rcAction = { 215, 14, 215 + 138, 14 + 175 }; // increase height from 169 -> 175

		MapDialogRect(hDlg, &rcAction);

		SetWindowPos(
			hGroup,
			NULL,
			rcAction.left,
			rcAction.top,
			rcAction.right - rcAction.left,
			rcAction.bottom - rcAction.top,
			SWP_NOZORDER
		);
	}

	WNDPROC originalReferenceBatchActionFn;
	BOOL CALLBACK ReferenceBatchActionCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_INITDIALOG)
		{
			auto result = CallWindowProc(originalReferenceBatchActionFn, hDlg, msg, wParam, lParam);

//			AddCustomReferenceBatchActionCombobox(hDlg);

			// make the "Do" button not grayed out if an action is already selected when initialising the dialog
			int* iButtonIdToControlId = (int*)0xE8C908;
			auto currentAction = ReferenceBatchAction::GetAction();
			if (currentAction && currentAction < ReferenceBatchAction::VANILLA_COUNT)
			{
				auto buttonId = iButtonIdToControlId[currentAction];
				SendMessageA(hDlg, WM_COMMAND, buttonId, NULL);
			}

			return result;
		}
		if (msg == WM_COMMAND)
		{
			if (LOWORD(wParam) >= 1033 && LOWORD(wParam) <= 1042)
			{
				return TRUE;
			}
		}
		return CallWindowProc(originalReferenceBatchActionFn, hDlg, msg, wParam, lParam);
	}

	void Init()
	{
		originalReferenceBatchActionFn = (WNDPROC)DetourVtable(0x456B3E, UInt32(ReferenceBatchActionCallback));
	}
}