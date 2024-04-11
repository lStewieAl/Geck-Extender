#include "Settings.h"
Settings config;
char IniPath[MAX_PATH];
void ReadAllSettings()
{
	//	ini thing - credit to carxt
	GetModuleFileNameA(NULL, IniPath, MAX_PATH);
	strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\geckextender.ini");

	config.bEnableSpellChecker = GetOrCreateINIInt("General", "bEnableSpellChecker", 0, IniPath);
	config.bFastExit = GetOrCreateINIInt("General", "bFastExit", 1, IniPath);
	config.bListEditFix = GetOrCreateINIInt("General", "bListEditFix", 0, IniPath);
	config.bIgnoreNAMFiles = GetOrCreateINIInt("General", "bIgnoreNAMFiles", 1, IniPath);
	config.bVersionControlMode = GetOrCreateINIInt("General", "bVersionControlMode", 0, IniPath);
	config.bHighResLandscapeLOD = GetOrCreateINIInt("General", "bHighResLandscapeLOD", 0, IniPath);
	config.bRemoveStatusBarLoadingText = GetOrCreateINIInt("General", "bRemoveLoadingText", 1, IniPath);
	config.bPlaySoundEndOfLoading = GetOrCreateINIInt("General", "bPlaySoundEndOfLoading", 1, IniPath);
	config.bNoDXSoundCaptureErrorPopup = GetOrCreateINIInt("General", "bNoDXSoundCaptureErrorPopup", 0, IniPath);
	config.bNoPreviewWindowAutoFocus = GetOrCreateINIInt("General", "bNoPreviewWindowAutoFocus", 1, IniPath);
	config.bNoLODMeshMessage = GetOrCreateINIInt("General", "bNoLODMeshMessage", 0, IniPath);
	config.bAutoLoadFiles = GetOrCreateINIInt("General", "bAutoLoadFiles", 0, IniPath);
	config.bShowLoadFilesAtStartup = GetOrCreateINIInt("General", "bShowLoadFilesAtStartup", 0, IniPath) | config.bAutoLoadFiles;
	config.bNoVersionControlWarning = GetOrCreateINIInt("General", "bNoVersionControlWarning", 0, IniPath);
	config.bSkipVanillaLipGen = GetPrivateProfileIntA("General", "bSkipVanillaLipGen", 0, IniPath);
	config.bShowAdditionalToolbarButtons = GetOrCreateINIInt("General", "bShowAdditionalToolbarButtons", 0, IniPath);
	config.bAllowMultipleSearchAndReplace = GetOrCreateINIInt("General", "bAllowMultipleSearchAndReplace", 0, IniPath);
	config.bCacheSearchAndReplaceWindow = GetOrCreateINIInt("General", "bCacheSearchAndReplaceWindow", 1, IniPath);
	config.bNoFactionReactionMessage = GetOrCreateINIInt("General", "bNoFactionReactionMessage", 0, IniPath);
	config.bUISpeedHooks = GetOrCreateINIInt("General", "bUISpeedHooks", 1, IniPath);
	config.bLibdeflate = GetOrCreateINIInt("General", "bLibDeflate", 0, IniPath);
	config.bExpandFormIDColumn = GetOrCreateINIInt("General", "bExpandFormIDColumn", 0, IniPath);
	config.bAllowEditLandEdges = GetOrCreateINIInt("General", "bAllowEditLandEdges", 0, IniPath);
	config.bAllowRecompileAll = GetOrCreateINIInt("General", "bAllowRecompileAll", 0, IniPath);
	config.bNavmeshFindCoverConfirmPrompt = GetOrCreateINIInt("General", "bNavmeshFindCoverConfirmPrompt", 0, IniPath);
	config.bFaceGenOnlyEdited = GetOrCreateINIInt("General", "bFaceGenOnlyEdited", 1, IniPath);
	config.bDisableTextureMirroring = GetOrCreateINIInt("General", "bDisableTextureMirroring", 0, IniPath);
	config.bObjectWindowOnlyShowEditedByDefault = GetOrCreateINIInt("General", "bObjectWindowOnlyShowEditedForms", 0, IniPath);
	config.bPreventFaceAndBodyModExports = GetOrCreateINIInt("General", "bFaceBodyExportPreventTGAFiles", 0, IniPath);
	config.bIgnoreD3D9 = GetOrCreateINIInt("General", "bIgnoreD3D9", 1, IniPath);
	config.bNoRecordCompression = GetOrCreateINIInt("General", "bNoRecordCompression", 1, IniPath);
	config.bPreserveTimestamps = GetOrCreateINIInt("General", "bPreserveTimestamps", 0, IniPath);
	config.bNoDirtyCellWhenNonPersistentRefsDeleted = GetOrCreateINIInt("Experimental", "bNoDirtyCellWhenNonPersistentRefsDeleted", 0, IniPath);

	config.bPatchScriptEditorFont = GetOrCreateINIInt("Script", "bPatchEditorFont", 1, IniPath);
	config.bScriptCompileWarningPopup = GetOrCreateINIInt("Script", "bScriptCompileWarningPopup", 0, IniPath);
	config.bShowScriptChangeTypeWarning = GetOrCreateINIInt("Script", "bShowChangeScriptTypeWarning", 0, IniPath);

	config.bAutoScroll = GetOrCreateINIInt("Log", "bAutoScroll", 1, IniPath);

	config.bRenderWindowUncap = GetOrCreateINIInt("Render Window", "bRenderWindowUncap", 1, IniPath);
	config.bPreviewWindowUncap = GetOrCreateINIInt("Render Window", "bPreviewWindowUncap", 1, IniPath);
	config.bSwapRenderCYKeys = GetOrCreateINIInt("Render Window", "bSwapCandYKeys", 0, IniPath);
	config.bUseAltShiftMultipliers = GetOrCreateINIInt("Render Window", "bUseAltShiftMultipliers", 1, IniPath);
	config.fMovementAltMultiplier = GetOrCreateINIInt("Render Window", "iAltSpeedPct", 15, IniPath) / 100.0F;
	config.fMovementShiftMultiplier = GetOrCreateINIInt("Render Window", "iShiftSpeedPct", 200, IniPath) / 100.0F;
	config.bShowTimeOfDaySlider = GetOrCreateINIInt("Render Window", "bShowTimeOfDaySlider", 1, IniPath);
	config.bNavmeshAllowPlaceAboveOthers = GetOrCreateINIInt("Render Window", "bNavmeshAllowPlaceAboveOthers", 1, IniPath);
	config.bSnapToGridRotationUseDoublePrecision = GetOrCreateINIInt("Render Window", "bSnapToGridRotationUseDoublePrecision", 0, IniPath);
	config.bAutoLightWarnings = GetOrCreateINIInt("Render Window", "bAutoLightWarnings", 0, IniPath);
	config.iRenderFOV = GetOrCreateINIInt("Render Window", "iFOV", 90, IniPath);

	config.bSmoothFlycamRotation = GetOrCreateINIInt("Flycam", "bSmoothRotation", 1, IniPath);
	config.bFlycamUpDownRelativeToWorld = GetOrCreateINIInt("Flycam", "bFlycamUpDownRelativeToWorld", 1, IniPath);
	config.fFlycamRotationSpeed = GetOrCreateINIInt("Flycam", "iRotationSpeedPct", 100, IniPath) * -0.001F;
	config.fFlycamNormalMovementSpeed = GetOrCreateINIInt("Flycam", "iMovementSpeed", 10, IniPath) * 3.0F;
	config.fFlycamShiftMovementSpeed = GetOrCreateINIInt("Flycam", "iRunMovementSpeedPct", 300, IniPath) / 100.0F;
	config.fFlycamAltMovementSpeed = GetOrCreateINIInt("Flycam", "iWalkMovementSpeedPct", 20, IniPath) / 100.0F;

	config.bAppendAnimLengthToName = GetOrCreateINIInt("Preview Window", "bAppendAnimLengthToName", 0, IniPath);

	config.bObjectPaletteAllowRandom = GetOrCreateINIInt("Object Palette", "bObjectPaletteAllowRandom", 1, IniPath);
	config.bObjectPaletteRandomByDefault = GetOrCreateINIInt("Object Palette", "bObjectPaletteRandomByDefault", 0, IniPath) != 0;

	config.bRemoveDialogSoundFilter = GetOrCreateINIInt("Dialog", "bRemoveDialogSoundFilter", 1, IniPath) != 0;
	config.bCacheComboboxes = GetOrCreateINIInt("Dialog", "bCacheComboboxes", 1, IniPath) != 0;
}

#define INI_SETTING_NOT_FOUND -1
int GetOrCreateINIInt(const char* sectionName, const char* keyName, int defaultValue, const char* filename) {
	int settingValue = GetPrivateProfileIntA(sectionName, keyName, INI_SETTING_NOT_FOUND, filename);
	if (settingValue == INI_SETTING_NOT_FOUND) {
		char arr[11];
		WritePrivateProfileString(sectionName, keyName, _itoa(defaultValue, arr, 10), filename);
		settingValue = defaultValue;
	}
	return settingValue;
}
#undef INI_SETTING_NOT_FOUND