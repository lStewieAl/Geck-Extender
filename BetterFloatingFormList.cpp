#include "BetterFloatingFormList.h"
#include <commctrl.h>
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
	LRESULT CALLBACK WindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
					CdeclCall(0x47E410, listView, &addedForms, nullptr, nullptr);

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

	enum
	{
		COLUMN_TYPE = 3
	};

	void __cdecl InitFormIdNameAndFormTypeColumns(HWND hWnd, WPARAM index, char* pszText, int width, int format)
	{
		width = 64;
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