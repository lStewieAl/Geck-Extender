#include <commctrl.h>

#include "SafeWrite.h"
#include "ZeLogWindow.h"
#include "GameObjects.h"
#include "GECKUtility.h"
#include "Utilities.h"

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

	void SetDeferListUpdate(HWND hWnd, bool bDefer = true)
	{
		if (bDefer)
		{
			SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
		}
		else
		{
			SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
			RedrawWindow(hWnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
	}

	LRESULT CALLBACK SubclassedListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_DELETE)
			{
				SetDeferListUpdate(hWnd, true);
				int iSelected = -1;
				while ((iSelected = ListView_GetNextItem(hWnd, iSelected, LVNI_SELECTED)) != -1)
				{
					ListView_DeleteItem(hWnd, iSelected);
					iSelected--; // because the indices shift after deletion, decrement iSelected to ensure no item is skipped
				}
				SetDeferListUpdate(hWnd, false);
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
	LRESULT CALLBACK WindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_INITDIALOG:
			SubclassListView(GetDlgItem(Hwnd, 1018));
			DragAcceptFiles(Hwnd, TRUE);
			break;

		case BSMsg_AcceptsDropType:
			SetWindowLong(Hwnd, DWL_MSGRESULT, wParam != kFormType_Reference);
			return TRUE;

		case BSMsg_HandleDrop:
		{
			auto selected = RenderWindow::SelectedData::GetSelected();
			auto listView = GetDlgItem(Hwnd, 1018);
			if (auto selectedFormIter = selected->selectedForms)
			{
				tList<TESForm> formsList;
				formsList.Init();
				do
				{
					if (auto form = selectedFormIter->ref)
					{
						if (!IsFormInListView(listView, form))
						{
							formsList.Insert(form);
						}
					}
				} while (selectedFormIter = selectedFormIter->next);

				SetDeferListUpdate(listView, true);
				CdeclCall(0x47E410, listView, &formsList, nullptr, nullptr);
				SetDeferListUpdate(listView, false);

				formsList.RemoveAll();
			}
			break;
		}
		}

		return CallWindowProc(originalWindowCallback, Hwnd, Message, wParam, lParam);
	}

	void __fastcall OnSelectForm(TESForm* form, HWND parent)
	{
		if (form)
		{
			form->OpenDialog(parent, 0, 0);
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

	void Init()
	{
		// don't close the list view when clicking on an item
		WriteRelJump(0x483B32, UInt32(OnSelectFormHook));

		originalWindowCallback = *(WNDPROC*)0x44BB19;
		SafeWrite32(0x44BB19, UInt32(WindowCallback));

		// adjust default column sizes
		WriteRelCall(0x48387D, UInt32(InitFormNameColumn)); // Form
		SafeWrite32(0x483888, 0x40); // ID
	}
}