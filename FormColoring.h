#pragma once
namespace FormColoring
{
	void Init();
	HMENU CreateSubMenu(HWND listView);
	bool HandlePopupMenuCommand(HWND listView, UInt32 commandID);
	LRESULT HandleCustomDraw(LPARAM lParam);
}