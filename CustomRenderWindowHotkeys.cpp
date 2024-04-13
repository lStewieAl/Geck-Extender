#include "GECKUtility.h"
#include "RenderWindowHotkeyEvent.h"

namespace CustomRenderWindowHotkeys
{
	void __cdecl RenderWindowHotkeys__PopulateList(HWND hDlg)
	{
		static RenderWindowHotkey TestHotkey = RenderWindowHotkey("TEST");
		auto listView = GetDlgItem(hDlg, 5142);
		CdeclCall(0x41A020, listView, &TestHotkey, 0, -1); // ListView::InsertItem
	}

	void __cdecl RenderWindowHotkeys__ResetMap()
	{
		void* hotkeysNiTMap = (void*)0xE8CF3C;
	}

	void __fastcall OnSetCustomHotkey(int hotkeyID, RenderWindowHotkey* hotkey)
	{
		// TODO
	}

	RenderWindowHotkey::eKeys __cdecl RenderWindow__GetPressedHotkeyID(char aiKeyCode, char abShift, char abControl, char abAlt, char abIsZ, char abIsNavMesh)
	{
		auto result = CdeclCall<RenderWindowHotkey::eKeys>(0x412F00, aiKeyCode, abShift, abControl, abAlt, abIsZ, abIsNavMesh);
		if (result > RenderWindowHotkey::kRenderHotkey_COUNT)
		{
			RenderWindowHotkeyEvent::HandleEvent(result);
		}
	}

	__declspec(naked) void RenderWindowHotkeys__OnSetHotkeyHook()
	{
		_asm
		{
			cmp eax, 
			jge customHotkey
			lea ebp, [eax + eax * 4]
			add ebp, ebp
			mov eax, 0x4133F8
			jmp eax

			mov ecx, eax
			mov edx, edi
			push 0x413440
			jmp OnSetCustomHotkey
		}
	}

	void InitHooks()
	{
		// add support for custom render window preferences
		WriteRelJump(0x412D8B, UInt32(RenderWindowHotkeys__PopulateList));
		WriteRelJump(0x412FF0, UInt32(RenderWindowHotkeys__ResetMap));
		WriteRelJump(0x4133F3, UInt32(RenderWindowHotkeys__OnSetHotkeyHook));

		WriteRelCall(0x456975, UInt32(RenderWindow__GetPressedHotkeyID));
	}
}