#include "Utilities.h"
#include "GeckUtility.h"
#include "GameData.h"
#include "NiTypes.h"
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

	char __fastcall RegionEditorData_ZoomIn(RegionEditor::Data* pData, void* edx, float afZoom, char abRender)
	{
		float oldWidth = pData->fViewWidth;
		float oldHeight = pData->fViewHeight;

		if (afZoom < 0.0f || afZoom > 1.0f)
			return 0;

		float scale = 1.0f - afZoom * 0.5f;

		float newWidth = oldWidth * scale;
		float newHeight = oldHeight * scale;

		if (newWidth < 1365.0f || newHeight < 819.0f)
			return 0;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(RegionEditor::GetBlackRect(), &pt);

		NiPoint2 kBefore;
		pData->ScreenToWorld(&kBefore, pt.x, pt.y);

		// Change zoom
		pData->fViewWidth = newWidth;
		pData->fViewHeight = newHeight;

		NiPoint2 kAfter;
		pData->ScreenToWorld(&kAfter, pt.x, pt.y);

		// Adjust offsets to compensate
		pData->fOffsetX += kBefore.x - kAfter.x;
		pData->fOffsetY += kBefore.y - kAfter.y;

		if (abRender)
		{
			pData->Render();
		}
		else
		{
			EnableWindow(pData->hScrollbarHorizontal, 0);
			EnableWindow(pData->hScrollbarVertical, 0);
		}

		return 1;
	}

	bool __fastcall RegionEditorData_ZoomOut(RegionEditor::Data* pData, void* edx, float afZoom, char abRender)
	{
		float oldWidth = pData->fViewWidth;
		float oldHeight = pData->fViewHeight;

		if (afZoom < 0.0f || afZoom > 1.0f)
			return 0;

		float scale = 1.0f + afZoom;
		float newWidth = oldWidth * scale;
		float newHeight = oldHeight * scale;

		// increase min zoom by 3x
		if (newWidth > 327680.0f * 3.f || newHeight > 196608.0f * 3.f)
			return 0;

		// make the zoom happen relative to the mouse position
		// instead of the center
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(RegionEditor::GetBlackRect(), &pt);

		NiPoint2 kBefore;
		pData->ScreenToWorld(&kBefore, pt.x, pt.y);

		pData->fViewWidth = newWidth;
		pData->fViewHeight = newHeight;

		NiPoint2 kAfter;
		pData->ScreenToWorld(&kAfter, pt.x, pt.y);

		pData->fOffsetX += kBefore.x - kAfter.x;
		pData->fOffsetY += kBefore.y - kAfter.y;

		if (abRender)
		{
			pData->Render();
		}
		else
		{
			EnableWindow(pData->hScrollbarHorizontal, 0);
			EnableWindow(pData->hScrollbarVertical, 0);
		}

		return 1;
	}

	__HOOK OnLoadRegionsHook()
	{
		// ebp (TESWorldSpace*) should be null if it's invalid not -1
		_asm
		{
			cmp dword ptr ds : [esi + 0x10], ebx
			je nullRegion

			cmp ebp, -1
			jne done

			xor ebp, ebp
		done:
			pop eax
			mov eax, 0x743F9D
			jmp eax

		nullRegion:
			ret
		}
	}

	void Init()
	{
		originalRegionEditorFn = DetourRelCall(0x7488B2, UInt32(RegionEditorCallback));

		// allow hiding regions from the dropdown
		WriteRelJump(0x742D80, UInt32(OnLoadRegionListItemHook));

		GetModuleFileNameA(NULL, IniPath, MAX_PATH);
		strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\GeckExtender\\RegionEditor\\IgnoredRegions.ini");

		DataLoadEvent::RegisterCallback(InitIgnoredRegions);

		// only show the LOD renderer when holding CTRL if there is LOD textures to render... to avoid showing a black screen
		WriteRelCall(0x7497EB, UInt32(OnRenderLODHook));

		// zoom into where the cursor is instead of centered
		WriteRelCall(0x74719E, UInt32(RegionEditorData_ZoomIn));
		WriteRelCall(0x7471CA, UInt32(RegionEditorData_ZoomOut));

		// fix crash when clicking on 'Regions' without an esm loaded
		WriteRelCall(0x743F8E, UInt32(OnLoadRegionsHook));
	}
}