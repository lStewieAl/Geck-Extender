#pragma once
#pragma comment(lib, "libdeflate/libdeflatestatic.lib")

#include "Editor.h"
#include "resource.h"

#define GH_NAME				"ZeGaryHax"		// this is the string for IsPluginInstalled and GetPluginVersion (also shows in nvse.log)
#define GH_VERSION			0				// set this to 0 to enable log output from _DMESSAGE (useful for debug traces)

HMODULE ZeGaryHaxHandle;

IDebugLog		gLog("EditorWarnings.log");

struct z_stream_s
{
	const void *next_in;
	uint32_t avail_in;
	uint32_t total_in;
	void *next_out;
	uint32_t avail_out;
	uint32_t total_out;
	const char *msg;
	struct internal_state *state;
};

struct DialogOverrideData
{
    DLGPROC DialogFunc; // Original function pointer
    LPARAM Param;       // Original parameter
    bool IsDialog;      // True if it requires EndDialog()
};

std::recursive_mutex g_DialogMutex;
std::unordered_map<HWND, DialogOverrideData> g_DialogOverrides;
__declspec(thread) DialogOverrideData *DlgData;

PluginHandle g_pluginHandle = kPluginHandle_Invalid;
NVSEInterface* g_nvse = NULL;
DataHandler* g_data = NULL;
NVSEMessagingInterface* g_msg = NULL;
NVSECommandTableInterface* g_cmd = NULL;;
NVSEScriptInterface* g_script = NULL;
NVSEStringVarInterface* g_string = NULL;
NVSEArrayVarInterface* g_array = NULL;
NVSESerializationInterface* g_serial = NULL;
NVSEConsoleInterface* g_console = NULL;

char* nvseMSG[20] =
{
	"PostLoad",
	"ExitGame",
	"ExitToMainMenu",
	"LoadGame",
	"SaveGame",
	"Precompile",
	"PreLoadGame",
	"QQQ",
	"PostLoadGame",
	"PostPostLoad",
	"ScriptError",
	"DeleteGame",
	"RenameGame",
	"RenameNewGame",
	"NewGame",
	"DeleteGameName",
	"RenameGameName",
	"RenameNewGameName",
};

int bPatchScriptEditorFont = 0;
int bHighResLandscapeLOD = 0;
int bListEditFix = 0;
int bVersionControlMode = 0;
int bFastExit = 0;
int bIgnoreNAMFiles = 0;
int bEnableSpellChecker = 1;
int bAutoScroll = 1;
int bRenderWindowUncap = 1;
int bPreviewWindowUncap = 1;
int bRemoveStatusBarLoadingText = 1;
int bPlaySoundEndOfLoading = 1;
int bNoDXSoundCaptureErrorPopup = 0;
int bNoPreviewWindowAutoFocus = 1;
int bNoLODMeshMessage = 0;
int bSwapRenderCYKeys = 0;

char filename[MAX_PATH];
static const char *geckwikiurl = "https://geckwiki.com/index.php/";
static const char *geckwikiscriptingurl = "https://geckwiki.com/index.php/Category:Scripting";
static const char *geckwikicommandsurl = "https://geckwiki.com/index.php/Category:Commands";
static const char *geckwikifunctionsurl = "https://geckwiki.com/index.php/Category:Functions";

// From NVSE Hooks_Editor.cpp
// Patch script window font

static HANDLE	fontHandle;
static LOGFONT	fontInfo;

static const LOGFONT kConsolas9 =
{
	13,			// lfHeight
	0,			// lfWidth
	0,			// lfEscapement
	0,			// lfOrientation
	500,		// lfWeight
	0,			// lfItalic
	0,			// lfUnderline
	0,			// lfStrikeOut
	0,			// lfCharSet
	3,			// lfOutPrecision
	2,			// lfClipPrecision
	1,			// lfQuality
	49,			// lfPitchAndFamily
	"Consolas"	// lfFaceName[LF_FACESIZE]
};

