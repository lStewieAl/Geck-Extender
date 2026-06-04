#include "GECKUtility.h"
#include "GameObjects.h"
#include "Utilities.h"
#include "BetterFloatingFormList.h"

extern HWND g_MainHwnd;
HWND WINAPI hk_CreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
namespace ModifiedFormViewer
{
	LRESULT CALLBACK SubclassedListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		if (uMsg == WM_KEYDOWN)
		{
			if (wParam == 'A' && GetAsyncKeyState(VK_CONTROL) < 0)
			{
				SelectAllItemsInListView(hWnd);
			}
			else if (wParam == VK_DELETE)
			{
				return true;
			}
		}

		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}

	void SubclassListView(HWND hListView)
	{
		SetWindowSubclass(hListView, SubclassedListViewProc, 0, 0);
	}

	LRESULT CALLBACK WindowCallback(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == WM_INITDIALOG)
		{
			SendMessageA(hWnd, BetterFloatingFormList::BFL_SET_FILTER, 0, 0);
		}
		auto result = BetterFloatingFormList::BaseWindowCallback(hWnd, Message, wParam, lParam);
		if (Message == WM_INITDIALOG)
		{
			// prevent the delete button deleting items from the list as it's misleading if users expect it to actually remove modified forms
			auto listView = GetDlgItem(hWnd, 1018);
			SubclassListView(listView);
			DragAcceptFiles(hWnd, FALSE);
		}
		return result;
	}

	void Show()
	{
		struct ObjectWindowNodeData
		{
			UInt8 formType;
			UInt32 numColumns;
			UInt32 unk08;
			tList<TESForm> forms;
		} data;

		data.forms.Init();
		auto modifiedForms = *(NiTLargeArray<TESForm*>**)0xED56D0;

		for (int i = 0, size = modifiedForms->firstFreeEntry; i < size; ++i)
		{
			data.forms.Insert(modifiedForms->data[i]);
		}

		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);
		auto hWnd = hk_CreateDialogParamA(hInstance, (LPCSTR)189, g_MainHwnd, (DLGPROC)WindowCallback, (LPARAM)&data);
		SendMessageA(hWnd, WM_SETTEXT, 0, (LPARAM)"Modified Forms");
		data.forms.RemoveAll();
	}
}