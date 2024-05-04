#include "GameForms.h"
#include "GameObjects.h"
#include "GameData.h"
#include "GameSettings.h"
#include "GECKUtility.h"
#include "NavMeshPickPreventer.h"

#include <CommCtrl.h>

namespace CustomRenderWindowHotkeys
{
	void PlaceXMarker()
	{
		NiPoint3 pos, rot;
		if (RenderWindow::GetMousePos(&pos, &rot))
		{
			TESObjectSTAT* xMarker = *(TESObjectSTAT**)0xEDDA54;
			if (auto ref = DataHandler::GetSingleton()->CreateReferenceAtLocation(xMarker, &pos, &NiPoint3::ZERO, 0))
			{
				CdeclCall(0x44F470);
				CdeclCall(0x44F260, ref); // RenderWindow::AddRef

				HistoryManager::GetSingleton()->AddAction(2, RenderWindow::SelectedData::GetSelected());

				if (*(byte*)0xECFB74) // CellView::bLoaded
				{
					auto pos = &RenderWindow::SelectedData::GetSelected()->pos;
					CdeclCall(0x42E3C0, &pos, 1); // CellView::Refresh
				}
			}
		}
	}

	void ToggleRefVisible(TESObjectREFR* ref)
	{
		if (auto node = ref->Get3D())
		{
			if (node = ThisCall<NiNode*>(0x68B370, node, 0xF1FDD4)) // NiRTTI::HasType(&NiNode::ms_RTTI);
			{
				node->SetAlphaRecurse(1.0F);
				node->m_flags &= ~0x200001;
				node->UpdatePropertiesUpward();
			}
		}
	}

	void SetAllObjectsVisible()
	{
		RunCallbackOnAllCellRefs(ToggleRefVisible);
	}

	static bool bAllDisabledRefsHidden = false;
	void GetAnyDisabledRefsVisible(TESObjectREFR* ref)
	{
		if (ref->IsDisabled())
		{
			if (auto node = ref->Get3D())
			{
				if (node->GetAsNiNode())
				{
					if (node->IsVisible())
					{
						bAllDisabledRefsHidden = false;
					}
				}
			}
		}
	}

	void SetIsDisabledRefHidden(TESObjectREFR* ref)
	{
		if (ref->IsDisabled())
		{
			if (auto node = ref->Get3D())
			{
				if (node->GetAsNiNode())
				{
					node->SetVisible(!bAllDisabledRefsHidden);
				}
			}
		}
	}

	void ToggleShowDisabledObjects()
	{
		bAllDisabledRefsHidden = true;
		RunCallbackOnAllCellRefs(GetAnyDisabledRefsVisible);
		bAllDisabledRefsHidden = !bAllDisabledRefsHidden;
		RunCallbackOnAllCellRefs(SetIsDisabledRefHidden);
	}

	enum CustomHotkey
	{
		kHotkey_NONE = 0x47, // max ID of the vanilla IDs
		kHotkey_FIRST,
		kHotkey_ToggleShowLightMarkers = kHotkey_FIRST,
		kHotkey_SetAllObjectsVisible,
		kHotkey_PlaceXMarker,
		kHotkey_NavMeshIgnoreLastPick,
		kHotkey_NavMeshIgnoreSelectedPicks,
		kHotkey_ToggleShowDisabledObjects,
	};

	static RenderWindowHotkey CustomHotkeys[] =
	{
		RenderWindowHotkey("Toggle show light markers", "ToggleShowLightMarkers", 'I', RenderWindowHotkey::kRenderWindowPreferenceFlag_NONE, RenderWindowHotkey::kRenderHotkeyCategory_Visibility),
		RenderWindowHotkey("Set all objects visible", "SetAllObjectsVisible", VK_OEM_4, RenderWindowHotkey::kRenderWindowPreferenceFlag_NONE, RenderWindowHotkey::kRenderHotkeyCategory_Visibility),
		RenderWindowHotkey("Place xMarker", "PlaceXMarker", 'O'),
		RenderWindowHotkey("Navmesh: Ignore last picked form", "NavMeshIgnoreLastPick", 'K', RenderWindowHotkey::kRenderWindowPreferenceFlag_Navmesh, RenderWindowHotkey::kRenderHotkeyCategory_Navmesh),
		RenderWindowHotkey("Navmesh: Ignore selected forms", "NavMeshIgnoreSelectedPicks", 'K', RenderWindowHotkey::kRenderWindowPreferenceFlag_Shift, RenderWindowHotkey::kRenderHotkeyCategory_Navmesh),
		RenderWindowHotkey("Toggle Show disable objects", "ToggleShowDisabledObjects", 'N', RenderWindowHotkey::kRenderWindowPreferenceFlag_NONE, RenderWindowHotkey::kRenderHotkeyCategory_Visibility),
	};

