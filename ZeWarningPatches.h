#pragma once

static const UInt32 kPatch_Warnings1[] =
//	These are pushes to 0x00AA4830 for FaceFX warnings
{
	0x0077BDF1,	//	FxLinkFn
	0x0077BF21,	//	FxNullLinkFn
	0x0077BFB1,	//	FxLinearLinkFn
	0x0077C041,	//	FxQuadraticLinkFn
	0x0077C0D1,	//	FxCubicLinkFn
	0x0077C161,	//	FxSqrtLinkFn
	0x0077C1F1,	//	FxNegateLinkFn
	0x0077C281,	//	FxInverseLinkFn
	0x0077C311,	//	FxOneClampLinkFn
	0x0077C3A1,	//	FxConstantLinkFn
	0x0077C431,	//	FxCorrectiveLinkFn
	0x0077C4C1,	//	FxClampedLinearLinkFn
	0x007807F1,	//	FxFaceGraphNode
	0x00783A51,	//	FxActorInstance
};

static const UInt32 kPatch_Warnings2[] =

//	These are movs to 0x00AA4830 for unknown warnings

{
	0x007DEE86,	//	
	0x007DEEA4,	//	
	0x007DF31B,	//	
	0x007DF339,	//	
	0x007DFEE7,	//	
	0x007DFF05,	//	
	0x007E10E6,	//	
	0x007E1104,	//	
	0x007E1937,	//	
	0x007E1955,	//	
};

