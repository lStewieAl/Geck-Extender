#include "nvse/GameData.h"
#include "nvse/PluginAPI.h"
#include "nvse/GameForms.h"
#include "nvse/GameObjects.h"
#include "nvse/GameRTTI.h"
#include "nvse/SafeWrite.h"
#include "nvse/ParamInfos.h"
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
#include <string>
#include <regex>
#include <libdeflate/libdeflate.h>
#include "ZeGaryHax.h"
#include "ZeLogWindow.h"
#include "ZeWarningPatches.h"
#include "ZeWarningHax.h"
#include "Editor.h"
#include "xutil.h"

const NVSEInterface* savedNVSE = NULL;

extern "C"
{
	  BOOL WINAPI DllMain(HANDLE  hDllHandle, DWORD dwReason, LPVOID lpreserved)
    {
        switch (dwReason)
        {
			case (DLL_PROCESS_ATTACH):
				ZeGaryHaxHandle = (HMODULE)hDllHandle;
				break;
        }
        return TRUE;
    }
}

void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	switch(msg->type)
	{
	case NVSEMessagingInterface::kMessage_PostPostLoad:
		g_msg->RegisterListener(g_pluginHandle, NULL, MessageHandler);
		break;
	default:	// this is hacky bullshit to log ALL messages sent by NVSE (save, load, exit, etc.) and by other plugins (none?)
		_DMESSAGE("MSG: [%s] %s %s", msg->sender, !strcmp(msg->sender, "NVSE") ? nvseMSG[msg->type] : std::to_string(msg->type).c_str(),
			msg->dataLen ? msg->dataLen > 1 ? (char*)msg->data : std::to_string(*(char*)&msg->data).c_str() : "");
		break;
	}
}

_declspec(naked) void EndLoadingHook() {
	PlaySound("MouseClick", NULL, SND_ASYNC);
	_asm {
	originalCode:
		add esp, 8
		pop esi
		retn
	}
}

bool NVSEPlugin_Query(const NVSEInterface * nvse, PluginInfo * info)
{
	if(!nvse->isEditor)
	{
		return false;	//	unload if the game
	} 

	_MESSAGE("Gary?");
    info->infoVersion = PluginInfo::kInfoVersion;
    info->name = GH_NAME;
    info->version = GH_VERSION;
	if(!nvse->isEditor)
	{
		return false;	//	returning FALSE from Query or Load skips your plugin with the "incompatible" message in nvse.log
	}
    if(nvse->nvseVersion < 0x05000020)
    {
		_ERROR("NVSE does not meet version requirements. %08X < %08X", nvse->nvseVersion, 0x05000020);			// 5.0.2
        return false;
    }
    return true;
}

