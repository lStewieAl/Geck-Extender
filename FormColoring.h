#pragma once
namespace FormColoring
{
	constexpr UInt32 NUM_MAX_COLORS = 0x20;
	enum { COLOR_ITEM_CLEAR = 0x10005, COLOR_ITEM_CUSTOM, COLOR_ITEM_CONFIG_BASE, COLOR_ITEM_MAX = COLOR_ITEM_CONFIG_BASE + NUM_MAX_COLORS };

	void Init();
	HMENU CreateSubMenu(HWND listView);
	bool HandlePopupMenuCommand(HWND listView, UInt32 commandID);
	LRESULT HandleCustomDraw(LPARAM lParam);
}