static const UInt32 kPatch_Warnings3[] =
//	These are the normal warning message calls to 0x00AA4830
{
	0x004161F0,	//	Failed to load data from file %s.
	0x0041D507,	//	Audio: Could not find TESSound corresponding to form name "%s"
	0x0041E3CC,	//	Facial animation analysis failed for %s.
	0x0042A4E7,	//	Renderer creation failed with error:\n\n%s
	0x0042BCD9,	//	Facial animations - Could not open file %s.
	0x0042BFB8,	//	Facial animation analysis problem - result may not be accurate %s.
	0x0042C02C,	//	Facial animation - unknown error resampling file %s.
	0x0042C173,	//	Facial animations - Sound format error: Bits per sample = %i (should be 16) Channels = %i (should be 1) in file %s.
	0x0042CFCC,	//	TESBitArrayFile::OpenAndMapBitArrayFile could not find file '%s'.
	0x0042D06C,	//	DeviceIoControl failed to open "%s" in TESBitArrayFile::OpenAndMapBitArrayFile\n%s
	0x0042D0AB,	//	File %s is an invalid size for a bit array file.
	0x0042D169,	//	This operation was not completed because CreateFileMapping failed to open file \"%s\" with error:\n%s\n\nExit other applications, restart the Construction Set, and try again.
	0x00437BC1,	//	(%08X) '%s' checked out by '%s'
	0x00442997,	//	WARNING: The first child of placeable water object %s (%08X) is not a multibound node.
	0x00442A1D,	//	Could not load geometry for placeable water object %s (%08X).
	0x00443921,	//	Warnings were encountered (either a reference was checked out or a refraction flag was cleared). Check warnings file for more details.
	0x00444247,	//	Failed to check out object '%s' (%08X) to fix FG model availability.
	0x00445F8D,	//	Remapping computerName from %s to %s. To stop, change ReMapIDs INI setting for user.
	0x0044AAAD,	//	Full Name for '%s' (%08X) ends in a space.
	0x0044AAF7,	//	--Could not fix!!
	0x0044FA11,	//	Failed to move reference '%s' (%08X). Unable to find new parent cell.
	0x0046802D,	//	[General] sNetwork Path needs to be set in file :\r\n\r\n%s.
	0x00468108,	//	All the user slots are filled in %s.
	0x00468C93,	//	File "%s" exceeds maximum file size. Operation canceled.
	0x00468D55,	//	This operation was not completed because CreateFileMapping failed to open file \"%s\" with error:\n%s\n\nExit other applications, restart the Construction Set, and try again.
	0x00468E1A,	//	This operation was not completed because CreateFileMapping failed to open file \"%s\" with error:\n%s\n\nExit other applications, restart the Construction Set, and try again.
	0x00469839,	//	Form at offset %i has an invalid length.
	0x0046992D,	//	Form Group at offset %i has an invalid length.
	0x00469C8C,	//	File index %i is invalid. Clamping to 01. Does a master file depend on more than one master?
	0x0046A918,	//	Master file %s does not contain form groups. This master file must be upgraded before it can be used.
	0x0046AA36,	//	DeviceIoControl failed to open \"%s\" in TESVersionControl::OpenFile\n%s
	0x0046AD80,	//	Form Group at offset %i extends past the file end.
	0x0046AD99,	//	Form Group at offset %i has length 0.
	0x0046ADD7,	//	Form Group at offset %i contains a FORM it should not at offset %i.
	0x0046AE2A,	//	Form Group at offset %i does not end at a FORM.
	0x0046AE6E,	//	Form Group at offset %i ends at a FORM it should not at offset %i.
	0x0046B204,	//	Form (%08X) exceeds maximum size.
	0x0046B2BC,	//	Form insert at offset %i beyond the end of group at offset %i is changing that group's size. File may be corrupt.
	0x0046C5CD,	//	File index %i is invalid. Clamping to 01. Does a master file depend on more than one master?
	0x0046C83E,	//	File index %i is invalid. Clamping to 01. Does a master file depend on more than one master?
	0x0046C881,	//	ID for Form %s '%s' (%08X) is not marked as used.
	0x0046C8F4,	//	User for Form %s '%s' (%08X) is %i (%s) in file but %i (%s) as loaded.
	0x0046C9CF,	//	Failed to create bit array files for file %s.
	0x0046D7D7,	//	Duplicate Form Group at offset %i in the file. The file may need to be re-sorted.
	0x0046DA9F,	//	rying to close group with invalid length.\r\nGroup offset: %i\tFORM offset: %i.
	0x0046DC12,	//	Failed to create temp copy file with error %i.
	0x0046DCE9,	//	Failed to create buffer for new file.
	0x0046E282,	//	Failed to open file for ID fix.
	0x0046E540,	//	Failed to create temp copy file with error %i.
	0x0046E62A,	//	Failed to create buffer for new file.
	0x0046EB70,	//	Form (%08X) delta exceeds maximum size.
	0x0046ED11,	//	Form removal exceeds maximum size. File may end in uninitialized data.
	0x0046FED8,	//	Could not find TESForm for FORM %s (%08X).
	0x00470067,	//	Could not find form %s '%s' (%08X).
	0x004700D0,	//	Form %s '%s' (%08X) was found outside the bounds of the open group at offset %i.
	0x00470273,	//	Form Group at offset %i extends past the file end.
	0x0047028C,	//	Form Group at offset %i has length 0.
	0x0047079E,	//	Check in the cell '%s' (%08X) before checking in form '%s' (%08X).
	0x00470A1D,	//	Check in the preceding Topic Info (%08X) before checking in Topic Info (%08X) \"%s\".
	0x00470CA2,	//	Failed to create offset array for worldspace '%s' (%08X) due to invalid width (%i) or height (%i).
	0x00470DD0,	//	Cell (%08X) (%i, %i) is out of bounds for worldspace '%s' (%08X).
	0x00470E0A,	//	Could not find cell (%08X) for worldspace '%s' (%08X).
	0x00471206,	//	File index %i is invalid. Clamping to 01. Does a master file depend on more than one master?
	0x004714A2,	//	File index %i is invalid. Clamping to 01. Does a master file depend on more than one master?
	0x00471CE8,	//	Could not find form for %s (%08X).
	0x00472095,	//	Failed to create temp copy file with error %i.
	0x004721F0,	//	Failed to create buffer for new file.
	0x0047243F,	//	Failed to update offset array for worldspace '%s' (%08X).
	0x004725A0,	//	Failed to find form (%08X). SKIPPING GMST
	0x004725BA,	//	Failed to find form (%08X).
	0x00472E1B,	//	Failed to create temp copy file with error %i.
	0x00472F3A,	//	Failed to create buffer for new file.
	0x00473D4F,	//	Persistent size: %i MB\tVoice Data size: %i MB
	0x00475C7E,	//	Attempting to change file buffersize after file is mapped!
	0x00475EE7,	//	Failed to check in form %s '%s' (%08X).
	0x00476123,	//	Failed to update offset array for worldspace '%s' (%08X).
	0x004761A2,	//	Failed to update owned ID array for file '%s'.
	0x0047A080,	//	TESDialog trying to use dialog box template for unknown type '%s'.
	0x0047A096,	//	TESDialog trying to use dialog box template for unknown type '%s'.
	0x00480232,	//	Dialog box instantiated incorrectly.
	0x00480C04,	//	Dialog box instantiated incorrectly.
	0x00483DAC,	//	Dialog box instantiated incorrectly.
	0x0048ECF8,	//	Could not convert icon file \"%s\" to RGBA32.
	0x0048F24D,	//	Icon file \"%s\" not found.
	0x0048F2B1,	//	Icon/Texture file '%s' (%dx%d) incorrect size. Image must be %dx%d.
	0x0048F2FF,	//	Icon/Texture file '%s' (%dx%d) incorrect size. Image must be %dx%d.
	0x0048F344,	//	Icon/Texture file '%s' has %d mipmaps but it shouldn't have any.
	0x004942F8,	//	FORMS: Could not find combat style (%08X) on package '%s' (%08X).
	0x0049433D,	//	FORMS: Package '%s' (%08X) is marked as must complete but cannot be completed.
	0x004944A3,	//	FORMS: Package '%s' (%08X) has an invalid flag set to true. To fix, check out, view, verify flags, hit OK, then check back in.
	0x00496F40,	//	On Begin script for package %s has text but has not been compiled.
	0x00496F7D,	//	On Change script for package %s has text but has not been compiled.
	0x00496FB7,	//	On End script for package %s has text but has not been compiled.
	0x0049A0D1,	//	FORMS: Failed to create tab window.
	0x0049B0EE,	//	FORMS: Failed to find topic (%08X) in init for dialogue package data on package '%s' (%08X).
	0x0049CFFC,	//	MASTERFILE: Unable to find Idle (%08X) on owner object (%08X).
	0x0049D0B5,	//	MASTERFILE: Unable to find Topic (%08X) on owner object (%08X).
	0x0049D161,	//	MASTERFILE: Errors were encountered during InitItem for result script on eventaction for package '%s' (%08X)\n\nSee Warnings file for more information.
	0x0049E3ED,	//	MASTERFILE: Could not find Package (%08X) for %s.
	0x0049FAE8,	//	MASTERFILE: Unable to find Package Location Object (%08X) on owner object \"%s\".
	0x0049FB0F,	//	MASTERFILE: Unable to find Package Location Object (%08X) on owner object (%08X).
	0x0049FB86,	//	MASTERFILE: Unreachable Package Location Cell (%08X) '%s' on owner object \"%s\".
	0x0049FBC9,	//	MASTERFILE: Invalid Package Location Cell (%08X) '%s' (%i, %i) on owner object \"%s\". Only Interior cells are allowed
	0x0049FC13,	//	MASTERFILE: Unable to find Package Location Cell (%08X) on owner object \"%s\".
	0x0049FC3A,	//	MASTERFILE: Unable to find Package Location Cell (%08X) on owner object (%08X).
	0x0049FCA0,	//	MASTERFILE: Package Location Reference (%08X) on owner object \"%s\" is not persistent. Initialization may fail in game.
	0x0049FCF0,	//	MASTERFILE: Unable to find Package Location Reference (%08X) on owner object (%08X).
	0x004A15E6,	//	MASTERFILE: Unable to find Package Target Object (%08X) on owner object \"%s\".
	0x004A1602,	//	MASTERFILE: Unable to find Package Target Object (%08X) on owner object (%08X).
	0x004A161A,	//	MASTERFILE: Unable to find Package Target Object (%08X) on unknown owner.
	0x004A1687,	//	MASTERFILE: Unable to find Package Target Reference (%08X) on owner object \"%s\".
	0x004A16A3,	//	MASTERFILE: Unable to find Package Target Reference (%08X) on owner object (%08X).
	0x004A16BB,	//	MASTERFILE: Unable to find Package Target Reference (%08X) on unknown owner.
	0x004A16EB,	//	MASTERFILE: Package Target Reference (%08X) on owner object \"%s\" is not persistent. Initialization may fail in game.
	0x004A4724,	//	MASTERFILE: Could not find reference (%08X) for condition item on owner form %s '%s' (%08X).
	0x004A4738,	//	MASTERFILE: Could not find reference (%08X) for condition item on UNKNOWN owner.
	0x004A4872,	//	MASTERFILE: %s in file %s contains bad condition item data. Function \"%s\" is not a condition function.
	0x004A4A42,	//	MASTERFILE: Unable to find Value TESGlobal (%08X) in TESConditionItem Init for form:\n\n%s.
	0x004A68FD,	//	MASTERFILE: Unable to find Function Info TESForm (%08X) in TESConditionItem Parameter Init for %s.
	0x004A69B8,	//	MASTERFILE: Non-Persistent Function Info Reference (%08X) in TESConditionItem Parameter for %s. Initialization may fail in game.
	0x004A6A56,	//	MASTERFILE: Invalid Function Info Cell (%08X) '%s' (%i, %i) in TESConditionItem Parameter for %s. Only Interior cells are allowed
	0x004A6B23,	//	MASTERFILE: Unable to find variableID %d on script '%s' in TESConditionItem Parameter Init for %s.
	0x004A7879,	//	MASTERFILE: Failed to find owner form (%08X) in init for ContainerItemExtra for form '%s' (%08X).
	0x004A790B,	//	MASTERFILE: Failed to find owner global (%08X) in init for ContainerItemExtra for form '%s' (%08X).
	0x004AD024,	//	FORMS: Reference %s %08X has no parent save cell
	0x004AF76D,	//	MASTERFILE: Could not find radio range position ref (%08X).
	0x004AF78F,	//	MASTERFILE: Invalid radio range position ref (%08X). Exteriors only.
	0x004AF7DB,	//	MASTERFILE: Invalid LOCK extra found in extra data list.
	0x004AF84A,	//	MASTERFILE: Unable to find key %08X for lock data. Lock will be removed.
	0x004AF88E,	//	MASTERFILE: Invalid TELEPORT extra found in extra data list.
	0x004AF98C,	//	MASTERFILE: Could not find MediaLocationController (%08X) for a AudioMarker in extra data list.
	0x004AF9C9,	//	MASTERFILE: Unable to find ownership owner form %08X. Ownership will be removed.
	0x004AFA37,	//	MASTERFILE: Unable to find ownership condition global %08X. Ownership will be removed.
	0x004AFA9C,	//	MASTERFILE: Unable to find poison %08X. Poison data will be removed.
	0x004AFB01,	//	MASTERFILE: Unable to find cell climate %08X. Climate data will be removed.
	0x004AFB66,	//	MASTERFILE: Unable to find cell imagespace %08X. ImageSpace data will be removed.
	0x004AFBE3,	//	MASTERFILE: Unable to find cell acoustic space %08X. AcousticSpace data will be removed.
	0x004AFC48,	//	MASTERFILE: Unable to find cell water type %08X. Water data will be removed.
	0x004AFCAD,	//	MASTERFILE: Unable to find cell music type %08X. Music data will be removed.
	0x004AFCF8,	//	MASTERFILE: Unable to find package start location cell %08X. Package start location extra data will be removed.
	0x004AFD59,	//	MASTERFILE: Unable to find enable state parent %08X. Enable state parent data will be removed.
	0x004AFDB7,	//	MASTERFILE: Enable state parent loop detected. Parent removed.
	0x004AFE1E,	//	MASTERFILE: Unable to find random door teleport marker %08X. Random door teleport marker data will be removed.
	0x004AFE7F,	//	MASTERFILE: Unable to find linked reference %08X. Linked reference data will be removed.
	0x004AFF52,	//	No occlusion plane for occlusion plane ref extra.
	0x004AFF85,	//	MASTERFILE: Unable to find occlusion plane reference %08X. Occlusion plane reference data will be removed.
	0x004B000E,	//	MASTERFILE: Unable to find portal linked reference %08X. Portal linked reference data will be removed.
	0x004B0043,	//	MASTERFILE: Both of portal %08X's linked rooms are the same (%08X). Attempting to fix.
	0x004B006F,	//	MASTERFILE: Both of portal %08X's linked rooms are NULL.
	0x004B0176,	//	MASTERFILE: Unable to find activate reference %08X.
	0x004B023A,	//	MASTERFILE: Removing empty decal extra.
	0x004B029F,	//	MASTERFILE: Unable to find merchant container %08X. Merchant container data will be removed.
	0x004B0304,	//	MASTERFILE: Unable to find XMarker target %08X. XMarker target data will be removed.
	0x004B0369,	//	Unable to find encounter zone %08X. Encounter zone data will be removed.
	0x004B03CC,	//	MASTERFILE: Unable to find emittance source %08X. Emittance source data will be removed.
	0x004B044A,	//	MASTERFILE: Unable to find multibound ref %08X used by reference %08X. Multibound ref data will be removed.
	0x004B0482,	//	MASTERFILE: Removing empty reflector water extra.
	0x004B04BA,	//	MASTERFILE: Removing empty lit water extra.
	0x004B0525,	//	Unable to find ammo %08X. Ammo data will be removed.
	0x004B05B0,	//	DOOR: Reference %s %08X has portal to navmesh %08X which is not found.  Please finalize navmesh in cell %s %08X to resolve this.
	0x004B18A1,	//	AI: No Copy function available for Extra Data type %i.
	0x004B3416,	//	MASTERFILE: Failed to load RagDoll Data.
	0x004B426F,	//	MASTERFILE: Invalid Extra Data - Region List in file \"%s\".
	0x004B47D7,	//	Unable to complete operation due to failure removing previous file.
	0x004B4810,	//	Unable to complete operation due to failure removing previous file.
	0x004B4F19,	//	FORMS: Could not check out form '08X' during import
	0x004B4F56,	//	FORMS: Could not check out form '08X' during import
	0x004B502F,	//	FORMS: Could not check out form '08X' during import
	0x004B5042,	//	FORMS: Form %08X no longer has an override, yet I'm trying to set it to %s
	0x004BADDA,	//	MASTERFILE: Could not find linked door (%08X) in teleport data init.
	0x004BAE27,	//	MASTERFILE: Linked door (%08X) in teleport data points to invalid object (%s %s(%08X)).
	0x004BAE57,	//	MASTERFILE: Linked door '%s' (%08X) in teleport data has no parent cell.
	0x004BC1E6,	//	MODELS: %s: Reexport '%s' to get rid of the ZBuffer and/or VertextColor property.
	0x004BC1FA,	//	MODELS: Reexport '%s' to get rid of the ZBuffer and/or VertextColor property.
	0x004BC3ED,	//	LoadedFile %s was loaded at %i and added to the map, but is being thrown away without being used
	0x004BC8C2,	//	MODELS: Could not get file for texture with file entry offset %i and size %i.
	0x004BC8D9,	//	MODELS: Could not get file for texture %s.
	0x004BE54F,	//	MODEL ERROR: Could not %s file '%s'.
	0x004C1849,	//	ANIMATION: Could not create ControllerManager Sequence for \"%s\".\r\n
	0x004C1A45,	//	MODEL ERROR: Could not %s file '%s'.
	0x004C3493,	//	MODELS: Could not get file for %s.
	0x004C50CB,	//	MODELS:\t%3d\t%3d\t%3d\t%s\r\n
	0x004C51C5,	//	ANIMATION:\t%3d\t%3d\t%3d\t%s\r\n
	0x004C5BBA,	//	MODELS: ModelLoader still contains %d NIF files.\r\n
	0x004C5CF3,	//	ANIMATION: ModelLoader still has %d KF files.\r\n
	0x004C9624,	//	MODELS: TES::CreateTextureImage unable to create image for \"%s\".\r\n
	0x004CAE13,	//	MODELS: Model '%s' for non-light object '%s' contains %i lights.
	0x004CBF60,	//	Unable to complete operation due to failure removing previous file.
	0x004CD451,	//	CELLS: \r\n\r\n**************** %s (%d,%d) - (%d,%d) ****************
	0x004CD558,	//	CELLS: \r\n\r\n**************** Interiors ****************
	0x004CD827,	//	Unknown
	0x004CD88B,	//	CELLS: Cell \"%s\" (Interior) M#: %s T#: %.0f Time: %.1f Lights: %d Portals: %s TexTotal: %i MB Actors: %i
	0x004CD93C,	//	CELLS: Cell \"%s\" (%d, %d) M#: %s T#: %.0f Time: %.1f Portals: %s TexTotal: %i MB Actors: %i
	0x004CE55A,	//	TEXTURES: Export \"%s\" again without textures embedded.\r\n
	0x004CF254,	//	CELLS: Trying to get exterior cell for invalid cell coordinate. Values must be between %i and %i.
	0x004D0740,	//	FORMS: TESDataHandler trying to create TESForm for unknown type '%s'.
	0x004D1184,	//	TEXTURES: Texture missing for %s '%s'.
	0x004D128B,	//	FORMS: Icon missing for %s '%s'.
	0x004D12F4,	//	MENUS: Icon '%s' missing for %s '%s'.
	0x004D13C1,	//	MODELS: Could not load 3D for model %s
	0x004D177E,	//	CELLS: %d duplicate references were removed.\r\n
	0x004D19FD,	//	CELLS: Region \"%s\" (%08X) could not be added to cell \"%s\" (%d, %d) (%08X) (checked out by \"%s\").
	0x004D4DF7,	//	MODELS: Object \"%s\" invalid type.
	0x004D4E92,	//	MODELS: Object \"%s\" (%s) not placed in the world.
	0x004D5139,	//	MODELS: %sModel \"%s\" not found for %s \"%s\".
	0x004D52DC,	//	Failed to check out %s form '%s' (%08X). Bound change will not be saved.
	0x004D53B1,	//	Warnings were encountered loading %smodel '%s' for %s %s.
//	0x004D53BB,	//	NULL
	0x004D55D3,	//	MODELS: No model selected for %smodel %s \"%s\".
	0x004D71C7,	//	FORMS: Form '%s' (%08X) of type %s in file '%s' was not freed.
	0x004D721B,	//	FORMS: Forms were leaked during ClearData. Check Warnings file for more info.
	0x004D723A,	//	Exception encountered clearing leaked forms.
	0x004D7F62,	//	Failed to release %s form '%s' (%08X).
	0x004D8A1E,	//	FORMS: Form (%08X) is checked out to %s and cannot be checked out. The file may still reference the deleted form and give errors on load.\n\nProblem Form: %s
	0x004D8B17,	//	FORMS: Addon Node %08X \"%s\" is trying to use index %i, but Addon Node %08X \"%s\" already uses that index.  Addon Node %08X \"%s\" will be remapped to a new index.
	0x004D9000,	//	Creating new user plugin '%s.
	0x004D935E,	//	MASTERFILE: Invalid forms were encountered on load and need to be removed from the master file. Contact a programmer immediately to fix any problems in the file.
	0x004D962B,	//	FORMS: Some of the bad forms encountered during load could not be removed from the file.
	0x004D9719,	//	SAVELOAD: Save restarted before previous Save was finished.
	0x004D9950,	//	SAVELOAD: Post processing failed on form '%s'. Save aborted.
	0x004D9A3F,	//	SAVELOAD: CreateTES failed for file '%s'. Save aborted.
	0x004DAE2B,	//	FORMS: Unknown form type '%s' encountered in AddFormToDataHandler.
	0x004DBB3A,	//	ESM %s contains existing form:\t%s\t'%s'\t(%08X)\t(%i, %i)
	0x004DBB69,	//	ESM %s contains existing form:\t%s\t'%s'\t(%08X)
	0x004DBB97,	//	!!!Same object in multiple DLC!!!
	0x004DBBEE,	//	File '%s' contains a form with a master file ID but no match in the master file: (%08X)
	0x004DBC1E,	//	FORMS: Duplicate form ID (%08X) encountered in file '%s'.
	0x004DBC42,	//	FORMS: Assigning duplicate ID form new ID (%08X).
	0x004DBCAB,	//	FORMS: Non-parent form '%s' (%08X) exists in both file '%s' and '%s'
	0x004DBD2A,	//	FORMS: Form (%08X) in file \"%s\" has wrong type.\n\nShould be:\t%s\nIs:\t\t%s
	0x004DBE40,	//	FORMS: Attempting to use existing form %s '%s' %08X as parent form for following child forms.
	0x004DBECC,	//	FORMS: Failed to find replacement form for parent form in file. Following child forms will not load correctly.
	0x004DBFAD,	//	FORMS: Partial form (%08X) found with no matching original data.
	0x004DBFD8,	//	FORMS: Partial form originated in current file. Clearing erroneous partial data flag.
	0x004DC0DB,	//	MASTERFILE: Master File '%s' contains an improperly positioned interior cell.\n\nCell: '%s' (%08X)\n\nChecking in a deleted WorldSpace may cause this cell to be destroyed. Merge the master file to fix this issue.
	0x004DC10A,	//	MASTERFILE: Cell: '%s' (%08X) is not flagged as interior but has no parent worldspace in file. Forcing to interior.
	0x004DC229,	//	CELLS: Error adding %scell (%08X) to world space (%08X). Cell will be destroyed.
	0x004DC467,	//	File '%s' is changing ref (%08X):\r\nfrom base '%s' (%08X) to base '%s' (%08X)\r\nfrom cell '%s' (%08X) to cell '%s' (%08X).
	0x004DC546,	//	CELLS: ref (%08X) to base object %s (%08X) in cell %s (%i, %i) (%08X) is persistent in the non-persistent file %s.
	0x004DCBA2,	//	FORMS: Unknown %s_ID in ConstructObject.
	0x004DCBB5,	//	FORMS: NO_FORM trying to load in ConstructObject.
	0x004DCDC9,	//	DataHandler: internal error
	0x004DCF14,	//	FORMS: Non-parent %s form '%s' (%08X) in file '%s' is not checked out to the current user.
	0x004DD017,	//	DataHandler: internal error
	0x004DD0D6,	//	===========================================================
	0x004DD564,	//	Unknown
	0x004DD620,	//	Too many selected files to compile!
	0x004DD6C4,	//	DataHandler: internal error
	0x004DD8BD,	//	DataHandler: unrecognized form\r\nLook in the %s file for more info.\r\n
	0x004DE9F5,	//	File %s is a higher version than this EXE can load.
	0x004DEE7C,	//	MASTERFILE: Missing Masterfile: %s
	0x004DF167,	//	MASTERFILE: Write Error in TES file
	0x004DF60A,	//	MASTERFILE: Could not create check-in backup file: \n\n'%s'
	0x004DF72C,	//	MASTERFILE: Could not create local check-in backup file: \n\n'%s'
	0x004DFAC8,	//	TESFile: Failed to read in buffer data for compressed form.
	0x004DFB4F,	//	TESFile: Error initializing ZLib inflate stream.
	0x004DFC29,	//	TESFile: Error inflating ZLib stream.
	0x004E0575,	//	MASTERFILE: First ReadFile() in GetChunkData failed with error:\n%s
	0x004E065A,	//	FORMS: Chunk size %d too big in chunk %s_ID in form %s_ID (%08X).\r\nMax size is %d, data truncated to \"%s\".\r\n
	0x004E06B9,	//	MASTERFILE: Second ReadFile() in GetChunkData failed with error:\n%s
	0x004E0A8A,	//	FORMS: Failed to CreateGroupData for %s form '%s' (%08X)
	0x004E0ECD,	//	MASTERFILE: Failed to create temp file.\r\nPlugin '%s' cannot be saved.
	0x004E0F01,	//	CurrentDir: '%s'\r\nEXEdir: '%s'
	0x004E0F4D,	//	MASTERFILE: Unable to complete operation due to failure removing previous file.\r\nPlugin '%s' may be locked or read only.\r\n\r\nTemp file '%s' contains saved data.
	0x004E0F81,	//	CurrentDir: '%s'\r\nEXEdir: '%s'
	0x004E0FCB,	//	MASTERFILE: Unable to complete operation due to failure renaming temp file.\r\nTemp file '%s' contains saved data.
	0x004E0FFF,	//	CurrentDir: '%s'\r\nEXEdir: '%s'
	0x004E1502,	//	FORMS: SetOffsetChunk failed for compressed form.
	0x004E15C0,	//	FORMS: SetFilePointer() in SetOffsetChunk failed with error:\n%s
	0x004E1983,	//	MASTERFILE: SetFilePointer() in SetOffset failed with error:\n%s
	0x004E1E73,	//	MASTERFILE: CreateFile('%s') in CreateTES failed with error:\n%s
	0x004E1EB2,	//	CurrentDir: '%s'\r\nEXEdir: '%s'
	0x004E21B0,	//	FORMS: SetFilePointer() in NextForm failed with error:\n%s
	0x004E2B70,	//	MASTERFILE: File '%s' is not a valid TES file.
	0x004E2B89,	//	MASTERFILE: Could not close file '%s'.
	0x004E31AE,	//	MASTERFILE: Unable to find ammo (%08X) on owner object \"%s\".
	0x004E31CA,	//	MASTERFILE: Unable to find ammo (%08X) on owner object (%08X).
	0x004E31E2,	//	MASTERFILE: Unable to find ammo (%08X) on unknown owner.
	0x004E367A,	//	MASTERFILE: Unable to find biped model list (%08X) for owner object \"%s\".
	0x004E3698,	//	MASTERFILE: Unable to find biped model list (%08X) for owner object (%08X).
	0x004E36B2,	//	MASTERFILE: Unable to find biped model list (%08X) for unknown owner.
	0x004E3743,	//	MASTERFILE: Form '%s' (%08X) in form list '%s' (%08X) is not a biped model. BipedModelList on '%s' (%08X) will be cleared.
	0x004E4C6E,	//	DIALOGUE: Could not select explosion \"%s\" (%08X) in InitDialog for BGSDestructibleObjectForm form component.
	0x004E4CC7,	//	DIALOGUE: Could not select debris \"%s\" (%08X) in InitDialog for BGSDestructibleObjectForm form component.
	0x004E4FFA,	//	MASTERFILE: Unable to find stage %i explosion (%08X) on owner object \"%s\".
	0x004E5013,	//	MASTERFILE: Unable to find stage %i explosion (%08X) on owner object (%08X).
	0x004E5028,	//	MASTERFILE: Unable to find stage %i explosion (%08X) on unknown owner.
	0x004E50DF,	//	MASTERFILE: Unable to find stage %i debris (%08X) on owner object \"%s\".
	0x004E50F8,	//	MASTERFILE: Unable to find stage %i debris (%08X) on owner object (%08X).
	0x004E510D,	//	MASTERFILE: Unable to find stage %i debris (%08X) on unknown owner.
	0x004E5BFB,	//	MASTERFILE: Old Destruction data found on form %08X %s.  It needs to be resaved.
	0x004E759C,	//	MASTERFILE: Unable to find Idle (%08X) on form '%s' (%08X).
	0x004E86A9,	//	MASTERFILE: Unable to find pickup sound (%08X) for owner object \"%s\".
	0x004E86C1,	//	MASTERFILE: Unable to find pickup sound  (%08X) for owner object (%08X).
	0x004E86D5,	//	MASTERFILE: Unable to find pickup sound  (%08X) for unknown owner.
	0x004E8779,	//	MASTERFILE: Unable to find putdown sound (%08X) for owner object \"%s\".
	0x004E8796,	//	MASTERFILE: Unable to find putdown sound  (%08X) for owner object (%08X).
	0x004E87AF,	//	MASTERFILE: Unable to find putdown sound  (%08X) for unknown owner.
	0x004E8B59,	//	MASTERFILE: Unable to find repair item list (%08X) for owner object \"%s\".
	0x004E8B75,	//	MASTERFILE: Unable to find repair item list (%08X) for owner object (%08X).
	0x004E8B8D,	//	MASTERFILE: Unable to find repair item list (%08X) for unknown owner.
	0x004E90CE,	//	DIALOGUE: Could not select touch spell \"%s\" (%08X) in LoadDialog for BGSTouchSpellForm form component.
	0x004E9196,	//	DIALOGUE: Could not select touch spell anim\"%s\" in LoadDialog for BGSTouchSpellForm form component.
	0x004E932F,	//	MASTERFILE: Unable to find Actor Effect (%08X) on owner object \"%s\".
	0x004E9347,	//	MASTERFILE: Unable to find Actor Effect (%08X) on owner object (%08X).
	0x004E935B,	//	MASTERFILE: Unable to find Actor Effect (%08X) on unknown owner.
	0x004E93C5,	//	MASTERFILE: Enchanting (%08X) '%s' has invalid effect trigger type or effect for object (%08X) '%s'.
	0x004EC83C,	//	MASTERFILE: Could not find faction (%08X) on ActorBase (%08X) '%s'.
	0x004EC8FA,	//	MASTERFILE: Could not find voice type (%08X) on NPC (%08X) '%s'.
	0x004EC970,	//	MASTERFILE: Unable to find death item (%08X) on ActorBase (%08X) '%s'.
	0x004EC9CC,	//	MASTERFILE: Unable to find template form (%08X) on ActorBase (%08X) '%s'.
	0x004ECC3C,	//	Invalid karma value found. Unable to fix.
	0x004F43E9,	//	MASTERFILE: Unable to find container object (%08X) on owner object \"%s\".
	0x004F4401,	//	MASTERFILE: Unable to find container object (%08X) on owner object (%08X).
	0x004F4415,	//	MASTERFILE: Unable to find container object (%08X) on unknown owner.
	0x004F71E3,	//	DIALOGUE: Could not select enchanting \"%s\" (%08X) in LoadDialog for TESEnchantableForm form component.
	0x004F733F,	//	MASTERFILE: Unable to find enchanting (%08X) on owner object \"%s\".
	0x004F7357,	//	MASTERFILE: Unable to find enchanting (%08X) on owner object (%08X).
	0x004F736B,	//	MASTERFILE: Unable to find enchanting (%08X) on unknown owner.
	0x004F73BE,	//	MASTERFILE: Enchanting (%08X) '%s' has invalid effect trigger type or effect for object (%08X) '%s'.
	0x004F7FB3,	//	FORMS: Trying to copy form \"%s\".\r\nNo copy function for %s form types.\r\n
	0x004F7FE3,	//	FORMS: Trying to compare form \"%s\".\r\nNo compare function for %s form types.\r\n
	0x004F8836,	//	MASTERFILE: Trying to SaveData that's too large.
	0x004F8F79,	//	DIALOGUE: Dialog box missing IDC_ID control for TESForm form component.
	0x004FA3F9,	//	FORMS: Failed to revert %s form '%s' (%08X). Reload to remove current changes.
	0x004FA510,	//	FORMS: Warnings were encountered during Revert InitItem for form '%s' (%08X).
	0x004FA552,	//	FORMS: Failed to reload form '%s' (%08X) from file %s.
	0x004FA592,	//	FORMS: Could not find form '%s' (%08X) in file %s.
	0x004FA5B1,	//	MASTERFILE: Failed to open file '%s'.
	0x004FB5C6,	//	EDITOR: Editor ID '%s' (%08X) is not unique, previous object (%08X) is type %s. Editor ID will be set to '%s'.
	0x004FC904,	//	FORMS: SetFormID bashing entry in form ID map at %08X.\n\nForm basher: type %s_ID\n\nBashed form: %s
	0x004FD425,	//	FORMS: formEnumString[ %d ].cFormID in TESForm.cpp is out of order.
	0x004FD44A,	//	FORMS: formEnumString[ %d ] and formEnumString[ %d ] have the same iFormString %s in TESForm.cpp.
	0x004FD716,	//	Deleting used form: %s
	0x004FD748,	//	---used by: %s
	0x004FEB52,	//	DIALOGUE: Could not select imagespace modifier \"%s\" (%08X) in LoadDialog for TESImageSpaceModifiableForm form component.
	0x004FEC4F,	//	MASTERFILE: Unable to find imagespacemodifier (%08X) on owner object \"%s\".
	0x004FEC67,	//	MASTERFILE: Unable to find imagespacemodifier (%08X) on owner object (%08X).
	0x004FEC7B,	//	MASTERFILE: Unable to find imagespacemodifier (%08X) on unknown owner.
	0x005004BB,	//	MASTERFILE: Unable to find Leveled Object Form (%08X) for owner object \"%s\".
	0x005004D3,	//	MASTERFILE: Unable to find Leveled Object Form (%08X) for owner object (%08X).
	0x005004E7,	//	MASTERFILE: Unable to find Leveled Object Form (%08X) for unknown owner.
	0x005005A9,	//	MASTERFILE: Unable to find Leveled Chance Global (%08X) for owner object \"%s\".
	0x005005C8,	//	MASTERFILE: Unable to find Leveled Chance Global (%08X) for owner object (%08X).
	0x005005E3,	//	MASTERFILE: Unable to find Leveled Chance Global (%08X) for unknown owner.
	0x0050231C,	//	MASTERFILE: Model '%s' uses too many textures. (%i / %i max)
	0x00502724,	//	TEXTURES: Failed to find archive file entry for texture '%s' for model '%s'.
	0x005034AA,	//	MASTERFILE: Could not checkout form '%s' (%08X) with type %s to update the texture list.  Currently checked out to %s
	0x005036ED,	//	MASTERFILE: Could not checkout form '%s' (%08X) with type %s to update the texture list.  Currently checked out to %s
	0x00504AAA,	//	MASTERFILE: Could not load 3D for model %s
	0x00504B55,	//	MASTERFILE: Could not checkout form '%s' (%08X) with type %s to update the texture list.  Currently checked out to %s
	0x00505A97,	//	Removing invalid texture swap for object '%s'
	0x00505C40,	//	Found invalid swap index.
	0x00505C78,	//	Not all texture swaps were performed.
	0x00505CB5,	//	Warnings were encountered during texture swap for model '%s'. Check warnings file for more details.
	0x005065B3,	//	MASTERFILE: Failed to find texture set (%08X) in model texture swap init for form '%s' (%08X).
	0x005066BA,	//	Warnings were encountered updating texture swap indexes for form '%s' (%08X). Check warnings file for more details.
	0x005066E8,	//	Unable to fix texture swap indexes for form '%s' (%08X).
	0x0050776D,	//	DIALOGUE: Dialog box missing IDC_INGREDIENT control for TESProduceForm form component.
	0x005077B2,	//	DIALOGUE: Dialog box missing IDC_PRODUCTION_CHANCE %s control for TESProduceForm form component.
	0x005079DF,	//	MASTERFILE: Unable to find ingredient (%08X) on owner object \"%s\".
	0x005079FB,	//	MASTERFILE: Unable to find ingredient (%08X) on owner object (%08X).
	0x00507A13,	//	MASTERFILE: Unable to find ingredient (%08X) on unknown owner.
	0x00508474,	//	DIALOGUE: Could not select race \"%s\" (%08X) in LoadDialog for TESRaceForm form component.
	0x0050856F,	//	MASTERFILE: Unable to find race (%08X) on owner object \"%s\".
	0x0050858B,	//	MASTERFILE: Unable to find race (%08X) on owner object (%08X).
	0x005085A3,	//	MASTERFILE: Unable to find race (%08X) on unknown owner.
	0x0050931B,	//	MASTERFILE: Form (%08X) not found for reaction of pForm (%08X) '%s'.\r\n
	0x0050A0A2,	//	DIALOGUE: Could not select script \"%s\" (%08X) in LoadDialog for TESScriptableForm form component.
	0x0050A1A0,	//	MASTERFILE: Unable to find script (%08X) on owner object \"%s\".
	0x0050A1C2,	//	MASTERFILE: Unable to find script (%08X) on owner object (%08X).
	0x0050A1E0,	//	MASTERFILE: Unable to find script (%08X) on unknown owner.
	0x0050A28C,	//	MASTERFILE: Script '%s' on carryable form '%s' (%08X) contains GetSelf function.
	0x0050B432,	//	MASTERFILE: Unable to find Actor Effect (%08X) for owner object \"%s\".
	0x0050B44D,	//	MASTERFILE: Unable to find Actor Effect (%08X) for owner object (%08X).
	0x0050B464,	//	MASTERFILE: Unable to find Actor Effect (%08X) for unknown owner.
	0x0050E062,	//	ANIMATION: Could not find AccumRoot in animation '%s'.
	0x0050E286,	//	ANIMATION: AnimGroup '%s%s%s' for '%s' was exported with 'Animate in Place' from MAX.\r\n
	0x0050F176,	//	ANIMATION: Morph Error - Trying to morph from sequence to itself.\r\n'%s' on '%s'.
	0x0050F294,	//	ANIMATION: Morph Error - Morph tags are different in '%s' and '%s' for '%s'.
	0x0050F2BA,	//	ANIMATION: Morph Error - Controller count not the same.\r\n'%s' has %d controllers and\r\n'%s' has %d on '%s'.
	0x0050F6A2,	//	ANIMATION: Trying to SpecialIdleFree an animation '%s' that just started.
	0x0050F7BE,	//	ANIMATION: Trying to SpecialIdleFree an animation '%s' that just started.
	0x00510861,	//	ANIMATION: Animation sequence '%s' not found in TESAnimGroup::GetSequenceType for file '%s'.
	0x00510BA0,	//	ANIMATION: Unable to add '%s' to keyframe manager on '%s'.\r\nMake sure the animation is not skinned to bones that don't exist in our skeleton.
	0x00510C22,	//	ANIMATION: Object '%s' in sequence but not skeleton.
	0x00510E1F,	//	'%s' is trying to free a special idle '%s' sequence '%s' that is animating.
	0x00516515,	//	DECAL: Issuing Occlusion Query: %s
	0x0051653E,	//	DECAL: Origin: (%.2f, %.2f, %.2f)
	0x00516562,	//	DECAL: Direction: (%.2f, %.2f, %.2f)
	0x0051659C,	//	DECAL: Frustum: Near=0.1f, Far=%.2f, Left=%.2f, Right=%.2f, Top=%.2f, Bottom=%.2f )
	0x005169A8,	//	DECAL: Reached max non-skinned decals per frame. Ignoring decal.
	0x00516BD7,	//	DECAL: Non-Skinned decal count this frame: #%d
	0x00516D53,	//	DECAL: Reached max skinned decals per frame. Ignoring decal.
	0x00516F5D,	//	DECAL: Skinned decal count this frame: #%d
	0x00517089,	//	DECAL: Placing Skin Decal #%d
	0x0051762A,	//	DECAL: Decal Succeeded Occlusion Query : %s
	0x00517658,	//	DECAL: Decal Failed Occlusion Query : %s
	0x005176BB,	//	DECAL: Placing Simple Decal #%d : %s
	0x005179B2,	//	DECAL: Reached Max Skin Decal Limit! : %d
	0x00517AF6,	//	DECAL: Reached Max Skin Decal Limit! : %d
	0x00517F09,	//	DECAL: Simple Decal Instantiated
	0x0051F45E,	//	TEXTURES: Failed to translate texture size on line %i
	0x0051F51A,	//	TEXTURES: Failed to find texture user '%s' (%08X). ESM may not match map file. 
	0x0051F569,	//	MASTERFILE: Failed to find of line %i
	0x0051F5B5,	//	TEXTURES: Failed to texture on line %i
	0x0051F5D6,	//	TEXTURES: Failed to create texture '%s' on line %i
	0x0051F5EF,	//	TEXTURES: Failed to find texture size on line %i
	0x0051F61A,	//	FORMS: Failed to find form ID on line %i
	0x0051F646,	//	FORMS: Invalid form ID (%s) on line %i
	0x0051FB12,	//	MODELS: Bad skin name '%s' on '%s'.
	0x0051FC83,	//	MODELS: Missing bone '%s' for '%s'
	0x00520DD7,	//	MODELS: Could not find parent node '%s' for object '%s'.
	0x00520DF5,	//	MODELS: Could not find parent node extra data for '%s'.
	0x00520F95,	//	HAVOK: Weapon '%s' doesn't have a good Havok shape on it.
	0x00520FD1,	//	HAVOK: Weapon '%s' doesn't have any Havok data on it.
	0x005214E0,	//	MODELS: Tyring to add skinned object '%s' as an add on to skeleton.
	0x0052174C,	//	MODELS: Bone '%s' not found for part '%s->%s'.\r\nMake sure all the verticies are skinned to a bone in Max.
	0x00521772,	//	MODELS: Bone '%s' not found for part '%s'.\r\nRequested by model '%s'.
	0x00521788,	//	MODELS: Bone '%s' not found for part 'UNKNOWN'.\r\nRequested by model 'UNKNOWN'.
	0x005218A0,	//	Body part '%s'->'%s' for skeleton '%s' was exported incorectly.\r\nHide the skeleton before you export body parts.
	0x00521B0E,	//	TEXTURES: Missing '%s' race texture for '%s' in race '%s'.
	0x0052239F,	//	MODELS: Bone %i for part '%s->%s' is unnamed.\r\nMake sure all the vertices are skinned to a named bone in Max.
	0x00522C89,	//	FaceGen NIF contains non-geometry at root: '%s'
	0x00522F13,	//	MODELS: The biped part '%s' should be skinned for '%s'.
	0x0052399F,	//	FORMS: Invalid FormID sent to FormIDToString().\r\n
	0x0052E7C5,	//	MENUS: Miscellaneous stat %s could not be found.  Returning MS_COUNT.
	0x005367B2,	//	WATER: Adjusting water object ('%s') height from %f to closeby water group at height : %f
	0x00538F86,	//	NULL object name found in tag array for sequence '%s'.
	0x0053A74A,	//	MODELS: Invalid Region Grass Object Data in file \"%s\".
	0x0053AD79,	//	SetImposterVisible: ObjectREFR is not flagged as an imposter (%s:%X)
	0x0053C836,	//	MASTERFILE: Invalid Region Object Data in file \"%s\" for Region \"%s\".
	0x0053C85F,	//	MASTERFILE: Invalid Region Object Data in file \"%s\".
	0x0053C914,	//	MASTERFILE: Could not locate parent Region Object for form (%08X) in Region Object Data Load for Region \"%s\".
	0x0053C929,	//	MASTERFILE: Could not locate parent Region Object for form (%08X) in Region Object Data Load.
	0x0053C9D4,	//	MASTERFILE: Could not locate form (%08X) in Region Object Data Load for Region \"%s\".
	0x0053C9E6,	//	MASTERFILE: Could not locate form (%08X) in Region Object Data Load.
	0x0053CB26,	//	MASTERFILE: Invalid Region Object Data in file \"%s\" for Region \"%s\".
	0x0053CB4F,	//	MASTERFILE: Invalid Region Object Data in file \"%s\".
	0x0053CC18,	//	MASTERFILE: Could not locate parent Region Object for form (%08X) in Region Object Data Load for Region \"%s\".
	0x0053CC2D,	//	MASTERFILE: Could not locate parent Region Object for form (%08X) in Region Object Data Load.
	0x0053CCD8,	//	MASTERFILE: Could not locate form (%08X) in Region Object Data Load for Region \"%s\".
	0x0053CCEA,	//	MASTERFILE: Could not locate form (%08X) in Region Object Data Load.
	0x0053DF5B,	//	Warnings were encountered while loading weather list chunk from region %s
	0x0053E667,	//	Instantiating a Grass region object for region %s (%d) with a non-Grass object.
	0x0053E714,	//	Instantiating a Grass region object for region %s (%d) without an object.
	0x0053E723,	//	Instantiating a Grass region object without an object.
	0x0053E754,	//	Instantiating a Grass region object with a parent that isn't a land texture.
	0x00542E26,	//	Invalid Region Point List data in file \"%s\".
	0x00546493,	//	MASTERFILE: Could not find ragdoll (%08X) for bodypart data '%s' (%08X).
	0x00546E5D,	//	MASTERFILE: Body part data for '%s' (%08X) advanced file too far. Attempting recovery.
	0x00546E8E,	//	MASTERFILE: Warnings encountered loading part '%s' for body part data '%s' (%08X).
	0x00546ECB,	//	MODELS: Invalid body part index %i in body part data %s (%08X).
	0x00546F31,	//	MASTERFILE: Load failed for body part data '%s' (%08X).
	0x005470FC,	//	MODELS: No model to check against for body part data '%s' (%08X).
	0x00547288,	//	MODELS: BodyPart with no node name in body part data '%s' (%08X).
	0x005472D4,	//	MODELS: BodyPart with invalid node name '%s' in body part data '%s' (%08X).
	0x005472FF,	//	MODELS: Duplicate node name '%s' in body part data '%s' (%08X).
	0x00547332,	//	MODELS: Duplicate part type '%s' in body part data '%s' (%08X).
	0x00547881,	//	%s body part %s does not have a proper part name!
	0x00547D4C,	//	FORMS: Could not checkout form '08X' during import
	0x00547D68,	//	MASTERFILE: Could not find body part data '08X' during import.
	0x00547E88,	//	MODELS: No model selected for body part data '%s' (%08X).
	0x00547F26,	//	MODELS: Invalid model '%s' selected for body part data '%s' (%08X).
	0x00548D79,	//	DIALOGUE: Could not select actor value \"%s\" (%i) in LoadDialog for BGSEntryPointFunctionDataTwoValue.
	0x0054906F,	//	DIALOGUE: Could not select leveled item \"%s\" (%08X) in LoadDialog for BGSEntryPointFunctionDataLeveledList.
	0x00549385,	//	MASTERFILE: Errors were encountered during InitItem for result script for perk '%s' (%08X)\n\nSee Warnings file for more information.
	0x005498E9,	//	MASTERFILE: Unable to find leveled list (%08X) on owner object \"%s\".
	0x00549905,	//	MASTERFILE: Unable to find leveled list (%08X) on owner object (%08X).
	0x0054A4EF,	//	Unable to find extra part (%08X) for head part '%s' (%08X).
	0x0054BCFD,	//	MASTERFILE: Unable to find owner quest (%08X) for Note '%s' (%08X).
	0x0054BDE4,	//	MASTERFILE: Unable to find sound (%08X) for Note '%s' (%08X).
	0x0054BE7E,	//	MASTERFILE: Unable to find topic (%08X) for Note '%s' (%08X).
	0x0054BF16,	//	MASTERFILE: Unable to find actor base (%08X) for Note '%s' (%08X).
	0x0054D0F1,	//	DEFAULT: Could not checkout form %08X during import
	0x0054D126,	//	EDITOR: Could not find note with editor ID %s during import
	0x0054E060,	//	FORMS: BGSEntryPointPerkEntry::CheckConditionFilters() was called with %i filter forms, but there are %i conditions
	0x0054E11A,	//	DEFAULT: Bad Filter form %08X %s sent to BGSEntryPointPerkEntry::CheckConditionFilters.
	0x0054E672,	//	MASTERFILE: Function data chunk is loading without function data existing on perk entry
	0x0054EFC6,	//	MENUS: Failed to create Conditions window.
	0x0054F0F9,	//	MASTERFILE: Unable to find quest (%08X) on owner object \"%s\".
	0x0054F115,	//	MASTERFILE: Unable to find quest (%08X) on owner object (%08X).
	0x0054F202,	//	QUESTS: Could not select quest \"%s\" (%08X) in LoadDialog for BGSQuestPerkEntry.
	0x0054F2C9,	//	MASTERFILE: Unable to find ability (%08X) on owner object \"%s\".
	0x0054F2E5,	//	MASTERFILE: Unable to find ability (%08X) on owner object (%08X).
	0x0054F396,	//	DIALOGUE: Could not select ability \"%s\" (%08X) in LoadDialog for BGSAbilityPerkEntry.
	0x005503AD,	//	FORMS: Could not checkout form '08X' during import
	0x005503C3,	//	MASTERFILE: Could not find body part data '08X' during import.
	0x00551558,	//	DEFAULT: Failed to create Conditions window.
	0x00552C9E,	//	MASTERFILE: Could not find body part data (%08X) for ragdoll '%s' (%08X).
	0x00552D1E,	//	MASTERFILE: Could not find preview actor (%08X) for ragdoll '%s' (%08X).
	0x0055976D,	//	FORMS: Failed to create tab window.
	0x00559AD1,	//	MASTERFILE: Skeleton for ragdoll '%s' (%08X) does not have powered constraints.  Please update the max file & re-export.
	0x0055D480,	//	AUDIO: Unable to find CreatureSound ID (%08X) for creature '%s' (%08X).
	0x0055E7D7,	//	ANIMATION: Invalid anim group action (action %d too small) in %s (%04X).
	0x0055E809,	//	ANIMATION: Invalid anim group action (action %d too big %d max) in %s (%04x).
	0x0055E83F,	//	ANIMATION: Time %d in group %04X is QNAN
	0x0055ECF0,	//	No object name at index %i for sequence '%s'.
	0x0055EDDE,	//	ANIMATION: AnimGroup empty note key at time %.2f in sequence '%s' in model '%s'.
	0x0055F25F,	//	ANIMATION: '%s' should be a looping animation.
	0x0055F28D,	//	ANIMATION: '%s' should NOT be a looping animation.
	0x0055F2F2,	//	ANIMATION: Bad note string \"%s\" frame %d in \"%s\".
	0x0055F324,	//	ANIMATION: AnimGroup unable to find sequence '%s' in model '%s'.
	0x0055F4F0,	//	ANIMATION: Bad note string \"%s\" frame %d in \"%s\".
	0x0055F790,	//	ANIMATION: (%s) End frame is less than or equal to Start frame in \"%s\".
	0x0055F7DF,	//	ANIMATION: (%s) too many blend values in \"%s\". Blend %d, BlendIn %d, BlendOut %d
	0x0055F81C,	//	ANIMATION: Animation group note problem. See %s file.
	0x0055FA4F,	//	MASTERFILE: Could not find Form (%08X) on challenge '%s' (%08X).
	0x0055FAB5,	//	MASTERFILE: Could not find Form (%08X) on challenge '%s' (%08X).
	0x005614B3,	//	MASTERFILE: Class %s contains invalid data. Make sure all attributes and skills are unique.
	0x00561CF6,	//	MASTERFILE: Sound Creature (%08X) not found in InitItem for Creature (%08X) '%s'.
	0x00561D40,	//	MASTERFILE: Invalid sound creature '%s' (%08X) on creature '%s' (%08X).
	0x00561DE4,	//	MASTERFILE: Combat Style (%08X) not found in InitItem for Creature (%08X) '%s'.
	0x00561E64,	//	MASTERFILE: BodyPartData (%08X) not found in InitItem for Creature (%08X) '%s'.
	0x00561EE5,	//	MASTERFILE: Could not find impact data set (%08X) for weapon '%s' (%08X).
	0x00561F65,	//	Could not find list form (%08X) for weapon '%s' (%08X).
	0x005630E3,	//	MASTERFILE: Could not checkout form '%s' (%08X) with type %s to update the texture list.  Currently checked out to %s
	0x00563B22,	//	MASTERFILE: Speed multiplier is zero on creature '%s' (%08X).
	0x00565E77,	//	DEFAULT: Failed to create tab window.
	0x00568979,	//	DEFAULT: (Faction Reaction Error) Faction '%s' (%08X) is a Friend or Ally of Faction '%s' (%08X), but Faction '%s' (%08X) is not a Friend or Ally of Faction '%s' (%08X). 
	0x00568CAE,	//	DEFAULT: Could not find faction rank #%i during import of faction %08X.
	0x00568D17,	//	DEFAULT: Could not find faction %08X during import.
	0x0056B79C,	//	MASTERFILE: Invalid anim group section encountered on idle '%s' (%08X). Resetting to Special Idle.
	0x0056B833,	//	MASTERFILE: Could not find parent idle (%08X) for %s.
	0x0056B8DE,	//	MASTERFILE: Could not find previous idle (%08X) for %s.
	0x0056BA0E,	//	MASTERFILE: Multiple next idles for %s.
	0x0056C1D4,	//	Failed to find loose idle array for '%s' (%08X)
	0x0056C280,	//	MASTERFILE: Could not find parent array for %s.
	0x005705BF,	//	MASTERFILE: Could not find class (%08X) on NPC (%08X) '%s'.
	0x00570602,	//	MASTERFILE: NPC '%s' (%08X) matches PC level but is not autocalc.
	0x005706A4,	//	MASTERFILE: Combat Style (%08X) not found in InitItem for NPC (%08X) '%s'.
	0x00570700,	//	MASTERFILE: Hair '%s' (%08X) is invalid for NPC '%s' (%08X).
	0x00570752,	//	MASTERFILE: Eye color '%s' (%08X) is invalid for NPC '%s' (%08X).
	0x0057162A,	//	ANIMATION: This npc \"%s\" has already been used.\r\nOnly the first reference will be used.\r\n
	0x0057170C,	//	MODELS: Bad part '%s' in TESNPC::InitWornObject.
	0x00571AAC,	//	MODELS: Cannot create a head for an NPC (%d) that does not have a biped-head node.
	0x00572027,	//	MODELS: Need to add support for BipedObject '%s' object name '%s' in TESNPC::InitWornObject.
	0x00573004,	//	MODELS: Cannot create a head for an NPC (%d) (no race or bad race data).
	0x00573239,	//	MODELS: Could not correctly handle skinning for duplicate NPC \"%s\" (%08X).
	0x0057324F,	//	MODELS: Cannot create a head for an NPC (%d) that does not have a biped-head node.
	0x00575E4F,	//	MASTERFILE: Found face texture for NPC '%s' (%08X) in file '%s'
	0x00575F07,	//	NPC '%s' (%08X) has invalid data format.
	0x005761B3,	//	MASTERFILE: Could not find headpart (%08X) on NPC '%s' (%08X).
	0x0057622E,	//	MASTERFILE: Could not find hair (%08X) on NPC '%s' (%08X).
	0x00576305,	//	MASTERFILE: Could not find eyes (%08X) on NPC '%s' (%08X).
	0x0057677E,	//	MASTERFILE: Speed multiplier is zero on NPC '%s' (%08X).
	0x00577681,	//	DIALOGUE: Failed to create valid color control in NPC face dialog tab.
	0x00577F7D,	//	DIALOGUE: Failed to create tab window.
	0x0057BDE2,	//	QUESTS: Warnings encountered recompiling Quest %s, stage %i.
	0x0057BF0A,	//	QUESTS: Failed to remove quest from Topic '%s' (%08X).
	0x0057DB9F,	//	QUESTS: Could not find stage index %i in quest %08X during import.
	0x0057DBC9,	//	QUESTS: Could not find stage item #%i during import of stage %i in quest %08X.
	0x0057DC60,	//	QUESTS: Could not find quest %08X during import.
	0x0057DF78,	//	FORMS: Could not check out form %08X during import
	0x0057DFA4,	//	QUESTS: Could not find objective index %i in quest %08X during import.
	0x0057DFB4,	//	QUESTS: Could not find quest %08X during import.
	0x0057E52E,	//	QUESTS: Failed to create Conditions window.
	0x0057EE9B,	//	QUESTS: Unable to complete operation due to failure removing previous file.
	0x0057F27A,	//	DIALOGUE: Dialog box instantiated incorrectly.
	0x0057FD70,	//	QUESTS: Failed to create tab window.
	0x0057FE10,	//	DIALOGUE: Dialog box instantiated incorrectly.
	0x00580915,	//	QUESTS: Failed to create tab window.
	0x00581907,	//	MASTERFILE: Errors were encountered during InitItem for result script on quest stage %d, quest '%s' (%08X)\n\nSee Warnings file for more information.
	0x00581965,	//	MASTERFILE: Unable to find next quest (%08X) on quest stage.
	0x005819C7,	//	Result script for quest stage item %d from quest %s (%08X) has text but has not been compiled.
	0x005830BD,	//	DIALOGUE: Failed to create Conditions window.
	0x00583192,	//	MASTERFILE: Could not find target reference (%08X) on quest (%08X) '%s'.
	0x005831BA,	//	MASTERFILE: No reference on target for quest (%08X) '%s'.
	0x005834F0,	//	DIALOGUE: Failed to create valid reference select control for quest target tab in quest dialog.
	0x0058388D,	//	DIALOGUE: Failed to create Conditions window.
	0x005841FE,	//	MASTERFILE: Could not find male voice type (%08X) for race '%s' (%08X).
	0x0058427E,	//	MASTERFILE: Could not find female voice type (%08X) for race '%s' (%08X).
	0x005842FE,	//	MASTERFILE: Could not find male default hair (%08X) for race '%s' (%08X).
	0x0058437E,	//	MASTERFILE: Could not find female default hair (%08X) for race '%s' (%08X).
	0x005843FE,	//	MASTERFILE: Could not find older race (%08X) for race '%s' (%08X).
	0x00584484,	//	MASTERFILE: Could not find younger race (%08X) for race '%s' (%08X).
	0x00587DBE,	//	Failed to find body mod texture '%s' for '%s' (%08X). Creating from scratch.
	0x00588A52,	//	MASTERFILE: Could not find hair (%08X) for race '%s' (%08X).
	0x00588B46,	//	MASTERFILE: Could not find eyes (%08X) for race '%s' (%08X).
	0x0058A307,	//	DIALOGUE: Failed to create tab window. <- bug, should be DIALOG
	0x0058D1AC,	//	MASTERFILE: Unable to find TESTopic (%08X) in TESResponse init for %s.
	0x0058D2ED,	//	MASTERFILE: Unable to find TESSound (%08X) in TESResponse init for %s.
	0x0058D434,	//	MASTERFILE: Could not find speaker idle (%08X) in TESResponse init for %s.
	0x0058D51F,	//	MASTERFILE: Could not find listener idle (%08X) in TESResponse init for %s.
	0x00591053,	//	SCRIPTS: Warnings encountered recompiling Topic %s, Quest %s, Info (%08X).
	0x00593424,	//	TESTopic::InitItem: Topic contained a link to a deleted info node. Checked out and removed. \n Topic: (%08X)%s wanted to link to info: (%08X) for quest (%08X)%s.\n(Topic checked out and reference removed)
	0x00593551,	//	MASTERFILE: No Quest Reference on Topic \"%s\" (%d non-deleted infos attached to this quest).
	0x00594282,	//	DIALOGUE: Could not find response #%i during import of topic %08X.
	0x00594292,	//	DIALOGUE: Could not find topic %08X during import.
	0x005944DF,	//	QUESTS: Failed to remove quest from Topic '%s' (%08X).
	0x005949AA,	//	FORMS: Info %08X under topic %08X no longer exists
	0x00594A08,	//	MASTERFILE: Could not find named topic %08X during import.
	0x00595659,	//	DEFAULT: Failed to create Conditions window.
	0x005962A7,	//	DIALOGUE: Unable to complete operation due to failure removing previous file.
	0x0059688F,	//	QUESTS: Could not find previous info (%08X) for TopicInfo (%08X) in Topic \"%s\" (%08X).
	0x0059E76E,	//	MASTERFILE: Unable to find topic (%08X) for conversation data 'Link To' on %s
	0x0059E942,	//	MASTERFILE: Unable to find topic (%08X) for conversation data 'Link From' on %s
	0x0059EB2B,	//	MASTERFILE: Unable to find topic (%08X) for conversation data 'Follow Up' on %s
	0x0059F085,	//	MASTERFILE: Unable to find topic (%08X) for %s
	0x0059F296,	//	MASTERFILE: Errors were encountered during InitItem for result script on %s\n\nSee Warnings file for more information.
	0x0059F366,	//	MASTERFILE: Unable to find Perk/Skill Reference (%08X) on %s.
	0x0059F44C,	//	On Begin script for topic info \"%s\" (%08X) has text but has not been compiled. Quest: %s, topic: %s.
	0x0059F4DC,	//	On End script for topic info \"%s\" (%08X) has text but has not been compiled. Quest: %s, topic: %s.
	0x0059F97B,	//	MASTERFILE: File '%s' contains old info data for form (%08X).
	0x0059FE35,	//	MASTERFILE: Unable to insert topic info (%08X) into topic '%s' (%08X), quest (%08X).
	0x005A42B8,	//	MASTERFILE: Could not find pDawnSound (%08X) for activator '%s' (%08X).
	0x005A4332,	//	MASTERFILE: Could not find pNoonSound (%08X) for activator '%s' (%08X).
	0x005A43AC,	//	MASTERFILE: Could not find pDuskSound (%08X) for activator '%s' (%08X).
	0x005A4426,	//	MASTERFILE: Could not find pNightSound (%08X) for activator '%s' (%08X).
	0x005A44A0,	//	MASTERFILE: Could not find sound region (%08X) for activator '%s' (%08X).
	0x005A451A,	//	MASTERFILE: Could not find pWallaSound (%08X) for activator '%s' (%08X).
	0x005A68AD,	//	MASTERFILE: Unable to find Camera Shot Form (%08X) for Camera Path \"%s\".
	0x005A68C5,	//	MASTERFILE: Unable to find Camera Shot Form (%08X) for Camera Path (%08X).
	0x005A69CE,	//	DEFAULT: Multiple next paths for %s.
	0x005A7431,	//	DEFAULT: Could not find parent path (%08X) for %s.
	0x005A74E2,	//	DEFAULT: Could not find previous path (%08X) for %s.
	0x005A75AD,	//	DEFAULT: Could not find parent array for %s.
	0x005A9568,	//	Bad VATS CameraShot Data: '%s' has a Global Time Multiplier that is too small.
	0x005A9C32,	//	Unable to find valid '%s' default object for ID (%08X).
	0x005AB072,	//	MASTERFILE: Unable to find texture set (%08X) on impact data %s (%08X).
	0x005AB0EC,	//	MASTERFILE: Unable to find sound (%08X) on impact data %s (%08X).
	0x005AB166,	//	MASTERFILE: Unable to find sound (%08X) on impact data %s (%08X).
	0x005AB5C0,	//	MASTERFILE: Unable to find impact data (%08X) on impact data set %s (%08X).
	0x005AC18F,	//	Could not find old ImpactData (%08X) for ImpactSwap on '%s' (%08X).
	0x005AC1FE,	//	Could not find new ImpactData (%08X) for ImpactSwap on '%s' (%08X).
	0x005ADD2A,	//	MASTERFILE: Unable to find form (%08X) on list form %s (%08X).
	0x005B46EB,	//	MASTERFILE: Could not find MediaSet (%08X) for %s.
	0x005B483C,	//	MASTERFILE: Could not find reputation (%08X) for MediaLocationController
	0x005B4A3E,	//	MASTERFILE: Could not find open sound (%08X) for MediaSet '%s' (%08X).
	0x005B4ABE,	//	MASTERFILE: Could not find open sound (%08X) for MediaSet '%s' (%08X).
	0x005B7376,	//	DEFAULT: Failed to create window.
	0x005BBD61,	//	MASTERFILE: Loading GameSetting:\t%s\t%.04f
	0x005BBD94,	//	MASTERFILE: Loading GameSetting:\t%s\t%i
	0x005BBEBE,	//	FORMS: Non-temporary GameSetting changing type during copy!!
	0x005BC713,	//	FORMS: Could not checkout form '%s' during import
	0x005BC72C,	//	MASTERFILE: Could not find game setting '%s' during import.
	0x005BDA58,	//	MASTERFILE: Could not find LoadForm for Loading Screen '%s' (%08X).
	0x005BF8B2,	//	MASTERFILE: Recipe Category (%08X) not found in InitItem for Recipe (%08X) '%s'.
	0x005BF92C,	//	MASTERFILE: Recipe (sub)Category (%08X) not found in InitItem for Recipe (%08X) '%s'.
	0x005C04E6,	//	MENUS: Failed to create Conditions window.
	0x005C1190,	//	FORMS: Failed to create tab window.
	0x005C2852,	//	SCRIPTS: Trying to access local variable %d in script '%s' -- variable not found.\r\n
	0x005C2C4E,	//	MASTERFILE: Referenced object %d on Script (%08X) '%s' is invalid.
	0x005C2C8A,	//	MASTERFILE: Referenced object list on Script (%08X) '%s' is corrupt.  Expected %d objects, found %d.
	0x005C3230,	//	SCRIPTS: Script ID \"%s\" is not unique! Previous object type is UNKNOWN.\r\n
	0x005C3283,	//	SCRIPTS: Script ID \"%s\" is not unique! Previous object is type %s.\r\n
	0x005C36A2,	//	MASTERFILE: Script '%s' in file '%s' has not been compiled.\r\n
	0x005C36F5,	//	Script %s has text but has not been compiled.	<- change to SCRIPTS: Script ID %s has text but has not been compiled.
	0x005C39F8,	//	SCRIPTS: Script ID "%s" is not unique! Previous object is type %s.
	0x005C3A2B,	//	SCRIPTS: Warnings encountered recompiling script '%s' (%08X).
	0x005C3AC5,	//	SCRIPTS: Warnings encountered recompiling scripts for package '%s' (%08X).
	0x005C3B50,	//	SCRIPTS: Warnings encountered recompiling scripts for terminal '%s' (%08X).
	0x005C3BB9,	//	SCRIPTS: Warnings encountered recompiling scripts for perk '%s' (%08X).
	0x005C3C29,	//	SCRIPTS: Warnings encountered recompiling all scripts. See warnings file for more information.
	0x005CA101,	//	Script error: objective %d does not exist in quest %s (%08X)
	0x005CA127,	//	Script error: quest %s (%08X) does not exist.
	0x005CA1E8,	//	Script error: objective %d does not exist in quest %s (%08X)
	0x005CA20E,	//	Script error: quest %s (%08X) does not exist.
	0x005CD465,	//	MASTERFILE: Unable to find sound (%08X) on addon node '%s'.
	0x005CDBE0,	//	MASTERFILE: Current master particle flag does not match 3D. Hit OK to check out node and fix this.
	0x005CDF25,	//	MASTERFILE: Could not find item list (%08X) on constructible object '%s' (%08X).
	0x005CDFA5,	//	MASTERFILE: Could not find created item (%08X) on constructible object '%s' (%08X).
	0x005CEDBA,	//	MODELS: Could not load 3D for model %s
	0x005CF501,	//	MODELS: No last debris data when loading texture list for debris '%s' (%08X)
	0x005CFE1F,	//	MASTERFILE: Could not find impact data set (%08X) for explosion '%s' (%08X).
	0x005CFE9F,	//	MASTERFILE: Could not find impact placed object (%08X) for explosion '%s' (%08X).
	0x005D127A,	//	MASTERFILE: Could not find icon (%08X) on weapon '%s' (%08X).
	0x005D133D,	//	MASTERFILE: Errors were encountered during InitItem for conditions on menu item %d for terminal '%s' (%08X)\n\nSee Warnings file for more information.
	0x005D26E9,	//	EDITOR: Could not find message with editor ID %s during import
	0x005D32BA,	//	MASTERFILE: Could not find looping sound (%08X) for movable static '%s' (%08X).
	0x005D3BC2,	//	MASTERFILE: Water type %08X could not be found on PlaceableWater %s (%08X)
	0x005D84D5,	//	Unable to find static (%08X) in collection '%s' (%08X). Recreate and Fragment functionality for this collection will no longer function properly.
	0x005D9CF2,	//	AV Object '%s' with no shader property was found in scene graph. This object will be skipped.
	0x005DA123,	//	Failed to load 3D for object '%s' (%08X).
	0x005DA85A,	//	Warnings were encountered creating file '%s'. Check warnings file for more info.
	0x005DA92F,	//	Failed to check out all selected objects.
	0x005DB19F,	//	MASTERFILE: Could not find voice type (%08X) for activator '%s' (%08X).
	0x005DBACA,	//	MASTERFILE: Unable to find password (%08X) on terminal '%s' (%08X).
	0x005DBB7A,	//	MASTERFILE: Unable to find display note (%08X) for menu item %i on terminal '%s' (%08X).
	0x005DBBF3,	//	MASTERFILE: Unable to find submenu (%08X) for menu item %i on terminal '%s' (%08X).
	0x005DBC95,	//	MASTERFILE: Errors were encountered during InitItem for result script on menu item %d for terminal '%s' (%08X)\n\nSee Warnings file for more information.
	0x005DBD4E,	//	MASTERFILE: Errors were encountered during InitItem for conditions on menu item %d for terminal '%s' (%08X)\n\nSee Warnings file for more information.
	0x005DC3E1,	//	DIALOGUE: Failed to create conditions controls.
	0x005DDC1B,	//	EDITOR: Could not find terminal with editor ID %s during import
	0x005DEFDD,	//	MASTERFILE: Could not find projectile (%08X) on ammo '%s' (%08X).
	0x005DF043,	//	MASTERFILE: Could not find ammo consumed form (%08X) on ammo '%s' (%08X).
	0x005DF0CB,	//	MASTERFILE: Could not find Ammo Effect object (%08X) for ammo '%s' (%08X).
	0x005E0F46,	//	FORMS: Could not checkout form '08X' during import.
	0x005E0F56,	//	MASTERFILE: Could not find ammo '08X' during import.
	0x005E0F64,	//	MASTERFILE: Could not find ammo '08X' during import.
	0x005E8A94,	//	MODELS: You have not selected any FurnitureMarkers for '%s'.
	0x005E9AE0,	//	MODELS: Missing model filename on %s
	0x005EC65D,	//	MASTERFILE: Unable to find sound (%08X) on object '%s'.
	0x005ECE79,	//	DIALOGUE: Failed to create valid color control in Light dialog.
	0x005F16A9,	//	MODELS: Bound for object '%s' (%08X) exceeds 32000 unit limits (%.0f,%.0f,%.0f),(%.0f,%.0f,%.0f). Clamping to Max Bound.
	0x005F1857,	//	Failed to check out %s form '%s' (%08X). Bound change will not be saved.
	0x005F197C,	//	MODELS: %s has a scale of 0.  Change it in the editor.
	0x005F1C12,	//	Invalid scale found on ModelLoader copy of file '%s'
	0x005F1F60,	//	MODELS: LOD Mesh '%s' for object %08X '%s' could not be found.
	0x005F2061,	//	MASTERFILE: Could not find looping sound (%08X) for activator '%s' (%08X).
	0x005F20E1,	//	MASTERFILE: Could not find activate sound (%08X) for activator '%s' (%08X).
	0x005F2161,	//	MASTERFILE: Could not find pRadioTemplate (%08X) for activator '%s' (%08X).
	0x005F21E1,	//	MASTERFILE: Could not find water form (%08X) for activator '%s' (%08X).
	0x005F2288,	//	MASTERFILE: Could not find radio station (%08X) for activator '%s' (%08X).
	0x005F3FB6,	//	MASTERFILE: Armor '%s' needs to have biped slots selected in the editor.
	0x005F4B6A,	//	MASTERFILE: Armor '%s' needs to have biped slots selected in the editor.
	0x005F799F,	//	DEFAULT: Could not checkout form %08X during import
	0x005F79EB,	//	EDITOR: Could not find book with editor ID %s during import
	0x005F7DB6,	//	MASTERFILE: Clothing '%s' needs to have biped slots selected in the editor.
	0x005F885A,	//	MASTERFILE: Could not find open sound (%08X) for container '%s' (%08X).
	0x005F88DA,	//	MASTERFILE: Could not find open sound (%08X) for container '%s' (%08X). <- bug? (close sound)
	0x005F895A,	//	MASTERFILE: Could not find pLoopingSound (%08X) for container '%s' (%08X).
	0x005FA92E,	//	MASTERFILE: Could not find RandomTeleport (%08X) for Door '%s' (%08X).
	0x005FAA5F,	//	MASTERFILE: Animating door '%s' (%08X) does not have collision layer L_ANIMSTATIC or L_TRANSPARENT on a keyframed body.
	0x005FB94D,	//	MASTERFILE: Could not find pLoopingSound (%08X) for container '%s' (%08X).
	0x005FC4F9,	//	MASTERFILE: Could not find looping/random sound (%08X) for static '%s' (%08X).
	0x006001CD,	//	MASTERFILE: Could not find projectile (%08X) on weapon '%s' (%08X).
	0x0060024D,	//	MASTERFILE: Could not find scope effect (%08X) on weapon '%s' (%08X).
	0x006002CD,	//	MASTERFILE: Could not find Spell Item (%08X) on weapon '%s' (%08X).
	0x0060034D,	//	MASTERFILE: Could not find vats Spell Item (%08X) on weapon '%s' (%08X). <- bug ? (VATS)
	0x006003CD,	//	MASTERFILE: Could not find open sound (%08X) for weapon '%s' (%08X). <- bug ? (Pickup Sound?)
	0x0060044D,	//	MASTERFILE: Could not find open sound2 (%08X) for weapon '%s' (%08X). <- bug ? (Putdown Sound?)
	0x006004CD,	//	MASTERFILE: Could not find open sound (%08X) for weapon '%s' (%08X). <- bug ?
	0x0060054D,	//	MASTERFILE: Could not find sound (%08X) for weapon '%s' (%08X). <- bug ?
	0x006005CD,	//	MASTERFILE: Could not find open sound (%08X) for weapon '%s' (%08X). <- bug?
	0x0060064D,	//	MASTERFILE: Could not find open sound (%08X) for weapon '%s' (%08X). <- bug ?
	0x006006CD,	//	MASTERFILE: Could not find idle sound (%08X) for weapon '%s' (%08X).
	0x0060074D,	//	MASTERFILE: Could not find equip sound (%08X) for weapon '%s' (%08X).
	0x006007CD,	//	MASTERFILE: Could not find unequip sound (%08X) for weapon '%s' (%08X).
	0x0060084D,	//	MASTERFILE: Could not find unequip sound (%08X) for weapon '%s' (%08X). <- bug ?
	0x006008CD,	//	MASTERFILE: Could not find unequip sound (%08X) for weapon '%s' (%08X). <- bug ?
	0x0060094D,	//	MASTERFILE: Could not find unequip sound (%08X) for weapon '%s' (%08X). <- bug ?
	0x006009CD,	//	MASTERFILE: Could not find impact data set (%08X) for weapon '%s' (%08X).
	0x00600A4D,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600ACD,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600B4D,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600BCD,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600C4D,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600CCD,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600D4D,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600DCD,	//	MASTERFILE: Could not find 1st person model object (%08X) for weapon '%s' (%08X).
	0x00600E4D,	//	MASTERFILE: Could not find misc object (%08X) for weapon '%s' (%08X).
	0x00600ECD,	//	MASTERFILE: Could not find misc object (%08X) for weapon '%s' (%08X).
	0x00600F4D,	//	MASTERFILE: Could not find misc object (%08X) for weapon '%s' (%08X).
	0x006016FB,	//	COMBAT: Unknown weapon type %i encountered in function TESObjectWEAP::GetCombatWeaponType().
	0x00603E28,	//	MASTERFILE: Clearing invalid type on weapon '%s' (%08X).
	0x00606571,	//	FORMS: Failed to create tab window.
	0x006076AA,	//	Unable to find owner form (%08X) on encounter zone '%s' (%08X).
	0x00609C5A,	//	MASTERFILE: Warnings were encountered while loading weather list chunk from climate %s
	0x0060A70A,	//	DEFAULT: Unable to add default weather to default climate.  ( TESClimate::MakeDefault() )
	0x0060B9E8,	//	MASTERFILE: Could not find pIntroSound (%08X) for ImageSpaceModifier '%s' (%08X).
	0x0060BA62,	//	MASTERFILE: Could not find pOutroSound (%08X) for ImageSpaceModifier '%s' (%08X).
	0x0061BEB9,	//	CELLS: Failed to load landscape data for LAND (%08X) in Cell (%i, %i) from file '%s'.
	0x0061C43C,	//	TEXTURES: Land (%i, %i) found unrecognized vertex texture data in file %s.
	0x0061C50A,	//	TEXTURES: Land (%i, %i) unable to find base texture ID (%08X) for block %i.
	0x0061C58B,	//	CELLS: Land (%i, %i) clamping invalid index %i for block %i.
	0x0061C602,	//	TEXTURES: Land (%i, %i) unable to find additional texture ID (%08X) for block %i.
	0x0061D9C6,	//	MASTERFILE: Potentially duplicate Land (%08x) encountered in file %s.
	0x0061E001,	//	MASTERFILE: Land (%i, %i) found unrecognized vertex texture data in file %s.
	0x0061E104,	//	MASTERFILE: Land (%i, %i) unable to find base texture ID (%08X) for block %i.
	0x0061E1C8,	//	MASTERFILE: Land (%i, %i) in world '%s' has invalid texture index %i for block %i.
	0x0061E26E,	//	MASTERFILE: Land (%i, %i) clamping invalid index %i for block %i.
	0x0061E316,	//	MASTERFILE: Land (%i, %i) unable to find additional texture ID (%08X) for block %i.
	0x00624210,	//	MASTERFILE: Error saving land height Data for cell (%i, %i). Error correction attempted.\r\n
	0x006250C3,	//	MASTERFILE: Could not find Grass (%08X) for LandTexture '%s' (%08X).
	0x006251A2,	//	MASTERFILE: Could not find Texture Set (%08X) for LandTexture '%s' (%08X).
	0x006251E6,	//	MASTERFILE: TextureSet '%s' (%08X) missing diffuse texture (required for landscape textures).
	0x0062521C,	//	MASTERFILE: TextureSet '%s' (%08X) missing normal texture (required for landscape textures).
	0x0062763E,	//	HAVOK ERROR: Interior is too large for the havok world! World must be expanded or cell must be made smaller. Cell: '%s' (%08X)\nCell Extents:\t x:%f y:%f z:%f\nWorld Size:\t\t %f
	0x00627D87,	//	DEFAULT: --------Found Duplicate references:\r\n%s (%08X) - %i use\r\n%s (%08X) - %i use
	0x00627EA2,	//	DEFAULT: Did not delete NAMED duplicate reference (%08X) to object \"%s\" (%08X) at (%0.f,%0.f,%0.f) in cell %s.\r\n
	0x00627F34,	//	DEFAULT: Did not delete USED duplicate reference (%08X) to object \"%s\" (%08X) at (%0.f,%0.f,%0.f) in cell %s.\r\n
	0x00627FD4,	//	DEFAULT: DELETED Duplicate reference (%08X) to object \"%s\" (%08X) at (%0.f,%0.f,%0.f) in cell %s.\r\n
	0x0062B1C6,	//	Portal %08X, room %08X, and room %08X in cell %s (%08X) are not coplanar. The normals differ by %f and the D coefficient by %f.
	0x0062BCF6,	//	Linked rooms %08X and %08X in cell %s (%08X) do not overlap/touch.
	0x00631E9F,	//	Multibound for ref %08X in cell %s (%08X) is outside of the loaded world's extents
	0x00633523,	//	DEFAULT: Could not check out : %s
	0x006341F2,	//	CELLS: Failed to create valid Ambient color control in Cell dialog.
	0x00634283,	//	CELLS: Failed to create valid Fog color control in Cell dialog.
	0x00634314,	//	DIALOGUE: Failed to create valid Directional color control in Cell dialog.
	0x00635159,	//	Reference %08X (%s) in cell %08X (%s) should not be refracted by auto water; the flag was reset.
	0x006351EF,	//	Reference %08X (%s) in cell %08X (%s) should not be refracted by auto water; however, the flag could not be reset, as the reference is checked out by %s.
	0x006382A2,	//	PLUGINS: ERROR! Full check in failed for %s.\n\nThe FID and FUD files may need to be updated and the plugin will need to be checked in by hand from a backup.
	0x00638644,	//	DEFAULT: Failed to create tab window.
	0x00638CBC,	//	DIALOGUE: Dialog box instantiated incorrectly.
	0x0063AA7A,	//	Testing %i NavMesh triangles.
	0x0063AB62,	//	Testing %i reference positions.
	0x0063AC83,	//	!!!RenderTestCell failed!!! (see file in RTF folder for more info)
	0x0063AF28,	//	Error encountered reading line %i of BetaComment file.
	0x0063B600,	//	CELLS: Could not find cell (%i, %i) in world '%s' (%08X) to add reference '%s' (%08X) to.
	0x0063BA7B,	//	Parent cell of ref (%08X) has changed since betacomment was generated. The betacommented location will be loaded instead of the ref location.
	0x0063C453,	//	Cell name %s for cell %s (%08X) is too long. The maximum length allowed is 33 characters.
	0x0063C4DE,	//	Errors were encountered during InitItem for exterior cell:\n\nCell '%s' (%08X) at (%i, %i) in world %s (%08X)\n\nCheck Warnings for details.
	0x0063C51A,	//	Errors were encountered during InitItem for exterior cell:\n\nCell '%s' (%08X) at (%i, %i) in UNKNOWN world\n\nCheck Warnings for details.
	0x0063C53A,	//	Errors were encountered during InitItem for interior cell:\n\nCell '%s' (%08X)\n\nCheck Warnings for details.
	0x0063C5B8,	//	MASTERFILE: Unable to find lighting template (%08X) for cell '%s' (%08X).
	0x0063C686,	//	Cell at (%i, %i) in world %s (%08X) may be a dangling cell.
	0x0063C715,	//	MASTERFILE: Cell contains more than one COC marker:\r\n%s
	0x0063C894,	//	MASTERFILE: Reference attached to wrong cell for its location:\n%s
	0x0063C8E5,	//	MASTERFILE: Bad cell value. Attempting to force ref to (0, 0)
	0x0063C93D,	//	MASTERFILE: -Unable to fix reference.
	0x0063CA9A,	//	MASTERFILE: Cell (%i, %i) in worldspace '%s' has invalid region '%s' (%08X).
	0x0063CAB7,	//	MASTERFILE: Cell (%i, %i) in worldspace UNKNOWN has invalid region '%s' (%08X).
	0x0063CB2C,	//	MASTERFILE: Could not fix because cell was checked out.
	0x00643061,	//	3D for Ref '%s' (%08X) does not support external emittance but ref has explicit emittance data
	0x00647E7A,	//	DEFAULT: The linked door for this reference already contains lock data. Any lock data added to this reference will cause the lock data on the linked reference to be removed on dialog box save.
	0x006483A2,	//	Selected reference checked out by user: 
	0x0064845F,	//	DIALOGUE: On dialog box save, conflicting data on linked door will be removed.
	0x0064D6F3,	//	FORMS: Could not checkout form %08X.
	0x0064FBA4,	//	Patrol point event script for point %s has text but has not been compiled.
	0x0064FCA3,	//	MASTERFILE: Missing/Invalid base object (%08X) for reference (%08X)
	0x0064FF84,	//	MASTERFILE: Chunk %c%c%c%c abnormally terminated TESObjectREFR::Load.\n\n
	0x00652226,	//	HAVOK: The following reference could not be checked out for Havok data update:\n\n%s
	0x0065263B,	//	HAVOK: Disabling collision on ref '%s' (%08X).
	0x00652E14,	//	MODELS: Actor '%s' needs to have a 'skeleton*.nif' model instead of '%s'.
	0x00652EBB,	//	ANIMATION: SetAnimation cleared 3D because %s (%08X) was moved
	0x00652F04,	//	ANIMATION: Bad InitAnimation for Creature '%s' (%08X). Missing 'Idle' animation.
	0x006530C4,	//	ANIMATION: Bad InitAnimation for NPC '%s' (%08X). Missing 'Idle' animation.
	0x006531C2,	//	ANIMATION: %s '%s' is cumulative. This is not allowed. Re-export to correct this problem.
	0x00653351,	//	ANIMATION: Idle animation must be looping '%s'.
	0x00653EA0,	//	DIALOGUE: Failed to create valid reference select control for tab in reference dialog.
	0x00656C4B,	//	%s
	0x00658749,	//	DEFAULT: Conflicting shared data removed from linked door reference.
	0x006591B0,	//	DEFAULT: Failed to create tab window.
	0x00659A2F,	//	No collision object for placeable water object %08X.
	0x00659A5A,	//	No shape for placeable water object %08X's collision object.
	0x00659DC8,	//	MODELS: LOD Mesh '%s' for reference %08X '%s' in cell '%s' could not be found.
	0x00659E05,	//	MODELS: LOD Mesh '%s' for reference %08X '%s' in cell '%s' (%d, %d) could not be found.
	0x00659EE0,	//	MASTERFILE: Invalid base object. Leveled Character/Creature refs no longer allowed. Create a template instead.
	0x00659F88,	//	MASTERFILE: Corrupt location found on reference, setting to (0, 0, 0).
	0x0065A02D,	//	MASTERFILE: Corrupt angle found on reference, setting to (0, 0, 0).
	0x0065A0F3,	//	MASTERFILE: Ref contains old style health data. Setting health percent to %.2f
	0x0065A222,	//	MASTERFILE: Random teleport doors cannot have preexisting teleport data. Teleport data will be removed.
	0x0065A24C,	//	MASTERFILE: Random door has no teleport marker.
	0x0065A31D,	//	MASTERFILE: Errors were encountered during InitItem for reference:\n\nBase: '%s' (%08X)\nRef: '%s' (%08X)\nCell: '%s' (%08X)\n\nSee Warnings file for more information.
	0x0065A3D1,	//	MASTERFILE: Errors were encountered during InitItem for reference:\n\nBase: '%s' (%08X)\nRef: '%s' (%08X)\nCell: '%s' (%08X) (%i, %i) in world '%s' (%08X) \n\nSee Warnings file for more information.
	0x0065A403,	//	MASTERFILE: Errors were encountered during InitItem for reference:\n\nBase: '%s' (%08X)\nRef: '%s' (%08X)\nCell: NONE\n\nSee Warnings file for more information.
	0x0065A5F6,	//	MASTERFILE: Ref (%08X) to base object %s (%08X) in cell %s (%i, %i) (%08X) should be persistent but is not.
	0x0065A6CE,	//	MASTERFILE: Potentially Invalid Z value (%.2f) on reference:\n\n%s
	0x0065A7EC,	//	MASTERFILE: MustBeVisibleDistant ref (%08X) to base object %s (%08X) in cell %s (%i, %i) (%08X) could not be checked out.
	0x0065A887,	//	MASTERFILE: Potentially Invalid X value (%.2f) on reference:\n\n%s
	0x0065A917,	//	MASTERFILE: Potentially Invalid Y value (%.2f) on reference:\n\n%s
	0x0065A9CA,	//	MASTERFILE: No leveled actor modifier on reference:\n\n%s
	0x0065C320,	//	MASTERFILE: Could not find sound (%08X) for water type '%s' (%08X).
	0x0065C3A6,	//	MASTERFILE: Could not find sound (%08X) for spell item '%s' (%08X).
	0x0065C3E1,	//	MASTERFILE: Actor Effect '%s' (%08X) targets more than the player's self!
	0x0065C516,	//	MASTERFILE: Could not find WaterForm (%08X) for water type '%s' (%08X).
	0x0065C945,	//	DIALOGUE: Failed to create valid Fill color control in WaterForm dialog.
	0x0065C9C1,	//	DIALOGUE: Failed to create valid Fill color control in WaterForm dialog.
	0x0065CA3D,	//	DIALOGUE: Failed to create valid Fill color control in WaterForm dialog.
	0x0065F8BC,	//	DEFAULT: Failed to create tab window.
	0x00661289,	//	MASTERFILE: Unable to find imagespacemodifier (%08X) on owner object \"%s\".
	0x006612A1,	//	MASTERFILE: Unable to find imagespacemodifier (%08X) on owner object (%08X).
	0x006612B5,	//	MASTERFILE: Unable to find imagespacemodifier (%08X) on unknown owner.
	0x0066143F,	//	DIALOGUE: Could not select imagespace modifier \"%s\" (%08X) in LoadDialog for TESImageSpaceModifiableCountForm form component.
	0x006620B6,	//	DIALOGUE: Failed to create valid color control in Weather dialog.
	0x00662460,	//	DIALOGUE: Failed to create valid color control for lightning color in Weather dialog.
	0x006637CD,	//	DEFAULT: Failed to create tab window.
	0x00663B05,	//	DEFAULT: Could not get number of layers from the clouds .nif
	0x00665BA9,	//	MASTERFILE: Unable to find climate (%08X) on owner worldspace \"%s\".
	0x00665BC1,	//	MASTERFILE: Unable to find climate (%08X) on owner worldspace (%08X).
	0x00665C69,	//	MASTERFILE: Unable to find imagespace (%08X) on owner worldspace \"%s\".
	0x00665C81,	//	MASTERFILE: Unable to find imagespace (%08X) on owner worldspace (%08X).
	0x00665D29,	//	Unable to find encounter zone (%08X) on owner worldspace \"%s\".
	0x00665D41,	//	Unable to find encounter zone (%08X) on owner worldspace (%08X).
	0x00665DE9,	//	MASTERFILE: Unable to find water type (%08X) on owner worldspace \"%s\".
	0x00665E01,	//	MASTERFILE: Unable to find water type (%08X) on owner worldspace (%08X).
	0x00665EA9,	//	MASTERFILE: Unable to find lod water type (%08X) on owner worldspace \"%s\".
	0x00665EC1,	//	MASTERFILE: Unable to find lod water type (%08X) on owner worldspace (%08X).
	0x00665F69,	//	MASTERFILE: Unable to find music type (%08X) on owner worldspace \"%s\".
	0x00665F81,	//	MASTERFILE: Unable to find music type (%08X) on owner worldspace (%08X).
	0x00666029,	//	MASTERFILE: Unable to find landscape world (%08X) on owner worldspace \"%s\".
	0x00666041,	//	MASTERFILE: Unable to find landscape world (%08X) on owner worldspace (%08X).
	0x00666188,	//	Failed to auto-checkout worldspace '%s' (%08X).
	0x00666334,	//	CELLS: Trying to get exterior cell for invalid cell coordinate. Values must be between %i and %i.
	0x00666420,	//	CELLS: Cell (%08X) %s already exists at coord (%i, %i ).
	0x006669F0,	//	CELLS: Region \"%s\" (%08X) could not be removed from cell \"%s\" (%d, %d) (%08X) (checked out by \"%s\").
	0x0066909F,	//	DEFAULT: Failed to check out land (%i, %i).
	0x0066A5A5,	//	CELLS: Trying to get exterior cell for invalid cell coordinate. Values must be between %i and %i.
	0x0066B5E7,	//	MASTERFILE: Failed to find addiction item (%08X) for ingestible '%s' (%08X).
	0x0066B667,	//	MASTERFILE: Failed to find consumption sound (%08X) for ingestible '%s' (%08X).
	0x0066CB2D,	//	GAME EFFECTS: Unknown EffectSetting encountered when loading EffectItem in actor effect '%s'
	0x0066DD0B,	//	DIALOGUE: Failed to create conditions controls.
	0x0066E8DD,	//	MASTERFILE: Unable to load EffectItem '%s' in Actor Effect '%s'
	0x0066E926,	//	MASTERFILE: Unable to find EffectSetting %d in Actor Effect '%s'.
	0x0066E968,	//	MASTERFILE: Trying to load condition data with no effect item in Actor Effect '%s'.
	0x0066FC26,	//	MASTERFILE: Could not find associated item (%08X) for Effect '%s' (%08X).
	0x0066FCB2,	//	MASTERFILE: Could not find light (%08X) for Effect '%s' (%08X).
	0x0066FD32,	//	MASTERFILE: Could not find cast sound (%08X) for Effect '%s' (%08X).
	0x0066FDB2,	//	MASTERFILE: Could not find bolt sound (%08X) for Effect '%s' (%08X).
	0x0066FE32,	//	MASTERFILE: Could not find hit sound (%08X) for Effect '%s' (%08X).
	0x0066FEB2,	//	MASTERFILE: Could not find area sound (%08X) for Effect '%s' (%08X).
	0x0066FF32,	//	MASTERFILE: Could not find effect shader (%08X) for Effect '%s' (%08X).
	0x0066FFB2,	//	MASTERFILE: Could not find enchant effect shader (%08X) for Effect '%s' (%08X).
	0x00670CF6,	//	FORMS: Invalid form ID (%d) for effect setting found while loading counter effects for %s.
	0x00674857,	//	GAME EFFECTS: Effect Item (%08X) %s has no effects defined.
	0x00674CB7,	//	GAME EFFECTS: Effect Item (%08X) %s has no effects defined.
	0x0067949B,	//	%s:%d: %s : %s'
	0x006794CB,	//	%s:%d: %s : %s'
	0x006794FB,	//	%s:%d: %s : %s'	: File Characters\\_Male\\Skeleton.NIF contains an old hkpRigidBody! Converting cinfo at runtime is slow and should be done by the build process.
//	0x00679517,	//	Unknown/Unused
	0x00697C8C,	//	MODELS: Failed to load '%s' in BSFaceGenModel::LoadModelMesh.
	0x00699867,	//	MODELS: Statistical and Differential FaceGen morphs found for expression \"%s\".  Only statistical will be used.
	0x00699A1C,	//	MODELS: Statistical and Differential FaceGen morphs found for modifier \"%s\".  Only statistical will be used.
	0x00699BD0,	//	MODELS: Statistical and Differential FaceGen morphs found for phoneme \"%s\".  Only statistical will be used.
	0x00699D36,	//	MODELS: Statistical and Differential FaceGen morphs found for Custom Morph \"%s\".  Only statistical will be used.
	0x0069A8E2,	//	MODELS: Could not find bone \"%s\" for the head node.
	0x0069CDD2,	//	MODELS: Tree %s has too many leaf maps (greater than six).  Leaves may not display properly.
	0x006A72C4,	//	TEXTURES: Decal Parent '%s' has no normals!
	0x006A7B94,	//	DECAL: No Decal for : %s
	0x006A99FA,	//	PATHFINDING: Navmesh %08x Cell %s and Navmesh %08x Cell %s have at least one edge in common but are not connected. Are the navmeshes Finalized?
	0x006AAA14,	//	PATHFINDING: Navmesh %08x Cell %s, has bad vertex X coordinate, Please regenerate
	0x006AAA49,	//	PATHFINDING: Navmesh %08x Cell %s, has a large number of triangles, %d, please check and optimize
	0x006AAAD3,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d, edge %d has bad Triangle index, Clearing the connection
	0x006AAC0B,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d, has a downfacing normal, flipping the triangle
	0x006AADDC,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d, edge %d has bad extra info index, Clearing extra info flag
	0x006AB273,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d, edge %d has bad Portal (Navmesh %08x does not have a triangle index %d), Clearing the portal
	0x006AB3C0,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d, edge %d has a Portal to a Navmesh (%08x) in a different Worldspace, Clearing the portal
	0x006AB55A,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d is degenerate, Vertices 0 and 1 both use vertex index %d
	0x006AB6B1,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d is degenerate, Vertices 0 and 2 both use vertex index %d
	0x006AB7FF,	//	PATHFIDING: Navmesh %08x Cell %s, Triangle %d is degenerate, Vertices 1 and 2 both use vertex index %d
	0x006AB99C,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edges 0 and 1 both point to the same triangle %d
	0x006ABB12,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edges 0 and 2 both point to the same triangle %d
	0x006ABC90,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edges 1 and 2 both point to the same triangle %d
	0x006ABEC3,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d and %d have opposite normals but are linked
	0x006AC044,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edge %d has a portal data to Triangle %d, but Triangle %d doesn't have any Portal info for its matching edge %d
	0x006AC192,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edge %d Should have a link to Triangle %d, but doesn't
	0x006AC2D4,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edge %d Should have a link to Triangle %d, but doesn't
	0x006AC422,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d Edge %d Should have Portal information, but doesn't
	0x006AC68E,	//	PATHFINDING: Navmesh %08x Cell %s, Triangle %d has a link to Triangle %d, but Triangle %d doesn't have the matching link
	0x006AC820,	//	PATHFINDING: Navmesh %08x Cell %s, Triangles %d and %d are linked, but their vertices do not match
	0x006AE630,	//	MASTERFILE: Did not find matching NavMeshInfo for NavMesh ID %08X in cell %s, Creating a temporary one
	0x006AE6B6,	//	MASTERFILE: NavMesh ID %08X in cell %s is altered but not checked out to the current user.
	0x006AE921,	//	MASTERFILE: Potentially duplicate Navmesh (%08x) encountered in file %s.
	0x006AEA87,	//	CELL: Could not find parent cell (%08X) for NavMesh (%08X).
	0x006AEAA6,	//	CELL: Attempting to remap NavMesh parent to cell (%08X) instead.
	0x006AEB6E,	//	MASTERFILE: Navmesh %s has a bad navmesh grid, please touch and resave
	0x006AEC8B,	//	NavMesh: Door reference %08X is not found but is linked to %s.  Please finalize to resolve this.
	0x006AEF35,	//	MASTERFILE: %s has unresolved warnings.
	0x006AEFCF,	//	Depreciated NavMesh Door Portals found in %s, removing portals, please refinalize cell to create new door portals
	0x006AF3DF,	//	NavMesh AutoGen: Cell %s has warnings that need to be fixed
	0x006EB4B9,	//	MASTERFILE: Potentially duplicate Navmesh map (%08x) encountered in file %s.
	0x006EB943,	//	PATHFINDING: NavMeshInfo %08x in cell %s, has no parent space, removing it from the map
	0x006EB9E0,	//	PATHFINDING: NavMeshInfo %08x in cell %s, has no matching navmesh, removing it from the map
	0x006EBA53,	//	PATHFINDING: NavMeshInfo %08x in cell %s, has a navmesh that is marked deleted, removing it from the map
	0x006EBB25,	//	PATHFINDING: NavMeshInfo %08x in cell %s, has a portal with no navmesh, removing the portal
	0x006EBC03,	//	PATHFINDING: NavMeshInfo %08x in cell %s, has a portal with no navmesh, removing the portal
	0x006EBEF8,	//	PATHFINDING: NavMeshInfo %08x has a portal to NavMeshInfo %08x, but there is no triangle portal between the two, updating the NavMeshInfo
	0x006EBFBC,	//	PATHFINDING: NavMeshInfo %08x has a portal to NavMeshInfo %08x, but there is no triangle portal between the two, updating the NavMeshInfo
	0x006EC0C9,	//	PATHFINDING: NavMeshInfo %08x does not have a portal to NavMeshInfo %08x, but the navmesh does, updating the NavMeshInfo
	0x006EC115,	//	PATHFINDING: NavMeshInfo %08x does not have a Door portal for door %08x, but the navmesh does, updating the NavMeshInfo
	0x006EC165,	//	PATHFINDING: NavMeshInfo %08x has a portal to Door %08x, but the navmesh does not, updating the NavMeshInfo
	0x006ED31A,	//	MASTERFILE: Could not find Parent Space with ID %08x for NavmeshInfo %08x
	0x006F063D,	//	Finalize NavMesh: Could not find close enough navmesh triangle for door %s (%08x) in cell %s (%08x)
	0x006FC725,	//	CELL: Object in Cell %s (%08x) is fully under the land (found by navmesh generation) and should be removed:\n
	0x0071054E,	//	NavMesh AutoGen: Hit limit of %d, done
	0x0071CF9F,	//	TEXTURE WRAPPING DETECTED: A vertex in geom '%s' has the texture coordinate (%f,%f) which is significantly outside of the 0.0 to 1.0 range required to avoid texture wrapping.  This will result in texture distortion.
	0x0071D04A,	//	MODELS: BGSDistantObjectBuilder::RemapTexCoord() called with no current texture atlas.  Coord will not be remapped.
	0x00720C66,	//	MODELS: LOD diffuse texture '%s' missing for geometry '%s'
	0x00720D01,	//	MODELS: LOD normal texture '%s' missing for geometry '%s'
	0x00720F67,	//	MODELS: LOD diffuse texture '%s' and normal texture '%s' are not the same size (but should be).
	0x00720FBE,	//	MODELS: Missing DDS textures for object %s--check editorwarnings.txt for texture name.
	0x0072105D,	//	MODELS: LOD build error: Geometry '%s' missing a texture property.
	0x00721838,	//	MODELS: Way too much LOD object data.  Truncating.  LOD art must be simplified.
	0x0072277D,	//	MODELS: Way too much LOD object data.  Truncating.  LOD art must be simplified.
	0x00724A48,	//	MODELS: LOD object named '%s' probably has too many vertices (%i).  Consider simplifying this object.
	0x007252F5,	//	MODELS: LOD Mesh '%s' could not be found.
	0x0072577A,	//	MODELS: LOD Object Generation for segment '%i:%i,%i' of world '%s' failed.
	0x0072701E,	//	Object '%s' is flagged as having tree LOD but does not have a mesh.
	0x00739A83,	//	Object Generation - Failed to place reference to \"%s\" at ( %02f, %02f, %02f ).
	0x0073EC22,	//	Region System - Unable to generate in cell (%d, %d) - land is checked out, and the regions have landscape textures to place.
	0x0073ED24,	//	Region System - Unable to generate in cell (%d, %d), some regions (including (%08x) '%s') that affect it are checked out to others.
	0x0073FDC0,	//	Region System - Unable to generate in cell (%d, %d), some regions (including (%08x) '%s') that affect it are checked out to others.
	0x0080C104,	//	Unknown <- script editor warnings?
	0x0085A690,	//	DEFAULT: FR2 ASSERT violation in %s line %i. Code may crash.
	0x0085A6B4,	//	DEFAULT: FR2 ERROR in %s line %i : '%s'. Code may crash.
	0x00888020,	//	AUDIO: ID %i Still In Map
	0x0088CD51,	//	Sound %s is trying to load no filename\n
	0x00893582,	//	Trying to create a sound with no filename\n
	0x008935CE,	//	Audio: Attempting to init %s without defining it as a 2D or 3D sound.
	0x008946CE,	//	AUDIO: Attempted to set audio cache size to %i. Cache size must be < 4MB
	0x008991E9,	//	Audio: Could not open file %s\n
	0x008995CA,	//	Audio: Attempted to play multi-channel sound (%s) in 3D. Forcing 2D playback.
	0x00899D08,	//	Audio: Loop envelope sound (%s) missing smpl chunk. Use different file or change sound definition
	0x0089A7BF,	//	Audio: Attempting to play %i channel sound \"%s\" in 3D (must be mono)
	0x0089A7E1,	//	Audio: Wav file too small for DirectSound to process\"%s\" 
	0x0089E103,	//	Attempted to seek to %.2fs in a sound of only %.2fs duration.
	0x0089E256,	//	Audio: Could not close file.\n
	0x0089EBD9,	//	Audio: Cound not create DSound buffer - unknown warning\n
	0x008A393D,	//	Loading a user created or misnamed BSA
	0x008A3953,	//	Loading a user created or misnamed BSA
	0x008A8AA4,	//	DEFAULT: HashMap Collision between %s and %s
	0x008A8ED8,	//	DEFAULT: HashMap Collision between %s and %s
	0x0092067C,	//	\t%s Geometry has a height map for parallax, but no parallax flag set.
	0x009206AA,	//	\t%s Geometry has parallax flag set, but no height map for parallax.
	0x009EAFA6,	//	BSCompoundFrustum : prethreading an effectively empty op list
	0x009F0692,	//	DEFAULT: Trying to add an emitter (%s) to a master particle system (idx: %d) that hasn't been loaded.
	0x009F0A65,	//	DEFAULT: Trying to remove a master particle system (idx: %d) that hasn't been loaded.
	0x009F10A6,	//	DEFAULT: \"%s\" is a Master Particle System not emitting in world space.
	0x009F1138,	//	DEFAULT: \"%s\" is a Master Particle System not pointing to the proper root.
	0x009F11CB,	//	DEFAULT: \"%s\" is a Master Particle System without a multi-target emitter.
	0x009F13C4,	//	DEFAULT: \"%s\" is a Master Particle System without a multi-target emitter.
	0x009FF405,	//	Called FindBoundaryCW on a dummy edge without a pair.
	0x009FF42B,	//	Called FindBoundaryCW on a dummy edge that has an invalid (non-border) pair.
	0x009FF623,	//	Sanity check FAILED: A vertex has a NULL edge pointer.
	0x009FF65E,	//	Sanity check FAILED: A non-collapsed vertex points to a collapsed edge.
	0x009FF6C5,	//	Sanity check FAILED: Pair edges have the same origin vertex (bad winding).
	0x009FF6E5,	//	Sanity check FAILED: A border edge's pNext points to a dummy edge.
	0x009FF70B,	//	Sanity check FAILED: A non-border, non-collapsed edge has a null facet.
	0x009FF739,	//	Sanity check FAILED: Mismatched origin/target vertices.
	0x009FF764,	//	Sanity check FAILED: A non-collapsed edge has become degenerate.
	0x009FF78D,	//	Sanity check FAILED: A collapsed edge has a pair that is not collapsed.
	0x009FF7CD,	//	Sanity check FAILED: Two pairs of same edge loop share an edge loop--probably a facet inversion.
	0x009FF7F6,	//	Sanity check FAILED: A dummy edge has an interior pair.
	0x009FF821,	//	Sanity check FAILED: An edge has a mismatched pair.
	0x009FF83F,	//	Sanity check FAILED: An edge fails to form a loop.
	0x009FF942,	//	Sanity check FAILED: An edge has collapsed without being flagged as such.
	0x009FF9B6,	//	Sanity check FAILED: Mesh contains a non-collapsed but degenerate triangle.
	0x009FFAB9,	//	CollapseEdge: Invalid pair pointer.
	0x009FFB4A,	//	CollapseEdge: Invalid pair pointer.
	0x009FFD7E,	//	Found no new edge for a vertex.
	0x00A000B3,	//	Mesh is too big to convert to an NiGeom.
	0x00A009F2,	//	Mesh is too big to convert to an NiGeom.
	0x00A01236,	//	Mesh will still be too big to save with specified decimation percentage.
	0x00A0229E,	//	Called BuildTerrainSkirt() on a non-terrain mesh.  Ignoring.
	0x00A026C8,	//	Called BuildTerrainSkirtAlongVector on a non-terrain mesh.
	0x00A02706,	//	BuildTerrainSkirtAlongVector: Must start and end at a corner vertex of the terrain. 
	0x00A02744,	//	BuildTerrainSkirtAlongVector: Must start and end at a corner vertex of the terrain. 
	0x00A02BD8,	//	Called FindNumVerticesAlongEdge on a non-terrain mesh.
	0x00A02C18,	//	FindNumVerticesAlongEdge: Must start and end at a corner vertex of the terrain. 
	0x00A02C58,	//	FindNumVerticesAlongEdge: Must start and end at a corner vertex of the terrain. 
	0x00A02D72,	//	Attempted to add a degenerate edge to the queue.
};