bool NVSEPlugin_Load(const NVSEInterface * nvse)
{
	if (!nvse->isEditor) return false;
	savedNVSE = nvse;

	_MESSAGE("Haha Gary!");
	_DMESSAGE("ZGH Address: %08X", GetModuleHandle("ZeGaryHax.dll"));

	//	ini thing - credit to carxt
	GetModuleFileNameA(NULL, filename, MAX_PATH);
	strcpy((char *)(strrchr(filename, '\\') + 1), "Data\\nvse\\plugins\\geckextender.ini");
	bEnableSpellChecker = GetPrivateProfileIntA("General", "bEnableSpellChecker", 0, filename);
	bAutoScroll = GetPrivateProfileIntA("General", "bAutoScroll", 0, filename);
	bPatchScriptEditorFont = GetPrivateProfileIntA("General", "bPatchScriptEditorFont", 0, filename);
	bFastExit = GetPrivateProfileIntA("General", "bFastExit", 0, filename);
	bListEditFix = GetPrivateProfileIntA("General", "bListEditFix", 0, filename);
	bIgnoreNAMFiles = GetPrivateProfileIntA("General", "bIgnoreNAMFiles", 0, filename);
	bVersionControlMode = GetPrivateProfileIntA("General", "bVersionControlMode", 0, filename);
	bHighResLandscapeLOD = GetPrivateProfileIntA("General", "bHighResLandscapeLOD", 0, filename);
	bRenderWindowUncap = GetPrivateProfileIntA("General", "bRenderWindowUncap", 0, filename);
	bPreviewWindowUncap = GetPrivateProfileIntA("General", "bPreviewWindowUncap", 0, filename);
	bRemoveStatusBarLoadingText = GetPrivateProfileIntA("General", "bRemoveLoadingText", 1, filename);
	bPlaySoundEndOfLoading = GetPrivateProfileIntA("General", "bPlaySoundEndOfLoading", 1, filename);
	bNoDXSoundCaptureErrorPopup = GetPrivateProfileIntA("General", "bNoDXSoundCaptureErrorPopup", 0, filename); 
	bNoPreviewWindowAutoFocus = GetPrivateProfileIntA("General", "bNoPreviewWindowAutoFocus", 1, filename);
	bNoLODMeshMessage = GetPrivateProfileIntA("General", "bNoLODMeshMessage", 0, filename);
	bSwapRenderCYKeys = GetPrivateProfileIntA("General", "bSwapRenderCYKeys", 0, filename);
	bShowLoadFilesAtStartup = GetPrivateProfileIntA("General", "bShowLoadFilesAtStartup", 0, filename);
	bScriptCompileWarningPopup = GetPrivateProfileIntA("General", "bScriptCompileWarningPopup", 0, filename);
	
	bSmoothFlycamRotation = GetPrivateProfileIntA("Flycam", "bSmoothRotation", 1, filename);
	fFlycamRotationSpeed = GetPrivateProfileIntA("Flycam", "iRotationSpeedPct", 100, filename) * - 0.001F;
	fFlycamNormalMovementSpeed = GetPrivateProfileIntA("Flycam", "iMovementSpeed", 10, filename) * 1.0F;
	fFlycamModifiedMovementSpeed = GetPrivateProfileIntA("Flycam", "iModifierMovementSpeed", 2, filename) * 1.0F;

	//	stop geck crash with bUseMultibounds = 0 in exterior cells with multibounds - credit to roy
	WriteRelCall(0x004CA48F, (UInt32)FixMultiBounds);
	XUtil::PatchMemoryNop(0x004CA494, 0x05);

	//	enable wasteland level 2 lod generation - credit to roy
	SafeWrite8(0x00419557, 0xEB);

	//	enable 512px landscape LOD textures and normals - credit to roy
	if (bHighResLandscapeLOD == 1)
	{
		SafeWrite8(0x00728A3C, 0x02);
		SafeWrite8(0x00728A41, 0x02);
		SafeWrite8(0x0072971B, 0x02);
		SafeWrite8(0x00729720, 0x02);
		SafeWrite8(0x0072F090, 0x02);
		SafeWrite8(0x0072F095, 0x02);
	}

	//	change help urls to our wiki - credit to roy
	SafeWrite32(0x00441952, (UInt32)geckwikiurl);
	SafeWrite32(0x005C4B0F, (UInt32)geckwikiscriptingurl);
	SafeWrite32(0x005C4B29, (UInt32)geckwikicommandsurl);
	SafeWrite32(0x005C4B43, (UInt32)geckwikifunctionsurl);

	//	force patch script editor font - credit to nvse team
	if (bPatchScriptEditorFont == 1)
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

	//	enable big guns for weapon skill - credit to roy
	SafeWrite16(0x0047A830, 0x9090);

	//	uncap framerate on render window - credit to shademe
	if (bRenderWindowUncap == 1)
	{
		SafeWrite8(0x0045911B, 0x0A);
	}

	//	uncap framerate on preview window - credit to shademe
	if (bPreviewWindowUncap == 1)
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

	if (bListEditFix == 1)
	{
		//	fix windows 8/10 conditions collapsed column bug - credit to nukem
		WriteRelCall(0x004A2791, (UInt32)hk_sub_4A1C10);
		WriteRelCall(0x004A382D, (UInt32)hk_sub_4A1C10);
		WriteRelCall(0x00765214, (UInt32)hk_sub_4A1C10);
		WriteRelCall(0x00766A4D, (UInt32)hk_sub_4A1C10);
		WriteRelCall(0x0076E195, (UInt32)hk_sub_4A1C10);
	}

	//	ignore .nam Files - credit to roy
	if (bIgnoreNAMFiles == 1)
	{
		SafeWrite8(0x004D8DFE, 0xEB);
	}

	if (bVersionControlMode == 0)
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
	}

	//	fix Rock-It Launcher crash - credit to jazzisparis
	WriteRelJump(0x005B8FF4, (UInt32)CheckIsRILHook);

	//	Fix message bugs/change message formatting - credit to roy
	SafeWrite32(0x00468D13 + 1, (UInt32)messageCreateFileMapping);
	SafeWrite32(0x00468DD8 + 1, (UInt32)messageCreateFileMapping);
	SafeWrite32(0x005503A8 + 1, (UInt32)messageCouldNotCheckoutForm);
	SafeWrite32(0x00547D47 + 1, (UInt32)messageCouldNotCheckoutForm);
	SafeWrite32(0x005E0F41 + 1, (UInt32)messageCouldNotCheckoutForm);
	SafeWrite32(0x00547D5B + 1, (UInt32)messageBodyPartDataNotFound);
	SafeWrite32(0x005503B8 + 1, (UInt32)messageBodyPartDataNotFound);
	SafeWrite32(0x005E0F51 + 1, (UInt32)messageAmmoNotFound);
	SafeWrite32(0x005E0F5F + 1, (UInt32)messageAmmoNotFound);
	SafeWrite32(0x006AB7FA + 1, (UInt32)messageNavmeshTriangleDegenerate);
	SafeWrite32(0x0065A6C9 + 1, (UInt32)messagePotentiallyInvalidZValue);
	SafeWrite32(0x0065A882 + 1, (UInt32)messagePotentiallyInvalidXValue);
	SafeWrite32(0x0065A912 + 1, (UInt32)messagePotentiallyInvalidYValue);

	//	Warnings more verbose - credit to roy and jazzisparis
	WriteRelJump(0x004B04B5, (UInt32)hk_RemoveEmptyLitWaterMsgHook);
	SafeWrite8(0x004B04C1, 0x08);

	WriteRelJump(0x004AFDB2, (UInt32)hk_EnableStateParentLoopMsgHook);
	SafeWrite8(0x004AFDC2, 0x08);
	SafeWrite8(0x004AFDBF, 0x18);
	WriteRelJump(0x004B020F, (UInt32)hk_RemoveEmptyActivateParentMsgHook);	//	must be with EnableStateParentLoopMsgHook

	WriteRelJump(0x005004B6, (UInt32)hk_UnableToFindLeveledObjectMsgHook);
	SafeWrite16(0x005004B4, 0x9090);
	SafeWrite8(0x005004C2, 0x10);

	WriteRelJump(0x005C36EC, (UInt32)hk_ScriptHasTextButNotCompiledMsgHook);
	SafeWrite32(0x005C36F1, 0x90909090);
	SafeWrite8(0x005C36FC, 0x14);

	WriteRelJump(0x00496F37, (UInt32)hk_OnPackageBeginScriptNotCompiledMsgHook);
	SafeWrite32(0x00496F3C, 0x90909090);
	SafeWrite8(0x00496F47, 0x14);

	WriteRelJump(0x00496F74, (UInt32)hk_OnPackageChangeScriptNotCompiledMsgHook);
	SafeWrite32(0x00496F79, 0x90909090);
	SafeWrite8(0x00496F84, 0x14);

	WriteRelJump(0x00496FAE, (UInt32)hk_OnPackageEndScriptNotCompiledMsgHook);
	SafeWrite32(0x00496FB3, 0x90909090);
	SafeWrite8(0x00496FBE, 0x14);

	WriteRelJump(0x0064FF5B, (UInt32)hk_ChunkAbnormallyTerminatedMsgHook);
	SafeWrite8(0x0064FF8B, 0x18);

	WriteRelJump(0x004AF986, (UInt32)hk_CouldNotFindMediaLocationControllerMsgHook);
	SafeWrite8(0x004AF98B, 0x90);
	SafeWrite8(0x004AF993, 0x0C);

	WriteRelJump(0x004AFE75, (UInt32)hk_UnableToFindLinkedReferenceMsgHook);
	XUtil::PatchMemoryNop(0x004AFE7A, 5);
	SafeWrite8(0x004AFE86, 0x0C);

	WriteRelJump(0x005C284B, (UInt32)hk_TryingToAccessLocalVariableInScriptMsgHook);
	SafeWrite16(0x005C2850, 0x9090);
	SafeWrite8(0x005C2859, 0x10);

	WriteRelJump(0x0049FCD5, (UInt32)hk_UnableToFindPackageLocationMsgHook);

	WriteRelJump(0x004A167C, (UInt32)hk_UnableToFindPackageReferenceMsgHook);
	SafeWrite8(0x004A168E, 0x10);

	WriteRelJump(0x004AFD4F, (UInt32)hk_UnableToFindEnableStateParentMsgHook);
	SafeWrite8(0x004AFD60, 0x0C);

	//	patch warning messages - credit to jazzisparis
	for (UInt32 patchAddr : kPatch_Warnings1)
		SafeWrite32(patchAddr + 1, (UInt32)EditorUI_Log2);

	for (UInt32 patchAddr : kPatch_Warnings2)
		SafeWrite32(patchAddr + 6, (UInt32)EditorUI_Log2);

	for (UInt32 patchAddr : kPatch_Warnings3)
		WriteRelCall(patchAddr, (UInt32)EditorUI_Log2);

	for (UInt32 patchAddr : kPatch_Warnings4)
		WriteRelCall(patchAddr, (UInt32)EditorUI_Log);

	SafeWrite32(0x00C1A543 + 4, (UInt32)EditorUI_Log2);
	SafeWrite32(0x00C1A6DC + 1, (UInt32)EditorUI_Log2);

	if (bNoLODMeshMessage) {
		// remove the MODELS: LOD Mesh ... messages
		XUtil::PatchMemoryNop(0x659DC8, 5);
		XUtil::PatchMemoryNop(0x659E05, 5);
	}

	//	patch script editor messages - credit to roy
	WriteRelJump(0x005C57E2, (UInt32)hk_EnableScriptErrorsMsgHook);
	XUtil::PatchMemoryNop(0x005C57E7, 0x06);

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
	WriteRelJump(0x004463CD, (UInt32)hk_SplashScreen);
	SafeWrite8(0x004463D2, 0x90);

	//	About Dialog Hax - credit to nukem and roy
	WriteRelJump(0x00441976, (UInt32)hk_AboutDialog);
	SafeWrite8(0x0044197B, 0x90);

	//	Patch some SendMessageA to speed up loading - credit to nukem
	XUtil::PatchMemoryNop(0x004DC191, 0x05);
	XUtil::PatchMemoryNop(0x004DC641, 0x05);
	XUtil::PatchMemoryNop(0x004DC91C, 0x05);

	//	Patch Navmesh editing to allow placing vertices over existing navmesh - credit to StewieA
	SafeWrite16(0x0045C590, 0x9090);

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

	//	nop useless calls to _vsprintf and SendMessage for 'Processing topic %s' - credit to roy/nukem
	XUtil::PatchMemoryNop(0x00591CCE, 0x24);
	XUtil::PatchMemoryNop(0x00592F08, 0x3F);
	SafeWriteBuf(0x592F4A, "\x10\x90\x90\x90", 4); // above removes pushed args, so need to change mov offset and nop the add esp

	//	Make search and replace window stay open unless explicitly close - credit to StewieA
	WriteRelJump(0x0047CE7F, (UINT32)hk_SearchDestroyHook);
	SafeWrite8(0x0047CE84, 0x90);

	// Make the "Do" button of "Reference Batch Action" not grayed out if an action is already selected when initialising the dialog
	WriteRelJump(0x411CCF, UInt32(ReferenceBatchActionDoButtonHook));

	// Remove call to SetFocus(0) when closing Reference Batch Action dialog
	XUtil::PatchMemoryNop(0x411CFA, 8);

	//	Replace zlib with libinflate - credit to nukem/StewieA
	WriteRelCall(0x004DFB34, (UInt32)hk_inflateInit);
	WriteRelCall(0x004DFB9E, (UInt32)hk_inflate);

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
	if (bNoPreviewWindowAutoFocus) {
		WriteRelJump(0x48CAC9, UInt32(hk_PreviewWindowCheckForeground));
	}
	// no "DirectSoundCaptureCreate Error: No sound driver is available for use" popup
	if (bNoDXSoundCaptureErrorPopup) {
		WriteRelCall(0x42B6E0, UInt32(EditorUI_Log));
	}

	if (bRemoveStatusBarLoadingText) {
		// patch additional loading messages to the bottom status bar
		XUtil::PatchMemoryNop(0x4DCC24, 5);
		XUtil::PatchMemoryNop(0x4DC96F, 5);
		XUtil::PatchMemoryNop(0x4DDD4F, 5);

		// make the cell loading bar only update every 128 references instead of every ref
		SafeWriteBuf(0x4BFFA2, "\xF7\xC3\x7F\x00\x00\x00\x74\x3C\xEB\x68", 10);
	}

	if (bPlaySoundEndOfLoading) {
		WriteRelJump(0x464D32, UInt32(EndLoadingHook));
	}

	// add a more detailed warning when recompiling all
	WriteRelJump(0x5C497E, UInt32(RecompileAllWarningScriptHook));
	WriteRelJump(0x4442C7, UInt32(RecompileAllWarningMainHook));

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

	// fix arrow keys in dialogue menu making the window become unfocussed, allowing consecutive arrow presses without having to click again
	SafeWrite16(0x5A1817, 0x0FEB); // jump over SetFocus(0)
	SafeWrite16(0x5993EC, 0x0FEB);
	
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

	// hook Load ESP/ESM window callback
	SafeWrite32(0x44192A, UInt32(hk_LoadESPESMCallback));
	
	// make flycam mouse rotation grid smaller (1*1 instead of 2*2)
	// the game was checking if the difference in mouse position was > 1 rather than >= 1
	SafeWrite8(0x45D3AD, 0x7C);
	SafeWrite8(0x45D3BD, 0x7F);
	SafeWrite8(0x45D3D9, 0x7C);
	SafeWrite8(0x45D3EA, 0x7F);

	// make holding shift slow down the flycam movement by 80%
	WriteRelJump(0x455D12, UInt32(FlycamMovementSpeedMultiplierHook));

	if (bSmoothFlycamRotation) {
		WriteRelJump(0x45D3A3, UInt32(FlycamRotationSpeedMultiplierHook));
		WriteRelJump(0x456A9F, 0x456AE6); // skip call to SetCursorPos when changing to flycam mode
	}
	
	if (bSwapRenderCYKeys) {
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

	//	Create log window - credit to nukem
	InitCommonControls();
	LoadLibraryA("MSFTEDIT.dll");

	if (!EditorUI_CreateLogWindow())
	{
		MessageBoxA(nullptr, "Failed to create console log window", "Error", MB_ICONERROR);
	}

	SafeWrite32(0x0044612D, (UInt32)EditorUI_WndProc);
	OldEditorUI_WndProc = (WNDPROC)0x00440780;

	return true;
}