static void DoModScriptWindow(HWND wnd)
{
	SendMessage(wnd, EM_EXLIMITTEXT, 0, 0x00FFFFFF);

	// try something nice, otherwise fall back on SYSTEM_FIXED_FONT
	fontHandle = CreateFontIndirect(&kConsolas9);
	if (fontHandle)
	{
		fontInfo = kConsolas9;
	}
	else
	{
		fontHandle = GetStockObject(SYSTEM_FIXED_FONT);
		GetObject(fontHandle, sizeof(fontInfo), &fontInfo);
	}
	SendMessage(wnd, EM_SETTEXTMODE, TM_PLAINTEXT, 0);
	SendMessage(wnd, WM_SETFONT, (WPARAM)fontHandle, 1);
	SendMessage(wnd, EM_SETMODIFY, 1, 0);

	// one tab stop every 16 dialog units
	UInt32	tabStopSize = 16;
	SendMessage(wnd, EM_SETTABSTOPS, 1, (LPARAM)&tabStopSize);
	DeleteObject(fontHandle);
}

static __declspec(naked) void ModScriptWindow()
{
	__asm
	{
		call	CreateWindowExA
		pushad
		push	eax
		call	DoModScriptWindow
		add		esp, 4
		popad
		push	0x0B0001
		push	0x005C4336
		ret
	}
}

static UInt32 pModScriptWindow = (UInt32)&ModScriptWindow;

static void FixEditorFont(void)
{
	// Right after call d:CreateWindowA after aRichedit20a
	UInt32	basePatchAddr = 0x005C432B;

	WriteRelJump(basePatchAddr, pModScriptWindow);
	SafeWrite8(basePatchAddr + 5, 0x90);
}

static __declspec(naked) void FixMultiBounds()
{
	__asm
	{
		cmp		eax, 0x7F7FFFFF
		je		AhhGary
		mov		ecx, dword ptr ds : [eax + 0x0C]
		mov		dword ptr ds : [ecx + 0x08], 0x00000000
	AhhGary:
		ret
	}
}

//	make sure geckcustom.ini is written if bFastExit is enabled - credit to jazzisparis
bool GetINIExists()
{
	char iniPath[MAX_PATH];
	strcpy(iniPath, (const char*)0x00F2CC80);
	strcat(iniPath, "GECKCustom.ini");
	DWORD attr = GetFileAttributes(iniPath);
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

__declspec(naked) void FastExitHook()
{
	static const UInt32 retnAddr = 0x004CC540;

	__asm
	{
		call	GetINIExists
		test	al, al
		jnz		GGary
	Gaarrry:
		mov		ecx, edi
		jmp		retnAddr
	GGary:
		mov		eax, bFastExit
		test	eax, eax
		jz		Gaarrry
		push	0
		call	GetCurrentProcess
		push	eax
		call	TerminateProcess
	}
}

//	patch splash screen - credit to roy and nukem
static __declspec(naked) void hk_SplashScreen()
{
	static const UInt32 kRetnAddr = 0x004463D3;
	HWND hWnd;

	__asm
	{
		call	dword ptr ds:[0x00D234CC]		//	CreateDialogParam
		push	ebp
		mov		ebp, esp
		push	eax
		mov		hWnd, eax
	}
	SendMessage(GetDlgItem((HWND)hWnd, 1962), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_SPLASHBITMAP), IMAGE_BITMAP, 0, 0, 0));
	__asm
	{
		pop		eax
		pop		ebp
		jmp		kRetnAddr
	}
}

//	patch about dialog - credit to roy and nukem
static __declspec(naked) void hk_AboutDialog()
{
	static const UInt32 kRetnAddr = 0x0044197C;
	HWND hWnd;

	__asm
	{
		call	dword ptr ds:[0x00D234CC]		//	CreateDialogParam
		push	ebp
		mov		ebp, esp
		push	eax
		mov		hWnd, eax
	}
	SendMessage(GetDlgItem((HWND)hWnd, 1963), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_SPLASHBITMAP), IMAGE_BITMAP, 0, 0, 0));
	__asm
	{
		pop		eax
		pop		ebp
		jmp		kRetnAddr
	}
}

//	fix destruction dialog close button - credit to roy
BOOL WINAPI hk_DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (hWnd && uMsg == WM_CLOSE)
	{
		EndDialog(hWnd, 0);
		DestroyWindow(hWnd);
		return TRUE;
	}
		__asm
		{
			pop esi
			pop ebp
			push 0x004E5E30
			ret
		}
}