static const UInt32 kPatch_Warnings4[] =
//	these are report calls to 846650
{
	0x004018D2,	//	Building relationship map from cell %s (%i/%i)
	0x00401911,	//	--Finding relationships for file %s (%i/%i)
	0x00401A1C,	//	--Clearing cell processed flags for file %s (%i/%i)
	0x00401D05,	//	Outputting data for file %s (%i/%i)
	0x00401E68,	//	Outputting data for group %i with %i files (%i/%i)
	0x0040328F,	//	Reading files for cell %s (%i/%i)
	0x00403447,	//	Finished reading %i files for cell %s
	0x0040366B,	//	Analyzing data for file %s (%i/%i)
	0x00403E58,	//	Building temp file array
	0x00403F5C,	//	Finding ordered file %i/%i
	0x00475A1D,	//	Checking In...
	0x00475BD4,	//	...file: '%s'
	0x00475C68,	//	...buffer extension size: %i MB
	0x00475D4C,	//	......ID update
	0x00475D65,	//	......form data
	0x00475F25,	//	......running full check ins
	0x00476038,	//	......updating worldspace offsets
	0x00476168,	//	......updating owned ID array
	0x004761AF,	//	......closing target files
	0x00476227,	//	......clearing deleted forms
	0x0047635E,	//	......updating form file indices
	0x004763E4,	//	......FAILURE!!
	0x004763EE,	//	......resetting form files
	0x00476484,	//	......closing target files
	0x004764E7,	//	......file done
	0x00476511,	//	...Backing up Plugin
	0x0047653B,	//	...COMPLETED.
	0x00476715,	//	VersionControl clean up...
	0x00476EC5,	//	...Done.
	0x0048972C,	//	intersect: %s    xFaceIntersect: %s
	0x0048978A,	//	Delta: %s    DragVert: %i
	0x0048DA60,	//	NiRenderer::BeginUsingRenderTargetGroup> Failed - NULL pointer for render target. Call BeginUsingDefaultRenderTargetGroup if you intended to use the backbuffer instead.
	0x004DD34C,	//	Unable to find masterfile: %s
	0x004DD35B,	//	Unable to find masterfile: <unknown>
	0x004DD5EC,	//	Too many selected files to compile!
	0x004DD682,	//	Too many selected files to compile!
	0x0050284E,	//	Model: '%s'\t\tTexture: '%s'
	0x0052449E,	//	%s
	0x005244BC,	//	Computer Name: %s
	0x005B1783,	//	%s
	0x005B1CD1,	//	%s
	0x005B1D02,	//	%s
	0x005B1D33,	//	%s
	0x005C5384,	//	The script function list doesn't have all the names filled in.\r\n
	0x005C539D,	//	The script console-command list doesn't have all the names filled in.\r\n
	0x00725CF9,	//	BAD TYPE DATA: GROUP=%i TYPE PTR=%p
	0x007263DE,	//	Added new group %i, type=%i/%i, ptr=%p
	0x00726533,	//	Added new type %i, ptr=%p
	0x00726B1F,	//	Adding a new tree type for model %s...
	0x00726DEA,	//	Building for cell range (%i,%i)-(%i,%i)
	0x00727099,	//	Tree Block (%i,%i):%i has %i LOD trees in it.
	0x007FED62,	//	Transform data loading has more than 65535 rotation keys!
	0x007FEE12,	//	Transform data loading has more than 65535 position keys!
	0x007FEEC2,	//	Transform data loading has more than 65535 scale keys!
	0x0080D62E,	//	%s has a geometry with more than one texture coordinate set, wasting %dk of disk space.
	0x00898336,	//	Audio system could not be initialized.
	0x009D5FA8,	//	NiCullingProcess::Append> Geometry failed RegisterObject, not rendering: %s\n
	0x009FF231,	//	Half-edge mesh constructed.  Stats:
	0x009FF257,	//	Interior edges: %i\nBorder edges: %i\nDummy edges: %i\nTotal edges: %i
	0x009FF26E,	//	Facets: %i
	0x009FFCDF,	//	Couldn't find a new edge to point a vertex to.  Doing linear search (slow).
	0x009FFD6A,	//	Found one.
	0x00A004D3,	//	%i water edge verts pushed out.
	0x00A0089D,	//	Output number of indices: %i
	0x00A008AF,	//	                vertices: %i
	0x00A010BD,	//	Output number of indices: %i
	0x00A010CF,	//	                vertices: %i
	0x00A014E5,	//	Done.  Actual number of output facets: %i
	0x00A028CC,	//	Last Vert: ( %f, %f, %f )
	0x00A028FC,	//	Current Vert: ( %f, %f, %f )
	0x00A02998,	//	Last Skirt Vert: ( %f, %f, %f )
	0x00A029C8,	//	Current Skirt Vert: ( %f, %f, %f )
	0x00C03E2F,	//	Create D3D9Ex instance...FAILED\n
	0x00C03E4D,	//	Create D3D9Ex instance...SUCCESSFUL\n
	0x00C03E74,	//	Create D3D9 instance...FAILED\n
	0x00C03E87,	//	Create D3D9 instance...SUCCESSFUL\n
	0x00C06994,	//	NiDX9Renderer::Initialize> No matching framebuffer format available\n
	0x00C06A08,	//	NiDX9Renderer::Initialize> No matching depth/stencil format available\n
	0x00C06B16,	//	NiDX9Renderer::Initialize> Could not support requested antialiasing mode - disabling\n
	0x00C06B99,	//	NiDX9Renderer::Initialize> Could not find matching fullscreen display mode\n
	0x00C06BB6,	//	NiDX9Renderer::Initialize> Could not match requested fullscreen refresh rate - using closest\n
	0x00C06C4A,	//	NiDX9Renderer::Initialize> Could not query device caps - FAILING\n
	0x00C06F09,	//	NiDX9Renderer::Initialize> Device does not support single-pass multi-texture - FAILING\n
	0x00C087FD,	//	NiDX9Renderer::LockDynamicTexture Failed> LockRect - %s
	0x00C088B7,	//	NiDX9Renderer::UnLockDynamicTexture Failed> UnlockRect - %s
	0x00C08900,	//	NiDX9Renderer::UnLockDynamicTexture Failed> UpdateTexture - %s
	0x00C0958B,	//	NiDX9Renderer::DestroyWindowedFramebuffer - destroying swap chain that is a current render target\n
	0x00C0998C,	//	NiDX9Renderer::BeginUsingRenderTargetGroup> Failed - %s	:	Setting the depth/stencil buffer failed. Resetting to default target.
	0x00C099C7,	//	NiDX9Renderer::BeginUsingRenderTargetGroup> Failed - %s	:	Setting an output target failed. Resetting to default target.
	0x00C09A15,	//	NiDX9Renderer::BeginUsingRenderTargetGroup> Failed - %s	:	Setting the depth/stencil buffer to NULL failed. Resetting to default target.
	0x00C0A0D6,	//	NiDX9Renderer::FastCopy> Failed - %s	:	Pixel formats do not match
	0x00C0A183,	//	NiDX9Renderer::FastCopy> Failed StretchRect - %s
	0x00C0A1AD,	//	NiDX9Renderer::FastCopy> Failed - %s	:	NULL Surface found
	0x00C0A1CB,	//	NiDX9Renderer::FastCopy> Failed - %s	:	No RendererData found
	0x00C0A2F4,	//	NiDX9Renderer::Copy> Failed StretchRect - %s
	0x00C0A31E,	//	NiDX9Renderer::Copy> Failed - %s	:	NULL Surface found
	0x00C0A33C,	//	NiDX9Renderer::Copy> Failed - %s	:	No RendererData found
	0x00C0AD20,	//	NiDX9IndexBufferManager::PackBuffer> Failed to lock index buffer\n
	0x00C0B53B,	//	NiDX9IndexBufferManager::PackBuffer> Failed to lock index buffer\n
	0x00C0D912,	//	NiDX9Renderer::Initialize> Invalid device ID specified - FAILING\n
	0x00C0D986,	//	NiDX9Renderer::Initialize> Invalid Adapter specified - FAILING\n
	0x00C0D9E5,	//	NiDX9Renderer::Initialize> Attempting to create rendering device\n
	0x00C0DA46,	//	NiDX9Renderer::Initialize> No device of specified type \n
	0x00C0DBFA,	//	NiDX9Renderer::Initialize> Specified number of backbuffers was invalid - trying fewer backbuffers\n
	0x00C0DCA5,	//	NiDX9Renderer::Initialize> Could not create device of desired type - trying next-lower device type\n
	0x00C0DCCC,	//	NiDX9Renderer::Initialize> Invalid device ID specified - FAILING\n
	0x00C0DE00,	//	NiDX9Renderer::Initialize> Could not initialize texture defaults - FAILING\n
	0x00C0E023,	//	NiDX9Renderer::Initialize> Could not create hardware device of any type - FAILING\n
	0x00C0E42F,	//	NiDX9Renderer::Recreate> Reset notification function failed before device reset - FAILING\n
	0x00C0E5C7,	//	NiDX9Renderer::Recreate> Could not initialize texture defaults - FAILING\n
	0x00C0E82E,	//	NiDX9Renderer::Recreate> Reset notification function failed after device reset - FAILING\n
	0x00C0E89E,	//	NiDX9Renderer::Recreate> Could not reset device to desired parameters - trying to recover original values\n
	0x00C0E8C3,	//	NiDX9Renderer::Recreate> Could not recover original presentation parameters\n
	0x00C0EAB0,	//	PrecacheGeometry> No shader found for object %s!\n
	0x00C0F5AB,	//	AddToBatch> No shader found for object %s!\n
	0x00C0F790,	//	NiDX9Renderer::LostDeviceRestore> Lost device notification function failed\n
	0x00C10CB0,	//	NiDX9Renderer::Create> Invalid device ID specified - FAILING\n
	0x00C11193,	//	NiDX9AdditionalDepthStencilBufferData::Recreate> FAILED %s
	0x00C112D1,	//	NiDX9Renderer::Recreate> Using %s backbuffer format\n
	0x00C11383,	//	NiDX9Renderer::Recreate> Using %s backbuffer format\n
	0x00C1144D,	//	NiDX9Renderer::Recreate> Using %s depth/stencil format\n
	0x00C11637,	//	NiDX9TextureBufferData::Create> FAILED- GetSurfaceLevel - %s
	0x00C11746,	//	NiDX9TextureBufferData::Create> FAILED- GetSurfaceLevel - %s
	0x00C119E9,	//	NiDX9Renderer::Create> Using %s backbuffer format\n
	0x00C11BED,	//	NiDX9Renderer::Create> Using %s backbuffer format\n
	0x00C11D93,	//	NiDX9SwapChainDepthStencilBufferData::Create> Using %s depth/stencil format\n
	0x00C1201C,	//	NiDX9AdditionalDepthStencilBufferData::Create> FAILED %s
	0x00C120C6,	//	NiDX9AdditionalDepthStencilBufferData::Create> FAILED %s
	0x00C12B20,	//	NiDX9SourceTextureData::CreateSurf> Failed CreateImageSurface - %s\n
	0x00C12B65,	//	NiDX9SourceTextureData::CopyDataToSurfaceLevel> Failed GetDesc - %s
	0x00C12B93,	//	NiDX9SourceTextureData::CopyDataToSurfaceLevel> Failed GetSurfaceLevel - %s
	0x00C12DA7,	//	NiDX9SourceTextureData::CreateSurfFromRendererData> FAILED to create texture of format: 0x%x, D3D error: %s
	0x00C12DE5,	//	NiDX9SourceTextureData::CopyRendererDataToSurfaceLevel> Failed GetDesc - %s
	0x00C12E13,	//	NiDX9SourceTextureData::CopyRendererDataToSurfaceLevel> Failed GetSurfaceLevel - %s
	0x00C132FB,	//	NiDX9SourceTextureData::CopyDataToSurface> Failed GetSurfaceLevel (%d)- %s
	0x00C133AA,	//	NiDX9SourceTextureData::CopyRendererDataToSurface> Failed GetSurfaceLevel (%d)- %s
	0x00C13A1B,	//	Failed to find texture %s\n
	0x00C181B0,	//	Failed to create shader program factory!
	0x00C181CA,	//	Failed to create shader program factory!
	0x00C289B3,	//	NiDX9IndexBufferManager::CreateIndexBuffer FAILED
	0x00C28A82,	//	NiDX9IndexBufferManager::CreateIndexBuffer FAILED
	0x00C28A9F,	//	NiDX9IndexBufferManager::PackBuffer> Failed to create index buffer\n
	0x00C28B07,	//	NiDX9IndexBufferManager::PackBuffer> Failed to lock index buffer\n
	0x00C28C09,	//	NiDX9IndexBufferManager::CreateIndexBuffer FAILED
	0x00C28C26,	//	NiDX9IndexBufferManager::ResizeTempPointConnectivity> Failed to create index buffer\n
	0x00C28CBD,	//	NiDX9IndexBufferManager::PackBuffer> Failed to lock index buffer\n
	0x00C28E51,	//	NiDX9IndexBufferManager::ResizeTempLineConnectivity> Failed to create index buffer\n
	0x00C28E6A,	//	NiDX9IndexBufferManager::PackBuffer> Failed to lock index buffer\n
	0x00C296FA,	//	NiDX9RenderedTextureData::CreateSurf> Failed CreateImageSurface - %s
	0x00C297D7,	//	NiDX9RenderedTextureData::Create> Failed QueryInterface IID_IDirect3DTexture9 - %s
	0x00C2982A,	//	NiDX9RenderedTextureData::Create> Failed NiDX92DBufferData::Create - %s
	0x00C298DE,	//	NiDX9SourceCubeMapData::CreateCubeMap> Failed CreateCubeTexture - %s\n
	0x00C29962,	//	NiDX9SourceCubeMapData::CopyDataToSurface> Failed GetSurfaceLevel (%d)- %s
	0x00C299E2,	//	NiDX9SourceCubeMapData::CopyDataToSurface> Failed GetSurfaceLevel (%d)- %s
	0x00C29A36,	//	NiDX9SourceCubeMapData::Create> No Cube Map texture formats supported\n
	0x00C29B0C,	//	Failed to find texture %s\n
	0x00C2A067,	//	NiDX9RenderedCubeMapData::CreateSurf> Failed CreateImageSurface - %s
	0x00C2A1AC,	//	NiDX9RenderedCubeMapData::Create> NiDX9TextureBufferData::CreateCubeFace - %s
	0x00C2A328,	//	NiDX9DynamicTextureData::CreateSurf> Failed CreateImageSurface - %s
	0x00C2A380,	//	NiDX9DynamicTextureData::CreateSurf> Failed CreateImageSurface - %s
	0x00C2A3C0,	//	NiDX9DynamicTextureData Failed> Can not create a tiled dynamic texture.\n
	0x00C2C9ED,	//	Failed to create shader factory!
	0x00C32CFF,	//	NiD3DHLSLPixelShader::SetShaderConstant - %s - variable %s not found\n
	0x00C32F3F,	//	NiD3DHLSLPixelShader::SetShaderConstantArray - %s - variable %s not found\n
	0x00C3323F,	//	NiD3DHLSLVertexShader::SetShaderConstant - %s - variable %s not found\n
	0x00C33474,	//	NiD3DHLSLVertexShader::SetShaderConstantArray - %s - variable %s not found\n
};
