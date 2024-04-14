#pragma once
#pragma comment(lib, "libdeflate/libdeflatestatic.lib")
#include "GECKUtility.h"
#include "Editor.h"
#include "resource.h"
#include <filesystem>
#include <unordered_set>
#include <functional>

#include "NiNodes.h"
#include "NiObjects.h"
#include "GameScript.h"
#include "GameSettings.h"
#include "FormSearch.h"
#include "Settings.h"
#include "NavMeshPickPreventer.h"

#define GH_NAME				"ZeGaryHax"		// this is the string for IsPluginInstalled and GetPluginVersion (also shows in nvse.log)
#define GH_VERSION			0.41
#define RES_HACKER_ADDR_TO_ACTUAL 0x4CF800

HMODULE ZeGaryHaxHandle;
extern HWND g_trackBarHwnd;
extern HWND g_timeOfDayTextHwnd;
extern HWND g_allowCellWindowLoadsButtonHwnd;
IDebugLog	gLog;

BOOL __stdcall HavokPreviewCallback(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
extern "C" IMAGE_DOS_HEADER __ImageBase;

struct z_stream_s
{
	const void* next_in;
	uint32_t avail_in;
	uint32_t total_in;
	void* next_out;
	uint32_t avail_out;
	uint32_t total_out;
	const char* msg;
	struct internal_state* state;
};

struct DialogOverrideData
{
	DLGPROC DialogFunc; // Original function pointer
	LPARAM Param;       // Original parameter
	bool IsDialog;      // True if it requires EndDialog()
};

std::recursive_mutex g_DialogMutex;
std::unordered_map<HWND, DialogOverrideData> g_DialogOverrides;
__declspec(thread) DialogOverrideData* DlgData;

const char* nvseMSG[20] =
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

static const char* geckwikiurl = "https://geckwiki.com/index.php/";
static const char* geckwikiscriptingurl = "https://geckwiki.com/index.php/Category:Scripting";
static const char* geckwikicommandsurl = "https://geckwiki.com/index.php/Category:Commands";
static const char* geckwikifunctionsurl = "https://geckwiki.com/index.php/Category:Functions";

#define ID_CMB_IDLE_SPEAKER 2170
#define ID_CMB_IDLE_LISTENER 2173
#define ID_CMB_SPEAKER 2564

int cb_ids[3] = { ID_CMB_IDLE_SPEAKER, ID_CMB_IDLE_LISTENER, ID_CMB_SPEAKER };
bool cb_filled[_countof(cb_ids)] = { };
bool cb_reset[_countof(cb_ids)] = { };

// From NVSE Hooks_Editor.cpp
// Patch script window font

static HANDLE	fontHandle;
static LOGFONT	fontInfo;

static LOGFONT editorFont =
{
	13,			// lfHeight
	0,			// lfWidth
	0,			// lfEscapement
	0,			// lfOrientation
	FW_MEDIUM,		// lfWeight
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

LPLOGFONT GetEditorFont()
{
	return &editorFont;
}

void MoveDlgItem(HWND hWnd, int ID, int deltaX, int deltaY)
{
	RECT rect;
	POINT point;
	HWND element = GetDlgItem(hWnd, ID);
	GetWindowRect(element, &rect);
	point.x = rect.left;
	point.y = rect.top;
	ScreenToClient(hWnd, &point);
	SetWindowPos(element, NULL, point.x + deltaX, point.y + deltaY, NULL, NULL, SWP_NOSIZE);
}

static void DoModScriptWindow(HWND wnd)
{
	SendMessage(wnd, EM_EXLIMITTEXT, 0, 0x00FFFFFF);

	GetPrivateProfileStringA("Script", "Font", "Consolas", editorFont.lfFaceName, 31, IniPath);
	editorFont.lfHeight = GetOrCreateINIInt("Script", "FontSize", 13, IniPath);
	editorFont.lfWeight = GetOrCreateINIInt("Script", "FontWeight", FW_MEDIUM, IniPath);

	// try something nice, otherwise fall back on SYSTEM_FIXED_FONT
	fontHandle = CreateFontIndirect(&editorFont);
	if (fontHandle)
	{
		fontInfo = editorFont;
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
	SendMessage(wnd, EM_SETTABSTOPS, 1, (LPARAM)& tabStopSize);
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

static void FixEditorFont(void)
{
	// Right after call d:CreateWindowA after aRichedit20a
	UInt32	basePatchAddr = 0x005C432B;

	WriteRelJump(basePatchAddr, UInt32(ModScriptWindow));
	SafeWrite8(basePatchAddr + 5, 0x90);
}

static __declspec(naked) void FixMultiBounds()
{
	__asm
	{
		cmp		eax, 0x7F7FFFFF
		je		AhhGary
		mov		ecx, dword ptr ds : [eax + 0x0C]
		mov		dword ptr ds : [ecx + 0x08] , 0x00000000
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

//	fix destruction dialog close button - credit to roy
BOOL WINAPI hk_DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE)
	{
		EndDialog(hWnd, 0);
		DestroyWindow(hWnd);
		return TRUE;
	}
	return StdCall<LRESULT>(0x004E5E30, hWnd, uMsg, wParam, lParam);
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

	// Override certain default dialogs to use this DLL's resources and callbacks
	switch ((uintptr_t)lpTemplateName)
	{
	case IDD_HAVOK_PREVIEW_DIALOG:// "Havok Preview Window"
		hInstance = (HINSTANCE)& __ImageBase;
		lpDialogFunc = HavokPreviewCallback;
		break;
	}

	//	EndDialog MUST NOT be used
	DialogOverrideData* data = new DialogOverrideData;
	data->DialogFunc = lpDialogFunc;
	data->Param = dwInitParam;
	data->IsDialog = false;

	DlgData = data;

	return CreateDialogParamA(hInstance, lpTemplateName, hWndParent, DialogFuncOverride, dwInitParam);
}

INT_PTR WINAPI hk_DialogBoxParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	//	EndDialog MUST be used
	DialogOverrideData* data = new DialogOverrideData;
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

__declspec(naked) void hk_QuestWindowLoad()
{
	__asm
	{
		mov eax, dword ptr ds : [0x00ECC3C8]
		mov cb_reset[0], 1
		mov cb_reset[1], 1
		mov cb_reset[2], 1
		ret
	}
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

	if (Msg == CB_ADDSTRING || Msg == CB_RESETCONTENT)
	{
		if (config.bCacheComboboxes)
		{
			int id = GetDlgCtrlID(hWnd);
			for (int i = 0; i < _countof(cb_ids); i++)
			{
				if (id == cb_ids[i])
				{
					if (Msg == CB_ADDSTRING)
					{
						if (cb_filled[i])
						{
							return CB_ERR;
						}
					}
					else if (Msg == CB_RESETCONTENT)
					{
						if (!cb_reset[i])
						{
							cb_filled[i] = !!SendMessageA(hWnd, CB_GETCOUNT, NULL, NULL);
							return CB_ERR;
						}
						cb_filled[i] = false;
						cb_reset[i] = false;
					}
					break;
				}
			}
		}
	}
	return SendMessageA(hWnd, Msg, wParam, lParam);
}

HWND __stdcall SplashScreenHook(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	HWND hWnd = hk_CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	SendMessage(GetDlgItem((HWND)hWnd, 1962), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_SPLASHBITMAP), IMAGE_BITMAP, 0, 0, 0));
	return hWnd;
}

//	patch about dialog - credit to roy and nukem
HWND __stdcall AboutDialogHook(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	HWND hWnd = hk_CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	SendMessage(GetDlgItem((HWND)hWnd, 1963), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadImage(ZeGaryHaxHandle, MAKEINTRESOURCE(IDB_SPLASHBITMAP), IMAGE_BITMAP, 0, 0, 0));
	return hWnd;
}

void __stdcall ConditionDataDialog_SetupColumnHeaders(HWND ListViewControl)
{
	StdCall(0x4A1C10, ListViewControl);

	// set the list view headers to resize automatically
	PostMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 0, LVSCW_AUTOSIZE_USEHEADER);
	PostMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 1, LVSCW_AUTOSIZE_USEHEADER);
	PostMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 2, LVSCW_AUTOSIZE_USEHEADER);
	PostMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 3, LVSCW_AUTOSIZE_USEHEADER);
	PostMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 4, LVSCW_AUTOSIZE_USEHEADER);
	PostMessageA(ListViewControl, LVM_SETCOLUMNWIDTH, 5, LVSCW_AUTOSIZE_USEHEADER);
}

//	fix crash with esm as active file - credit to hlp
static __declspec(naked) void hk_addr_42CD31()
{
	__asm
	{
		test ecx, ecx
		jne HahaGary
		xor al, al
		mov dword ptr ss : [esp] , 0x0042CD6D
		ret

	HahaGary:
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
		mov dword ptr ds : [esi + 0x434] , eax
		mov edx, 0x04
		mul edx
		seto cl
		ret
	}
}

//	Fix Rock-It Launcher crash - credit to jazzisparis
TESForm* (*GetFormByID)(UInt32 formID) = (TESForm * (*)(UInt32))0x004F9620;

bool __fastcall GetIsRIL(TESForm* form)
{
	static UInt32 RIL_FormID = 0;
	if (!RIL_FormID)
	{
		RIL_FormID = 0xFFFFFFFF;
		DataHandler* dataHandler = *(DataHandler**)0x00ED3B0C;
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
		cmp		config.bEnableSpellChecker, 0
		je		AwwGary
		call	dword ptr ds : [0x00D23548]	//	GetDlgItem
		jmp		kRetnAddrYes
	AwwGary:
		add		esp, 0x0C
		jmp		kRetnAddrNo
	}

}

// make search and replace window stay open unless explicitly closed - credit to StewieA
void __declspec(naked) hk_SearchDestroyHook()
{
	_asm
	{
		push 0x47CE8C
		jmp dword ptr ds : [0x00D2353C] // MessageBoxA
	}
}

BOOL IsWindowHidden(HWND hWnd)
{
	return !(GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE);
}

void RestoreCachedSearchAndReplaceWindow(HWND hDlg)
{
	auto searchForDropdown = GetDlgItem(hDlg, 1439);
	auto replaceWithDropdown = GetDlgItem(hDlg, 1440);

	int index = -1;
	auto selected = RenderWindow::SelectedData::GetSelected();
	if (selected->selectedForms)
	{
		if (auto ref = CdeclCall<TESObjectREFR*>(0xC5D114, selected->selectedForms->ref, 0, 0xE8C57C, 0xE8E3E4, 0)) // DYNAMIC_CAST(TESForm, TESObjectREFR)
		{
			if (ref->baseForm)
			{
				index = SendMessageA(searchForDropdown, CB_SELECTSTRING, 0xFFFFFFFF, (LPARAM)ref->baseForm->GetEditorID());
			}
		}
	}

	if (index == -1)
	{
		SendMessageA(searchForDropdown, CB_SETCURSEL, 0, 0);
		SendMessageA(replaceWithDropdown, CB_SETCURSEL, 1u, 0);
	}
	else
	{
		SendMessageA(replaceWithDropdown, CB_SETCURSEL, index + 1, 0);
	}

	ShowWindow(hDlg, SW_SHOW);
}