//	fix handle memory leak - credit to nukem
INT_PTR WINAPI DialogFuncOverride(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DLGPROC proc = nullptr;

	g_DialogMutex.lock();
	{
		auto itr = g_DialogOverrides.find(hwndDlg);
		if (itr != g_DialogOverrides.end())
			proc = itr->second.DialogFunc;

		//	if (is new entry)
		if (!proc)
		{
			g_DialogOverrides[hwndDlg] = *DlgData;
			proc = DlgData->DialogFunc;

			delete DlgData;
			DlgData = nullptr;
		}

		//	Purge old entries every now and then
		if (g_DialogOverrides.size() >= 50)
		{
			for (auto itr = g_DialogOverrides.begin(); itr != g_DialogOverrides.end();)
			{
				if (itr->first == hwndDlg || IsWindow(itr->first))
				{
					itr++;
					continue;
				}

				itr = g_DialogOverrides.erase(itr);
			}
		}
	}
	g_DialogMutex.unlock();

	return proc(hwndDlg, uMsg, wParam, lParam);
}

HWND WINAPI hk_CreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	//	EndDialog MUST NOT be used
	DialogOverrideData *data = new DialogOverrideData;
	data->DialogFunc = lpDialogFunc;
	data->Param = dwInitParam;
	data->IsDialog = false;

	DlgData = data;
	return CreateDialogParamA(hInstance, lpTemplateName, hWndParent, DialogFuncOverride, dwInitParam);
}

INT_PTR WINAPI hk_DialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	//	EndDialog MUST be used
	DialogOverrideData *data = new DialogOverrideData;
	data->DialogFunc = lpDialogFunc;
	data->Param = dwInitParam;
	data->IsDialog = true;

	DlgData = data;
	return DialogBoxParamA(hInstance, lpTemplateName, hWndParent, DialogFuncOverride, dwInitParam);
}

BOOL WINAPI hk_EndDialog(HWND hDlg, INT_PTR nResult)
{
	std::lock_guard<std::recursive_mutex> lock(g_DialogMutex);

	//	Fix for the CK calling EndDialog on a CreateDialogParamA window
	auto itr = g_DialogOverrides.find(hDlg);
	if (itr != g_DialogOverrides.end() && !itr->second.IsDialog)
	{
		DestroyWindow(hDlg);
		return TRUE;
	}

	return EndDialog(hDlg, nResult);
}

LRESULT WINAPI hk_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (hWnd && Msg == WM_DESTROY)
	{
		std::lock_guard<std::recursive_mutex> lock(g_DialogMutex);

		// If this is a dialog, we can't call DestroyWindow on it
		auto itr = g_DialogOverrides.find(hWnd);
		if (itr != g_DialogOverrides.end())
		{
			if (!itr->second.IsDialog)
				DestroyWindow(hWnd);
		}

		return 0;
	}

	return SendMessageA(hWnd, Msg, wParam, lParam);
}

void __stdcall hk_sub_4A1C10(HWND ListViewControl)
{
	((void(__stdcall *)(HWND))(0x004A1C10))(ListViewControl);

	SendMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 0, LVSCW_AUTOSIZE_USEHEADER);
	SendMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 1, LVSCW_AUTOSIZE_USEHEADER);
	SendMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 2, LVSCW_AUTOSIZE_USEHEADER);
	SendMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 3, LVSCW_AUTOSIZE_USEHEADER);
	SendMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 4, LVSCW_AUTOSIZE_USEHEADER);
	SendMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 5, LVSCW_AUTOSIZE_USEHEADER);
}

//	fix crash with esm as active file - credit to hlp
static __declspec(naked) void hk_addr_42CD31()
{
	__asm
	{
		test ecx, ecx
		jne HahaGary
		xor al, al
		mov dword ptr ss : [esp], 0x0042CD6D
		ret

	HahaGary :
		mov esi, dword ptr ds : [ecx + 0x110]
		mov edx, dword ptr ds : [eax]
		ret
	}
}

//	patch ONAM count - credit to hlp
static __declspec(naked) void hk_addr_4E26BA()
{
	__asm
	{
		mov dword ptr ds : [esi + 0x434], eax
		mov edx, 0x04
		mul edx
		seto cl
		ret
	}
}

