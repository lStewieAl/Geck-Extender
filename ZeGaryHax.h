#pragma once
#pragma comment(lib, "libdeflate/libdeflatestatic.lib")

#include "Editor.h"
#include "resource.h"

#define GH_NAME				"ZeGaryHax"		// this is the string for IsPluginInstalled and GetPluginVersion (also shows in nvse.log)
#define GH_VERSION			0				// set this to 0 to enable log output from _DMESSAGE (useful for debug traces)

HMODULE ZeGaryHaxHandle;

IDebugLog		gLog("EditorWarnings.log");

void PrintCmdTable();

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
int bShowLoadFilesAtStartup = 0;
int bScriptCompileWarningPopup = 0;

int bAltShiftMovementMultipliers = 1;
float fMovementAltMultiplier = 0.15F;
float fMovementShiftMultiplier = 2.0F;


int bSmoothFlycamRotation = 1;
float fFlycamRotationSpeed;
float fFlycamNormalMovementSpeed;
float fFlycamShiftMovementSpeed;
float fFlycamAltMovementSpeed;

char filename[MAX_PATH];
static const char *geckwikiurl = "https://geckwiki.com/index.php/";
static const char *geckwikiscriptingurl = "https://geckwiki.com/index.php/Category:Scripting";
static const char *geckwikicommandsurl = "https://geckwiki.com/index.php/Category:Commands";
static const char *geckwikifunctionsurl = "https://geckwiki.com/index.php/Category:Functions";

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

static void DoModScriptWindow(HWND wnd)
{
	SendMessage(wnd, EM_EXLIMITTEXT, 0, 0x00FFFFFF);

	GetPrivateProfileStringA("Script", "Font", "Consolas", editorFont.lfFaceName, 31, filename);
	editorFont.lfHeight = GetPrivateProfileIntA("Script", "FontSize", 13, filename);
	editorFont.lfWeight = GetPrivateProfileIntA("Script", "FontWeight", FW_MEDIUM, filename);

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

void doKonami(int);

/* small konami easter egg */
BOOL __stdcall hk_LoadESPESMCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NOTIFY && ((LPNMHDR)lParam)->code == LVN_KEYDOWN) {
		doKonami(((LPNMLVKEYDOWN)lParam)->wVKey);
	}
	return ((BOOL(__stdcall *)(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam))(0x432A80))(hDlg, msg, wParam, lParam);
}

BOOL __stdcall hk_SearchAndReplaceCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	return ((BOOL(__stdcall *)(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam))(0x47C990))(hDlg, msg, wParam, lParam);
}

_declspec(naked) void EndLoadingHook() {
	PlaySound("MouseClick", NULL, SND_ASYNC);
//	PrintCmdTable();
	_asm {
	originalCode:
		add esp, 8
			pop esi
			retn
	}
}

// hooks before movement speed is determined for flycam mode
static int lastFlycamTime = 0;
_declspec(naked) void FlycamMovementSpeedMultiplierHook() {
	static const UInt32 retnAddr = 0x455D17;
	*(float*)(0xED12C0) = fFlycamNormalMovementSpeed;

	if (GetAsyncKeyState(VK_SHIFT) < 0) {
		*(float*)(0xED12C0) *= fFlycamShiftMovementSpeed;
	}

	if (GetAsyncKeyState(VK_MENU) < 0) {
		*(float*)(0xED12C0) *= fFlycamAltMovementSpeed;
	}

	/* fix flycam speed being dependent on framerate by slowing down movement if framerate is greater than 30fps (33ms/frame)*/
	if (GetTickCount() - lastFlycamTime < 33) {
		*(float*)(0xED12C0) *= (GetTickCount() - lastFlycamTime)/33.0;
	}

	lastFlycamTime = GetTickCount();
	
	_asm {
	originalCode:
		mov     eax, dword ptr ds:[0xF1FBF4]
		jmp retnAddr
	}
}

// edi       : difference in x pos, 
// esp+0x324 : difference in y pos
// Hook to make flycam rotation speed based on x/y position difference rather than 1.0 for any difference
_declspec(naked) void FlycamRotationSpeedMultiplierHook() {
	static const UInt32 retnAddr = 0x45D3F8;
	_asm {
		mov dword ptr ds : [0x00ED140C], ebx
		
		push edi
		fild dword ptr ss : [esp]
		add esp, 4
		
		fmul fFlycamRotationSpeed
		fstp dword ptr ss : [esp + 0xF0]

		fild dword ptr ss : [esp+0x324]
		fmul fFlycamRotationSpeed
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



void __cdecl SaveWindowPositionToINI(HWND hWnd, char* name) {
	((void(__cdecl*)(HWND hWnd, char* Src))(0x43E170))(hWnd, name);
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
		
		cmp byte ptr ds:[esp+0x1F], 0 // check if control pressed
		jz notSave
		
		jmp saveAddr 
		
	skip:
		jmp skipAddr
	notSave:
		jmp retnAddr
	}
}

bool __fastcall ScriptEdit__Save(byte* thiss, void* dummyEDX, HWND hDlg, char a3, char a4) {
	bool saveSuccess = ((bool (__thiscall *)(byte* thiss, HWND hDlg, char a3, char a4))(0x5C2F40))(thiss, hDlg, a3, a4);
	if (saveSuccess) {
		SendDlgItemMessageA(hDlg, 1166, EM_SETMODIFY, 0, 0);
		
		// remove the '*' from the end of the window name
		char windowName[MAX_PATH];
		int len = GetWindowTextA(hDlg, windowName, MAX_PATH)-1;

		if (len > 2 && windowName[len] == '*') {
			windowName[len-1] = '\0'; // strip the '* '
			SetWindowTextA(hDlg, windowName);
		}
	}
	return saveSuccess;
}

double GetRefSpeedMultiplier() {
	double multiplier = 1.0F;
	if (GetAsyncKeyState(VK_SHIFT) < 0) {
		multiplier *= fMovementShiftMultiplier;
	}
	if (GetAsyncKeyState(VK_MENU) < 0) {
		multiplier *= fMovementAltMultiplier;
	}
	return multiplier;
}

char __cdecl hk_DoRenderPan(int a1, int a2, float a3) {
	if (GetAsyncKeyState(VK_MENU) < 0) {
		a3 *= fMovementAltMultiplier;
	}
	return ((char(__cdecl*)(int a1, int a2, float a3))(0x464210))(a1, a2, a3);
}

char __cdecl hk_DoRenderMousewheelScroll(int a1, int a2, float a3) {
	return ((char(__cdecl*)(int a1, int a2, float a3))(0x464210))(a1, a2, a3 * GetRefSpeedMultiplier());
}

double __fastcall hk_CalculateVectorMagnitude(float* vector, void* dummyEDX) {
	return ((double(__thiscall*) (float* thiss))(0x40B3D0))(vector) * GetRefSpeedMultiplier();
}

/* multiply the reference movement speed dependent */
_declspec(naked) void RenderWindowReferenceMovementSpeedHook() {
	static const UInt32 retnAddr = 0x455398;
	_asm {
		pushad
		call GetRefSpeedMultiplier
		fmul
		popad

originalCode:
		fmul dword ptr ds:[0xECFD1C]
		jmp retnAddr
	}
}

void __fastcall FastExitHook(volatile LONG** thiss);