HWND cachedSearchAndReplaceWindow;
void __stdcall OnCreateSearchAndReplaceWindow(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	if (cachedSearchAndReplaceWindow && IsWindow(cachedSearchAndReplaceWindow))
	{
		if (IsWindowHidden(cachedSearchAndReplaceWindow))
		{
			RestoreCachedSearchAndReplaceWindow(cachedSearchAndReplaceWindow);
		}
		else
		{
			// create a new dialog (case where there's multiple search dialogs opened, and set the last opened as the cached window)
			cachedSearchAndReplaceWindow = CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
		}
	}
	else
	{
		cachedSearchAndReplaceWindow = CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	}
}

void __stdcall OnDestroySearchAndReplaceWindow(HWND hWnd)
{
	if (hWnd == cachedSearchAndReplaceWindow)
	{
		ShowWindow(hWnd, SW_HIDE);
	}
	else
	{
		DestroyWindow(hWnd);
	}
}

int hk_inflateInit(z_stream_s* Stream, const char* Version, int Mode)
{
	// Force inflateEnd to error out and skip frees
	Stream->state = nullptr;

	return 0;
}

int hk_inflate(z_stream_s* Stream, int Flush)
{
	size_t outBytes = 0;
	libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();

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

	skipSetFocus:
		jmp returnAddr
	}
}

const char* recompileAllWarning = { "Are you sure you want to recompile every script in every plugin?\nYou should never need to do this." };
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

void SetNthFileEnabled(HWND* pListView, int n, bool bEnabled)
{
	if (n < 0)
	{
		return;
	}

	if (auto file = DataHandler::GetSingleton()->GetNthFile(n))
	{
		file->SetEnabled(bEnabled);
		if (!file->IsEnabled())
		{
			file->SetActive(false);

			if (file == *(ModInfo**)0xECF5D0)
			{
				*(ModInfo**)0xECF5D0 = 0;
			}
		}

		SendMessageA(*pListView, LVM_UPDATE, n, 0);
	}
}

bool AreAllFilesEnabled(HWND* pListView)
{
	unsigned int index = -1;
	do
	{
		index = SendMessageA(*pListView, LVM_GETNEXTITEM, index, LVIS_SELECTED);

		if (index >= 0)
		{
			if (auto file = DataHandler::GetSingleton()->GetNthFile(index))
			{
				if (!file->IsEnabled())
				{
					return false;
				}
			}
		}
	} while (index != -1);
	return true;
}

void ToggleSelectedFiles(HWND* pListView)
{
	bool bEnabled = AreAllFilesEnabled(pListView);
	unsigned int index = -1;
	do
	{
		index = SendMessageA(*pListView, LVM_GETNEXTITEM, index, LVIS_SELECTED);

		if (index != -1)
		{
			SetNthFileEnabled(pListView, index, !bEnabled);
		}
	} while (index != -1);
}

void doKonami(int);
BOOL __stdcall hk_LoadESPESMCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static RECT WindowSize;
	if (msg == WM_NOTIFY && ((LPNMHDR)lParam)->code == LVN_KEYDOWN)
	{
		auto key = ((LPNMLVKEYDOWN)lParam)->wVKey;
		/* small konami easter egg */
		doKonami(key);

		HWND* pListView = *(HWND**)0xECF5C8;
		
		bool isControlHeld = (GetKeyState(VK_CONTROL) & 0x8000);
		if (isControlHeld)
		{
			if (key == 'A')
			{
				SelectAllItemsInListView(*pListView);
				return true;
			}
		}
		else
		{
			if (key == VK_SPACE)
			{
				ToggleSelectedFiles(pListView);
				return true;
			}
		}
	}
	return StdCall<LRESULT>(0x432A80, hDlg, msg, wParam, lParam);
}

void doKonami(int key) {
	static int konamiStage = 0;
	switch (konamiStage) {
	case 0:
		(key == VK_UP) ? konamiStage++ : konamiStage = 0;
		break;
	case 1:
		(key == VK_UP) ? konamiStage++ : konamiStage = 0;
		break;
	case 2:
		if (key == VK_DOWN) konamiStage++;
		else if (key != VK_UP) konamiStage = 0;
		break;
	case 3:
		(key == VK_DOWN) ? konamiStage++ : konamiStage = 0;
		break;
	case 4:
		(key == VK_LEFT) ? konamiStage++ : konamiStage = 0;
		break;
	case 5:
		(key == VK_RIGHT) ? konamiStage++ : konamiStage = 0;
		break;
	case 6:
		(key == VK_LEFT) ? konamiStage++ : konamiStage = 0;
		break;
	case 7:
		(key == VK_RIGHT) ? konamiStage++ : konamiStage = 0;
		break;
	case 8:
		(key == 'B') ? konamiStage++ : konamiStage = 0;
		break;
	case 9:
		if (key == 'A') {
			Console_Print("Konami!");
		}
		konamiStage = 0;
		break;
	}
	/* handles the case where up is pressed in the middle of the sequence */
	if (konamiStage == 0 && key == VK_UP) konamiStage++;
}

BOOL __stdcall hk_SearchAndReplaceCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	return StdCall<LRESULT>(0x47C990, hDlg, msg, wParam, lParam);
}

_declspec(naked) void EndLoadingHook() {
	PlaySound("MouseClick", NULL, SND_ASYNC);
	_asm
	{
		//	originalCode
		add esp, 8
		pop esi
		retn
	}
}

// hooks before movement speed is determined for flycam mode
static int lastFlycamTime = 0;
_declspec(naked) void FlycamMovementSpeedMultiplierHook() {
	_asm
	{
		pushad
	}
	static const UInt32 retnAddr = 0x455D17;
	*(float*)(0xED12C0) = config.fFlycamNormalMovementSpeed;

	if (GetAsyncKeyState(VK_SHIFT) < 0) {
		*(float*)(0xED12C0) *= config.fFlycamShiftMovementSpeed;
	}

	if (GetAsyncKeyState(VK_MENU) < 0) {
		*(float*)(0xED12C0) *= config.fFlycamAltMovementSpeed;
	}

	/* fix flycam speed being dependent on framerate by slowing down movement if framerate is greater than 30fps (33ms/frame)*/
	if (GetTickCount() - lastFlycamTime < 33) {
		*(float*)(0xED12C0) *= (GetTickCount() - lastFlycamTime) / 33.0;
	}

	lastFlycamTime = GetTickCount();

	_asm
	{
		popad
		//	originalCode
		mov     eax, dword ptr ds : [0xF1FBF4]
		jmp retnAddr
	}
}

// edi       : difference in x pos, 
// esp+0x324 : difference in y pos
// Hook to make flycam rotation speed based on x/y position difference rather than 1.0 for any difference
_declspec(naked) void FlycamRotationSpeedMultiplierHook() {
	static const UInt32 retnAddr = 0x45D3F8;
	_asm {
		mov dword ptr ds : [0x00ED140C] , ebx

		push edi
		fild dword ptr ss : [esp]
		add esp, 4

		fmul config.fFlycamRotationSpeed
		fstp dword ptr ss : [esp + 0xF0]

		fild dword ptr ss : [esp + 0x324]
		fmul config.fFlycamRotationSpeed
		fstp dword ptr ss : [esp + 0x18]
		jmp retnAddr
	}
}

_declspec(naked) void ReferenceBatchActionDoButtonHook() {
	static const UInt32 retnAddr = 0x411CD4;
	_asm {
		push dword ptr ds : [0xECED38] // replaces a 'push 0' with the selected action
		push 1
		push esi
		jmp retnAddr
	}
}

/* ideally this would be replaced with a wrapper around the Script Edit callback function */
_declspec(naked) void ScriptEditKeypressHook(HWND hWnd) {
	static const UInt32 skipAddr = 0x5C40EC;
	static const UInt32 saveAddr = 0x5C4961;
	static const UInt32 retnAddr = 0x5C3ED3;
	_asm {
		ja skip
		cmp ebx, 'S'
		jne notSave

		cmp byte ptr ds : [esp + 0x1F] , 0 // check if control pressed
		jz notSave

		jmp saveAddr

	skip:
		jmp skipAddr
	notSave:
		jmp retnAddr
	}
}

bool __fastcall ScriptEdit__Save(byte* thiss, void* dummyEDX, HWND hDlg, char a3, char a4) {
	bool saveSuccess = ThisCall<bool>(0x5C2F40, thiss, hDlg, a3, a4);
	if (saveSuccess) {
		SendDlgItemMessageA(hDlg, 1166, EM_SETMODIFY, 0, 0);

		// remove the '*' from the end of the window name
		char windowName[MAX_PATH];
		int len = GetWindowTextA(hDlg, windowName, MAX_PATH) - 1;

		if (len > 2 && windowName[len] == '*') {
			windowName[len - 1] = '\0'; // strip the '* '
			SetWindowTextA(hDlg, windowName);
		}
	}
	return saveSuccess;
}

float GetRefSpeedMultiplier() {
	float multiplier = 1.0F;
	if (GetAsyncKeyState(VK_SHIFT) < 0) {
		multiplier *= config.fMovementShiftMultiplier;
	}
	if (GetAsyncKeyState(VK_MENU) < 0) {
		multiplier *= config.fMovementAltMultiplier;
	}
	return multiplier;
}

char __cdecl hk_DoRenderPan(int a1, int a2, float a3) {
	if (GetAsyncKeyState(VK_MENU) < 0) {
		a3 *= config.fMovementAltMultiplier;
	}
	return CdeclCall<char>(0x464210, a1, a2, a3);
}

char __cdecl hk_DoRenderMousewheelScroll(int a1, int a2, float a3) {
	return CdeclCall<char>(0x464210, a1, a2, a3 * GetRefSpeedMultiplier());
}

double __fastcall hk_CalculateVectorMagnitude(float* vector, void* dummyEDX) {
	return ThisCall<float>(0x40B3D0, vector) * GetRefSpeedMultiplier();
}

/* multiply the reference movement speed dependent */
_declspec(naked) void RenderWindowReferenceMovementSpeedHook() {
	static const UInt32 retnAddr = 0x455398;
	_asm {
		pushad
		call GetRefSpeedMultiplier
		fmul
		popad

		//	originalCode:
		fmul dword ptr ds : [0xECFD1C]
		jmp retnAddr
	}
}

float GetOrthoSpeedMultiplier() {
	if (GetAsyncKeyState(VK_MENU) < 0) {
		return config.fMovementAltMultiplier;
	}
	return 1.0F;
}

_declspec(naked) void hk_OrthographicZoom() {
	static const UInt32 retnAddr = 0x45F667;
	_asm {
		pushad
		call GetOrthoSpeedMultiplier
		fmul
		popad

		//	originalCode
		fmul    dword ptr ds : [0xD2E0D0]
		jmp retnAddr
	}
}

_declspec(naked) void hk_OrthographicZoom2() {
	static const UInt32 retnAddr = 0x4602DE;
	_asm {
		fild dword ptr ss : [esp + 0x24]
		fmul dword ptr ds : [eax]

		pushad
		call GetOrthoSpeedMultiplier
		fmul
		popad

		jmp retnAddr
	}
}