//	Fix Rock-It Launcher crash - credit to jazzisparis
TESForm* (*GetFormByID)(UInt32 formID) = (TESForm* (*)(UInt32))0x004F9620;

bool __fastcall GetIsRIL(TESForm *form)
{
	static UInt32 RIL_FormID = 0;
	if (!RIL_FormID)
	{
		RIL_FormID = 0xFFFFFFFF;
		DataHandler *dataHandler = *(DataHandler**)0x00ED3B0C;
		if (dataHandler)
		{
			for (tList<ModInfo>::Iterator infoIter = dataHandler->modList.modInfoList.Begin(); !infoIter.End(); ++infoIter)
			{
				if (_stricmp(infoIter->name, "Fallout3.esm")) continue;
				RIL_FormID = (infoIter->modIndex << 0x18) | 0x434B;
				break;
			}
		}
	}
	return form->refID == RIL_FormID;
}

__declspec(naked) void CheckIsRILHook()
{
	static const UInt32 kRetnAddr = 0x005B8FFD;

	__asm
	{
		mov		ecx, esi
		call	GetIsRIL
		test	al, al
		jz		notRIL
		retn
	notRIL:
		push	edi
		mov		edi, [esp + 0xC]
		jmp		kRetnAddr
	}
}

//	enable/disable spell checker - credit to roy
__declspec(naked) void hk_SpellCheck()
{
	static const UInt32 kRetnAddrYes = 0x0041B613;
	static const UInt32 kRetnAddrNo = 0x0041B61A;

	__asm
	{
		cmp		bEnableSpellChecker, 0
		je		AwwGary
		call	dword ptr ds:[0x00D23548]	//	GetDlgItem
		jmp		kRetnAddrYes
	AwwGary:
		add		esp, 0x0C
		jmp		kRetnAddrNo
	}

}

