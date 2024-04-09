#include "SafeWrite.h"
#include "ZeLogWindow.h"
#include "GameObjects.h"
#include "GECKUtility.h"

namespace BetterFloatingFormList
{
	WNDPROC originalWindowCallback;
	LRESULT CALLBACK WindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		switch (Message)
		{
		case WM_INITDIALOG:
//			DragAcceptFiles(Hwnd, TRUE);
			break;

		case BSMsg_AcceptsDropType:
//			EditorUI_Log("AcceptsDropType");
			return (HWND)lParam != Hwnd && wParam != kFormType_Reference;

		case BSMsg_HandleDrop:
//			EditorUI_Log("HandleDrop");
			break;
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

	void Init()
	{
		// don't close the list view when clicking on an item
		WriteRelJump(0x483B32, UInt32(OnSelectFormHook));

		originalWindowCallback = *(WNDPROC*)0x44BB19;
		SafeWrite32(0x44BB19, UInt32(WindowCallback));
	}
}