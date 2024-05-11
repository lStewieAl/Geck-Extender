#pragma once
struct Settings
{
	int bPatchScriptEditorFont;
	int bHighResLandscapeLOD;
	int bListEditFix;
	int bVersionControlMode;
	int bFastExit;
	int bIgnoreNAMFiles;
	int bEnableSpellChecker;
	int bAutoScroll;
	int bRenderWindowUncap;
	int bPreviewWindowUncap;
	int bRemoveStatusBarLoadingText;
	int bPlaySoundEndOfLoading;
	int bNoDXSoundCaptureErrorPopup;
	int bNoPreviewWindowAutoFocus;
	int bNoLODMeshMessage;
	int bSwapRenderCYKeys;
	int bAutoLoadFiles;
	int bShowLoadFilesAtStartup;
	int bScriptCompileWarningPopup;
	int bNoVersionControlWarning;
	int bShowTimeOfDaySlider;
	int bSkipVanillaLipGen;
	int bShowAdditionalToolbarButtons;
	int bAllowMultipleSearchAndReplace;
	int bCacheSearchAndReplaceWindow;
	int bNoFactionReactionMessage;
	int bUISpeedHooks;
	int bLibdeflate;
	int bExpandFormIDColumn;
	int bAllowEditLandEdges;
	int bNavmeshAllowPlaceAboveOthers;
	int bAllowRecompileAll;
	int bNavmeshFindCoverConfirmPrompt;
	int bShowScriptChangeTypeWarning;
	int bAppendAnimLengthToName;
	int bObjectPaletteAllowRandom;
	bool bObjectPaletteRandomByDefault;
	int bSnapToGridRotationUseDoublePrecision;
	int bFaceGenOnlyEdited;
	int bObjectWindowOnlyShowEditedByDefault;
	int bDisableTextureMirroring;
	int bPreventFaceAndBodyModExports;
	int bIgnoreD3D9;
	int bAutoLightWarnings;
	int bRemoveDialogSoundFilter;
	int bCacheComboboxes;
	int bNoRecordCompression;
	int bNoFacegenCompression;
	int bPreserveTimestamps;
	int bNoDirtyCellWhenNonPersistentRefsDeleted;
	bool bHideRenderWindow;
	bool bHideObjectWindow;
	bool bHideCellViewWindow;
	bool bDarkMode;
	bool bNumericEditorIdWarning;
	bool bSkipSplashScreen;
	bool bDataMenuSpaceTogglesSelectedFile;
	bool bNewPluginsDefaultToESM;
	bool bAllowHardDeletionInESMs;

	int bUseAltShiftMultipliers;
	float fMovementAltMultiplier;
	float fMovementShiftMultiplier;

	int bSmoothFlycamRotation;
	int bFlycamUpDownRelativeToWorld;
	float fFlycamRotationSpeed;
	float fFlycamNormalMovementSpeed;
	float fFlycamShiftMovementSpeed;
	float fFlycamAltMovementSpeed;

	int iRenderFOV;

	UInt8 iPreviewWindowRed, iPreviewWindowGreen, iPreviewWindowBlue;

	char* sLaunchExeName;
};

extern Settings config;
extern char IniPath[MAX_PATH];

int GetOrCreateINIValue(const char* sectionName, const char* keyName, int defaultValue, const char* filename);
char* GetOrCreateINIValue(const char* sectionName, const char* keyName, const char* defaultValue, const char* filename);
void ReadAllSettings();