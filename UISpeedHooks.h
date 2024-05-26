#pragma once
#include "Editor.h"

//	fix list view lag when changing cells in the render window - credit to nukem
void hk_UpdateCellViewListView(HWND ControlHandle, void* a2, void* a3)
{
	SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
	CdeclCall(0x0042DE00, ControlHandle, a2, a3);
	SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
}

// fix object list caching for speed - credit to nukem
LRESULT __fastcall hk_ObjectListViewListView(DWORD* thisptr, void* ignorethisthing, char* a2, char* a3)
{
	HWND listWindow = GetDlgItem(*(HWND*)0x00ECFB70, 1041);
	SendMessage(listWindow, WM_SETREDRAW, FALSE, 0);
	ThisStdCall(0x439630, thisptr, a2, a3);
	SendMessage(listWindow, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(listWindow, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
	return 0;
}


void __cdecl OnPopulateFloatingObjectsList(HWND listWindow, tList<TESForm>* list, unsigned __int8(__cdecl* filterFn)(TESForm*, int), int filterData)
{
	SendMessage(listWindow, WM_SETREDRAW, FALSE, 0);
	CdeclCall(0x47E410, listWindow, list, filterFn, filterData);
	SendMessage(listWindow, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(listWindow, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
}

void __fastcall sub_406C40(void* thiss, void* edx, HWND comboBox, TESForm* a3)
{
	SendMessage(comboBox, WM_SETREDRAW, FALSE, 0);
	ThisStdCall(0x406C40, thiss, comboBox, a3);
	WM_CLOSE;
	WM_DESTROY;
	SendMessage(comboBox, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(comboBox, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
}

_declspec(naked) void DialogueListViewBeginUI() {
	static const UInt32 retnAddr = 0x592EB4;
	static const UInt32 skipAddr = 0x592FB8;
	_asm
	{
		jne defer
		jmp skipAddr

	defer :
		push edi
		call BeginUIDefer
		pop edi
		jmp retnAddr
	}
}

_declspec(naked) void DialogueListViewEndUI() {
	static const UInt32 retnAddr = 0x592FB8;
	EndUIDefer();
	_asm
	{
		//	originalCode
		mov esi, dword ptr ds : [0xD23550] //SendMessageA
		jmp retnAddr
	}
}

void __stdcall hk_sub_59C7B0(HWND hWnd) {
	BeginUIDefer();
	StdCall(0x59C7B0, hWnd);
	EndUIDefer();
}

/*
It would be a lot faster with the BeginUIDefer calls, however it causes dirty edits in PNAM - Previous INFO and ANAM - Speaker
*/
void __fastcall hk_sub_59C950(char* thiss, void* dummyEDX, int a2, HWND hDlg) {
	SendMessage(hDlg, WM_SETREDRAW, FALSE, 0);
	ThisStdCall(0x59C950, thiss, a2, hDlg);
	SendMessage(hDlg, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(hDlg, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE);
}

int* __cdecl hk_sub_595410(HWND hDlg, int a2, char a3) {
	BeginUIDefer();
	int* res = CdeclCall<int*>(0x595410, hDlg, a2, a3);
	EndUIDefer();
	return res;
}

char* __fastcall hk_sub_56B270(int* thiss, void* dummyEDX, HWND hWnd) {
	BeginUIDefer();
	char* res = ThisStdCall<char*>(0x56B270, thiss, hWnd);
	EndUIDefer();
	return res;
}

/*
hook to speed up Select Topic dialog
*/
void __cdecl hk_sub_595800(HWND hDlg, int a2, int a3) {
	BeginUIDefer();
	CdeclCall(0x595800, hDlg, a2, a3);
	EndUIDefer();
}

void __cdecl hk_sub_47D910(HWND hWnd, char a2, char a3, char a4) {
	BeginUIDefer();
	CdeclCall(0x47D910, hWnd, a2, a3, a4);
	EndUIDefer();
}

void __cdecl hk_sub_47D330(HWND hWnd, char a2, char a3) {
	BeginUIDefer();
	CdeclCall(0x47D330, hWnd, a2, a3);
	EndUIDefer();
}

BOOL __fastcall hk_sub_49B260(int* thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	BOOL res = ThisStdCall<BOOL>(0x49B260, thiss, hDlg);
	EndUIDefer();
	return res;
}

BOOL __fastcall hk_sub_4979F0(void* thiss, void* dummyEDX, HWND hDlg, int a3) {
	BeginUIDefer();
	BOOL res = ThisStdCall<BOOL>(0x4979F0, thiss, hDlg, a3);
	EndUIDefer();
	return res;
}

void __fastcall hk_sub_4E3020(int* thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	ThisStdCall(0x4E3020, thiss, hDlg);
	EndUIDefer();
}

void __fastcall hk_sub_4E8A20(int* thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	ThisStdCall(0x4E8A20, thiss, hDlg);
	EndUIDefer();
}


void __cdecl hk_sub_47F7A0(HWND hWnd, char a2, char a3, char a4, int a5, int a6) {
	BeginUIDefer();
	CdeclCall(0x47F7A0, hWnd, a2, a3, a4, a5, a6);
	EndUIDefer();
}

void __cdecl hk_sub_47E0D0(HWND hWnd, char a2, int a3, char a4) {
	BeginUIDefer();
	CdeclCall(0x47E0D0, hWnd, a2, a3, a4);
	EndUIDefer();
}

void __fastcall hk_sub_666C10(int* thiss, void* dummyEDX, HWND hWnd, int a3) {
	BeginUIDefer();
	ThisStdCall(0x666C10, thiss, hWnd, a3);
	EndUIDefer();
}

void* __fastcall SearchAndReplaceBeginUI(void** boundObjectList, HWND hDlg)
{
	auto searchForDropdown = GetDlgItem(hDlg, 1439);
	SendMessage(searchForDropdown, WM_SETREDRAW, FALSE, 0);
	SendMessage(searchForDropdown, CB_INITSTORAGE, 30000, 600000); // pre-allocate the storage to the rounded vanilla item count and total string length

	auto replaceWithDropdown = GetDlgItem(hDlg, 1440);
	SendMessage(replaceWithDropdown, WM_SETREDRAW, FALSE, 0);
	SendMessage(replaceWithDropdown, CB_INITSTORAGE, 30000, 600000);
	return boundObjectList[1];
}

void* __fastcall SearchAndReplaceEndUI(void** selectedForms, HWND hDlg)
{
	SendMessage(GetDlgItem(hDlg, 1439), WM_SETREDRAW, TRUE, 0);
	SendMessage(GetDlgItem(hDlg, 1440), WM_SETREDRAW, TRUE, 0);
	return *selectedForms;
}

__HOOK SearchAndReplaceBeginUIHook()
{
	_asm
	{
		mov edx, ebp
		jmp SearchAndReplaceBeginUI
	}
}

__HOOK SearchAndReplaceEndUIHook()
{
	_asm
	{
		mov edx, ebp
		jmp SearchAndReplaceEndUI
	}
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

	// speed up worldspace list view
	WriteRelCall(0x47A652, UInt32(hk_sub_666C10));

	// speed up the search and replace dialog
	WriteRelCall(0x47D010, UInt32(SearchAndReplaceBeginUIHook));
	WriteRelCall(0x47D18B, UInt32(SearchAndReplaceEndUIHook));

	if (!config.bUISpeedHooks) return;

	WriteRelCall(0x59CCB3, UInt32(hk_sub_59C7B0)); // combo box item, required to fix multiple insertions tripping the assertion 
	WriteRelCall(0x409C08, UInt32(hk_sub_59C950));
	WriteRelCall(0x59957B, UInt32(hk_sub_59C950));
	WriteRelCall(0x59A728, UInt32(hk_sub_59C950));
	WriteRelCall(0x5A1952, UInt32(hk_sub_59C950));

	WriteRelCall(0x57E673, UInt32(hk_sub_595410));
	//	WriteRelCall(0x57E69A, UInt32(hk_sub_595410)); causes conversation next speaker to be blank
	WriteRelCall(0x57E6C1, UInt32(hk_sub_595410));

	WriteRelCall(0x57E6E8, UInt32(hk_sub_595410));
	WriteRelCall(0x57E70F, UInt32(hk_sub_595410));
	WriteRelCall(0x57E75D, UInt32(hk_sub_595410));
	WriteRelCall(0x57E736, UInt32(hk_sub_595410));


	WriteRelCall(0x59CC55, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x59CC70, UInt32(hk_sub_47F7A0));
	WriteRelCall(0x59CC8B, UInt32(hk_sub_47F7A0));

	// speed up select dialog topic
	WriteRelCall(0x597312, UInt32(hk_sub_595800));
	WriteRelCall(0x597504, UInt32(hk_sub_595800));
	WriteRelCall(0x59770E, UInt32(hk_sub_595800));
	WriteRelCall(0x5979EF, UInt32(hk_sub_595800));

	// speed up responses window by defering dropdown box rendering
	if (!config.bCacheComboboxes)
	{
		WriteRelCall(0x56CC12, UInt32(hk_sub_56B270));
		WriteRelCall(0x56CC52, UInt32(hk_sub_56B270));
	}

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
	WriteRelCall(0x49D1CA, UInt32(hk_sub_47F7A0));
	//WriteRelCall(0x49966C, UInt32(hk_sub_4979F0)); Use Weapon multi-combo box

	// speed up object palette
	WriteRelCall(0x40CF05, UInt32(hk_sub_47E0D0));

	// speed up the floating objects list
	WriteRelCall(0x4838C1, UInt32(OnPopulateFloatingObjectsList));

	// dialogue tree
	WriteRelCall(0x408A25, UInt32(sub_406C40));
	WriteRelCall(0x408A3C, UInt32(sub_406C40));
}