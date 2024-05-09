#include "Settings.h"
Settings config;
char IniPath[MAX_PATH];
void ReadAllSettings()
{
	//	ini thing - credit to carxt
	GetModuleFileNameA(NULL, IniPath, MAX_PATH);
	strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\geckextender.ini");

	config.bEnableSpellChecker = GetOrCreateINIValue("General", "bEnableSpellChecker", 0, IniPath);
	config.bFastExit = GetOrCreateINIValue("General", "bFastExit", 1, IniPath);
	config.bListEditFix = GetOrCreateINIValue("General", "bListEditFix", 0, IniPath);
	config.bIgnoreNAMFiles = GetOrCreateINIValue("General", "bIgnoreNAMFiles", 1, IniPath);
	config.bVersionControlMode = GetOrCreateINIValue("General", "bVersionControlMode", 0, IniPath);
	config.bHighResLandscapeLOD = GetOrCreateINIValue("General", "bHighResLandscapeLOD", 0, IniPath);
	config.bRemoveStatusBarLoadingText = GetOrCreateINIValue("General", "bRemoveLoadingText", 1, IniPath);
	config.bPlaySoundEndOfLoading = GetOrCreateINIValue("General", "bPlaySoundEndOfLoading", 1, IniPath);
	config.bNoDXSoundCaptureErrorPopup = GetOrCreateINIValue("General", "bNoDXSoundCaptureErrorPopup", 0, IniPath);
	config.bNoPreviewWindowAutoFocus = GetOrCreateINIValue("General", "bNoPreviewWindowAutoFocus", 1, IniPath);
	config.bNoLODMeshMessage = GetOrCreateINIValue("General", "bNoLODMeshMessage", 0, IniPath);
	config.bAutoLoadFiles = GetOrCreateINIValue("General", "bAutoLoadFiles", 0, IniPath);
	config.bShowLoadFilesAtStartup = GetOrCreateINIValue("General", "bShowLoadFilesAtStartup", 0, IniPath) | config.bAutoLoadFiles;
	config.bNoVersionControlWarning = GetOrCreateINIValue("General", "bNoVersionControlWarning", 0, IniPath);
	config.bSkipVanillaLipGen = GetPrivateProfileIntA("General", "bSkipVanillaLipGen", 0, IniPath);
	config.bShowAdditionalToolbarButtons = GetOrCreateINIValue("General", "bShowAdditionalToolbarButtons", 0, IniPath);
	config.bAllowMultipleSearchAndReplace = GetOrCreateINIValue("General", "bAllowMultipleSearchAndReplace", 0, IniPath);
	config.bCacheSearchAndReplaceWindow = GetOrCreateINIValue("General", "bCacheSearchAndReplaceWindow", 1, IniPath);
	config.bNoFactionReactionMessage = GetOrCreateINIValue("General", "bNoFactionReactionMessage", 0, IniPath);
	config.bUISpeedHooks = GetOrCreateINIValue("General", "bUISpeedHooks", 1, IniPath);
	config.bLibdeflate = GetOrCreateINIValue("General", "bLibDeflate", 0, IniPath);
	config.bExpandFormIDColumn = GetOrCreateINIValue("General", "bExpandFormIDColumn", 0, IniPath);
	config.bAllowEditLandEdges = GetOrCreateINIValue("General", "bAllowEditLandEdges", 0, IniPath);
	config.bAllowRecompileAll = GetOrCreateINIValue("General", "bAllowRecompileAll", 0, IniPath);
	config.bNavmeshFindCoverConfirmPrompt = GetOrCreateINIValue("General", "bNavmeshFindCoverConfirmPrompt", 0, IniPath);
	config.bFaceGenOnlyEdited = GetOrCreateINIValue("General", "bFaceGenOnlyEdited", 1, IniPath);
	config.bDisableTextureMirroring = GetOrCreateINIValue("General", "bDisableTextureMirroring", 0, IniPath);
	config.bObjectWindowOnlyShowEditedByDefault = GetOrCreateINIValue("General", "bObjectWindowOnlyShowEditedForms", 0, IniPath);
	config.bPreventFaceAndBodyModExports = GetOrCreateINIValue("General", "bFaceBodyExportPreventTGAFiles", 0, IniPath);
	config.bIgnoreD3D9 = GetOrCreateINIValue("General", "bIgnoreD3D9", 1, IniPath);
	config.bNoRecordCompression = GetOrCreateINIValue("General", "bNoRecordCompression", 1, IniPath);
	config.bPreserveTimestamps = GetOrCreateINIValue("General", "bPreserveTimestamps", 0, IniPath);
	config.bDarkMode = GetOrCreateINIValue("General", "bDarkMode", 0, IniPath);
	config.bNumericEditorIdWarning = GetOrCreateINIValue("General", "bNumericEditorIdWarning", 0, IniPath);
	config.bSkipSplashScreen = GetOrCreateINIValue("General", "bSkipSplashScreen", 0, IniPath);
	config.bDataMenuSpaceTogglesSelectedFile = GetOrCreateINIValue("General", "bDataMenuSpaceTogglesSelectedFile", 1, IniPath);

	config.bNoDirtyCellWhenNonPersistentRefsDeleted = GetOrCreateINIValue("Experimental", "bNoDirtyCellWhenNonPersistentRefsDeleted", 0, IniPath);

	config.bPatchScriptEditorFont = GetOrCreateINIValue("Script", "bPatchEditorFont", 1, IniPath);
	config.bScriptCompileWarningPopup = GetOrCreateINIValue("Script", "bScriptCompileWarningPopup", 1, IniPath);
	config.bShowScriptChangeTypeWarning = GetOrCreateINIValue("Script", "bShowChangeScriptTypeWarning", 0, IniPath);

	config.bAutoScroll = GetOrCreateINIValue("Log", "bAutoScroll", 1, IniPath);

	config.bRenderWindowUncap = GetOrCreateINIValue("Render Window", "bRenderWindowUncap", 1, IniPath);
	config.bPreviewWindowUncap = GetOrCreateINIValue("Render Window", "bPreviewWindowUncap", 1, IniPath);
	config.bSwapRenderCYKeys = GetOrCreateINIValue("Render Window", "bSwapCandYKeys", 0, IniPath);
	config.bUseAltShiftMultipliers = GetOrCreateINIValue("Render Window", "bUseAltShiftMultipliers", 1, IniPath);
	config.fMovementAltMultiplier = GetOrCreateINIValue("Render Window", "iAltSpeedPct", 15, IniPath) / 100.0F;
	config.fMovementShiftMultiplier = GetOrCreateINIValue("Render Window", "iShiftSpeedPct", 200, IniPath) / 100.0F;
	config.bShowTimeOfDaySlider = GetOrCreateINIValue("Render Window", "bShowTimeOfDaySlider", 1, IniPath);
	config.bNavmeshAllowPlaceAboveOthers = GetOrCreateINIValue("Render Window", "bNavmeshAllowPlaceAboveOthers", 1, IniPath);
	config.bSnapToGridRotationUseDoublePrecision = GetOrCreateINIValue("Render Window", "bSnapToGridRotationUseDoublePrecision", 0, IniPath);
	config.bAutoLightWarnings = GetOrCreateINIValue("Render Window", "bAutoLightWarnings", 0, IniPath);
	config.iRenderFOV = GetOrCreateINIValue("Render Window", "iFOV", 90, IniPath);

	config.bSmoothFlycamRotation = GetOrCreateINIValue("Flycam", "bSmoothRotation", 1, IniPath);
	config.bFlycamUpDownRelativeToWorld = GetOrCreateINIValue("Flycam", "bFlycamUpDownRelativeToWorld", 1, IniPath);
	config.fFlycamRotationSpeed = GetOrCreateINIValue("Flycam", "iRotationSpeedPct", 100, IniPath) * -0.001F;
	config.fFlycamNormalMovementSpeed = GetOrCreateINIValue("Flycam", "iMovementSpeed", 10, IniPath) * 3.0F;
	config.fFlycamShiftMovementSpeed = GetOrCreateINIValue("Flycam", "iRunMovementSpeedPct", 300, IniPath) / 100.0F;
	config.fFlycamAltMovementSpeed = GetOrCreateINIValue("Flycam", "iWalkMovementSpeedPct", 20, IniPath) / 100.0F;

	config.bAppendAnimLengthToName = GetOrCreateINIValue("Preview Window", "bAppendAnimLengthToName", 0, IniPath);

	config.bObjectPaletteAllowRandom = GetOrCreateINIValue("Object Palette", "bObjectPaletteAllowRandom", 1, IniPath);
	config.bObjectPaletteRandomByDefault = GetOrCreateINIValue("Object Palette", "bObjectPaletteRandomByDefault", 0, IniPath) != 0;

	config.bRemoveDialogSoundFilter = GetOrCreateINIValue("Dialog", "bRemoveDialogSoundFilter", 1, IniPath) != 0;
	config.bCacheComboboxes = GetOrCreateINIValue("Dialog", "bCacheComboboxes", 1, IniPath) != 0;

	config.sLaunchExeName = GetOrCreateINIValue("Launch Game", "ExecutableName", "FalloutNV.exe", IniPath);

	config.bHideRenderWindow = GetOrCreateINIValue("Windows", "bHideRenderWindow", 0, IniPath);
	config.bHideObjectWindow = GetOrCreateINIValue("Windows", "bHideObjectWindow", 0, IniPath);
	config.bHideCellViewWindow = GetOrCreateINIValue("Windows", "bHideCellViewWindow", 0, IniPath);
}

#define INI_SETTING_NOT_FOUND -1
int GetOrCreateINIValue(const char* sectionName, const char* keyName, int defaultValue, const char* filename) {
	int settingValue = GetPrivateProfileIntA(sectionName, keyName, INI_SETTING_NOT_FOUND, filename);
	if (settingValue == INI_SETTING_NOT_FOUND) {
		char arr[11];
		WritePrivateProfileString(sectionName, keyName, _itoa(defaultValue, arr, 10), filename);
		settingValue = defaultValue;
	}
	return settingValue;
}

char* GetOrCreateINIValue(const char* sectionName, const char* keyName, const char* defaultValue, const char* filename) {
	char buf[MAX_PATH]; *buf = '\0';
	GetPrivateProfileStringA(sectionName, keyName, "NOTFOUND", buf, sizeof(buf), filename);
	if (!strcmp(buf, "NOTFOUND")) {
		WritePrivateProfileString(sectionName, keyName, defaultValue, filename);
		return _strdup(defaultValue);
	}
	return _strdup(buf);
}

#undef INI_SETTING_NOT_FOUND