_declspec(naked) void hk_RefCameraRotation() {
	static const UInt32 retnAddr = 0x45F601;
	_asm {
		fld dword ptr[eax]

		pushad
		call GetOrthoSpeedMultiplier
		fmul
		popad

		push ecx
		fstp[esp]
		jmp retnAddr
	}
}

bool isFirstInit = true;
_declspec(naked) void hk_LoadFilesInit() {
	static const UInt32 autoLoadAddr = 0x432D7D;
	static const UInt32 noLoadAddr = 0x432E64;

	_asm {
		cmp byte ptr ds : [isFirstInit] , 0
		je notFirst
		mov byte ptr ds : [isFirstInit] , 0
		jmp autoLoadAddr
		notFirst :
		jmp noLoadAddr
	}
}

/* check if Q or E are held and modify the Z movement speed (stored in esp+0x2C) before it is passed to the view transform */
_declspec(naked) void RenderWindowFlycamPreTransformMovementHook() {
	static const UInt32 retnAddr = 0x455DB1;
	_asm {
		push 'Q'
		call esi
		movzx eax, ax
		test eax, 0x8000
		jz noQ

		fld  dword ptr ss : [esp + 0x2C]
		fsub dword ptr ds : [0xED12C0]
		fstp dword ptr ss : [esp + 0x2C]
	noQ:
		push 'E'
		call esi
		movzx eax, ax
		test eax, 0x8000
		jz noE

		fld dword ptr ss : [esp + 0x2C]
		fadd dword ptr ds : [0xED12C0]
		fstp dword ptr ss : [esp + 0x2C]
	noE:
		mov esi, dword ptr ds : [0xED116C]
		jmp retnAddr
	}
}

_declspec(naked) void RenderWindowFlycamPostTransformMovementHook() {
	static const UInt32 retnAddr = 0x455DCE;
	_asm {
		push eax
		push VK_LCONTROL
		call dword ptr ds : [0xD234D8] // GetAsyncKeyState
		movzx eax, ax
		test eax, 0x8000
		jz noDown

		pop eax
		fld  dword ptr ss : [eax + 8]
		fsub dword ptr ds : [0xED12C0]
		fstp dword ptr ss : [eax + 8]
		push eax

	noDown:
		push VK_SPACE
		call dword ptr ds : [0xD234D8] // GetAsyncKeyState
		movzx eax, ax
		test eax, 0x8000
		jz noUp

		pop eax
		fld  dword ptr ss : [eax + 8]
		fadd dword ptr ds : [0xED12C0]
		fstp dword ptr ss : [eax + 8]
		push eax

	noUp:
		pop eax
		mov edx, dword ptr ss : [eax]
		mov ecx, dword ptr ss : [eax + 4]
		jmp retnAddr
	}
}

LRESULT __stdcall UpdateTimeOfDayScrollbarHook(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	LRESULT scrollPos = SendMessageA(hWnd, Msg, wParam, lParam);
	SendMessageA(g_trackBarHwnd, TBM_SETPOS, TRUE, scrollPos);

	char timeBuf[100];
	sprintf(timeBuf, "%.2f", scrollPos / 4.0F);
	SetWindowTextA(g_timeOfDayTextHwnd, timeBuf);

	return scrollPos;
}

void __stdcall UpdateTimeOfDayInputBoxHook(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	SendMessageA(hWnd, Msg, wParam, lParam);
	SendMessageA(g_trackBarHwnd, TBM_SETPOS, TRUE, lParam);

	char timeBuf[100];
	sprintf(timeBuf, "%.2f", lParam / 4.0F);
	SetWindowTextA(g_timeOfDayTextHwnd, timeBuf);
}

_declspec(naked) void LipGenLoopHook() {
	static const UInt32 retnAddr = 0x41EA83;
	static const UInt32 skipAddr = 0x41EE65;
	_asm {
		// ecx contains TESForm
		test byte ptr ss : [ecx + 8] , 2 // form->flags & kModified

		je skip

		mov eax, [ecx]
		mov edx, [eax + 0xFC]
		jmp retnAddr

		skip :
		jmp skipAddr
	}
}

_declspec(naked) void LipGenCountTopicsHook() {
	static const UInt32 retnAddr = 0x41EA35;
	_asm {
		mov esi, dword ptr ss : [eax]
		test esi, esi
		jz done

		// esi contains TESForm
		test byte ptr ss : [esi + 8] , 2 // form->flags & kModified

		je done

		inc ecx
		done :
		jmp retnAddr
	}
}

_declspec(naked) void EmbeddedRenderWindowSoundNullCheck() {
	static const UInt32 noSoundFileAddr = 0x89357D;
	static const UInt32 soundFileAddr = 0x8935B5;
	_asm {
		test ecx, ecx
		je noSound

		movsx edx, byte ptr ds : [ecx]
		test edx, edx
		je noSound

		jmp soundFileAddr

		noSound :
		jmp noSoundFileAddr
	}
}

void __fastcall PreferencesWindowApplyButtonHook(int* thiss, void* dummyEDX, int a2) {
	ThisCall(0x855B30, thiss, a2);
	SendMessageA(g_allowCellWindowLoadsButtonHwnd, BM_SETCHECK, GetIsRenderWindowAllowCellLoads(), NULL);
}

BOOL __stdcall RenderWindowCallbackHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_KEYDOWN) {
		switch (wParam) {
		case VK_ESCAPE:
		case VK_LWIN:
			SetFlycamMode(0);
			break;
		}
	}
	else if (msg == WM_RBUTTONDOWN) {
		SetFlycamMode(0);
	}
	return StdCall<LRESULT>(0x455AA0, hWnd, msg, wParam, lParam);
}

extern HWND g_MainHwnd;
void ShowSaveFailureError()
{
	MessageBoxA(g_MainHwnd, "Save failed. Please close any programs with the file open.", "Save Failed", MB_ICONWARNING);
}

_declspec(naked) void SaveFailureHook()
{
	static const UInt32 retnAddr = 0x4E1DBF;
	_asm {
		je noWarn
		pushad
		call ShowSaveFailureError
		popad
		noWarn :
		jmp retnAddr
	}
}

_declspec(naked) void CellViewListViewCreateFormIDColumnHook()
{
	static const UInt32 retnAddr = 0x42EFC1;
	_asm
	{
		push 65 // pixels
		push 1
		push LVM_SETCOLUMNWIDTH
		push dword ptr ds : [0xECF548] // g_cellViewRightListHdlg
		call ebx // SendMessageA
		mov ecx, dword ptr ds : [0xECF554]
		jmp retnAddr
	}
}

_declspec(naked) void ObjectWindowListViewColumnSizeHook()
{
	static const UInt32 retnAddr = 0x449660;
	_asm
	{
		push 65 // pixels
		push 2
		push LVM_SETCOLUMNWIDTH
		push edi // g_objectWindowListHdlg
		call ebx // SendMessageA

		mov ecx, dword ptr ss : [esp + 0x54]
		push 1
		jmp retnAddr
	}
}

void __cdecl InsertListViewHeaderSetSizeHook(HWND hWnd, WPARAM wParam, int a3, int a4, int a5)
{
	CdeclCall(0x419F50, hWnd, wParam, a3, a4, a5);
	SendMessageA(hWnd, LVM_SETCOLUMNWIDTH, wParam, 65);
}

_declspec(naked) void EditLandCheckLandIsNull()
{
	static const UInt32 retnAddr = 0x61CA60;
	_asm
	{
		mov dword ptr ss : [esp + 0x4] , ebx
		test eax, eax
		je done
		cmp dword ptr ds : [eax] , 0
	done:
		 jmp retnAddr
	}
}

_declspec(naked) void FormListCheckNull()
{
	// add check if eax is null before dereferencing it
	static const UInt32 retnAddr = 0x501457;
	_asm
	{
		add esp, 0x4
		test eax, eax
		je done
		mov ecx, dword ptr ds : [eax]
		test ecx, ecx
	done :
		jmp retnAddr
	}
}

_declspec(naked) void FormListCheckNull2()
{
	static const UInt32 retnAddr = 0x5AE0AB;
	static const UInt32 skipAddr = 0x5AE118;//0x5AE12E; // 0x5AE37E
	_asm
	{
		test eax, eax
		je skip
		mov ecx, dword ptr ds : [0xECFB38] // g_hWndParent
		jmp retnAddr
	skip:
		add esp, 4
		jmp skipAddr
	}
}

void BadFormLoadHook();
void SaveScriptChangedType();

_declspec(naked) void MultipleMasterLoadHook()
{
	static const char* MultipleMastersMessage = "Multiple master files selected for load, do you wish to continue? (enable bAllowMultipleMasterLoads to remove this warning)";
	static const UInt32 continueLoadingAddr = 0x4DD394;
	static const UInt32 stopLoadingAddr = 0x4DD2F9;
	_asm
	{
		push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
		push 0xD41FE4 // "Invalid File Selection"
		push MultipleMastersMessage
		push 0
		call edi // MessageBoxA
		cmp eax, IDYES
		jne stopLoading
		mov byte ptr ds : [0xED3B80] , 1 // bAllowMultipleMasterLoads
		jmp continueLoadingAddr
	stopLoading:
		jmp stopLoadingAddr
	}
}

__declspec(naked) void RenderWindowHandlesRefRotationHook()
{
	static const UInt32 retnAddr = 0x4523D0;
	_asm
	{
		call GetRefSpeedMultiplier // no need to push/popad at this location
		fild dword ptr ss : [esp + 0x114]
		fmul
		test byte ptr ds : [0x00ECFCEC] , 0x2
		jmp retnAddr
	}
}

const char* NavmeshConfirmMessage = "Are you sure you want to find cover edges?";
_declspec(naked) void RenderWindowNavMeshConfirmFindCover()
{
	static const UInt32 skipAddr = 0x462C7A;

	_asm
	{
		push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
		push 0xD2D1F4 // "Navmesh: Find Cover"
		push NavmeshConfirmMessage
		push 0
		call MessageBoxA
		cmp eax, IDNO
		je skip

		//	doNavmesh
		push 0
		mov ecx, 0xF073F8
		push 0x456F2E // retnAddr
		mov eax, 0x6F51B0 // Navmesh::FindCoverEdges
		jmp eax
	skip:
		jmp skipAddr
	}
}

_declspec(naked) void MainWindowNavMeshConfirmFindCover()
{
	static const UInt32 skipAddr = 0x44382F;

	_asm
	{
		push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
		push 0xD2D1F4 // "Navmesh: Find Cover"
		push NavmeshConfirmMessage
		push 0
		call MessageBoxA
		cmp eax, IDNO
		je skip

		//	doNavmesh
		push 0
		mov ecx, 0xF073F8
		push 0x44451D // retnAddr
		mov eax, 0x6F51B0 // Navmesh::FindCoverEdges
		jmp eax
	skip :
		jmp skipAddr
	}
}

