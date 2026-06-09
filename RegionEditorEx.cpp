#include "Utilities.h"

namespace RegionEditorEx
{
	UInt32 originalRegionEditorFn;
	bool __fastcall RegionEditorCallback(struct RegionEditor* pRegionEditor, void* edx, HWND hWnd, unsigned int Msg, unsigned int wParam, UINT lParam)
	{
		enum { CLEAR_LAST_POINT = 152 };
		if (Msg == WM_KEYDOWN && wParam == 'Z' && (GetKeyState(VK_CONTROL) & 0x8000))
		{
			if (ThisCall<bool>(originalRegionEditorFn, pRegionEditor, hWnd, WM_COMMAND, CLEAR_LAST_POINT, 0))
			{
				return TRUE;
			}
		}
		return ThisCall<bool>(originalRegionEditorFn, pRegionEditor, hWnd, Msg, wParam, lParam);
	}

	void InitHooks()
	{
		originalRegionEditorFn = DetourRelCall(0x7488B2, UInt32(RegionEditorCallback));
	}
}