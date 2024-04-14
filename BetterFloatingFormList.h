#pragma once
namespace BetterFloatingFormList
{
	enum Messages
	{
		BFL_ADDED_ITEMS = WM_USER + 1000 // WPARAM (tList<TESForm>* modifiedForms) LPARAM (itemCount)
	};

	void Init();
}