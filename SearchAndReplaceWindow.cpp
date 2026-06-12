namespace SearchAndReplaceWindow
{
	enum CtrlIds
	{
		IDC_SEARCH_COMBO = 1439,
		IDC_REPLACE_COMBO = 1440,
		IDC_SWAP_BUTTON = 1009,
	};

	void AddSwapButton(HWND hDlg)
	{
		RECT rc1, rc2;
		GetWindowRect(GetDlgItem(hDlg, IDC_SEARCH_COMBO), &rc1);
		GetWindowRect(GetDlgItem(hDlg, IDC_REPLACE_COMBO), &rc2);

		MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT)&rc1, 2);
		MapWindowPoints(HWND_DESKTOP, hDlg, (LPPOINT)&rc2, 2);

		int x = rc1.right + 1;
		int y = (rc1.top + rc2.bottom) / 2 - 8;

		HWND hBtn = CreateWindowEx(
			0, TEXT("BUTTON"), TEXT("<>"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			x, y, 18, 16,
			hDlg, (HMENU)IDC_SWAP_BUTTON,
			GetModuleHandle(NULL), NULL);

		HFONT hFont = (HFONT)SendMessage(hDlg, WM_GETFONT, 0, 0);
		SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
	}

	void HandleSwapButton(HWND hDlg)
	{
		HWND hSearch = GetDlgItem(hDlg, IDC_SEARCH_COMBO);
		HWND hReplace = GetDlgItem(hDlg, IDC_REPLACE_COMBO);

		int selSearch = (int)SendMessage(hSearch, CB_GETCURSEL, 0, 0);
		int selReplace = (int)SendMessage(hReplace, CB_GETCURSEL, 0, 0);

		SendMessage(hSearch, CB_SETCURSEL, selReplace, 0);
		SendMessage(hReplace, CB_SETCURSEL, selSearch, 0);
	}

	WNDPROC originalWindowCallback;
	BOOL CALLBACK WindowCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_INITDIALOG)
		{
			AddSwapButton(hDlg);
		}
		else if (msg == WM_COMMAND)
		{
			if (wParam == IDC_SWAP_BUTTON)
			{
				HandleSwapButton(hDlg);
				return TRUE;
			}
		}
		return CallWindowProc(originalWindowCallback, hDlg, msg, wParam, lParam);
	}

	void Init()
	{
		originalWindowCallback = (WNDPROC)DetourVtable(0x441180, UInt32(WindowCallback));
	}
}