_declspec(naked) void NavMeshToolbarConfirmFindCover()
{
	static const UInt32 skipAddr = 0x40AC87;

	_asm
	{
		push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
		push 0xD2D1F4 // "Navmesh: Find Cover"
		push NavmeshConfirmMessage
		push 0
		call MessageBoxA
		cmp eax, IDNO
		je skip

		//	doNavmesh
		push 0
		mov ecx, 0xF073F8
		push 0x40AC7B // retnAddr
		mov eax, 0x6F51B0 // Navmesh::FindCoverEdges
		jmp eax
	skip:
		jmp skipAddr
	}
}

void __cdecl CrashSaveSetName(char* dst, size_t size, char* format, void* DEFAULT)
{
	ModInfo* activeFile = DataHandler::GetSingleton()->activeFile;
	const char* modName = "DEFAULT.esp";
	if (activeFile)
	{
		modName = activeFile->name;
	}
	sprintf(dst, "%s", modName);
}

LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

void CreateCrashSave()
{
	// create a save in the Data//CrashSaves folder called %s.esp
	WriteRelCall(0x4DB07A, UInt32(CrashSaveSetName));

	CreateDirectoryA("Data\\CrashSaves", 0);
	static const char* path = "Data\\CrashSaves\\";
	SafeWrite32(0x4DB0AC, UInt32(path));

	ThisStdCall(0x4DB020, DataHandler::GetSingleton()); // DoAutosave

	// restore original path (Data\\Backup\\)
	SafeWrite32(0x4DB0AC, 0xD415C4);
}