static int konamiStage = 0;
void doKonami(int key) {
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
			EditorUI_Log("Konami!");
		}
		konamiStage = 0;
		break;
	}
	/* handles the case where up is pressed in the middle of the sequence */
	if (konamiStage == 0 && key == VK_UP) konamiStage++;
}
	
void SaveLogWindowPosition() {
	char buffer[8];
	RECT pos;
	GetWindowRect(g_ConsoleHwnd, &pos);

	WritePrivateProfileString("Log", "iWindowPosX", _itoa(pos.left, buffer, 10), filename);
	WritePrivateProfileString("Log", "iWindowPosY", _itoa(pos.top, buffer, 10), filename);
	WritePrivateProfileString("Log", "iWindowPosDX", _itoa(pos.right-pos.left, buffer, 10), filename);
	WritePrivateProfileString("Log", "iWindowPosDY", _itoa(pos.bottom-pos.top, buffer, 10), filename);
}

void __fastcall FastExitHook(volatile LONG** thiss)
{
	SaveLogWindowPosition();
	if (GetINIExists() && bFastExit) TerminateProcess(GetCurrentProcess(), 0);
	((void(__thiscall *)(volatile LONG **thiss))(0x4CC540))(thiss);
}

/* TODO 
Creature + Leveled creature/character windows
NPC
GameEffects->BaseEffect
Items->Ammo
Items->LeveledItem
Items->Weapon
SpecialEffects->Explosion
WorldObjects->Container

Dirty editors:
ActorData->Race
Audio->Media Location
Audio->Media Set
Items->ArmorAddon
Items->Weapon

MultiCombo Boxes
0x6017A0
0x49A091
*/