	void HandleHotkey(UInt32 hotkey)
	{
		switch (hotkey)
		{
		case kHotkey_ToggleShowLightMarkers:
			SetIsShowLightMarkers(!GetIsShowLightMarkers());
			break;

		case kHotkey_PlaceXMarker:
			PlaceXMarker();
			break;

		case kHotkey_SetAllObjectsVisible:
			SetAllObjectsVisible();
			break;

		case kHotkey_NavMeshIgnoreLastPick:
			NavMeshPickPreventer::OnKeyDown(false);
			break;

		case kHotkey_NavMeshIgnoreSelectedPicks:
			NavMeshPickPreventer::OnKeyDown(true);
			break;

		case kHotkey_ToggleShowDisabledObjects:
			ToggleShowDisabledObjects();
			break;
		}
	}

	void __cdecl PopulateList(HWND hDlg)
	{
		auto listView = GetDlgItem(hDlg, 5142);
		for (int i = 0; i < _ARRAYSIZE(CustomHotkeys); ++i)
		{
			CdeclCall(0x41A020, listView, &CustomHotkeys[i], 0, -1); // ListView::InsertItem
		}
	}

	void AddCustomHotkey(RenderWindowHotkey::KeyCombo combo, unsigned int hotkey)
	{
		void* hotkeysNiTMap = (void*)0xE8CF3C;
		ThisCall(0x50D880, hotkeysNiTMap, combo.data, kHotkey_FIRST + hotkey);
	}

	void __cdecl AddKeysToMap()
	{
		for (int i = 0; i < _ARRAYSIZE(CustomHotkeys); ++i)
		{
			AddCustomHotkey(CustomHotkeys[i].kCombo, i);
		}
	}

	RenderWindowHotkey::eKeys __cdecl GetPressedHotkeyID(char aiKeyCode, char abShift, char abControl, char abAlt, char abIsZ, char abIsNavMesh)
	{
		auto hotkey = CdeclCall<RenderWindowHotkey::eKeys>(0x412F00, aiKeyCode, abShift, abControl, abAlt, abIsZ, abIsNavMesh);
		if (hotkey > RenderWindowHotkey::kRenderHotkey_COUNT)
		{
			HandleHotkey(hotkey);
		}
		return hotkey;
	}

	constexpr const char* INI_SECTION = "Render Window";
	char IniPath[MAX_PATH];
	void SetupINIPath()
	{
		GetModuleFileNameA(NULL, IniPath, MAX_PATH);
		strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\GeckExtender\\Hotkeys.ini");
	}

	void ReadINI()
	{
		for (int i = 0; i < _ARRAYSIZE(CustomHotkeys); ++i)
		{
			unsigned short combo = GetPrivateProfileIntA(INI_SECTION, CustomHotkeys[i].sInternalName, 0xFFFF, IniPath);
			if (combo != 0xFFFF)
			{
				CustomHotkeys[i].kCombo.data = combo;
			}
		}
	}

	void SaveINI()
	{
		char buffer[0x10];
		for (int i = 0; i < _ARRAYSIZE(CustomHotkeys); ++i)
		{
			_itoa(CustomHotkeys[i].kCombo.data, buffer, 10);
			WritePrivateProfileStringA(INI_SECTION, CustomHotkeys[i].sInternalName, buffer, IniPath);
		}
	}

	void RestoreDefaults()
	{
		for (int i = 0; i < _ARRAYSIZE(CustomHotkeys); ++i)
		{
			RenderWindowHotkey* hotkey = &CustomHotkeys[i];
			hotkey->kCombo = hotkey->kDefaultCombo;
		}
	}

	void __cdecl OnRestoreDefaults()
	{
		CdeclCall(0x413230);
		RestoreDefaults();
	}

	void __cdecl OnSaveChanges()
	{
		CdeclCall(0x412CE0);
		SaveINI();
	}

	void __cdecl OnCancelChanges()
	{
		CdeclCall(0x4130D0);
		ReadINI();
	}

	void InitHooks()
	{
		// add support for custom render window preferences
		WriteRelJump(0x412D8B, UInt32(PopulateList));
		WriteRelJump(0x412FF0, UInt32(AddKeysToMap));

		WriteRelCall(0x456975, UInt32(GetPressedHotkeyID));

		WriteRelCall(0x4134B5, UInt32(OnRestoreDefaults));
		WriteRelCall(0x4134A6, UInt32(OnSaveChanges));
		WriteRelCall(0x41348A, UInt32(OnCancelChanges));
	}

	void Init()
	{
		InitHooks();
		SetupINIPath();
		ReadINI();
	}
}