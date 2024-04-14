#include "GECKUtility.h"
#include <CommCtrl.h>

namespace CustomRenderWindowHotkeys
{
	enum CustomHotkey
	{
		kHotkey_NONE = 0x47, // max ID of the vanilla IDs
		kHotkey_FIRST,
		kHotkey_TEST = kHotkey_FIRST,
		kHotkey_TEST2
	};

	static RenderWindowHotkey CustomHotkeys[] =
	{
		RenderWindowHotkey("Test Setting", "bTEST1", 'P'),
		RenderWindowHotkey("Test Setting 2", "bTEST2", 'J', RenderWindowHotkey::kRenderWindowPreferenceFlag_Shift)
	};

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

	void HandleHotkey(UInt32 hotkey)
	{
		switch (hotkey)
		{
		case kHotkey_TEST:
			Console_Print("TEST HOTKEY");
			break;
		case kHotkey_TEST2:
			Console_Print("TEST HOTKEY 2");
			break;
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