LONG WINAPI DoCrashSave(EXCEPTION_POINTERS* info)
{
	CreateCrashSave();

	char buf[0x1000];

	auto contextRecord = info->ContextRecord;

	sprintf(buf, "The geck has quit unexpectedly. Please check the Data//CrashSaves folder for a crash save and verify it in xEdit.\r\nDebug Information:\r\nREGISTERS\r\neip: %08X", contextRecord->Eip);
	sprintf(buf, "%s\r\neax: %08X", buf, contextRecord->Eax);
	sprintf(buf, "%s\r\nebx: %08X", buf, contextRecord->Ebx);
	sprintf(buf, "%s\r\necx: %08X", buf, contextRecord->Ecx);
	sprintf(buf, "%s\r\nedx: %08X", buf, contextRecord->Edx);
	sprintf(buf, "%s\r\nedi: %08X", buf, contextRecord->Edi);
	sprintf(buf, "%s\r\nesi: %08X", buf, contextRecord->Esi);
	sprintf(buf, "%s\r\nebp: %08X", buf, contextRecord->Ebp);
	sprintf(buf, "%s\r\nesp: %08X", buf, contextRecord->Esp);

	UInt32* esp = (UInt32*)contextRecord->Esp;
	sprintf(buf, "%s\r\n\r\nSTACK", buf);

	char stack[0x1000];
	*stack = 0;
	__try
	{
		for (int row = 0; row < 16; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				sprintf(stack, "%s%08X |", stack, esp[row * 4 + col]);
			}
			strcat(stack, "\r\n");
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	sprintf(buf, "%s\r\n%s", buf, stack);
	_MESSAGE("%s", buf);
	MessageBoxA(nullptr, buf, "Error", MB_ICONERROR | MB_OK);
	
	return s_originalFilter && s_originalFilter(info);
};

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI FakeSetUnhandledExceptionFilter(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	s_originalFilter = lpTopLevelExceptionFilter;
	return nullptr;
}

void SetCrashSaveHandler()
{
	SafeWrite32(0xD2326C, UInt32(FakeSetUnhandledExceptionFilter));
	SetUnhandledExceptionFilter(&DoCrashSave);
}

void __fastcall InitPreviewWindowBackgroundColor(void* window, void* edx, UInt32 unusedColor)
{
	UInt8 r = config.iPreviewWindowRed;
	UInt8 g = config.iPreviewWindowGreen;
	UInt8 b = config.iPreviewWindowBlue;
	UInt32 color = r + (g << 8) + (b << 16);
	ThisStdCall(0x4793D0, window, color);
}

static double havokAnimationRate = 1000.0;
void SetHavokAnimationSpeed(float speed)
{
	havokAnimationRate = 1000.0 / speed;
}

#define ID_ANIMATIONSPEED_TRACKBAR 1337
#define ID_ANIMATIONSPEED_EDIT 1338
#define ID_ANIMATION_STATIC 1339
BOOL __stdcall HavokPreviewCallback(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	if (Message == WM_COMMAND)
	{
		const uint32_t param = LOWORD(wParam);
		switch (param)
		{
		case ID_ANIMATIONSPEED_EDIT:
			if (GetFocus() != (HWND)lParam) break;

			if (HIWORD(wParam) == EN_MAXTEXT) break;

			char text[16];
			int len = SendMessageA((HWND)lParam, EM_GETLINE, 0, (LPARAM)text);
			text[len] = '\0'; // null terminate the line since EM_GETLINE does not

			float speed = atof(text);
			SetHavokAnimationSpeed(speed);
			SendMessageA(GetDlgItem(hWnd, ID_ANIMATIONSPEED_TRACKBAR), TBM_SETPOS, TRUE, speed * 50); // update scrollbar
			return 0;
		}
	}
	else if (Message == WM_HSCROLL)
	{
		if ((HWND)lParam == GetDlgItem(hWnd, ID_ANIMATIONSPEED_TRACKBAR))
		{
			char timeBuf[100];

			const uint32_t param = LOWORD(wParam);

			if (param == SB_THUMBPOSITION || param == SB_THUMBTRACK || param == SB_ENDSCROLL)
			{
				float speed;
				if (param == SB_ENDSCROLL) speed = SendDlgItemMessageA(hWnd, ID_ANIMATIONSPEED_TRACKBAR, TBM_GETPOS, 0, 0);
				else speed = HIWORD(wParam);

				speed *= 0.02; // convert speed from 0-100 to 0-2

				SetHavokAnimationSpeed(speed);

				sprintf(timeBuf, "%.2f", speed);
				SetWindowTextA(GetDlgItem(hWnd, ID_ANIMATIONSPEED_EDIT), timeBuf);
			}
			return 0;
		}
	}
	else if (Message == WM_INITDIALOG)
	{
		// initialize the animation speed to 1.0 and update trackbars accordingly
		SetHavokAnimationSpeed(1);

		HWND trackbar = GetDlgItem(hWnd, ID_ANIMATIONSPEED_TRACKBAR);
		SendMessageA(trackbar, TBM_SETPOS, true, 50);
		SendMessageA(trackbar, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
		SendMessageA(trackbar, TBM_SETTICFREQ, 10, 0);
		SendDlgItemMessageA(hWnd, ID_ANIMATIONSPEED_EDIT, WM_SETTEXT, NULL, (LPARAM)"1.00");

		config.iPreviewWindowRed = GetOrCreateINIInt("Preview Window", "iBackgroundRed", 127, IniPath);
		config.iPreviewWindowGreen = GetOrCreateINIInt("Preview Window", "iBackgroundGreen", 127, IniPath);
		config.iPreviewWindowBlue = GetOrCreateINIInt("Preview Window", "iBackgroundBlue", 127, IniPath);
		int landHeight = GetOrCreateINIInt("Preview Window", "iLandHeight", 50, IniPath);

		SendDlgItemMessageA(hWnd, 2543, TBM_SETPOS, 1u, landHeight);

	}
	else if (Message == WM_DESTROY)
	{
		int red = GetDlgItemInt(hWnd, 1109, 0, 0);
		int green = GetDlgItemInt(hWnd, 1033, 0, 0);
		int blue = GetDlgItemInt(hWnd, 1111, 0, 0);
		char arr[11];
		WritePrivateProfileString("Preview Window", "iBackgroundRed", _itoa(red, arr, 10), IniPath);
		WritePrivateProfileString("Preview Window", "iBackgroundGreen", _itoa(green, arr, 10), IniPath);
		WritePrivateProfileString("Preview Window", "iBackgroundBlue", _itoa(blue, arr, 10), IniPath);

		int landHeight = SendDlgItemMessageA(hWnd, 2543, TBM_GETPOS, 0, 0);
		WritePrivateProfileString("Preview Window", "iLandHeight", _itoa(landHeight, arr, 10), IniPath);
	}

	return StdCall<BOOL>(0x4107F0, hWnd, Message, wParam, lParam);
}

RECT* previousHavokWindowRect = (RECT*)0xECECBC;
void __cdecl HavokPreviewResize(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	LONG deltaX = clientRect.right - previousHavokWindowRect->right;
	LONG deltaY = clientRect.bottom - previousHavokWindowRect->bottom;

	// move the trackbar and edit dialogs
	MoveDlgItem(hWnd, ID_ANIMATIONSPEED_TRACKBAR, deltaX / 2, deltaY);
	MoveDlgItem(hWnd, ID_ANIMATIONSPEED_EDIT, deltaX / 2, deltaY);
	MoveDlgItem(hWnd, ID_ANIMATION_STATIC, deltaX / 2, deltaY);
	InvalidateRect(hWnd, &clientRect, true);
	// call original function
	CdeclCall(0x40F930, hWnd);
}

/* Need to figure how to fill out the "length" column with the animation length
void __cdecl HavokPreviewAddColumns(HWND hWnd, WPARAM index, char* name, int width, int a5)
{
	CdeclCall(0x419F50, hWnd, index, name, width, a5);
	CdeclCall(0x419F50, hWnd, index+1, "Length", width, a5);
}

void AddAnimLengthColumnToHavokPreviewAnimsList()
{
	WriteRelCall(0x40FBD8, UInt32(HavokPreviewAddColumns));
}
*/

void __cdecl HavokPreviewSetAnimNameHook(char* dst, char* format, NiControllerSequence* anim)
{
	sprintf(dst, "%s (%.2fs)", anim->filePath, anim->end - anim->begin);
}

void HavokPreview_AddAnimLengthToName()
{
	// replace push NiControllerSequence->sequenceName with push NiControllerSequence
	XUtil::PatchMemoryNop(0x40F887, 3);

	WriteRelCall(0x40F894, UInt32(HavokPreviewSetAnimNameHook));
}

void SetupHavokPreviewWindow()
{
	SafeWrite32(0x4109E3, UInt32(&havokAnimationRate));
	SafeWrite32(0x410A50, UInt32(&havokAnimationRate));
	SafeWrite32(0x40FCE2, UInt32(&havokAnimationRate));
	SafeWrite32(0x40FD2C, UInt32(&havokAnimationRate));

	WriteRelCall(0x410868, UInt32(HavokPreviewResize));

	// read background color from ini
	WriteRelCall(0x4100A0, UInt32(InitPreviewWindowBackgroundColor));

	// make pause button not forget particle position (still fails occasionally)
	SafeWrite32(0x410C2A, 0xCCEB006A); // jump over calls setting dword ECECD8

	// skip setting ground height taskbar pos as it's done in the callback instead
	SafeWrite16(0x40FF78, 0x13EB);

	if (config.bAppendAnimLengthToName)	HavokPreview_AddAnimLengthToName();

	//	AddAnimLengthColumnToHavokPreviewAnimsList();
}

void InsertListViewColumn(HWND hWnd, UInt32 index, const char* text, int width)
{
	LVCOLUMN column;
	column.mask = 0xF;
	column.cx = width;
	column.pszText = (char*)text;
	column.fmt = 0;
	column.iSubItem = index;

	SendMessageA(hWnd, LVM_INSERTCOLUMNA, index, (LPARAM)& column);
}

void SetupConditonsColumns(HWND hWnd)
{
	SendMessageA(hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0x21, 33);
	while (SendMessageA(hWnd, LVM_DELETECOLUMN, 0, 0))
		;

	InsertListViewColumn(hWnd, 0, "Target", 10);
	InsertListViewColumn(hWnd, 1, "Function Name", 10);
	InsertListViewColumn(hWnd, 2, "Function Info", 10);
	InsertListViewColumn(hWnd, 3, "Comp", 10);
	InsertListViewColumn(hWnd, 4, "Value", 10);
	InsertListViewColumn(hWnd, 5, "", 10);
}

_declspec(naked) void RefreshCellHook()
{
	_asm
	{
		call RefreshSoundMarkers
		call RefreshLightMarkers
		mov eax, 0x462C73 // retnAddr
		jmp eax
	}
}

UInt32 __fastcall PlaceOPALObjectHook(ObjectPalette::Object* current, void* edx, float* point1, float* point2)
{
	ObjectPalette::Object* toPlace = current;
	if ((GetAsyncKeyState(VK_CAPITAL) < 0) ^ config.bObjectPaletteRandomByDefault)
	{
		ObjectPalette* opal = ObjectPalette::GetSingleton();

		UInt32 randomIndex = rand() % opal->list.Count();
		toPlace = opal->list.GetNthItem(randomIndex);
	}

	return ThisStdCall(0x40BF90, toPlace, point1, point2);
}

const double M_TAU = 6.28318530717958647692528676;
float __fastcall CalculateRefRotation(signed int amount)
{
	const double pi_on_180 = *(double*)0xD2C320;;
	float rotationSpeedMult = *(float*)0xECFD10;
	float* currentRotation = (float*)0xED1164;
	int iSnapToAngle = *(int*)0xECFD04;

	double snapRadians = iSnapToAngle * pi_on_180;
	double addend = (amount * rotationSpeedMult / 10.0);
	*currentRotation += addend;

	// computes how many "SnapRadians" the rotation should be scaled to
	int truncatedSnapRadians = *currentRotation / snapRadians;

	double rotation = snapRadians * truncatedSnapRadians;
	if (rotation == 0.0)
	{
		return 0;
	}

	*currentRotation = 0;
	while (rotation >= M_TAU) rotation -= M_TAU;
	while (rotation < 0) rotation += M_TAU;

	return rotation;
}

_declspec(naked) void RenderWindowHandleRefRotationHook()
{
	static const UInt32 retnAddr = 0x4524A2;
	_asm
	{
		mov ecx, [esp + 0x114] // rotation amount
		call CalculateRefRotation
		fstp[esp + 0x14]
		jmp retnAddr
	}
}


_declspec(naked) void ExportFaceGenCheckIsFormEdited()
{
	static const UInt32 retnAddr = 0x442055;
	static const UInt32 skipAddr = 0x44207A;
	_asm
	{
		cmp byte ptr ds : [esi + 0x4] , bl
		jne skip
		test byte ptr ss : [esi + 8] , 2 // form->flags & kModified
		je skip
		jmp retnAddr
	skip:
		jmp skipAddr
	}
}

const char* SpeedTreeGetTexturePath()
{
	return "Data\\Textures\\Trees\\Leaves\\";
}

typedef HRESULT(__stdcall* D3DXCreateTextureFromFileInMemoryEx_)(_In_    void* pDevice,
	_In_    LPCVOID            pSrcData,
	_In_    UINT               SrcDataSize,
	_In_    UINT               Width,
	_In_    UINT               Height,
	_In_    UINT               MipLevels,
	_In_    DWORD              Usage,
	_In_    int                Format,
	_In_    int                Pool,
	_In_    DWORD              Filter,
	_In_    DWORD              MipFilter,
	_In_    int                ColorKey,
	_Inout_ LPVOID             pSrcInfo,
	_Out_   PALETTEENTRY* pPalette,
	_Out_   LPVOID             ppTexture);

enum d3
{
	D3DFMT_UNKNOWN = 0,
	D3DX_DEFAULT = -1,
	D3DPOOL_DEFAULT = 0
};

D3DXCreateTextureFromFileInMemoryEx_ D3DXCreateTextureFromFileInMemoryEx;

HRESULT __stdcall CreateTextureHook(void* pDevice, LPCVOID pSrcData, UINT SrcDataSize, void* ppTexture) {

	return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture);
}

void patchTextureMemoryAllocator()
{
	if (*(UInt32*)0xC137BE != 0x0006F596)
	{
		// something has already patched the call address, so do nothing
		return;
	}

	if (HMODULE d3d9 = GetModuleHandleA("d3dx9_38.dll"))
	{
		if (D3DXCreateTextureFromFileInMemoryEx = (D3DXCreateTextureFromFileInMemoryEx_)GetProcAddress(d3d9, "D3DXCreateTextureFromFileInMemoryEx"))
		{
			WriteRelCall(0xC137BD, (UInt32)CreateTextureHook);
		}
	}
}

WNDPROC originalLandscapeEditCallback;
POINT savedLandscapeEditPos;
BOOL __stdcall LandscapeEditCallback(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) 
{
	if (Message == WM_DESTROY)
	{
		char buffer[8];
		WINDOWPLACEMENT pos;
		GetWindowPlacement(hWnd, &pos);

		LONG x = pos.rcNormalPosition.left;
		LONG y = pos.rcNormalPosition.top;

		savedLandscapeEditPos.x = x;
		savedLandscapeEditPos.y = y;

		WritePrivateProfileString("Landscape Editor", "iWindowPosX", _itoa(x, buffer, 10), IniPath);
		WritePrivateProfileString("Landscape Editor", "iWindowPosY", _itoa(y, buffer, 10), IniPath);
	}
	else if (Message == WM_INITDIALOG)
	{
		SetWindowPos(hWnd, NULL, savedLandscapeEditPos.x, savedLandscapeEditPos.y, NULL, NULL, SWP_NOSIZE);
	}
	return CallWindowProc(originalLandscapeEditCallback, hWnd, Message, wParam, lParam);
}


HWND __stdcall CreateLandscapeEditHook(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, WNDPROC lpDialogFunc, LPARAM dwInitParam)
{
	long posX = GetPrivateProfileIntA("Landscape Editor", "iWindowPosX", 0, IniPath);
	long posY = GetPrivateProfileIntA("Landscape Editor", "iWindowPosY", 0, IniPath);

	savedLandscapeEditPos.x = posX;
	savedLandscapeEditPos.y = posY;

	originalLandscapeEditCallback = lpDialogFunc;
	return CreateDialogParamA(hInstance, lpTemplateName, hWndParent, (DLGPROC)LandscapeEditCallback, dwInitParam);
}

void PatchRememberLandscapeEditSettingsWindowPosition()
{
	WriteRelCall(0x441297, UInt32(CreateLandscapeEditHook));
	SafeWrite8(0x441297 + 5, 0x90);
}

void ClearLandscapeUndosIfNearlyOutOfMemory()
{
	constexpr size_t MAX_MEMORY = 2560u * (1024u * 1024u); // 2.5gb
	if (GetCurrentMemoryUsage() > MAX_MEMORY)
	{
		auto hist = HistoryManager::GetSingleton();
		hist->ClearHistoryForCurrentElement();
		Console_Print("2.5gb memory usage exceeded, clearing land edit history.");
	}
}

_declspec(naked) void LandscapePaintHook1()
{
	static const UInt32 retnAddr = 0x45CA17;
	_asm
	{
		call ClearLandscapeUndosIfNearlyOutOfMemory
		mov ecx, dword ptr ds : [0xED13F8]
		jmp retnAddr
	}
}

_declspec(naked) void LandscapePaintHook2()
{
	static const UInt32 retnAddr = 0x45E107;
	_asm
	{
		call ClearLandscapeUndosIfNearlyOutOfMemory
		mov ecx, dword ptr ds : [0xED13F8]
		jmp retnAddr
	}
}

void PatchClearLandscapeEditUndoStackIfNearlyOOM()
{
	WriteRelJump(0x45CA11, UInt32(LandscapePaintHook1));
	WriteRelJump(0x45E101, UInt32(LandscapePaintHook2));
}

std::unordered_set<std::string> FileCacheMap;

void __cdecl hk_call_sub_41E8F0(bool a1)
{
    // Build the file cache map, run the LIP generation, then free it all
    if (std::filesystem::exists("Data\\Sound\\Voice"))
    {
        for (const auto& p : std::filesystem::recursive_directory_iterator("Data\\Sound\\Voice"))
        {
            if (std::filesystem::is_directory(p))
                continue;

            // Temp.lip/Temp.wav/FonixData.cdf/temp_resampled_audio.wav should probably be blacklisted here
            FileCacheMap.emplace(p.path().string());
        }
    }

	CdeclCall(0x0041E8F0, a1);
    FileCacheMap.clear();
}

int __fastcall hk_sub_8A1FC0(void *thisptr, void *_EDX, const char *Path, const char *a2, int a3, int a4)
{
    // Short circuit for GetFileAttributesA()
    if (!FileCacheMap.empty() && !FileCacheMap.count(Path))
        return 0;

    return ThisCall<int>(0x008A1FC0, thisptr, Path, a2, a3, a4);
}

void __declspec(naked) hk_call_41EBDE()
{
    // Unchecked return value that determines if the file exists or not
    __asm
    {
        mov eax, 0x0058D8F0
        call eax

        test eax, eax
        je fileDoesNotExist

        mov eax, 0x0041EBE3
        jmp eax

        fileDoesNotExist:
        mov eax, 0x0041EE13
        jmp eax
    }
}

void PatchFasterLipGen()
{
	WriteRelJump(0x0041EBDE, (UInt32)hk_call_41EBDE);

	WriteRelCall(0x00440C5D, (UInt32)hk_call_sub_41E8F0);
	WriteRelCall(0x00440C6E, (UInt32)hk_call_sub_41E8F0);

	WriteRelCall(0x0058DA9A, (UInt32)hk_sub_8A1FC0);
	WriteRelCall(0x0047964E, (UInt32)hk_sub_8A1FC0);
}

__declspec(naked) void LoadCellCheckLinkedRefNullHook()
{
	_asm
	{
		test eax, eax
		je isNull
		mov ecx, [eax]
		cmp ecx, [eax + 4]
		ret

	isNull:
		cmp al, 1 // set flags for jne at retnAddr
		ret
	}
}

void PatchCellExtraDataCrash()
{
	WriteRelCall(0x4B0025, UInt32(LoadCellCheckLinkedRefNullHook));
}


void* g_heapManager = reinterpret_cast<void*>(0xF21B5C);

size_t Heap_MemorySize(void* memory)
{
	auto msize = ThisStdCall(0x854720, g_heapManager, memory);
	return msize;
}

void __fastcall RemoveScriptDataSizeLimit(ScriptBuffer* scriptBuffer, ScriptLineBuffer* lineBuffer)
{
	size_t dataSize = Heap_MemorySize(scriptBuffer->scriptData);
	if (scriptBuffer->dataOffset + lineBuffer->dataOffset + 10 < dataSize)
		return;
	const size_t newMemSize = dataSize * 2;
	void* newMem = FormHeap_Allocate(newMemSize);
	memset(newMem, 0, newMemSize);
	memcpy(newMem, scriptBuffer->scriptData, scriptBuffer->dataOffset);
	FormHeap_Free(scriptBuffer->scriptData);
	scriptBuffer->scriptData = static_cast<UInt8*>(newMem);
}

__declspec(naked) void Hook_RemoveScriptDataLimit()
{
	__asm
	{
		mov edx, esi // line buffer
		mov ecx, edi // script buffer
		call RemoveScriptDataSizeLimit

		mov eax, [edi + 0x24] // dataOffset_1 = scriptBuf->dataOffset
		mov ecx, 0x5C9551 // skip function return
		jmp ecx
	}
}

HMODULE WINAPI hook_LoadLibrary(LPCSTR filename)
{
	if (_strcmpi(filename, "d3d9.dll")) return LoadLibrary(filename);

	char path[MAX_PATH];
	GetSystemDirectory(path, MAX_PATH);
	strncat(path, "\\d3d9.dll", sizeof(path));

	return LoadLibrary(path);
}

enum EffectType
{
	kArchType_ValueModifier = 0x0,
	kArchType_Script = 0x1,
	kArchType_Dispel = 0x2,
	kArchType_CureDisease = 0x3,
	kArchType_Absorb = 0x4,
	kArchType_Shield = 0x5,
	kArchType_Calm = 0x6,
	kArchType_Demoralize = 0x7,
	kArchType_Frenzy = 0x8,
	kArchType_CommandCreature = 0x9,
	kArchType_CommandHumanoid = 0xA,
	kArchType_Invisibility = 0xB,
	kArchType_Chameleon = 0xC,
	kArchType_Light = 0xD,
	kArchType_Darkness = 0xE,
	kArchType_NightEye = 0xF,
	kArchType_Lock = 0x10,
	kArchType_Open = 0x11,
	kArchType_BoundItem = 0x12,
	kArchType_SummonCreature = 0x13,
	kArchType_DetectLife = 0x14,
	kArchType_Telekinesis = 0x15,
	kArchType_DisintigrateArmor = 0x16,
	kArchType_DisinitgrateWeapon = 0x17,
	kArchType_Paralysis = 0x18,
	kArchType_Reanimate = 0x19,
	kArchType_SoulTrap = 0x1A,
	kArchType_TurnUndead = 0x1B,
	kArchType_SunDamage = 0x1C,
	kArchType_Vampirism = 0x1D,
	kArchType_CureParalysis = 0x1E,
	kArchType_CureAddiction = 0x1F,
	kArchType_CurePoison = 0x20,
	kArchType_Concussion = 0x21,
	kArchType_ValueAndParts = 0x22,
	kArchType_LimbCondition = 0x23,
	kArchType_Turbo = 0x24,
};

struct EntryPointFieldInfo
{
	enum FieldType : UInt32
	{
		kFieldType_FunctionDataStatic1 = 0,    // dlgID: 998 ( 2456 )
		kFieldType_FunctionDataStatic2 = 1,    // dlgID: 999 ( 2457 )
		kFieldType_FunctionDataStatic3 = 2,    // dlgID: 99C ( 2460 )
		kFieldType_Dropdown = 3, // dlgID: 496 ( 1174 )
		kFieldType_Input1 = 4,   // dlgID: 99A ( 2458 )
		kFieldType_Input2 = 5,   // dlgID: 99B ( 2459 )
		kFieldType_Checkbox = 8,    // dlgID: 1407 ( 5127 )
	};

	enum DropdownContentType : UInt32
	{
		kDropdownContent_ActorValue = 0,
		kDropdownContent_Quest = 1,
		kDropdownContent_LeveledItem = 2,
	};

	FieldType fieldType;
	DropdownContentType dropdownContentType;
	UInt32 displayText;
	UInt32 offsetX;
	UInt32 offsetY;
	UInt32 width;
	UInt32 height;
};

struct EntryPointFieldTypeInfo
{
	UInt32 numParams;
	EntryPointFieldInfo* fieldInfos;
	UInt32 entryPointType;
};

struct EntryPointFieldTypeInfoArray
{
	EntryPointFieldTypeInfo infos[9];
} *entryPointInfoArray = (EntryPointFieldTypeInfoArray*)0xE97D74;

void __cdecl ExportDialogueEndPlaySound(WPARAM wParam, LPARAM lParam)
{
	PlaySound("MouseClick", NULL, SND_ASYNC);
	CdeclCall(0x4657A0, wParam, lParam);
}

errno_t __cdecl OnGetMeshPathModifyIfDragDrop(char* Dst, rsize_t SizeInBytes, const char* Src)
{
	bool isDragDrop = *Src && Src[1] == ':' && Src[2] == '\\';
	if (isDragDrop)
	{
		return CdeclCall<errno_t>(0xC5BE82, Dst, SizeInBytes, Src); // strcpy_s
	}
	return CdeclCall<errno_t>(0xC5BEEA, Dst, SizeInBytes, Src); // strcat_s
}

__declspec(naked) void OnLoadRegionsHook()
{
	_asm
	{
		cmp dword ptr ds : [esi + 0x10], ebx
		je nullRegion
		cmp ebp, -1
		je noRegions

		pop eax
		mov eax, 0x743F9D
		jmp eax

	nullRegion:
		ret

	noRegions:
		pop eax
		pop ebp
		pop esi
		pop ebx
		ret 4
	}
}

namespace CustomFOV
{
	class NiCamera : public NiAVObject
	{
	public:
		NiCamera();
		~NiCamera();

		float			m_kWorldToCam[4][4];// 09C
		NiFrustum		m_kViewFrustum;		// 0DC
		float			m_fMinNearPlaneDist;// 0F8
		float			m_fMaxFarNearRatio;	// 0FC
		NiViewport		m_kPort;			// 100
		float			m_fLODAdjust;		// 110
	};
	STATIC_ASSERT(sizeof(NiCamera) == 0x114);
	STATIC_ASSERT(offsetof(NiCamera, m_kViewFrustum) == 0xDC);

	static float fFOVTan = 1.0F;
	bool __fastcall NiWindow__UpdateCamera(void* window, void* edx, NiCamera* apCamera, UInt32 auiWidth, UInt32 auiHeight, float afEndClipDist)
	{
		if (!apCamera)
			return false;
		float fScaledRatio = apCamera->m_fMaxFarNearRatio * 10.0;
		if (fScaledRatio < afEndClipDist)
			apCamera->m_fMaxFarNearRatio = afEndClipDist / 10.0;
		NiFrustum kNewFrustum = {};
		float fAspectRatio = static_cast<float>(auiHeight) / auiWidth;

		kNewFrustum.l = -fFOVTan * 0.5;
		kNewFrustum.r = fFOVTan * 0.5;
		kNewFrustum.b = -fFOVTan * fAspectRatio * 0.5;
		kNewFrustum.t = fFOVTan * fAspectRatio * 0.5;

		kNewFrustum.n = 10.0;
		kNewFrustum.f = afEndClipDist;
		apCamera->m_kViewFrustum = kNewFrustum;
		apCamera->m_kPort.l = 0.f;
		apCamera->m_kPort.r = 1.f;
		apCamera->m_kPort.t = 1.f;
		apCamera->m_kPort.b = 0.f;
		ThisCall(0x80DD60, apCamera, &DefaultNodeUpdateParams);
		return 1;
	}

	void SetFOV(float aFOV)
	{
		float fScaledFOV = aFOV * 0.01745329238474369 * 0.5;
		fFOVTan = tan(fScaledFOV);
	}

	void InitHooks()
	{
		SafeWrite32(0xD2F100, UInt32(NiWindow__UpdateCamera));
	}
}

int __stdcall OnMasterFileNotMatchedPopupSkipIfVersionControlDisabled(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	if (!*(byte*)0xED045C) // bUseVersionControl:General
	{
		return IDNO;
	}
	return MessageBoxA(hWnd, lpText, lpCaption, uType);
}

bool bSelectedFormsListInUse;
tList<TESForm> selectedForms;

TESForm* __cdecl OnCloseSelectFormsDialogPopulateList(HWND hWnd)
{
	if (!hWnd)
	{
		return 0;
	}
	unsigned int index = SendMessageA(hWnd, LVM_GETNEXTITEM, 0xFFFFFFFF, LVIS_SELECTED);

	auto result = GetNthListForm(hWnd, index);
	if (result && bSelectedFormsListInUse)
	{
		selectedForms.AddAt(result, -2);
		do
		{
			auto startIndex = index;
			index = SendMessageA(hWnd, LVM_GETNEXTITEM, startIndex, LVIS_SELECTED);

			if (auto form = GetNthListForm(hWnd, index))
			{
				selectedForms.AddAt(form, -2);
			}
		} while (index != -1);
	}
	
	return result;
}

TESForm* __cdecl OnMediaLocationControllerSelectForm(HWND hWndParent, TESForm* form, tList<TESForm>* list)
{
	selectedForms.RemoveAll();
	bSelectedFormsListInUse = true;
	auto result = CdeclCall<TESForm*>(0x485690, hWndParent, form, list);
	bSelectedFormsListInUse = false;

	auto listView = *(HWND*)0xED9884;

	auto iter = selectedForms.Head();
	do
	{
		if (auto form = iter->item)
		{
			if (CdeclCall<int>(0x41A0F0, listView, form) < 0)
			{
				CdeclCall(0x41A020, listView, form, 0, -1);
				int index = CdeclCall<int>(0x41A0F0, listView, form);
				CdeclCall(0x41A260, listView, index);
			}
		}
	} while (iter = iter->next);
	selectedForms.RemoveAll();
	return result;
}

void FixCommCtl32ScrollingBug()
{
	// stubs out ShouldScroll in comctl32 because it leads to a call to SetCursorPos which flings the mouse around
	if (auto commCtrl = (UInt32)GetModuleHandleA("comctl32.dll"))
	{
		UInt32 offset = 0x3BCB2;
		if (!memcmp((byte*)(commCtrl + offset), "\x08\x00\x00\x20\x00\x74\x1F", 7))
		{
			UInt32 patchAddr = commCtrl + offset - 0x11;
			if (!memcmp((byte*)(patchAddr), "\x8B\xFF\x55\x8B\xEC", 5))
			{
				SafeWriteBuf(patchAddr, "\x33\xC0\xC2\x04\x00", 5);
			}
		}
	}
}

void __cdecl FormatRenderWindowMemoryUsage(char* DstBuf, size_t SizeInBytes, char* Format, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
	int bytesWritten = CdeclCall<int>(0x401190, DstBuf, SizeInBytes, Format, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
	CdeclCall<int>(0x401190, DstBuf + bytesWritten, SizeInBytes - bytesWritten, " (%d MB)", GetCurrentMemoryUsage() / (1024*1024));
}

void CleanupFaceGenMap()
{
	// 0x44F180 but without clearing the cell data, lights or navmesh

	CdeclCall(0x9DEC60, 0); // NiObjectGarbage::ClearAll
	CdeclCall(0x68C790); // BSFaceGenManager::ClearModelCache
	CdeclCall(0x68C8B0); // BSFaceGenManager::ClearModelStrings
	ThisCall(0x4C5250, *(UInt32**)0xED36A0, 0); // ModelLoader::ClearUnusedModels
	*(UInt32*)0xEB452C = 0x7FFFFFFF;
	TES::GetSingleton()->CleanUpUnusedTextures();
	CdeclCall(0x81AF10); // NiGlobalStringTable::FreeUnusedStrings
}

void __fastcall OnFaceGen_PeriodicCleanup(void* sfg)
{
	static int numNPCsProcessed;
	if (++numNPCsProcessed % 100)
	{
		CleanupFaceGenMap();
	}
	ThisCall(0x573480, sfg);
}

void __fastcall OnInitDebrisScale(float& pDebrisScale, void* edx, HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned)
{
	CdeclCall(0x41AB10, hDlg, nIDDlgItem, pDebrisScale, 4);
}

__declspec(naked) void OnInitDebrisScaleHook()
{
	_asm
	{
		lea ecx, [edi + 0x98]
		jmp OnInitDebrisScale
	}
}

__declspec(naked) void RetnGlobalDialogIDHook()
{
	_asm
	{
		 mov eax, 192
		 ret
	}
}

struct EditResponseData
{
	struct RecordingPopup
	{
		HWND hWnd;
	};

	UInt32 windowTitle;
	UInt32 maxResponseLength;
	UInt32 ptr08;
	UInt32 ptr0C;
	RecordingPopup* pRecordingPopup;
	tList<void> list14;
	UInt32 unk1C;
	UInt32 unk20;
	UInt32 unk24;
	UInt32 unk28;

	static EditResponseData* GetSingleton() { return *(EditResponseData**)0xED9214; };
};

void __fastcall StopSound_ResetRecordAudioPopupIfInvalid(HWND parent)
{
	CdeclCall(0x5CB360);
	
	if (auto data = EditResponseData::GetSingleton())
	{
		if (!data->pRecordingPopup || !IsWindow(data->pRecordingPopup->hWnd))
		{
			data->pRecordingPopup = (EditResponseData::RecordingPopup*)FormHeap_Allocate(sizeof(EditResponseData::RecordingPopup));
			ThisCall(0x42C910, data->pRecordingPopup, parent);
		}
	}
}

__declspec(naked) void StopSound_ResetRecordAudioPopupIfInvalidHook()
{
	_asm
	{
		mov ecx, edi
		jmp StopSound_ResetRecordAudioPopupIfInvalid
	}
}

class DialogExtraSubWindow : BSExtraData
{
	struct ExtraSubWindow
	{
		UInt32 unk00;
		UInt32 unk04;
		HWND parent;
		UInt32 _hInstance;
		UInt32 posX;
		UInt32 posY;
		HWND unkDlgItem;
		HWND dialogWindow;
	};

public:
	ExtraSubWindow* subWindow;
	UInt32 menuID;
};

bool __fastcall IsWeaponModSubViewActive(HWND hWnd)
{
	if (auto xSubWindow = CdeclCall< DialogExtraSubWindow*>(0x47AB70, hWnd, 4))
	{
		return xSubWindow->menuID != 3327;
	}
	return false;
}

__declspec(naked) void OnTESModelTextureSwapDialogCallbackHook()
{
	_asm
	{
		mov ecx, esi
		call IsWeaponModSubViewActive
		test al, al
		jne noSkip
		mov eax, 0x605E72
		jmp eax

	noSkip:
		mov edx, dword ptr ss : [esp + 0x14]
		mov eax, dword ptr ds : [edi + 0x2E8]
		mov ecx, 0x605D5D
		jmp ecx
	}
}

void __stdcall OnUpdateColor_Repaint(HWND hWnd, UINT Msg, WPARAM colorRectangleId, LPARAM lParam)
{
	SendMessageA(hWnd, Msg, colorRectangleId, lParam);
	PostMessageA(hWnd, WM_PAINT, colorRectangleId, NULL);
}

void PreserveTESFileTimeStamp(ModInfo* apFile, bool State)
{
	// credits to ShadeMe
	static FILETIME SavedTimestamp = { 0 };

	if (apFile == nullptr)
	{
		return;
	}

	char filePath[0x200]; *filePath = '\0';
	snprintf(filePath, sizeof(filePath), "%s\\%s", apFile->filepath, apFile->name);

	HANDLE SaveFile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, NULL, nullptr, OPEN_EXISTING, 0, nullptr);
	if (SaveFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (State)
	{
		ZeroMemory(&SavedTimestamp, sizeof(FILETIME));
		GetFileTime(SaveFile, nullptr, nullptr, &SavedTimestamp);
	}
	else
	{
		if (SavedTimestamp.dwHighDateTime || SavedTimestamp.dwLowDateTime)
		{
			SetFileTime(SaveFile, nullptr, nullptr, &SavedTimestamp);
		}
	}

	CloseHandle(SaveFile);
}

void SaveFileTime(ModInfo* apFile)
{
	PreserveTESFileTimeStamp(apFile, true);
}

void RestoreFileTime(ModInfo* apFile)
{
	PreserveTESFileTimeStamp(apFile, false);
}

void __fastcall PreSaveStoreFileTime(ModInfo* apFile)
{
	SaveFileTime(apFile);
	ThisCall(0x4DE610, apFile);
}

void __cdecl PostSaveRestoreFileTime(void* a1)
{
	CdeclCall(0x851810, a1);
	RestoreFileTime(DataHandler::GetSingleton()->activeFile);
}

extern HWND g_ConsoleHwnd;
void SaveWindowPositions() {
	SaveWindowPositionToINI(RenderWindow::GetWindow(), "Render Window");

	char buffer[8];

	WINDOWPLACEMENT pos;
	GetWindowPlacement(g_ConsoleHwnd, &pos);
	WritePrivateProfileString("Log", "iWindowPosX", _itoa(pos.rcNormalPosition.left, buffer, 10), IniPath);
	WritePrivateProfileString("Log", "iWindowPosY", _itoa(pos.rcNormalPosition.top, buffer, 10), IniPath);
	WritePrivateProfileString("Log", "iWindowPosDX", _itoa(pos.rcNormalPosition.right - pos.rcNormalPosition.left, buffer, 10), IniPath);
	WritePrivateProfileString("Log", "iWindowPosDY", _itoa(pos.rcNormalPosition.bottom - pos.rcNormalPosition.top, buffer, 10), IniPath);
}

void __fastcall FastExitHook(volatile LONG** thiss)
{
	SaveWindowPositions();
	if (GetINIExists() && config.bFastExit) TerminateProcess(GetCurrentProcess(), 0);
	ThisCall(0x4CC540, thiss);
}

void __stdcall BadFormPrintID(TESForm* form)
{
	Console_Print("FORMS: Bad form %08X could not be removed from the file.", form->refID);
}

_declspec(naked) void BadFormLoadHook()
{
	static const UInt32 retnAddr = 0x4D957C;
	_asm
	{
		push esi
		call BadFormPrintID

		//	originalCode
		mov byte ptr ss : [esp + 0x1A] , 0x1 // wasBadFormEncountered
		jmp retnAddr
	}
}

void __cdecl PrintScriptTypeChangedMessage(UInt32 script, UInt8 newType)
{
	UInt8 scriptType = *(UInt8*)(script + 0x3C);

	char** scriptTypes = (char**)0xE9BC6C;
	char* previousTypeStr = scriptTypes[scriptType];
	char* newTypeStr = scriptTypes[newType];

	char errorMsg[100];
	sprintf(errorMsg, "Script Type Changed, Previous: %s - New: %s", previousTypeStr, newTypeStr);
	MessageBoxA(nullptr, errorMsg, "Warning", MB_ICONWARNING);
}

_declspec(naked) void SaveScriptChangedType()
{
	_asm
	{
		push ecx
		push esi
		call PrintScriptTypeChangedMessage
		pop esi
		pop ecx
		mov byte ptr ss : [esp + 0x15] , 1
		ret
	}
}

BOOL __stdcall ScriptEditCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return StdCall<LRESULT>(0x5C3D40, hWnd, msg, wParam, lParam);
}

__declspec(naked) void NiTreeCtrl_CreateTreeRecursiveHook()
{
	_asm
	{
	loopIter:
		mov ecx, [eax + 8]
		test ecx, ecx
		je skip
		ret
	skip:
		mov eax, [eax]
		test eax, eax
		jne loopIter

		pop eax
		mov eax, 0x53013E
		jmp eax
	}
}

__declspec(naked) void NiTreeCtrl_CreateTreeRecursiveHook2()
{
	_asm
	{
		test edi, edi
		je skip
		mov eax, [edi]
		ret
	skip:
		pop eax
		mov ebp, [ebp]
		mov eax, 0x5304FD
		jmp eax
	}
}

__declspec(naked) void NiAVObject_GetViewerStringsHook()
{
	_asm
	{
		test edi, edi
		je skip
		mov eax, [edi]
		ret
	skip:
		pop eax
		mov ebp, [ebp]
		mov eax, 0x80F13A
		jmp eax
	}
}

bool bInSetupDetailsText;
void __cdecl OnLoadEspEsmSetupDetailsText(HWND hDlg, ModInfo* file)
{
	bInSetupDetailsText = true;
	CdeclCall(0x432730, hDlg, file);
	bInSetupDetailsText = false;
}

__declspec(naked) void OnLoadEspEsmSetSummaryModifiedHook()
{
	_asm
	{
		cmp bInSetupDetailsText, 0
		jne skip

		mov byte ptr ds : [0xECF5C2], 1
	skip:
		ret
	}
}

void __fastcall OnObjectWindowNew(int* pFormType, HWND hDlg)
{
	bool bInAllForms = *pFormType == 0;
	if (bInAllForms)
	{
		auto listView = GetDlgItem(hDlg, 1041);
		if (auto form = CdeclCall<TESForm*>(0x41A390, listView))
		{
			*pFormType = form->typeID;
		}
	}
	ThisCall(0x437840, pFormType);

	if (bInAllForms)
	{
		*pFormType = 0;
	}
}

__declspec(naked) void OnObjectWindowNewHook()
{
	_asm
	{
		mov edx, ebp
		jmp OnObjectWindowNew
	}
}

WNDPROC originalObjectWindowListViewProc;
LRESULT CALLBACK ObjectWindowListViewCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_KEYDOWN)
	{
		if (GetAsyncKeyState(VK_CONTROL) < 0)
		{
			if (wParam == 'G')
			{
				FormSearch::Show();
				return TRUE;
			}
			else if (wParam == 'F')
			{
				SetFocus(GetDlgItem(GetParent(Hwnd), 2557));
				return TRUE;
			}
		}
		else if (wParam == VK_RETURN)
		{
			SInt32 index = SendMessageA(Hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			struct
			{
				UInt32 pad[3];
				SInt32 iItem;
				SInt32 iSubItem;
			} item;
			item.iItem = index;
			item.iSubItem = 0;

			int** nodeDatas = (int**)0xED0778;
			int iCurrentNode = *(int*)0xED0770;
			ThisCall(0x437540, nodeDatas[iCurrentNode], &item);
			return TRUE;
		}
	}
	return CallWindowProc(originalObjectWindowListViewProc, Hwnd, Message, wParam, lParam);
}

