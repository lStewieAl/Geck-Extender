#include "Utilities.h"
#include "GeckUtility.h"
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
	bool __fastcall RegionEditorCallback(RegionEditor* pRegionEditor, void* edx, HWND hWnd, unsigned int Msg, unsigned int wParam, UINT lParam)
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

	bool __fastcall ShouldHideRegionFromList(TESRegion* apRegion, RegionEditor* pEditor)
	{
		if (apRegion->worldSpace == pEditor->pData->pWrldSpc)
		{
			return false;
		}

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

			mov ecx, ebx
			mov edx, [esp + 0x14]
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

	__HOOK OnRenderLODHook()
	{
		_asm
		{
			cmp dword ptr ds : [ecx + 0x58], 0
			je skip
			mov eax, 0x748D80
			jmp eax


		skip:
			pop ecx
			mov eax, 0x749168
			jmp eax
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

		// only show the LOD renderer when holding CTRL if there is LOD textures to render... to avoid showing a black screen
		WriteRelCall(0x7497EB, UInt32(OnRenderLODHook));
	}
}