#include "BetterFloatingFormList.h"
#include <commctrl.h>
#include "GameObjects.h"
#include "GECKUtility.h"
#include "GameAPI.h"
#include "Utilities.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace BetterFloatingFormList
{
	bool IsFormInListView(HWND hListView, TESForm* form)
	{
		LVFINDINFO lvFindInfo;
		lvFindInfo.flags = LVFI_PARAM;
		lvFindInfo.lParam = (LPARAM)form;

		int index = ListView_FindItem(hListView, -1, &lvFindInfo);
		return index != -1;
	}

	LRESULT CALLBACK SubclassedListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_DELETE)
			{
				tList<TESForm> deletedForms;
				deletedForms.Init();
				int iSelected = -1;
				SetDeferListUpdate(hWnd, true);
				while ((iSelected = ListView_GetNextItem(hWnd, iSelected, LVNI_SELECTED)) != -1)
				{
					if (auto form = GetNthListForm(hWnd, iSelected))
					{
						deletedForms.Insert(form);
					}
					ListView_DeleteItem(hWnd, iSelected);
					iSelected--; // because the indices shift after deletion, decrement iSelected to ensure no item is skipped
				}

				SendMessageA(GetParent(hWnd), BFL_DELETED_ITEMS, (WPARAM)&deletedForms, (LPARAM)hWnd);
				SetDeferListUpdate(hWnd, false);
				deletedForms.RemoveAll();
			}
			else if (wParam == 'A' && GetAsyncKeyState(VK_CONTROL) < 0)
			{
				SelectAllItemsInListView(hWnd);
			}
			break;
		}

		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	void SubclassListView(HWND hListView)
	{
		SetWindowSubclass(hListView, SubclassedListViewProc, 0, 0);
	}

	WNDPROC originalWindowCallback;
	LRESULT CALLBACK BaseWindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_INITDIALOG:
		{
			auto listView = GetDlgItem(Hwnd, 1018);
			SubclassListView(listView);
			ListView_SetExtendedListViewStyleEx(listView, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
			LONG_PTR style = GetWindowLongPtr(listView, GWL_STYLE);
			SetWindowLongPtr(listView, GWL_STYLE, style & ~LVS_EDITLABELS);

			DragAcceptFiles(Hwnd, TRUE);

			// allow the window to be closed from the 'Open Windows' menu
			auto openWindowsList = OpenWindows::GetWindowList();
			openWindowsList->Append((HWND*)Hwnd);
			break;
		}

		case BSMsg_AcceptsDropType:
		{
			SetWindowLong(Hwnd, DWL_MSGRESULT, wParam != kFormType_Reference);
			return TRUE;
		}

		case BSMsg_HandleDrop:
		{
			auto selected = RenderWindow::SelectedData::GetSelected();
			auto listView = GetDlgItem(Hwnd, 1018);
			if (auto selectedFormIter = selected->selectedForms)
			{
				tList<TESForm> addedForms;
				addedForms.Init();
				int numItemsAdded = 0;
				do
				{
					if (auto form = selectedFormIter->ref)
					{
						if (!IsFormInListView(listView, form))
						{
							addedForms.Insert(form);
							++numItemsAdded;
						}
					}
				} while (selectedFormIter = selectedFormIter->next);

				if (numItemsAdded)
				{
					SetDeferListUpdate(listView, true);

					AddFormsToListView(listView, &addedForms);
					SendMessageA(Hwnd, BFL_ADDED_ITEMS, (WPARAM)&addedForms, (LPARAM)listView);

					SetDeferListUpdate(listView, false);

					addedForms.RemoveAll();
				}
			}
			break;
		}
		}

		return CallWindowProc(originalWindowCallback, Hwnd, Message, wParam, lParam);
	}

	bool ShowOpenFileDialog(HWND hWnd, char* dst, size_t dstLen, bool bSave = false) {
		
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = dst;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = dstLen;
		ofn.lpstrFilter = "All files (*.*)\0*.*\0Text files (*.txt)\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = "Data\\nvse\\plugins\\GeckExtender";
		ofn.Flags = OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | (bSave ? OFN_OVERWRITEPROMPT : OFN_FILEMUSTEXIST);

		return bSave ? GetSaveFileName(&ofn) : GetOpenFileName(&ofn);
	}

	bool ShowSaveFileDialog(HWND hWnd, char* dst, size_t dstLen)
	{
		return ShowOpenFileDialog(hWnd, dst, dstLen, true);
	}

	void SetWindowTitleToFileName(HWND listView, const char* path)
	{
		const char* fileName = strrchr(path, '\\');
		if (fileName)
		{
			fileName++; // skip the backslash
		}
		else
		{
			fileName = path; // no backslash found, the path is the file name
		}

		char fileNameNoExt[MAX_PATH];
		strcpy(fileNameNoExt, fileName);

		char* lastDot = strrchr(fileNameNoExt, '.');
		if (lastDot)
		{
			*lastDot = '\0';
		}

		SendMessageA(GetParent(listView), WM_SETTEXT, 0, (LPARAM)fileNameNoExt);
	}

	void LoadFormListFromFile(HWND listView, const char* path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			return;
		}

		tList<TESForm> forms;
		forms.Init();

		std::string line;
		while (getline(file, line))
		{
			std::stringstream ss(line);
			std::string formEditorId;

			while (getline(ss, formEditorId, ','))
			{
				if (auto form = LookupFormByName(formEditorId.c_str()))
				{
					forms.Append(form);
				}
			}
		}

		file.close();

		if (!forms.IsEmpty())
		{
			SetDeferListUpdate(listView, true);

			SendMessageA(listView, LVM_DELETEALLITEMS, 0, 0);
			AddFormsToListView(listView, &forms);

			SetDeferListUpdate(listView, false);
		}

		forms.RemoveAll();

		SetWindowTitleToFileName(listView, path);
	}

	void SaveFormListToFile(HWND listView, const char* path)
	{
		std::stringstream concatenatedIDs;
		bool isFirstItem = true;
		int iIndex = -1;
		while ((iIndex = ListView_GetNextItem(listView, iIndex, LVNI_ALL)) != -1)
		{
			if (auto form = GetNthListForm(listView, iIndex))
			{
				if (!isFirstItem)
				{
					concatenatedIDs << ',';
				}

				isFirstItem = false;
				concatenatedIDs << (form->GetEditorID());
			}
		}

		std::ofstream outFile(path);
		if (!outFile)
		{
			Console_Print("Failed to open file for writing: %s", path);
			return;
		}

		outFile << concatenatedIDs.str();
		outFile.close();
		if (outFile.fail())
		{
			Console_Print("Error occurred when writing to the file: %s", path);
			return;
		}

		SetWindowTitleToFileName(listView, path);
	}

	void LoadFormList(HWND listView)
	{
		char buf[MAX_PATH]; *buf = '\0';
		if (ShowOpenFileDialog(listView, buf, sizeof(buf)))
		{
			LoadFormListFromFile(listView, buf);
		}
	}

	void SaveFormList(HWND listView)
	{
		char buf[MAX_PATH]; *buf = '\0';
		if (ShowSaveFileDialog(listView, buf, sizeof(buf)))
		{
			SaveFormListToFile(listView, buf);
		}
	}

	constexpr int IDM_LOAD = 0x1001;
	constexpr int IDM_SAVE = 0x1002;
	constexpr int IDC_TOOLBAR = 0x1003;
	HWND CreateToolbar(HWND hParent)
	{
		HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
			WS_CHILD | TBSTYLE_WRAPABLE, 5, 0, 0, 0,
			hParent, (HMENU)IDC_TOOLBAR, GetModuleHandle(NULL), NULL);

		SendMessage(hWndToolbar, TB_SETBITMAPSIZE, 0, MAKELONG(16, 4));
		SendMessage(hWndToolbar, TB_SETBUTTONSIZE, 0, MAKELONG(24, 8));
		SendMessage(hWndToolbar, TB_SETPADDING, 0, MAKELPARAM(4, 2));

		TBBUTTON tbButtons[] =
		{
			{ MAKELONG(0, 0), IDM_LOAD, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)"Load" },
			{ MAKELONG(1, 0), IDM_SAVE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, (INT_PTR)"Save" },
		};

		SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
		SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)sizeof(tbButtons) / sizeof(TBBUTTON), (LPARAM)&tbButtons);

		SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
		ShowWindow(hWndToolbar, SW_SHOW);

		return hWndToolbar;
	}

	void UpdateToolbarAndListviewPositions(HWND Hwnd)
	{
		RECT rcClient;
		GetClientRect(Hwnd, &rcClient);

		// Resize the toolbar and let it auto-adjust its height
		auto hWndToolbar = GetDlgItem(Hwnd, IDC_TOOLBAR);
		SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);

		RECT rcToolbar;
		GetWindowRect(hWndToolbar, &rcToolbar);
		int toolbarHeight = rcToolbar.bottom - rcToolbar.top;

		// Calculate the new height and position for the list view
		int listViewHeight = rcClient.bottom - toolbarHeight;
		auto listView = GetDlgItem(Hwnd, 1018);
		SetWindowPos(listView, NULL, 0, toolbarHeight, rcClient.right, listViewHeight, SWP_NOZORDER);
	}

	LRESULT CALLBACK WindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_INITDIALOG:
		{
			CreateToolbar(Hwnd);
			UpdateToolbarAndListviewPositions(Hwnd);
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDM_LOAD:
				LoadFormList(GetDlgItem(Hwnd, 1018));
				break;
			case IDM_SAVE:
				SaveFormList(GetDlgItem(Hwnd, 1018));
				break;
			}
			break;
		}
		case WM_SIZE:
		{
			UpdateToolbarAndListviewPositions(Hwnd);
			break;
		}
		}
		return BaseWindowCallback(Hwnd, Message, wParam, lParam);
	}

	void __fastcall OnSelectForm(TESForm* form, HWND parent)
	{
		if (form)
		{
			OpenForm(form, parent);
		}
	}

	__declspec(naked) void OnSelectFormHook()
	{
		_asm
		{
			pop ecx
			mov edx, esi // hDlg
			mov ecx, eax // form
			push 0x483B3D
			jmp OnSelectForm
		}
	}

	void __cdecl InitFormNameColumn(HWND hWnd, WPARAM index, char* pszText, int width, int format)
	{
		width = 200;
		CdeclCall(0x419F50, hWnd, index, pszText, width, format);
	}

	enum
	{
		COLUMN_TYPE = 3
	};

	void __cdecl InitFormIdNameAndFormTypeColumns(HWND hWnd, WPARAM index, char* pszText, int width, int format)
	{
		width = 80;
		CdeclCall(0x419F50, hWnd, index, pszText, width, format);
		CdeclCall(0x419F50, hWnd, 2, "Name", width, format);
		CdeclCall(0x419F50, hWnd, COLUMN_TYPE, "Type", width, format);
	}

	const char* FormToTypeStr(TESForm* form)
	{
		auto formTypeNames = (const char**)0xE94404;
		return formTypeNames[form->typeID * 3];
	}

	int CompareTypes(TESForm* a1, TESForm* a2)
	{
		return _stricmp(FormToTypeStr(a1), FormToTypeStr(a2));
	}

	int __stdcall CompareForms(TESForm* a1, TESForm* a2, int a3)
	{
		int column = abs(a3) - 1;
		if (column == COLUMN_TYPE)
		{
			int result = CompareTypes(a1, a2);
			return (a3 < 0) ? -result : result;
		}
		else
		{
			return StdCall<int>(0x47B1E0, a1, a2, a3);
		}
	}

	void __fastcall SetupRow(TESForm* form, void* edx, LPNMLVDISPINFOA displayInfo)
	{
		if (displayInfo->item.mask & 1)
		{
			if (displayInfo->item.iSubItem == COLUMN_TYPE)
			{
				sprintf(displayInfo->item.pszText, FormToTypeStr(form));
			}
		}
		return form->SetupListViewDisplayInfo(displayInfo);
	}

	void Init()
	{
		// don't close the list view when clicking on an item
		WriteRelJump(0x483B32, UInt32(OnSelectFormHook));

		originalWindowCallback = *(WNDPROC*)0x44BB19;
		SafeWrite32(0x44BB19, UInt32(WindowCallback));

		// adjust default column sizes
		WriteRelCall(0x48387D, UInt32(InitFormNameColumn));
		WriteRelCall(0x48389C, UInt32(InitFormIdNameAndFormTypeColumns));

		// add support for sorting the 'Type' column
		SafeWrite32(0x48368A, UInt32(CompareForms));

		// support displaying the type column
		SafeWrite8(0x4836AD, 0xB8);
		SafeWrite32(0x4836AD + 1, UInt32(SetupRow));
		SafeWrite8(0x4836AD + 1 + 4, 0x90);
	}
}