WNDPROC originalObjectWindowFilterFieldProc;
LRESULT CALLBACK ObjectWindowFilterFieldCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		if (wParam == 'A')
		{
			SendMessage(Hwnd, EM_SETSEL, 0, -1);
			return TRUE;
		}
		else if (wParam == 'F')
		{
			SetFocus(GetDlgItem(GetParent(Hwnd), 1041));
			return TRUE;
		}
	}
	return CallWindowProc(originalObjectWindowFilterFieldProc, Hwnd, Message, wParam, lParam);
}

void RemoveSubMenuByHandle(HMENU parentMenu, HMENU submenuHandle) {
	UINT itemCount = GetMenuItemCount(parentMenu);
	for (UINT i = 0; i < itemCount; i++) {
		HMENU hSubMenu = GetSubMenu(parentMenu, i);
		if (hSubMenu == submenuHandle) {
			// Found the submenu by its HMENU, now remove it
			RemoveMenu(parentMenu, i, MF_BYPOSITION);
			DestroyMenu(submenuHandle);
			break;
		}
	}
}

constexpr UInt32 COPY_MENU_ID = 0x10001;
constexpr UInt32 COPY_EDITOR_ID_MENUID = 0x10002;
constexpr UInt32 COPY_REF_ID_MENUID = 0x10003;
constexpr UInt32 COPY_XEDIT_ID_MENUID = 0x10004;
void __cdecl OnSetupObjectWindowRightClickMenu(HMENU menu, LPPOINT cursorPos, HWND hWnd, HWND listView)
{
	HMENU hCopySubMenu = CreatePopupMenu();

	InsertMenuA(hCopySubMenu, 0xFFFFFFFF, MF_BYPOSITION, COPY_EDITOR_ID_MENUID, "Copy EditorID");
	InsertMenuA(hCopySubMenu, 0xFFFFFFFF, MF_BYPOSITION, COPY_REF_ID_MENUID, "Copy RefID");
	InsertMenuA(hCopySubMenu, 0xFFFFFFFF, MF_BYPOSITION, COPY_XEDIT_ID_MENUID, "Copy xEdit ID");

	InsertMenuA(menu, 0xFFFFFFFF, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hCopySubMenu, "Copy");

	CdeclCall(0x47F3B0, menu, cursorPos, hWnd, listView);

	RemoveSubMenuByHandle(menu, hCopySubMenu);
}

