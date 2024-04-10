#include "GECKUtility.h"
#include "GameObjects.h"
extern HWND g_MainHwnd;

INT_PTR WINAPI hk_DialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
namespace ModifiedFormViewer
{
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
		auto hWnd = CreateDialogParamA(hInstance, (LPCSTR)189, g_MainHwnd, *(DLGPROC*)0x44BB19, (LPARAM)&data);
		SendMessageA(hWnd, WM_SETTEXT, 0, (LPARAM)"Modified Forms");

		data.forms.RemoveAll();
	}
}