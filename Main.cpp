#include <string>
#include <Richedit.h>
#include <CommCtrl.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <set>
#include <regex>
#include <libdeflate/libdeflate.h>
#include <unordered_set>
#include <filesystem>

#include "nvse/GameData.h"
#include "nvse/PluginAPI.h"
#include "nvse/GameForms.h"
#include "nvse/GameObjects.h"
#include "nvse/GameRTTI.h"
#include "nvse/ParamInfos.h"
#include "nvse/nvse_version.h"
#include "xutil.h"
#include "Detours.h"

#include "Main.h"
#include "ExtensionsMenu.h"
#include "WarningPatches.h"
#include "WarningHooks.h"
#include "Editor.h"
#include "UISpeedHooks.h"
#include "ObjectWindowTreeHooks.h"
#include "DebugCellShaders.h"
#include "DialogResizing.h"
#include "CreatureMarkerSwapper.h"
#include "EasterEggs.h"
#include "OutOfMemoryHelper.h"
#include "BetterFloatingFormList.h"
#include "EditorUIDarkMode.h"
#include "Settings.h"
#include "NavMeshPickPreventer.h"
#include "CustomRenderWindowHotkeys.h"
#include "LaunchGame.h"
#include "FaceGenExporter.h"
#include "ONAMFix.h"
#include "SCOLConsistencyFix.h"
#include "MultiBoundsAdder.h"
#include "UnserializedIO.h"
#include "ToggleReferenceMovement.h"
#include "Allocator/MemoryManager.hpp"
#include "Allocator/BSMemory.hpp"

#include "Events/EventManager.h"
#include "Events/Events.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "PrintReplacer.hpp"

BS_ALLOCATORS

extern "C"
{
	BOOL WINAPI DllMain(HANDLE  hDllHandle, DWORD dwReason, LPVOID lpreserved)
	{
		if (dwReason == DLL_PROCESS_ATTACH)
		{
			ZeGaryHaxHandle = (HMODULE)hDllHandle;
		}
		return TRUE;
	}
}

bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = GH_NAME;
	info->version = GH_VERSION;
	UInt32 requiredVersion = nvse->isEditor ? 5 : NVSE_VERSION_INTEGER;
	return nvse->nvseVersion >= requiredVersion;
}

void CreateLogFile()
{
	// ensures the log file is created beside the EXE instead of the working directory...
	char logPath[MAX_PATH];
	GetModuleFileNameA(NULL, logPath, MAX_PATH);

	char* last_slash = strrchr(logPath, '\\');

	strncpy(last_slash, "\\EditorWarnings.log", sizeof(logPath) - (last_slash - logPath) - 1);
	gLog.Open(logPath);
}