void CopySelectedListViewItemData(HWND listView, std::function<void(std::string&, TESForm*)> aggregator)
{
	int index = -1;
	std::string aggregatedText;

	// Iterate over all selected items
	while ((index = SendMessageA(listView, LVM_GETNEXTITEM, index, LVNI_SELECTED)) != -1)
	{
		if (auto form = GetNthListForm(listView, index))
		{
			aggregator(aggregatedText, form);
		}
	}

	// Remove the last newline if there is one
	if (!aggregatedText.empty() && aggregatedText.back() == '\n')
	{
		aggregatedText.pop_back();
	}

	// Copy the aggregated text to the clipboard
	if (!aggregatedText.empty())
	{
		CopyTextToClipboard(aggregatedText.c_str());
	}
}

WNDPROC originalObjectWindowCallback;
LRESULT CALLBACK ObjectWindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_INITDIALOG)
	{
		HWND listView = GetDlgItem(Hwnd, 1041);
		if (!originalObjectWindowListViewProc)
		{
			originalObjectWindowListViewProc = (WNDPROC)SetWindowLongPtr(listView, GWLP_WNDPROC, (LONG_PTR)ObjectWindowListViewCallback);
		}
		HWND filterField = GetDlgItem(Hwnd, 2557);
		if (!originalObjectWindowFilterFieldProc)
		{
			originalObjectWindowFilterFieldProc = (WNDPROC)SetWindowLongPtr(filterField, GWLP_WNDPROC, (LONG_PTR)ObjectWindowFilterFieldCallback);
		}
	}
	else if (Message == WM_COMMAND)
	{
		if (wParam == COPY_EDITOR_ID_MENUID)
		{
			HWND listView = GetDlgItem(Hwnd, 1041);
			CopySelectedListViewItemData(listView, [](std::string& currentText, TESForm* form)
				{
					currentText += form->GetEditorID();
					currentText += "\n";
				}
			);
		}
		else if (wParam == COPY_REF_ID_MENUID)
		{
			HWND listView = GetDlgItem(Hwnd, 1041);
			CopySelectedListViewItemData(listView, [](std::string& currentText, TESForm* form)
				{
					currentText += std::format("{:X}\n", form->refID);
				}
			);
		}
		else if (wParam == COPY_XEDIT_ID_MENUID)
		{
			HWND listView = GetDlgItem(Hwnd, 1041);
			CopySelectedListViewItemData(listView, [](std::string& currentText, TESForm* form)
				{
					// e.g. DLC03LvlEnclaveSquad01 "Enclave Squad Sigma" [NPC_:090057BE]
					auto editorID = form->GetEditorID();
					auto formName = form->GetTheName();
					auto formTypeNames = (const char**)0xE94404;
					auto formTypeName = formTypeNames[form->typeID * 3];
					auto refID = form->refID;

					currentText += std::format("{} \"{}\" [{}:{:08X}]\n", editorID, formName, formTypeName, refID);
				}
			);
		}
	}
	return CallWindowProc(originalObjectWindowCallback, Hwnd, Message, wParam, lParam);
}