//	fix list view lag when changing cells in the render window - credit to nukem
void hk_UpdateCellViewListView(HWND ControlHandle, void *a2, void *a3)
{
	SendMessage(ControlHandle, WM_SETREDRAW, FALSE, 0);
	((void(__cdecl *)(HWND ControlHandle, void *a2, void *a3))(0x0042DE00))(ControlHandle, a2, a3);
	SendMessage(ControlHandle, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(ControlHandle, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
}

// fix object list caching for speed - credit to nukem
LRESULT __fastcall hk_ObjectListViewListView(DWORD *thisptr, void *ignorethisthing, char *a2, char *a3)
{
	HWND listWindow = GetDlgItem(*(HWND *)0x00ECFB70, 1041);
	SendMessage(listWindow, WM_SETREDRAW, FALSE, 0);
	((void(__thiscall *)(DWORD *, char *, char *))(0x439630))(thisptr, a2, a3);
	SendMessage(listWindow, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(listWindow, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
	return 0;
}

// make search and replace window stay open unless explicitly close - credit to StewieA
void __declspec(naked) hk_SearchDestroyHook()
{
	static const UINT32 kRetnAddr = 0x0047CE8C;

	_asm
	{
		call dword ptr ds:[0x00D2353C]
		jmp kRetnAddr
	}
}

int hk_inflateInit(z_stream_s *Stream, const char *Version, int Mode)
{
	// Force inflateEnd to error out and skip frees
	Stream->state = nullptr;

	return 0;
}

int hk_inflate(z_stream_s *Stream, int Flush)
{
	size_t outBytes = 0;
	libdeflate_decompressor *decompressor = libdeflate_alloc_decompressor();

	libdeflate_result result = libdeflate_zlib_decompress(decompressor, Stream->next_in, Stream->avail_in, Stream->next_out, Stream->avail_out, &outBytes);
	libdeflate_free_decompressor(decompressor);

	if (result == LIBDEFLATE_SUCCESS)
	{

		Stream->total_in = Stream->avail_in;
		Stream->total_out = (uint32_t)outBytes;

		return 1;
	}

	if (result == LIBDEFLATE_INSUFFICIENT_SPACE)
		return -5;

	return -2;
}

_declspec(naked) void hk_PreviewWindowCheckForeground() {
	static const UInt32 returnAddr = 0x48CACF;

	_asm {
		/* the hWnd was already pushed in esi */
		call dword ptr ds : [0xD2347C] //GetParent

		push ebp
		mov ebp, eax
		call dword ptr ds : [0xD233A0] // GetActiveWindow
		cmp eax, ebp
		pop ebp

		jne skipSetFocus
		push esi
		call dword ptr ds : [0xD234DC] //SetFocus

	skipSetFocus :
		jmp returnAddr
	}
}

char* recompileAllWarning = { "Are you sure you want to recompile every script in every plugin?\nYou should never need to do this." };
_declspec(naked) void RecompileAllWarningScriptHook() {
	static const UInt32 retnAddr = 0x5C498A;
	_asm {
		push 0x104 // change default button to No
		push 0xD2FA78
		push recompileAllWarning
		jmp retnAddr
	}
}
_declspec(naked) void RecompileAllWarningMainHook() {
	static const UInt32 retnAddr = 0x4442D3;
	_asm {
		push 0x104 // change default button to No
		push 0xD2FA78
		push recompileAllWarning
		jmp retnAddr
	}
}

_declspec(naked) void DialogueListViewBeginUI() {
	static const UInt32 retnAddr = 0x592EB4;
	static const UInt32 skipAddr = 0x592FB8;
	_asm {
		jne defer
		jmp skipAddr
		defer:
		push edi
	}
	BeginUIDefer();
	_asm {
		pop edi
	originalCode:
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
	((void(__stdcall *)(HWND window))(0x59C7B0))(hWnd);
	EndUIDefer();
}


/*
It would be a lot faster with the BeginUIDefer calls, however it causes dirty edits in PNAM - Previous INFO and ANAM - Speaker
*/
void __fastcall hk_sub_59C950(char* thiss, void* dummyEDX, int a2, HWND hDlg) {
//	BeginUIDefer();
	SendMessage(hDlg, WM_SETREDRAW, FALSE, 0);
	((char(__thiscall *)(char* str, int a2, HWND wnd))(0x59C950))(thiss, a2, hDlg);
	SendMessage(hDlg, WM_SETREDRAW, TRUE, 0);
	RedrawWindow(hDlg, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE);
//	EndUIDefer();
}

char *__fastcall hk_sub_56B270(int* thiss, void* dummyEDX, HWND hWnd) {
	BeginUIDefer();
	char* res = ((char*(__thiscall *)(int* something, HWND wnd))(0x56B270))(thiss, hWnd);
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
	((void (__cdecl *)(HWND hWnd, char a2, char a3))(0x47D330))(hWnd, a2, a3);
	EndUIDefer();
}

BOOL __fastcall hk_sub_49B260(int *thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	BOOL res = ((BOOL (__thiscall *)(int *thiss, HWND hDlg))(0x49B260))(thiss, hDlg);
	EndUIDefer();
	return res;
}

BOOL __fastcall hk_sub_4979F0(void *thiss, void* dummyEDX, HWND hDlg, int a3) {
	BeginUIDefer();
	BOOL res = ((BOOL (__thiscall *)(void *thiss, HWND hDlg, int a3))(0x4979F0))(thiss, hDlg, a3);
	EndUIDefer();
	return res;
}

void __fastcall hk_sub_4E3020(int *thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	((void (__thiscall *)(int *thiss, HWND hDlg))(0x4E3020))(thiss, hDlg);
	EndUIDefer();
}

void __fastcall hk_sub_4E8A20(int *thiss, void* dummyEDX, HWND hDlg) {
	BeginUIDefer();
	((void (__thiscall *)(int* thiss, HWND hDlg))(0x4E8A20))(thiss, hDlg);
	EndUIDefer();
}

void doKonami(int);

/* small konami easter egg */
BOOL __stdcall hk_LoadESPESMCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NOTIFY) {
		LPNMHDR notification = (LPNMHDR)lParam;
		if (notification->code == LVN_KEYDOWN) {
			LPNMLVKEYDOWN pnkd = (LPNMLVKEYDOWN)lParam;
			doKonami(pnkd->wVKey);
		}
	}

	return ((BOOL(__stdcall *)(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam))(0x432A80))(hDlg, msg, wParam, lParam);
}
