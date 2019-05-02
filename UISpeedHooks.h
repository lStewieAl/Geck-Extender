#include "Editor.h"
#pragma once

//	fix list view lag when changing cells in the render window - credit to nukem
void hk_UpdateCellViewListView(HWND ControlHandle, void* a2, void* a3)
{
	SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
	((void(__cdecl*)(HWND ControlHandle, void* a2, void* a3))(0x0042DE00))(ControlHandle, a2, a3);
	SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
}

// fix object list caching for speed - credit to nukem
LRESULT __fastcall hk_ObjectListViewListView(DWORD* thisptr, void* ignorethisthing, char* a2, char* a3)
{
	HWND listWindow = GetDlgItem(*(HWND*)0x00ECFB70, 1041);
	SendMessage(listWindow, WM_SETREDRAW, FALSE, 0);
	((void(__thiscall*)(DWORD*, char*, char*))(0x439630))(thisptr, a2, a3);
	SendMessage(listWindow, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(listWindow, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
	return 0;
}

_declspec(naked) void DialogueListViewBeginUI() {
	static const UInt32 retnAddr = 0x592EB4;
	static const UInt32 skipAddr = 0x592FB8;
	_asm {
		jne defer
		jmp skipAddr
		defer :
		push edi
	}
	BeginUIDefer();
	_asm {
		pop edi
		originalCode :
		jmp retnAddr
	}
}

_declspec(naked) void DialogueListViewEndUI() {
	static const UInt32 retnAddr = 0x592FB8;
	EndUIDefer();
	_asm {
	originalCode:
		mov esi, dword ptr ds : [0xD23550] //SendMessageA
			jmp retnAddr
	}
}

void __stdcall hk_sub_59C7B0(HWND hWnd) {
	BeginUIDefer();
	((void(__stdcall*)(HWND window))(0x59C7B0))(hWnd);
	EndUIDefer();
}


/*
It would be a lot faster with the BeginUIDefer calls, however it causes dirty edits in PNAM - Previous INFO and ANAM - Speaker
*/
void __fastcall hk_sub_59C950(char* thiss, void* dummyEDX, int a2, HWND hDlg) {
	//	BeginUIDefer();
	SendMessage(hDlg, WM_SETREDRAW, FALSE, 0);
	((char(__thiscall*)(char* str, int a2, HWND wnd))(0x59C950))(thiss, a2, hDlg);
	SendMessage(hDlg, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(hDlg, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE);
	//	EndUIDefer();
}

int* __cdecl hk_sub_595410(HWND hDlg, int a2, char a3) {
	BeginUIDefer();
	int* res = ((int* (__cdecl*)(HWND hDlg, int a2, char a3))(0x595410))(hDlg, a2, a3);
	EndUIDefer();
	return res;
}

char* __fastcall hk_sub_56B270(int* thiss, void* dummyEDX, HWND hWnd) {
	BeginUIDefer();
	char* res = ((char* (__thiscall*)(int* something, HWND wnd))(0x56B270))(thiss, hWnd);
	EndUIDefer();
	return res;
}

/*
hook to speed up Select Topic dialog
*/
void __cdecl hk_sub_595800(HWND hDlg, int a2, int a3) {
	BeginUIDefer();
	((LRESULT(__cdecl*)(HWND hDlg, int a2, int a3))(0x595800))(hDlg, a2, a3);
	EndUIDefer();
}

void __cdecl hk_sub_47D910(HWND hWnd, char a2, char a3, char a4) {
	BeginUIDefer();
	((void(_cdecl*)(HWND hWnd, char a2, char a3, char a4))(0x47D910))(hWnd, a2, a3, a4);
	void __cdecl sub_47D910(HWND hWnd, char a2, char a3, char a4);
	EndUIDefer();
}

void __cdecl hk_sub_47D330(HWND hWnd, char a2, char a3) {
	BeginUIDefer();
	((void(__cdecl*)(HWND hWnd, char a2, char a3))(0x47D330))(hWnd, a2, a3);
	EndUIDefer();
}

BOOL __fastcall hk_sub_49B260(int* thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	BOOL res = ((BOOL(__thiscall*)(int* thiss, HWND hDlg))(0x49B260))(thiss, hDlg);
	EndUIDefer();
	return res;
}

BOOL __fastcall hk_sub_4979F0(void* thiss, void* dummyEDX, HWND hDlg, int a3) {
	BeginUIDefer();
	BOOL res = ((BOOL(__thiscall*)(void* thiss, HWND hDlg, int a3))(0x4979F0))(thiss, hDlg, a3);
	EndUIDefer();
	return res;
}

void __fastcall hk_sub_4E3020(int* thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	((void(__thiscall*)(int* thiss, HWND hDlg))(0x4E3020))(thiss, hDlg);
	EndUIDefer();
}

void __fastcall hk_sub_4E8A20(int* thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	((void(__thiscall*)(int* thiss, HWND hDlg))(0x4E8A20))(thiss, hDlg);
	EndUIDefer();
}


void __cdecl hk_sub_47F7A0(HWND hWnd, char a2, char a3, char a4, int a5, int a6) {
	BeginUIDefer();
	((void(__cdecl*)(HWND, char, char, char, int, int))(0x47F7A0))(hWnd, a2, a3, a4, a5, a6);
	EndUIDefer();
}

void __cdecl hk_sub_47E0D0(HWND hWnd, char a2, int a3, char a4) {
	BeginUIDefer();
	((void(__cdecl*)(HWND hWnd, char a2, int a3, char a4))(0x47E0D0))(hWnd, a2, a3, a4);
	EndUIDefer();
}

void WriteUIHooks() {
	//	Fix list view lag when changing cells in the render window - credit to nukem
	WriteRelCall(0x0042F5A4, (UInt32)hk_UpdateCellViewListView);
	WriteRelCall(0x004305F5, (UInt32)hk_UpdateCellViewListView);
	WriteRelCall(0x004309B2, (UInt32)hk_UpdateCellViewListView);

	//	Fix object list view for speed - credit to nukem
	WriteRelCall(0x00449779, (UInt32)hk_ObjectListViewListView);
	WriteRelCall(0x00449AE4, (UInt32)hk_ObjectListViewListView);
	WriteRelCall(0x0044A439, (UInt32)hk_ObjectListViewListView);
	WriteRelCall(0x0044B375, (UInt32)hk_ObjectListViewListView);
	WriteRelCall(0x0044BE68, (UInt32)hk_ObjectListViewListView);

	/* speed up the dialogue views - credit to Nukem */
	WriteRelJump(0x419BC0, UInt32(InsertComboBoxItem));
	WriteRelJump(0x41A020, UInt32(InsertListViewItem));
	WriteRelJump(0x592EAE, UInt32(DialogueListViewBeginUI));
	WriteRelJump(0x592FB2, UInt32(DialogueListViewEndUI));

	WriteRelCall(0x59CCB3, UInt32(hk_sub_59C7B0)); // combo box item, required to fix multiple insertions tripping the assertion 
	WriteRelCall(0x409C08, UInt32(hk_sub_59C950));
	WriteRelCall(0x59957B, UInt32(hk_sub_59C950));
	WriteRelCall(0x59A728, UInt32(hk_sub_59C950));
	WriteRelCall(0x5A1952, UInt32(hk_sub_59C950));

	WriteRelCall(0x57E673, UInt32(hk_sub_595410));
	WriteRelCall(0x57E69A, UInt32(hk_sub_595410));
	WriteRelCall(0x57E6C1, UInt32(hk_sub_595410));
	WriteRelCall(0x57E6E8, UInt32(hk_sub_595410));
	WriteRelCall(0x57E70F, UInt32(hk_sub_595410));
	WriteRelCall(0x57E75D, UInt32(hk_sub_595410));
	WriteRelCall(0x57E736, UInt32(hk_sub_595410));

	WriteRelCall(0x59CC55, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x59CC70, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x59CC8B, UInt32(hk_sub_47F7A0));

	/* speed up select dialog topic */
	WriteRelCall(0x597312, UInt32(hk_sub_595800));
	WriteRelCall(0x597504, UInt32(hk_sub_595800));
	WriteRelCall(0x59770E, UInt32(hk_sub_595800));
	WriteRelCall(0x5979EF, UInt32(hk_sub_595800));

	// speed up responses window by defering dropdown box rendering
	WriteRelCall(0x56CC12, UInt32(hk_sub_56B270));
	WriteRelCall(0x56CC52, UInt32(hk_sub_56B270));

	// speed up weapons window
	WriteRelCall(0x50A046, UInt32(hk_sub_47D910));
	WriteRelCall(0x67088D, UInt32(hk_sub_47D910));

	// speed up weapon 'mods' window
	WriteRelCall(0x602157, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602401, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602451, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x60247B, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x6029AE, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x6029D8, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602A02, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602A2C, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602A56, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602A80, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602AD4, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602AFE, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602B28, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x602AAA, UInt32(hk_sub_47F7A0));

	// speed up reference batch action window 
	WriteRelCall(0x47FBF3, UInt32(hk_sub_47D330));
	WriteRelCall(0x48C0AF, UInt32(hk_sub_47D330));
	WriteRelCall(0x48C128, UInt32(hk_sub_47D330));

	// speed up packages window
	WriteRelCall(0x498CB4, UInt32(hk_sub_4979F0));
	WriteRelCall(0x499509, UInt32(hk_sub_4979F0));
	WriteRelCall(0x49957E, UInt32(hk_sub_4979F0));
	WriteRelCall(0x4995BC, UInt32(hk_sub_4979F0));
	WriteRelCall(0x4995FB, UInt32(hk_sub_4979F0));
	WriteRelCall(0x49963A, UInt32(hk_sub_4979F0));
	WriteRelCall(0x49966C, UInt32(hk_sub_4979F0));
	WriteRelCall(0x49D1CA, UInt32(hk_sub_47F7A0));

	// speed up object palette
	WriteRelCall(0x40CF05, UInt32(hk_sub_47E0D0));
}