bool NVSEPlugin_Load(const NVSEInterface* nvse)
{
	if (!nvse->isEditor)
	{
		LaunchGame::OnGamePluginLoad(nvse);
		return true;
	}

	PrintReplacer::InitHooks();

	UnserializedIO::InitHooks();

	CreateLogFile();

	_DMESSAGE("Geck Extender Base Address: %08X", GetModuleHandle("ZeGaryHax.dll"));
	ReadAllSettings();

	// Increase heap size and add memory pools for a hefty performance boost
	ReplaceCallEx(0x853BF1, &MemoryManager::Initialize);

	EventManager::InitHooks();

	DisableProcessWindowsGhosting();

	//	stop geck crash with bUseMultibounds = 0 in exterior cells with multibounds - credit to roy
	WriteRelCall(0x004CA48F, (UInt32)FixMultiBounds);
	XUtil::PatchMemoryNop(0x004CA494, 0x05);

	// fix various file path vtable entries to exclude the "//" in the path - credit to roy
	SafeWrite32(0x00D39FB0, 0x004FE910);	// skills icon in actor values
	SafeWrite32(0x00D47E28, 0x004FE910);	// landscape (and texture sets?)
	SafeWrite32(0x00D527D0, 0x004FE910);	// class image
	SafeWrite32(0x00D53980, 0x004FE910);	// eyes
	SafeWrite32(0x00D54050, 0x004FE910);	// hair
	SafeWrite32(0x00D58330, 0x004FE910);	// skills (not sure where this is viewable)
	SafeWrite32(0x00D5DFD8, 0x004FE910);	// loading screen
	SafeWrite32(0x00D7BED8, 0x004FE910);	// worldspace map

	SafeWrite32(0x00D73EE0, UInt32(SpeedTreeGetTexturePath));	// destructable tree (not sure where this is viewable)
	SafeWrite32(0x00D73F68, UInt32(SpeedTreeGetTexturePath));	// tree

	//	enable wasteland level 2 lod generation - credit to roy
	SafeWrite8(0x00419557, 0xEB);

	//	enable 512px landscape LOD textures and normals - credit to roy
	if (config.bHighResLandscapeLOD)
	{
		SafeWrite8(0x00728A3C, 0x02);
		SafeWrite8(0x00728A41, 0x02);
		SafeWrite8(0x0072971B, 0x02);
		SafeWrite8(0x00729720, 0x02);
		SafeWrite8(0x0072F090, 0x02);
		SafeWrite8(0x0072F095, 0x02);
	}

	// increase landscape edit config max icon size to 4096x4096 from 512x512
	SafeWrite8(0x448AA3, 0x10);

	//	change help urls to our wiki - credit to roy
	SafeWrite32(0x00441952, (UInt32)geckwikiurl);
	SafeWrite32(0x005C4B0F, (UInt32)geckwikiscriptingurl);
	SafeWrite32(0x005C4B29, (UInt32)geckwikicommandsurl);
	SafeWrite32(0x005C4B43, (UInt32)geckwikifunctionsurl);

	//	force patch script editor font - credit to nvse team
	if (config.bPatchScriptEditorFont)
	{
		FixEditorFont();
	}

	//	expand scriptlet box size - credit to roy and luthien
	SafeWrite32(0x0059B3FB, 0x7FFF);	// 2650
	SafeWrite32(0x0059B40F, 0x7FFF);	// 1445
	SafeWrite32(0x00582FA4, 0x7FFF);	// 1444

	//	double script size limit - credit to jazzisparis
	SafeWrite8(0x005C56B6, 0x7F);
	SafeWrite8(0x005C56D3, 0x7F);
	SafeWrite8(0x005C92F6, 0x7F);

	//	fast exit - credit to roy - tweaks by jazzisparis
	WriteRelCall(0x00440A4F, (UInt32)FastExitHook);

	//	enable big guns for weapon skill
	SafeWrite32(0x492D0C, 0x410);

	//	uncap framerate on render window - credit to shademe
	if (config.bRenderWindowUncap)
	{
		SafeWrite8(0x0045911B, 0x0A);
	}

	//	uncap framerate on preview window - credit to shademe
	if (config.bPreviewWindowUncap)
	{
		SafeWrite8(0x004100E5, 0x0A);
	}

	//	window handle leak fix - credit to nukem
	//	now uses thread local storage to fix parent issues
	SafeWrite32(0x00D234CC, (UInt32)hk_CreateDialogParamA);
	SafeWrite32(0x00D23510, (UInt32)hk_DialogBoxParamA);
	SafeWrite32(0x00D23530, (UInt32)hk_EndDialog);
	SafeWrite32(0x00D23550, (UInt32)hk_SendMessageA);

	//	fix WM_CLOSE in destruction data dialog - credit to roy
	SafeWrite32(0x004E640F, (UInt32)hk_DialogProc);

	//  removes sound filters that cause dailog window to take ages to load - credit to iranrmrf
	if (config.bRemoveDialogSoundFilter)
	{
		SafeWriteBuf(0x0058E4F6, "\x83\xC4\x10\xB0\x01", 5);
		SafeWriteBuf(0x0058E510, "\x83\xC4\x10\xB0\x01", 5);
	}

	if (config.bCacheComboboxes)
	{
		WriteRelCall(0x0057E5E1, (UInt32)hk_QuestWindowLoad);
	}

	if (config.bListEditFix)
	{
		//	fix windows 8/10 conditions collapsed column bug - credit to nukem
		WriteRelCall(0x004A382D, (UInt32)ConditionDataDialog_SetupColumnHeaders);
		WriteRelCall(0x00765214, (UInt32)ConditionDataDialog_SetupColumnHeaders);
		WriteRelCall(0x00766A4D, (UInt32)ConditionDataDialog_SetupColumnHeaders);
		WriteRelCall(0x0076E195, (UInt32)ConditionDataDialog_SetupColumnHeaders);
		WriteRelCall(0x004A2791, (UInt32)ConditionDataDialog_SetupColumnHeaders);
	}

	if (config.bExpandFormIDColumn)
	{
		// fix FormID column being collapsed
		WriteRelJump(0x42EFBB, UInt32(CellViewListViewCreateFormIDColumnHook));
		WriteRelJump(0x44965A, UInt32(ObjectWindowListViewColumnSizeHook));
		WriteRelCall(0x4682FE, UInt32(InsertListViewHeaderSetSizeHook));

		// set default object window formID column width
		SafeWrite16(0xE8E5C2, 0x42);
	}

	//	ignore .nam Files - initial credit to roy
	if (config.bIgnoreNAMFiles)
	{
		SafeWriteBuf(0xD412C1, "GAL", 3); // replace NAM extension with GAL ('Geck Auto-Load')
	}

	if (config.bVersionControlMode == 0)
	{
		//	enable esm as active file - credit to hlp
		XUtil::PatchMemoryNop(0x00430F07, 0x09);
		XUtil::PatchMemoryNop(0x004CF429, 0x09);

		//	fix crash
		WriteRelCall(0x0042CD31, (UInt32)hk_addr_42CD31);
		XUtil::PatchMemoryNop(0x0042CD36, 0x03);

		//	patch formid
		SafeWrite16(0x004DF7D8, 0x9090);

		//	patch esp as master
		XUtil::PatchMemoryNop(0x004D9CA5, 0x09);

		//	patch ONAM count
		WriteRelCall(0x004E26BA, (UInt32)hk_addr_4E26BA);
		XUtil::PatchMemoryNop(0x004E26BF, 0x0B);

		// enable editing file author and summary in an ESM
		SafeWrite8(0x00432776, 0xEB);
		SafeWrite8(0x004327BE, 0xEB);
	}

	//	fix Rock-It Launcher crash - credit to jazzisparis
	WriteRelJump(0x005B8FF4, (UInt32)CheckIsRILHook);

	WriteErrorMessageHooks();

	//	fix %s:%d: %s : %s warning message crashes - credit to roy and nukem
	SafeWrite16(0x006794F9, 0x9090);
	SafeWrite8(0x00679502, 0x14);

	SafeWrite16(0x006794C9, 0x9090);
	SafeWrite8(0x006794D2, 0x14);

	SafeWrite16(0x006794F9, 0x9090);
	SafeWrite8(0x00679502, 0x14);

	//	Suppress old bhkrigidbody cinfo warning that's useless - credit to roy
	SafeWrite16(0x006796CD, 0x9090);

	//	Spell Checker hack - credit to roy
	WriteRelJump(0x0041B60D, (UInt32)hk_SpellCheck);
	SafeWrite8(0x0041B612, 0x90);

	//	Splash Screen Hax - credit to nukem and roy
	WriteRelCall(0x4463CD, UInt32(SplashScreenHook));
	SafeWrite8(0x4463CD + 5, 0x90);

	//	About Dialog Hax - credit to nukem and roy
	WriteRelCall(0x00441976, UInt32(AboutDialogHook));
	SafeWrite8(0x00441976 + 5, 0x90);

	//	Patch some SendMessageA to speed up loading - credit to nukem
	XUtil::PatchMemoryNop(0x004DC191, 0x05);
	XUtil::PatchMemoryNop(0x004DC641, 0x05);
	XUtil::PatchMemoryNop(0x004DC91C, 0x05);

	//	nop useless calls to _vsprintf and SendMessage for 'Processing topic %s' - credit to roy/nukem
	XUtil::PatchMemoryNop(0x00591CCE, 0x24);
	XUtil::PatchMemoryNop(0x00592F08, 0x3F);
	SafeWriteBuf(0x592F4A, "\x10\x90\x90\x90", 4); // above removes pushed args, so need to change mov offset and nop the add esp
	XUtil::PatchMemoryNop(0x004084D2, 0x05); // prevent "Adding topic %s" when loading Dialogue Tree

	//	Make search and replace window stay open unless explicitly close - credit to StewieA
	if (config.bAllowMultipleSearchAndReplace) {
		WriteRelJump(0x47CE7F, (UINT32)hk_SearchDestroyHook);
		SafeWrite8(0x47CE7F + 5, 0x90);
	}

	if (config.bCacheSearchAndReplaceWindow)
	{
		WriteRelCall(0x44118B, UInt32(OnCreateSearchAndReplaceWindow));
		SafeWrite8(0x44118B + 5, 0x90);

		WriteRelCall(0x47CE86, UInt32(OnDestroySearchAndReplaceWindow));
		SafeWrite8(0x47CE86 + 5, 0x90);
	}

	// Make the "Do" button of "Reference Batch Action" not grayed out if an action is already selected when initialising the dialog
	WriteRelJump(0x411CCF, UInt32(ReferenceBatchActionDoButtonHook));

	// Remove call to SetFocus(0) when closing Reference Batch Action dialog
	XUtil::PatchMemoryNop(0x411CFA, 8);

	//	Replace zlib with libinflate - credit to nukem/StewieA
	if (config.bLibdeflate)
	{
		WriteRelCall(0x004DFB34, (UInt32)hk_inflateInit);
		WriteRelCall(0x004DFB9E, (UInt32)hk_inflate);
	}

	//	Expand ini GetPrivateProfileString buffer to 1024 bytes - credit to jazzisparis
	SafeWrite8(0x009D3D01, 0x04);
	SafeWrite8(0x009D3D65, 0x04);
	SafeWrite8(0x009D3EE4, 0x04);
	SafeWrite8(0x009D3F4B, 0x04);
	SafeWrite8(0x009D4027, 0x04);
	SafeWrite8(0x009D3EEA, 0xF9);
	SafeWrite8(0x009D3F21, 0xF9);
	SafeWrite8(0x009D3F51, 0xF9);
	SafeWrite8(0x009D3F98, 0xF9);
	SafeWrite8(0x009D402D, 0xF9);
	SafeWrite8(0x009D407B, 0xF9);

	// stop the Preview Object window taking focus when hovered over (unless the parent window is focussed)
	if (config.bNoPreviewWindowAutoFocus) {
		WriteRelJump(0x48CAC9, UInt32(hk_PreviewWindowCheckForeground));
	}
	// no "DirectSoundCaptureCreate Error: No sound driver is available for use" popup
	if (config.bNoDXSoundCaptureErrorPopup) {
		WriteRelCall(0x42B6E0, UInt32(Console_Print));
	}

	if (config.bRemoveStatusBarLoadingText) {
		// patch additional loading messages to the bottom status bar
		XUtil::PatchMemoryNop(0x4DCC24, 5);
		XUtil::PatchMemoryNop(0x4DC96F, 5);
		XUtil::PatchMemoryNop(0x4DDD4F, 5);

		// make the cell loading bar only update every 128 references instead of every ref
		SafeWriteBuf(0x4BFFA2, "\xF7\xC3\x7F\x00\x00\x00\x74\x3C\xEB\x68", 10); // replaces a test ebx, ebx with test ebx, 0x7F
	}

	if (config.bPlaySoundEndOfLoading) {
		WriteRelJump(0x464D32, UInt32(EndLoadingHook));
	}

	// add a more detailed warning when recompiling all
	WriteRelJump(0x5C497E, UInt32(RecompileAllWarningScriptHook));
	WriteRelJump(0x4442C7, UInt32(RecompileAllWarningMainHook));

	// fix arrow keys in dialogue menu making the window become unfocussed, allowing consecutive arrow presses without having to click again
	SafeWrite16(0x5A1817, 0x0FEB); // jump over SetFocus(0)
	SafeWrite16(0x5993EC, 0x0FEB);

	// speed up UI
	WriteUIHooks();

	// hook Load ESP/ESM window callback
	originalLoadEspEsmFn = (WNDPROC)DetourVtable(0x44192A, UInt32(LoadEspEsmCallback));

	// make flycam mouse rotation grid smaller (1*1 instead of 2*2)
	// the game was checking if the difference in mouse position was > 1 rather than >= 1
	SafeWrite8(0x45D3AD, 0x7C);
	SafeWrite8(0x45D3BD, 0x7F);
	SafeWrite8(0x45D3D9, 0x7C);
	SafeWrite8(0x45D3EA, 0x7F);

	// make holding shift slow down the flycam movement by 80%
	WriteRelJump(0x455D12, UInt32(FlycamMovementSpeedMultiplierHook));

	if (config.bSmoothFlycamRotation) {
		WriteRelJump(0x45D3A3, UInt32(FlycamRotationSpeedMultiplierHook));
		WriteRelJump(0x456A9F, 0x456AE6); // skip call to SetCursorPos when changing to flycam mode
	}

	if (config.bSwapRenderCYKeys) {
		// set C as hotkey for restricting movement to Y direction
		SafeWrite8(0x462D8B, 0x6); // (patch a switch table offset)
		SafeWrite8(0x462F5B, 0xF); // (patch a switch table offset)

		// remove Y as hotkey for Y movement
		SafeWrite8(0x462DA1, 0x8);
		SafeWrite8(0x462F71, 0x17);

		// allow Y as a hotkey in render window preferences
		SafeWrite8(0x4136C1, 'Z');
	}

	/* Hook the Script Editor Window */
	SafeWrite32(0x437892, UInt32(ScriptEditCallback));
	SafeWrite32(0x441CB0, UInt32(ScriptEditCallback));
	SafeWrite32(0x509F6B, UInt32(ScriptEditCallback));
	SafeWrite32(0x5C50C8, UInt32(ScriptEditCallback));

	/* allow ctrl S to save */
	WriteRelJump(0x5C3ECD, UInt32(ScriptEditKeypressHook));

	/* fix vanilla bug where modified flag was not reset upon successful saving */
	WriteRelCall(0x5C4974, UInt32(ScriptEdit__Save));

	// Fix for crash (invalid parameter termination) when the "Unable to find variable" warning would exceed the buffer size, credits to nukem
	XUtil::PatchMemory(0xD59DCC, (PBYTE)", Text \"%.240s\"", strlen(", Text \"%.240s\"") + 1);

	if (config.bUseAltShiftMultipliers) {
		// Scroll wheel and pan speed affected by shift/alt
		WriteRelCall(0x48B8C5, UInt32(hk_DoRenderMousewheelScroll)); // preview window
		WriteRelCall(0x48B74C, UInt32(hk_DoRenderMousewheelScroll)); // preview window
		WriteRelCall(0x48B7AC, UInt32(hk_DoRenderMousewheelScroll)); // preview window pan
		WriteRelCall(0x46040E, UInt32(hk_DoRenderMousewheelScroll)); // render window scroll

		// reference movement speed affected by shift/alt
		WriteRelJump(0x455392, UInt32(RenderWindowReferenceMovementSpeedHook));

		// render window panning, doesn't apply shift multiplier as it conflicts with keybinding
		WriteRelCall(0x45F7E5, UInt32(hk_DoRenderPan));
		WriteRelCall(0x45F846, UInt32(hk_DoRenderPan));

		// orthographic zoom 
		WriteRelJump(0x45F661, UInt32(hk_OrthographicZoom));
		WriteRelJump(0x4602D8, UInt32(hk_OrthographicZoom2));

		// camera rotation with a selected ref
		WriteRelJump(0x45F5FB, UInt32(hk_RefCameraRotation));

		// ref rotation
		WriteRelJump(0x4523C2, UInt32(RenderWindowHandlesRefRotationHook));
	}

	if (config.bNoVersionControlWarning) {
		// Don't show "Version control is currently disabled. Would you like to continue without version control?" prompt
		SafeWrite8(0x43F854, 0xEB);
	}

	if (config.bAutoLoadFiles) {
		// makes the load files prompt jump to OK if it's the first time it's opened
		WriteRelJump(0x432CC2, UInt32(hk_LoadFilesInit));
	}

	// add Q/E keys for up/down movement in flycam mode for render window
	// if the movment is applied Post transform, it will be relative to the world
	if (config.bFlycamUpDownRelativeToWorld) {
		WriteRelJump(0x455DC9, UInt32(RenderWindowFlycamPostTransformMovementHook));
	}
	else {
		WriteRelJump(0x455DAB, UInt32(RenderWindowFlycamPreTransformMovementHook));
	}

	if (config.bShowTimeOfDaySlider) {
		// hook for setting main menu scrollbar position when changed in the preferences pane
		WriteRelCall(0x44DD69, UInt32(UpdateTimeOfDayScrollbarHook));
		XUtil::PatchMemoryNop(0x44DD69 + 5, 1);
		WriteRelCall(0x44DDF5, UInt32(UpdateTimeOfDayScrollbarHook));
		XUtil::PatchMemoryNop(0x44DDF5 + 5, 1);
		WriteRelCall(0x44D04B, UInt32(UpdateTimeOfDayInputBoxHook));
		XUtil::PatchMemoryNop(0x44D04B + 5, 1);
	}

	if (config.bShowAdditionalToolbarButtons) {
		// hook preferences window apply button to update the "allow render window cell loads" toolbar button
		WriteRelCall(0x44EA74, UInt32(PreferencesWindowApplyButtonHook));
	}

	// remove unimplemented 'bytes' from Scene Info Window
	XUtil::PatchMemoryNop(0x4143CB, 5);
	XUtil::PatchMemoryNop(0x51E8D5, 5);
	XUtil::PatchMemoryNop(0x51EB19, 5);
	SafeWrite16(0x4143D6, 0xD5F0); // increase column width

	// fix bug where disabling orthogonal mode would cause a shader to be applied to the camera
	XUtil::PatchMemoryNop(0x456BD6, 4);

	// fix missing null check in embedded render window when checking a sound form
	WriteRelJump(0x893576, UInt32(EmbeddedRenderWindowSoundNullCheck));

	if (config.bSkipVanillaLipGen) {
		// skip topics that aren't modified
		WriteRelJump(0x41EA7B, UInt32(LipGenLoopHook));

		// don't include topics that aren't modified for the progress bar counter
		WriteRelJump(0x41EA30, UInt32(LipGenCountTopicsHook));

		// replace the "Do you want to forcibly recreate all facial animation files?" to for the current plugin
		static const char* facialAnimationsPopupText = "Do you want to forcibly recreate all facial animation files for the current plugin?";
		SafeWrite32(0x440C43, UInt32(facialAnimationsPopupText));
	}

	// add support for dragging leveled items onto render window
	SafeWrite8(0x463282 + kFormType_TESLevItem, 1);
	SafeWrite8(0x463282 + kFormType_TESLevCreature, 1);
	SafeWrite8(0x463282 + kFormType_TESLevCharacter, 1);
	WriteRelCall(0x46193B, UInt32(OnRenderWindowDragDrop__CreateReferenceAtLocation));
	SafeWrite32(0x4411A1, UInt32(RenderWindowCallbackHook));

	// fix lights not updating when dragged unless a multibound is in the cell (thanks Pr0bability)
	SafeWrite16(0x90C136, 0x9066); // remove check that the node is the root in ShadowSceneNode::UpdateQueuedLight 

	// allow toggling of "scroll" in TESObjectBOOK dialog
	SafeWrite8(0x10CD577, 0x50);

	// fix crash when sorting by model type in categories with Base Effects
	SafeWrite16(0x43BA8B, 0x9090);

	// workaround infinite wait if saving fails bug
	WriteRelJump(0x4E1DB9, UInt32(SaveFailureHook));

	// fix null pointer with checking edges when using landscape editor
	WriteRelJump(0x61CA59, UInt32(EditLandCheckLandIsNull));

	if (config.bAllowEditLandEdges)
	{
		// changes: for(int i = 1; i < uGridsToLoad - 1; ++i);
		// to :		for(int i = 0; i < uGridsToLoad; ++i);

		// north
		SafeWrite8(0x459B93, 0x90);
		SafeWrite8(0x45DADC, 0x90); // flattening 
		SafeWrite8(0x45DEFB, 0); // texture painter

		// east 
		SafeWrite8(0x459B79, 0x90);
		SafeWrite8(0x45DAB9, 0x90); // flattening 
		SafeWrite8(0x45DF84, 0x90);	// texture painter

		// south
		SafeWrite8(0x459AF5, 0);
		SafeWrite8(0x45DA30, 0); // flattening
		SafeWrite8(0x45DF21, 0); // texture painter

		// west
		SafeWrite8(0x459B16, 0);
		SafeWrite8(0x45DA56, 0); // flattening
		SafeWrite8(0x45DFA5, 0x90); // texture painter
	}

	// fix crash when doubling clicking an empty area in a form list
	WriteRelJump(0x501450, UInt32(FormListCheckNull));
	WriteRelJump(0x5AE0A5, UInt32(FormListCheckNull2));

	// give more informative error when "Bad forms are encountered by printing the bad form's formID"
	WriteRelJump(0x4D9577, UInt32(BadFormLoadHook));

	// add option to continue loading if multiple master files are selected
	WriteRelJump(0x4DD2E6, UInt32(MultipleMasterLoadHook));

	if (config.bNavmeshFindCoverConfirmPrompt)
	{
		WriteRelJump(0x456F22, UInt32(RenderWindowNavMeshConfirmFindCover));
		WriteRelJump(0x444511, UInt32(MainWindowNavMeshConfirmFindCover));
		WriteRelJump(0x40AC6F, UInt32(NavMeshToolbarConfirmFindCover)); // skip 0x40AC87, retn 0x40AC7B
	}

	if (config.bShowScriptChangeTypeWarning)
	{
		WriteRelCall(0x5C2FC2, UInt32(SaveScriptChangedType));
	}

	SetupHavokPreviewWindow();

	// attempt to save the active plugin to "CrashSave - PLUGINNAME.esp" when crashing (not compatible with NVAC)
	SetCrashSaveHandler();

	// hide markers where appropriate when refreshing a cell
	WriteRelJump(0x458593, UInt32(RefreshCellHook));
	WriteRelJump(0x4585B3, UInt32(RefreshCellHook));
	WriteRelJz(0x4585BF, UInt32(RefreshCellHook));

	// allow resizing the FormList and BGSListForm dialog (3274)
	SafeWrite32(0x43768B, UInt32(FormListCallback));
	originalBGSListFormDialogFn = DetourVtable(0xD5BABC, UInt32(BGSListForm__DialogCallback));

	// allow resizing the objects palette window (375)
	SafeWrite32(0x4440FA, UInt32(ObjectPaletteCallback));

	// allow resizing the Use Report window (220)
	for (UInt32 patchAddr : {0x47F429, 0x4822E3, 0x48280C, 0x483607})
	{
		SafeWrite32(patchAddr, UInt32(UseReportCallback));
	}

	// allow resizing the dialogue window
	SafeWrite32(0x441C8F, UInt32(DialogueWindowCallback));
	SafeWrite32(0x4EDC5E, UInt32(DialogueWindowCallback));
	SafeWrite32(0x5805A6, UInt32(DialogueWindowCallback));

	SafeWrite32(0x441CE9, UInt32(GlobalsWindowCallback));
	SafeWrite32(0x441B11, UInt32(GamesettingsWindowCallback));

	// add modifier CAPSLOCK for placing a random object from the objects palette, or select randomly if multiple items are highlighted
	WriteRelCall(0x45A6B8, UInt32(PlaceOPALObjectHook));

	// make the preferences window use 4 decimal places for config
	for (UInt32 patchAddr : {0x44DC03, 0x44DC1D, 0x44DC51, 0x44D483, 0x44D4D5, 0x44D573, 0x44D58D, 0x44D5C1, 0x44D5DB})
	{
		SafeWrite8(patchAddr, 4);
	}

	// make the perk entries value use 4 decimal places
	SafeWrite8(0x548A62, 4);
	SafeWrite8(0x548A8C, 4);
	SafeWrite32(0x548AEA, 0x00D5DB0C);

	// make the 'regen rate' use 4 decimal places
	SafeWrite8(0x604BAE, 4);

	// fix body part debris scale to use 4 decimal places
	WriteRelCall(0x54498B, UInt32(OnInitDebrisScaleHook));
	SafeWrite8(0x54498B + 5, 0x90);

	if (config.bSnapToGridRotationUseDoublePrecision)
	{
		// use double precision when calculating reference rotation to fix floating point errors
		WriteRelJump(0x4523E2, UInt32(RenderWindowHandleRefRotationHook));
	}

	if (config.bFaceGenOnlyEdited)
	{
		const char* faceGenMessage = "Export mod face gen textures for all edited NPCs?";
		WriteRelJump(0x442050, UInt32(ExportFaceGenCheckIsFormEdited));
		SafeWrite32(0x442026, UInt32(faceGenMessage));
	}

	//	Create log window - credit to nukem
	InitCommonControls();
	LoadLibraryA("MSFTEDIT.dll");

	if (config.bDisableTextureMirroring)
	{
		patchTextureMemoryAllocator();
	}

	if (!EditorUI_CreateLogWindow())
	{
		MessageBoxA(nullptr, "Failed to create console log window", "Error", MB_ICONERROR);
	}

	ExtensionsMenu_InitHooks();

	PatchRememberLandscapeEditSettingsWindowPosition();

	PatchClearLandscapeEditUndoStackIfNearlyOOM();

	PatchFasterLipGen();

	PatchCellExtraDataCrash();
	// prevent geck from crashing when using search
	// buffer too small, move to 16 megs - Kormakur
	SafeWrite32(0x485865 + 1, 0x100000);

	// Remove script data size limit in GECK - Kormakur
	WriteRelJump(0x5C92E6, UInt32(Hook_RemoveScriptDataLimit));

	// make weapon mods in object window show the same information as misc items
	SafeWrite8(0x438B94 + kFormType_TESObjectIMOD, 0x11);

	// Facegen section
	{
		// Skips console output from processed actors to speed up the export process
		FaceGenExporter::NoLogging();

		if (config.bNoFacegenCompression) {
			FaceGenExporter::InitHooks();
			config.bPreventFaceAndBodyModExports = true;
		}

		if (config.bPreventFaceAndBodyModExports) {
			// prevent .tga files when exporting
			SafeWriteBuf(0x57481B, "\x83\xC4\x0C\x90\x90", 5); // face mods
			SafeWriteBuf(0x574A0F, "\x83\xC4\x0C\x90\x90", 5); // face mods

			SafeWriteBuf(0x570D6B, "\x83\xC4\x0C\x90\x90", 5); // body mods
			SafeWriteBuf(0x570C50, "\x83\xC4\x0C\x90\x90", 5); // body mods
		}
	}
	if (config.bIgnoreD3D9)
	{
		// ignore the d3d9.dll from the game's folder since dxvk makes the geck super slow
		SafeWrite32(0xD2318C, (UInt32)hook_LoadLibrary);
	}

	// allow Telekinesis in base effect archtype dropdown
	SafeWrite8(0xEA8EE8, 0x0);

	RestoreRenderWindowDebugShaders();

	WriteRelCall(0x596581, UInt32(ExportDialogueEndPlaySound));

	// allow opening meshes outside the Data\\Meshes\\ folder
	SafeWrite8(0x410656, 0xEB);
	SafeWrite8(0x410678, 0x56);
	WriteRelCall(0x8A2CC8, UInt32(OnGetMeshPathModifyIfDragDrop));

	// always show imposters
	SafeWrite8(0x65688D, 0xEB);

	// fix crash when clicking on 'Regions' without an esm loaded
	WriteRelCall(0x743F8E, UInt32(OnLoadRegionsHook));

	if (config.bNoRecordCompression) {
		// Remove record compression
		SafeWriteBuf(0x5726C9, "\xC3\x90\x90\x90\x90", 5); // TESNPC

		XUtil::PatchMemoryNop(0x624632, 5); // TESObjectLAND
	}

	CustomFOV::InitHooks(); // credits to WallSoGB
	CustomFOV::SetFOV(config.iRenderFOV);

	WriteRelCall(0x4DD437, UInt32(OnMasterFileNotMatchedPopupSkipIfVersionControlDisabled));
	SafeWrite8(0x4DD437 + 5, 0x90);

	WriteRelCall(0x4824D8, UInt32(OnCloseSelectFormsDialogPopulateList));
	WriteRelCall(0x5B3661, UInt32(OnMediaLocationControllerSelectForm));

	static const char* RenderWindowLandscapeEditColorsSettingLabel = "Landscape: Toggle Edit Colors";
	SafeWrite32(0xE8CB94, UInt32(RenderWindowLandscapeEditColorsSettingLabel));
	SafeWrite8(0xD2D21D, 'h');

	// fix the render preferences window losing sort when assigning a hotkey
	WriteRelCall(0x4134BF, UInt32(RenderPreferences_PostPopulateList));
	WriteRelCall(0x413494, UInt32(RenderPreferences_PostPopulateList));
	WriteRelCall(0x413456, UInt32(RenderPreferences_PostPopulateList));

	WriteRelCall(0x465935, UInt32(FormatRenderWindowMemoryUsage));
	WriteRelCall(0x465913, UInt32(FormatRenderWindowMemoryUsage));
	WriteRelCall(0x4531E9, UInt32(FormatRenderWindowMemoryUsage));
	WriteRelCall(0x4530BF, UInt32(FormatRenderWindowMemoryUsage));
	WriteRelCall(0x45319B, UInt32(FormatRenderWindowMemoryUsage));
	WriteRelCall(0x46594C, UInt32(OnPrintRenderWindowStatusBarText));
	SafeWrite8(0x46594C + 5, 0x90);

	// prevent flickering caused by status bar updates by checking if the text changed
	WriteRelCall(0x460CDB, UInt32(TESCSMain__WriteToStatusBarIfChanged));
	WriteRelCall(0x460CE7, UInt32(TESCSMain__WriteToStatusBarIfChanged));
	WriteRelCall(0x460912, UInt32(TESCSMain__WriteToStatusBarIfChanged));
	WriteRelCall(0x460903, UInt32(TESCSMain__WriteToStatusBarIfChanged));
	WriteRelCall(0x460AA9, UInt32(TESCSMain__WriteToStatusBarIfChanged));

	WriteRelCall(0x57484D, UInt32(OnFaceGen_PeriodicCleanup));
	WriteRelCall(0x574A40, UInt32(OnFaceGen_PeriodicCleanup));

	FixCommCtl32ScrollingBug();

	// open the 'Globals' window when clicking 'New' in the objects window
	SafeWrite32(0x47A094 + 4 * (kFormType_TESGlobal), UInt32(RetnGlobalDialogIDHook));

	// fix freeze when clicking on 'Record Audio' twice in the dialogue editor
	WriteRelCall(0x58F495, UInt32(StopSound_ResetRecordAudioPopupIfInvalid));

	// skip checking weapon mod models if the weapon mod tab isn't active
	WriteRelJump(0x605D53, UInt32(OnTESModelTextureSwapDialogCallbackHook));

	// fix EffectShader form not updating the color preview rectangle
	WriteRelCall(0x479449, UInt32(OnUpdateColor_Repaint));
	SafeWrite8(0x479449 + 5, 0x90);

	if (config.bPreserveTimestamps)
	{
		// preserve file times when saving (ShadeMe)
		WriteRelCall(0x4D9A0E, UInt32(PreSaveStoreFileTime));
		WriteRelCall(0x4DA14D, UInt32(PostSaveRestoreFileTime));
	}

	if (config.bNoDirtyCellWhenNonPersistentRefsDeleted)
	{
		// prevent cells getting marked as modified when temp refs are deleted
		SafeWrite8(0x63255F, 0xEB); // credits to ShadeMe
	}

	CreatureMarkerSwapper::Init();

	// add null checks to fix the F10 ShowSceneGraph
	WriteRelCall(0x530126, UInt32(NiTreeCtrl_CreateTreeRecursiveHook));
	WriteRelCall(0x530508, UInt32(NiTreeCtrl_CreateTreeRecursiveHook2));
	WriteRelCall(0x80F145, UInt32(NiAVObject_GetViewerStringsHook));

	// remove a useless dialog callback as it caused the mesh selection dialog
	// to fallback on the old windows explorer
	SafeWrite32(0x47F287, 0);

	EasterEggs::Init();

	// fix the 'IsEdited' flag getting reset when canceling the ESP/ESM dialog
	SafeWrite8(0x441942, 0xAE);

	// fix ESP/ESM window counting clicking on the 'Summary' field as modifying it
	SafeWrite32(0x432DDE, EN_CHANGE); // fix the summary field detecting spurious changes due to use of EN_UPDATE
	WriteRelCall(0x432BAE, UInt32(OnLoadEspEsmSetupDetailsText)); // don't count changing files as changing the summary
	WriteRelCall(0x432DE4, UInt32(OnLoadEspEsmSetSummaryModifiedHook));
	SafeWrite16(0x432DE9, 0x9066);

	// add support for right clicking and doing 'New' in the 'All' tab of the Objects window
	WriteRelCall(0x44B05A, UInt32(OnObjectWindowNewHook));

	originalObjectWindowCallback = (WNDPROC)DetourVtable(0x4416A3, UInt32(ObjectWindowCallback));

	originalCellWindowCallback = *(WNDPROC*)0x441648;
	SafeWrite32(0x441648, UInt32(CellWindowCallback));

	// fix the undo menu button for NavMesh
	WriteRelCall(0x44104A, UInt32(OnMainWindowUndo));

	// fix the Text Search not updating Z position after loading a cell
	WriteRelCall(0x48706C, UInt32(OnTextViewLoadCell));
	WriteRelCall(0x486FE1, UInt32(OnTextViewLoadCell));

	// prevent the script save confirmation if the script is new but empty
	WriteRelCall(0x5C303F, UInt32(OnScriptConfirmCloseHook));
	SafeWrite8(0x5C303F + 5, 0x90);

	OutOfMemoryHelper::Init();

	WriteRelCall(0x44B294, UInt32(OnSetupObjectAndCellWindowRightClickMenu));
	WriteRelCall(0x42F2F5, UInt32(OnSetupObjectAndCellWindowRightClickMenu));

	BetterFloatingFormList::Init();

	// make the 'Open Windows' view call EndDialog instead of DestroyWindow on dialogs
	WriteRelCall(0x4775FE, UInt32(DestroySelectedWindowOrDialog));
	SafeWrite8(0x4775FE + 5, 0x90);
	WriteRelJump(0x4777A6, UInt32(OnDestroyAllWindowsHook));

	// refresh the list after destroying windows
	WriteRelJump(0x4778A9, 0x4778CC); // All windows
	WriteRelJump(0x477902, 0x4778CC); // Selected Window

	SafeWrite16(0x6C07D8, 0x07EB); // fix unnecessary duplicate function call in NavMeshRender::AttachEdge
	WriteRelCall(0x6778C2, UInt32(OnTAllocZeroMemory)); // microoptimize TAlloc::Allocate

	// fix NavMeshRenderer creating a new child slot whenever attaching children
	SafeWrite8(0x6BF799, 1); // sets bFirstAvail to true
	SafeWrite8(0x6C06F7, 1);
	SafeWrite8(0x6C20CF, 1);

	// fix 512 byte scrapheap leak when opening the find and replace window
	WriteRelCall(0x47D256, UInt32(OnShowSearchAndReplaceWindowHook));
	SafeWrite8(0x47D256 + 5, 0x90);

	// add 'Z' modifier to render window preferences
	WriteRelCall(0x41360A, UInt32(OnInsertRenderPreferencesComboHotkey));
	WriteRelCall(0x413653, UInt32(OnInsertRenderPreferencesComboHotkey));
	WriteRelJump(0x412E6E, UInt32(OnSelectRenderPreferencesComboHook));

	// add support for more keys to render window preferences
	WriteRelCall(0x4137D5, UInt32(OnInitRenderWindowComboBox));

	// remove superfluous call to invalidate the cell view window's cell list (ShadeMe)
	SafeWrite16(0x4309D4, 0x2AEB);

	// fix the cell view bunching up when resizing it the first time after loading a cell (ShadeMe)
	WriteRelJump(0x42EA05, UInt32(CellViewWindowResizeFixHook));

	// options to hide the cell/object/render windows at startup (ShadeMe)
	WriteRelCall(0x446594, UInt32(HideCSMainDialogsStartup));

	// prevent minimize anim when toggling main window visibilities (ShadeMe)
	SafeWrite8(0x442255 + 1, SW_HIDE); // RenderWindow
	SafeWrite8(0x441575 + 1, SW_HIDE); // CellWindow
	SafeWrite8(0x4415D6 + 1, SW_HIDE); // ObjectsWindow

	SafeWrite8(0x4CE7DA, 0xEB); // Skip calling InvalidateRect on the RenderWindow in TESDataHandler::CloseAllTES (ShadeMe)
	SafeWrite8(0x46252B, 0); // RenderWindowCallback - pass bUpdateSelection = 0 when copying (ShadeMe)

	CustomRenderWindowHotkeys::Init();

	if (config.bSkipSplashScreen)
	{
		SafeWrite16(0x446586, 0x05EB);
		SafeWrite16(0x4463BA, 0x21EB);
	}

	// add array index check
	WriteRelJump(0x4D1F28, UInt32(OnDataHandlerGetInteriorAtIndexHook));

	// fix geck calling a GWL_WNDPROC function pointer directly instead of using CallWindowProcA
	WriteRelJump(0x48CABC, UInt32(OnRenderWindowCallProcHook));
	// fix crash with water type window
	WriteRelJump(0x65F8EF, UInt32(OnWaterTypeCheckExtraSubWindowHook));
	// fix region editor auto selecting 'Enable this type of Data' by adding a check for Msg == WM_COMMAND for the buttons
	// otherwise they were toggling on WM_NOTIFY...
	SafeWriteBuf(0x732982, "\x81\xFF\x11\x01\x00\x00\x90", 7); // grass
	SafeWriteBuf(0x7345E2, "\x81\xFF\x11\x01\x00\x00\x90", 7); // imposter
	SafeWriteBuf(0x73497B, "\x81\xFF\x11\x01\x00\x00\x90", 7); // landscape
	SafeWriteBuf(0x735DF2, "\x81\xFF\x11\x01\x00\x00\x90", 7); // map
	SafeWriteBuf(0x739132, "\x81\xFF\x11\x01\x00\x00\x90", 7); // objects
	SafeWriteBuf(0x741302, "\x81\xFF\x11\x01\x00\x00\x90", 7); // sound
	SafeWriteBuf(0x74267D, "\x81\xFF\x11\x01\x00\x00\x90", 7); // weather

	// fix head preview not updating when adding head parts
	WriteRelCall(0x5785EF, UInt32(InsertHeadPartsColumnsHookAdd));
	WriteRelCall(0x5784F0, UInt32(PreInsertHeadPartsColumnsHookRemove)); // save the window (ESI) in a global because it's lost otherwise...
	WriteRelCall(0x578595, UInt32(InsertHeadPartsColumnsHookRemove));

	// support regex in the Object window filter
	WriteRelCall(0x439641, UInt32(OnObjectWindowFilter));
	WriteRelCall(0x439754, UInt32(ObjectWindowRegexContains));
	WriteRelCall(0x43977D, UInt32(ObjectWindowRegexContains));

	// support regex in the cell window
	WriteRelCall(0x42DE03, UInt32(OnCellWindowFilter));
	WriteRelCall(0x42DE5A, UInt32(CellWindowRegexContains));
	WriteRelCall(0x42DE85, UInt32(CellWindowRegexContains));
	WriteRelCall(0x42DEAE, UInt32(CellWindowRegexContains));
	WriteRelCall(0x42DF67, UInt32(CellWindowRegexContains));

	// trim whitespace in object window filter
	WriteRelCall(0x44B2FE, UInt32(OnGetObjectWindowText));
	SafeWrite8(0x44B2FE + 5, 0x90);

	// fix the dialog window columns being zero width by default
	WriteRelCall(0x59073B, UInt32(OnStoreDialogWindowColumnSizeHook));

	// add support for showing/hiding activators
	originalShowHideWindowCallback = *(WNDPROC*)0x4427E4;
	SafeWrite32(0x4427E4, UInt32(ShowHideWindowCallback));
	// fix the vanilla code changing the _name_ of settings
	WriteRelCall(0x4576EF, UInt32(OnSetNonLandVisible));

	// add copy/paste support to texture quads
	originalTextureUseCallback = *(WNDPROC*)0x4373B5;
	SafeWrite32(0x4373B5, UInt32(TextureUseCallback));

	// add spacebar to click on the selected body part data
	WriteRelCall(0x54826D, UInt32(OnSelectedBodyPartListViewButtonPressedHook));
	SafeWrite16(0x54826D + 5, 0x9066);

	// add CTRL-D to duplicate the selected body part
	originalBGSBodyPartDataDialogFn = DetourVtable(0xD4E1CC, UInt32(BGSBodyPartData__DialogCallback));

	// fix plugins becoming marked as modified when closing the weapon menu
	WriteRelCall(0x604909, UInt32(OnWeapCreateTempPlayer_MarkAsTemporary));
	WriteRelJnz(0x604735, UInt32(OnWeapCheckIsDifferentHook));

	// fix armor addons resetting the IsAddon flag when closing the form
	SafeWrite32(0x4F2695, 0x10086780);

	// fix media sets always marking as modified - incorrect strcmp
	SafeWrite8(0x5B79DF, 0x23); // if strings are equal return FALSE
	WriteRelJump(0x5B79E7, UInt32(MediaSetNameDiffersHook));
	SafeWrite8(0x5B79F9, 0x75); // if name && !this.name return TRUE

	// fix MediaLocationController's always marking as modified - would always return true if any lists were empty
	SafeWrite16(0x5B2333, 0x9066);
	SafeWrite16(0x5B233B, 0x9066);
	SafeWrite16(0x5B2396, 0x9066);
	SafeWrite16(0x5B23A2, 0x9066);
	XUtil::PatchMemoryNop(0x5B2406, 6);
	XUtil::PatchMemoryNop(0x5B2416, 6);
	XUtil::PatchMemoryNop(0x5B2490, 6);
	XUtil::PatchMemoryNop(0x5B24A3, 6);
	XUtil::PatchMemoryNop(0x5B2514, 6);
	XUtil::PatchMemoryNop(0x5B2527, 6);
	XUtil::PatchMemoryNop(0x5B25A0, 6);
	XUtil::PatchMemoryNop(0x5B25B3, 6);

	// skip checking for differences in TESRace::TESAttributes, since they're a holdover from oblivion and caused dirty edits
	SafeWrite16(0x585A24, 0x41EB);

	// show '(none)' on the empty option in the sound picker
	SafeWrite16(0x47BEB9, 0xD3F0);
	WriteRelCall(0x47BF9B, UInt32(OnSoundPickerInitDialog_FreeBufferHook)); // fix 512 byte scrapheap leak

	// skip 'Use a skill' in TESChallenge types
	SafeWriteBuf(0x55FF7A, "\x83\xFE\x07\x75\x01\x46", 6);
	SafeWrite8(0x55FF99, 0xE0);

	// populate the quest view when creating new entry
	WriteRelJump(0x57E252, UInt32(OnCreateQuestWindowGetParamHook));

	// add null check for idle form
	WriteRelCall(0x56CFC6, UInt32(TESIdleForm__56B5A0_Recurse));
	WriteRelCall(0x56D006, UInt32(TESIdleForm__56B5A0_Recurse));

	// fix havok preview anim timer increasing for non-anims
	TESPreviewControl__SetTimeAddr = DetourVtable(0xD38DE0, UInt32(TESPreviewControl__SetTime));

	// allow opening any form that has a model in the preview (e.g. Impact Datas)
	SafeWrite8(0x44B243, 0xB8);
	SafeWrite32(0x44B243 + 1, UInt32(IsBoundObjectOrHasModelPath));
	WriteRelCall(0x44A782, UInt32(OnSelectObjectCheckIsBoundObject));
	SafeWrite8(0x44B243 + 1 + 4, 0x90);
	WriteRelCall(0x44BB8E, UInt32(OnHavokPreviewSetup));
	WriteRelCall(0x44A793, UInt32(OnHavokPreviewSetup));

	// fix crash when duplicating worldspaces 
	SafeWriteBuf(0x623D96, "\x33\xF6\x56\x8B\xCD\xE8\xD0\x88\xFF\xFF", 10); // pass nullptr to TESObjectLAND::CreateLandscape to ensure the data is setup correctly...

	// check if x/y/z are held while you move refs to fix the axis lock keys not working after closing a menu
	WriteRelCall(0x45F490, UInt32(OnMoveRefCheckXYZHeld));

	// prevent x/y/z keys moving the mouse when pressed during navmesh mode
	SafeWriteBuf(0x41F3A0, "\xC2\x04\x00", 3);

	// show a warning when merging vertices from different navmeshes
	WriteRelCall(0x40AAA7, UInt32(NavMeshManager__OnMergeVertices));
	WriteRelCall(0x457E23, UInt32(NavMeshManager__OnMergeVertices));
	WriteRelCall(0x40A9FF, UInt32(NavMeshManager__OnCreateTriangle));
	WriteRelCall(0x4288AC, UInt32(NavMeshManager__OnCreateTriangle));
	WriteRelCall(0x4298CE, UInt32(NavMeshManager__OnCreateTriangle));
	WriteRelCall(0x456FBF, UInt32(NavMeshManager__OnCreateTriangle));
	WriteRelCall(0x4288BF, UInt32(NavMeshManager__OnCreateQuad));
	WriteRelCall(0x40AA20, UInt32(NavMeshManager__OnCreateQuad));
	WriteRelCall(0x456FD9, UInt32(NavMeshManager__OnCreateQuad));

	// clear the 'Creating Editor NavMeshes' print when it's done
	WriteRelJump(0x42216C, UInt32(NavMeshManager__PostRenderCellClearPrintHook));
	WriteRelJump(0x4220E2, UInt32(NavMeshManager__PostRenderCellClearPrintHook));

	// fix crash when saving with a single vertex in a cell with no navmeshes
	WriteRelJump(0x6ED482, UInt32(NavMeshInfoMap__CheckInfosHook));

	// fix refs getting marked as modified if their primitives are slightly different during a cell load
	WriteRelCall(0x643655, UInt32(IsMultiboundPointDifferent_IgnoreIfLoadingCell));
	WriteRelCall(0x63A323, UInt32(TESObjectCELL__OnLoad3D));

	NavMeshPickPreventer::Init();

	// allow saving as ESM
	const char* fileExtensions = "ESM Files (*.esm)\0*.esm\0ESP Files (*.esp)\0*.esp\0";
	const char* fileExtensionsEspFirst = "ESP Files (*.esp)\0*.esp\0ESM Files (*.esm)\0*.esm\0";
	SafeWrite32(0x47F36A, config.bNewPluginsDefaultToESM ? UInt32(fileExtensions) : UInt32(fileExtensionsEspFirst));
	if (config.bNewPluginsDefaultToESM)
	{
		SafeWrite8(0x47F374, 0xFC);
	}

	if (config.bAllowHardDeletionInESMs)
	{
		// make it so deleting forms added by the active ESM plugin completely removes them instead of marking them as deleted
		WriteRelJump(0x4CDD11, UInt32(OnSaveDeletedMasterFormHook));
	}

	if (config.bDarkMode)
	{
		if (!EditorUIDarkMode::Initialize())
		{
			Console_Print("DarkMode: Failed to initialize");
		}
	}

	if (config.bPlaySoundEndOfLoading)
	{
		DataLoadEvent::RegisterCallback(PlayMouseClickSound);
	}

	if (config.bNumericEditorIdWarning)
	{
		DataLoadEvent::RegisterCallback(AddEditorNumericIDWarningHooks);
	}

	// Fix ONAM not being written when not using Version Control
	ONAMFix::InitHooks();

	// Fix geometry data created for SCOLs always using MUTABLE consistency, which increases memory usage
	SCOLConsistencyFix::InitHooks();

    // Fix newly created multibound not being registered in cells they were created in
	MultiBoundsAdder::InitHooks();

    InitCustomPrimitiveColors();

	// prevent the infos refreshing when clicking on a result in the 'Find Text' window
	WriteRelCall(0x57D402, UInt32(OnLoadQuestGetTextSearchWindowHook));
	WriteRelCall(0x486F1A, UInt32(TextSearchOnOpenInfo));
	SafeWrite8(0x486F1A + 5, 0x90);

	// add recusive master loading
	WriteRelJump(0x4DD1A7, UInt32(CompileFilesHook));

	// allow opening a door marker form
	WriteRelJnz(0x45949E, UInt32(OnClickDoorMarkerHook));
	SafeWrite8(0x459485, MB_YESNOCANCEL);
	WriteRelJump(0x65AB1C, UInt32(OnSetupRefFormControlsHook)); // skip adding data tabs
	WriteRelCall(0x65B3F1, UInt32(OnSetupRefFormControls_DisableEditBaseHook)); // disable most buttons
	SafeWrite8(0x65B3F1 + 5, 0x90);

	// automatically focus the form list when opening a Select Form window
	WriteRelCall(0x482634, UInt32(OnInitSelectFormWindow));

	// rename Rating to DR in object window armor tab
	ObjectsView::SetColumnHeading(ObjectsView::kCol_Rating, (const char*)0xD8BF58); // Rating -> DR
	ObjectsView::SetColumnWidth(ObjectsView::kCol_Rating, 0x30);
	// add a DT column
	WriteRelCall(0x437F5D, UInt32(ObjectWindowNodeData__OnAddArmorRatingColumn));
	WriteRelJa(0x439901, UInt32(ObjectWindowListView_OnPopulateColumnExHook));

	// add Challenges to the Object Window 'All' node
	WriteRelCall(0x439272, UInt32(ObjectWindowNodeData__OnPopulateReputationList));

	ObjectWindowTreeHooks::InitHooks();

	// read the 4th tag skill combobox
	static constexpr int kTagSkillDialogIDs[] = {1035, 1036, 1037, 1038};
	SafeWrite32(0x560F44, UInt32(kTagSkillDialogIDs));
	SafeWrite32(0x56111B, UInt32(kTagSkillDialogIDs));
	SafeWrite32(0x560F93, UInt32(&kTagSkillDialogIDs[4]));
	SafeWrite32(0x561163, UInt32(&kTagSkillDialogIDs[4]));

	// add option for only finalizing navmeshes for cells from a specified esm
	WriteRelCall(0x4446E1, UInt32(NavMeshManager__ShowFinalizeAllNavMeshesPopup));
	WriteRelCall(0x426248, UInt32(OnForceFinalizeShouldProcessCell));
	WriteRelCall(0x4262CF, UInt32(OnForceFinalizeShowMessageBox));

	// fix TESChallenge 'Craft using an item' combobox only containing casino chips
	// and 'Acquire an item' only including armors
	SafeWrite8(0x5600FE, 0xB5);
	SafeWrite8(0x560159, 0x65);
	SafeWriteBuf(0x560160, "\x83\xC4\x18\xE9\x3B\xFF\xFF\xFF", 8);

	ToggleReferenceMovement::InitHooks();

#ifdef _DEBUG
	while(!IsDebuggerPresent())
	{
		Sleep(1);
	}
#endif
	return true;
}