#pragma once
namespace BetterFloatingFormList
{
	enum Messages
	{
		BFL_ADDED_ITEMS = WM_USER + 1000, // WPARAM (tList<TESForm>* modifiedForms) LPARAM (HWND listView)
		BFL_DELETED_ITEMS, // WPARAM (tList<TESForm>* modifiedForms) LPARAM (HWND listView)

		BFL_SET_FILTER,
		BFL_SET_INITIAL_SORT,
	};

	void Init();
	LRESULT CALLBACK BaseWindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
}