void __fastcall OnMainWindowUndo(HistoryManager* historyMgr, void* edx, int formal)
{
	if (NavMeshManager::IsActive())
	{
		NavMeshManager::GetSingleton()->Undo();
	}
	else
	{
		historyMgr->Undo();
	}
}

void __cdecl OnTextViewLoadCell(NiPoint3* pos, TESObjectCELL* cell)
{
	CdeclCall(0x465490, pos, cell);
	if (!cell->IsInterior())
	{
		cell->GetLandHeight(pos, &pos->z);
		CdeclCall(0x4652D0, pos);
	}
}

int __fastcall OnScriptConfirmClose(Script* script, HWND textEdit, HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	if (!*script->GetEditorID() && !GetWindowTextLengthA(textEdit))
	{
		return IDNO;
	}
	return MessageBoxA(hWnd, lpText, lpCaption, uType);
}

__declspec(naked) void OnScriptConfirmCloseHook()
{
	_asm
	{
		mov ecx, esi // script
		mov edx, ebp // textEdit
		jmp OnScriptConfirmClose
	}
}

void __stdcall DestroyWindowOrDialog(HWND hWnd)
{
	// ensure dialogs are closed with EndDialog instead of DestroyWindow
	if (IsDialog(hWnd))
	{
		EndDialog(hWnd, 0);
	}
	else
	{
		DestroyWindow(hWnd);
	}
}

void __stdcall DestroySelectedWindowOrDialog(HWND hWnd)
{
	// fix the open window not getting removed from the windows list after getting destroyed...
	auto openWindowsList = OpenWindows::GetWindowList();
	openWindowsList->Remove((HWND*)hWnd);

	DestroyWindowOrDialog(hWnd);
}

__declspec(naked) void OnDestroyAllWindowsHook()
{
	_asm
	{
		push esi
		push 0x477780
		jmp DestroyWindowOrDialog
	}
}

void __stdcall OnTAllocZeroMemory(int* pMemory, int size, int numAllocs, void* initFn, void* destroyFn)
{
	memset(pMemory, 0, 4 * numAllocs);
}

void PlayMouseClickSound()
{
	PlaySound("MouseClick", NULL, SND_ASYNC);
}

BOOL __fastcall OnShowSearchAndReplaceWindow(char* buffer, void* edx, HWND hWnd, int nCmdShow)
{
	auto scrapHeap = ThisCall<void*>(0x854540, (void*)0xF21B5C); // HeapManager::GetThreadScrapHeap
	ThisCall(0x855B30, scrapHeap, buffer); // ScrapHeapFree
	return ShowWindow(hWnd, nCmdShow);
}

__declspec(naked) void OnShowSearchAndReplaceWindowHook()
{
	_asm
	{
		mov ecx, [esp + 0x24] // buffer
		jmp OnShowSearchAndReplaceWindow
	}
}

void __cdecl OnInsertRenderPreferencesComboHotkey(HWND hWnd, const CHAR* text, LPARAM item, char bUpdateExtents)
{
	CdeclCall(0x419BC0, hWnd, text, item, bUpdateExtents);
	CdeclCall(0x419BC0, hWnd, "Z", 8, bUpdateExtents);
}

__declspec(naked) void OnSelectRenderPreferencesComboHook()
{
	_asm
	{
		test bl, 8
		je done

		neg ebp
		sbb ebp, ebp
		push 8
		add ebp, 0x14AA
		push ebp
		push esi
		call edi
		push eax
		mov eax, 0x419DD0
		call eax
		add esp, 8

	done:
		mov edx, dword ptr ss : [esp + 0x18]
		movzx eax, byte ptr ds : [edx + 0xE]
		mov ecx, 0x412E76
		jmp ecx
	}
}

void __cdecl RenderPreferences_PostPopulateList(HWND hWnd)
{
	CdeclCall(0x412D40, hWnd);
	auto hDlg = GetDlgItem(hWnd, 5142);
	auto sortColumn = GetWindowLongA(hDlg, -21);
	SendMessageA(hDlg, LVM_SORTITEMS, sortColumn, (LPARAM)0x412C10); // RenderWindowHotkeys::CompareFn
}

void __cdecl OnInitRenderWindowComboBox(HWND hWnd, const CHAR* text, LPARAM item, char bUpdateExtents)
{
	CdeclCall(0x419BC0, hWnd, text, item, bUpdateExtents);

	short c = 0;
	c = MapVirtualKeyA(VK_OEM_4, MAPVK_VK_TO_CHAR);
	CdeclCall(0x419BC0, hWnd, &c, VK_OEM_4, bUpdateExtents);
	c = MapVirtualKeyA(VK_OEM_5, MAPVK_VK_TO_CHAR);
	CdeclCall(0x419BC0, hWnd, &c, VK_OEM_5, bUpdateExtents);

	for (short c : { ';', '\'', ',', '.', '/' })
	{
		CdeclCall(0x419BC0, hWnd, &c, c, bUpdateExtents);
	}
	CdeclCall(0x419BC0, hWnd, "<TILDE>", '`', bUpdateExtents);
}