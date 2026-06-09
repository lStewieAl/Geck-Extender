#include "Utilities.h"
#include "GameData.h"
#include "Settings.h"

#include <string>
#include <sstream>
#include <unordered_set>

#include <Events/DataLoadEvent.h>

namespace RegionEditorEx
{
	std::unordered_set<std::string> ignoredRegionEditorIDs;

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

	bool __fastcall ShouldHideRegionFromList(TESForm* apRegion)
	{
		const char* pEditorID = apRegion->GetEditorID();
		if (!pEditorID || !*pEditorID)
		{
			return false;
		}

		return ignoredRegionEditorIDs.find(pEditorID) != ignoredRegionEditorIDs.end();
	}

	__HOOK OnLoadRegionListItemHook()
	{
		_asm
		{
			mov ebx, eax
			mov ecx, eax
			call ShouldHideRegionFromList
			test al, al
			jne skip

			mov ebx, [ebx + 0x38]
			test ebx, ebx
			mov eax, 0x742D85
			jmp eax

		skip:
			mov eax, 0x742E66
			jmp eax
		}
	}

	char IniPath[MAX_PATH];
	constexpr const char* INI_SECTION = "Settings";
	constexpr const char* INI_KEY = "sIgnoredRegionEditorIDs";
	void InitIgnoredRegions()
	{
		char buffer[0x2000] = {};

		GetPrivateProfileStringA(INI_SECTION, INI_KEY, "", buffer, sizeof(buffer), IniPath);

		ignoredRegionEditorIDs.clear();

		std::stringstream ss(buffer);
		std::string item;

		while (std::getline(ss, item, ','))
		{
			// Trim whitespace
			item.erase(0, item.find_first_not_of(" \t"));
			item.erase(item.find_last_not_of(" \t") + 1);

			if (!item.empty())
			{
				ignoredRegionEditorIDs.insert(std::move(item));
			}
		}
	}

	void InitHooks()
	{
		originalRegionEditorFn = DetourRelCall(0x7488B2, UInt32(RegionEditorCallback));

		// allow hiding regions from the dropdown
		WriteRelJump(0x742D80, UInt32(OnLoadRegionListItemHook));

		GetModuleFileNameA(NULL, IniPath, MAX_PATH);
		strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\GeckExtender\\RegionEditor\\IgnoredRegions.ini");

		DataLoadEvent::RegisterCallback(InitIgnoredRegions);
	}
}