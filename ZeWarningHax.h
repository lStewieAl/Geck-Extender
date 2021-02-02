#pragma once

/*
//	Messages to fix/make more verbose

0x004AF7DB,	//	MASTERFILE: Invalid LOCK extra found in extra data list.
0x004AF88E,	//	MASTERFILE: Invalid TELEPORT extra found in extra data list.
0x004AFF52,	//	No occlusion plane for occlusion plane ref extra.
0x004B000E,	//	MASTERFILE: Unable to find portal linked reference %08X. Portal linked reference data will be removed.
0x004B023A,	//	MASTERFILE: Removing empty decal extra.
0x004B0482,	//	MASTERFILE: Removing empty reflector water extra.
0x004B04BA,	//	MASTERFILE: Removing empty lit water extra.
0x004BADDA,	//	MASTERFILE: Could not find linked door (%08X) in teleport data init.
0x004BAE57,	//	MASTERFILE: Linked door '%s' (%08X) in teleport data has no parent cell.
0x004ECC3C,	//	Invalid karma value found. Unable to fix.
0x0065A222,	//	MASTERFILE: Random teleport doors cannot have preexisting teleport data. Teleport data will be removed.
0x0065A24C,	//	MASTERFILE: Random door has no teleport marker.
0x0066E8DD,	//	MASTERFILE: Unable to load EffectItem '%s' in Actor Effect '%s'
0x0066E968,	//	MASTERFILE: Trying to load condition data with no effect item in Actor Effect '%s'.
0x0064FBA4,	//	Patrol point event script for point %s has text but has not been compiled.

//	Messages to fix/make more verbose and add () to %08X

0x0066E926,	//	MASTERFILE: Unable to find EffectSetting %d in Actor Effect '%s'. <- can't find form id for this one easily
0x004AD024,	//	FORMS: Reference %s %08X has no parent save cell
0x004AF84A,	//	MASTERFILE: Unable to find key %08X for lock data. Lock will be removed.
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
0x004B5042,	//	FORMS: Form %08X no longer has an override, yet I'm trying to set it to %s
0x004D8B17,	//	FORMS: Addon Node %08X \"%s\" is trying to use index %i, but Addon Node %08X \"%s\" already uses that index.  Addon Node %08X \"%s\" will be remapped to a new index.
0x004DBE40,	//	FORMS: Attempting to use existing form %s '%s' %08X as parent form for following child forms.
0x004E5BFB,	//	MASTERFILE: Old Destruction data found on form %08X %s.  It needs to be resaved.
0x004FC904,	//	FORMS: SetFormID bashing entry in form ID map at %08X.\n\nForm basher: type %s_ID\n\nBashed form: %s
0x0054D0F1,	//	DEFAULT: Could not checkout form %08X during import
0x0054E11A,	//	DEFAULT: Bad Filter form %08X %s sent to BGSEntryPointPerkEntry::CheckConditionFilters.
0x00568CAE,	//	DEFAULT: Could not find faction rank #%i during import of faction %08X.
0x00568D17,	//	DEFAULT: Could not find faction %08X during import.
0x0057DB9F,	//	QUESTS: Could not find stage index %i in quest %08X during import.
0x0057DBC9,	//	QUESTS: Could not find stage item #%i during import of stage %i in quest %08X.
0x0057DC60,	//	QUESTS: Could not find quest %08X during import.
0x0057DF78,	//	FORMS: Could not check out form %08X during import
0x0057DFA4,	//	QUESTS: Could not find objective index %i in quest %08X during import.
0x0057DFB4,	//	QUESTS: Could not find quest %08X during import.
0x00594282,	//	DIALOGUE: Could not find response #%i during import of topic %08X.
0x00594292,	//	DIALOGUE: Could not find topic %08X during import.
0x005949AA,	//	FORMS: Info %08X under topic %08X no longer exists
0x00594A08,	//	MASTERFILE: Could not find named topic %08X during import.
0x005D3BC2,	//	MASTERFILE: Water type %08X could not be found on PlaceableWater %s (%08X)
0x005F1F60,	//	MODELS: LOD Mesh '%s' for object %08X '%s' could not be found.
0x005F799F,	//	DEFAULT: Could not checkout form %08X during import
0x00635159,	//	Reference %08X (%s) in cell %08X (%s) should not be refracted by auto water; the flag was reset.
0x006351EF,	//	Reference %08X (%s) in cell %08X (%s) should not be refracted by auto water; however, the flag could not be reset, as the reference is checked out by %s.
0x0064D6F3,	//	FORMS: Could not checkout form %08X.
0x00659A2F,	//	No collision object for placeable water object %08X.
0x00659A5A,	//	No shape for placeable water object %08X's collision object.
0x00659DC8,	//	MODELS: LOD Mesh '%s' for reference %08X '%s' in cell '%s' could not be found.
0x00659E05,	//	MODELS: LOD Mesh '%s' for reference %08X '%s' in cell '%s' (%d, %d) could not be found.
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
0x006AEC8B,	//	NavMesh: Door reference %08X is not found but is linked to %s.  Please finalize to resolve this.
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
*/

static const char* messageCreateFileMapping = "This operation was not completed because CreateFileMapping failed to open file \"%s\" with error:\n%s\n\nExit other applications, restart the Construction Kit, and try again.";
static const char* messageCouldNotCheckoutForm = "FORMS: Could not checkout form (%08X) during import.";
static const char* messageBodyPartDataNotFound = "MASTERFILE: Could not find body part data (%08X) during import.";
static const char* messageAmmoNotFound = "MASTERFILE: Could not find ammo (%08X) during import.";
static const char* messageNavmeshTriangleDegenerate = "PATHFINDING: Navmesh (%08X) Cell %s, Triangle %d is degenerate, Vertices 1 and 2 both use vertex index %d";
static const char* messageMediaLocationController = "MASTERFILE: Could not find Media Location Controller (%08X) for Audio Marker (%08X) in extra data list.";
static const char* messagePotentiallyInvalidZValue = "CELLS: Potentially Invalid Z value (%.2f) on reference:\n%s";
static const char* messagePotentiallyInvalidXValue = "CELLS: Potentially Invalid X value (%.2f) on reference:\n%s";
static const char* messagePotentiallyInvalidYValue = "CELLS: Potentially Invalid Y value (%.2f) on reference:\n%s";

__declspec(naked) void hk_UnableToFindEnableStateParentMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Unable to find enable state parent %08X. Enable state parent data will be removed in ref (%08X).";
	static const UInt32 kRetnAddr = 0x004AFD59;

	__asm
	{
		mov		eax, dword ptr ds : [ebp + 0x0C]
		push	eax
		mov		eax, dword ptr ss : [esp + 0x44]
		push	eax
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_UnableToFindPackageReferenceMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Unable to find Package Target Reference (%08X) on owner object \"%s\" (%08X).";
	static const UInt32 kRetnAddr = 0x004A1687;

	__asm
	{
		mov		ecx, dword ptr ds : [ebp + 0x0C]
		push	ecx
		mov		ecx, dword ptr ss : [esp + 0x08]
		push	eax
		push	ecx
		push	messageFormat
		jmp		kRetnAddr
	}
}
__declspec(naked) void hk_UnableToFindPackageLocationMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Unable to find Package Location Reference (%08X) on owner object \"%s\" (%08X).";
	static const UInt32 kCallAddr = (UInt32)EditorUI_Log2;
	static const UInt32 kRetnAddr = 0x0049FCFB;

	__asm
	{
		mov		ecx, dword ptr ds : [ebp + 0x0C]
		push	ecx
		mov		ecx, dword ptr ss : [esp + 0x0C]
		push	eax
		push	ecx
		push	messageFormat
		mov		eax, kCallAddr
		call	eax
		mov		byte ptr ds : [edi] , 0
		add		esp, 0x10
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_TryingToAccessLocalVariableInScriptMsgHook()
{
	static const char* messageFormat = "SCRIPTS: Trying to access local variable %d in script '%s' (%08X) -- variable not found.\r\n";
	static const UInt32 kRetnAddr = 0x005C2852;
	static uint32_t kTemp;

	__asm
	{
		mov		kTemp, eax
		mov		eax, dword ptr ds : [ecx + 0x0C]
		push	eax
		mov		eax, kTemp
		push	eax
		push	esi
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_UnableToFindLinkedReferenceMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Unable to find linked reference (%08X). Linked reference data will be removed in ref (%08X).";
	static const UInt32 kRetnAddr = 0x004AFE7F;

	__asm
	{
		mov		edx, dword ptr ds : [ebp + 0x0C]
		push	edx
		mov		edx, dword ptr ss : [esp + 0x4C]
		push	edx
		push	messageFormat
		jmp		kRetnAddr
	}
}
__declspec(naked) void hk_CouldNotFindMediaLocationControllerMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Could not find MediaLocationController (%08X) for AudioMarker (%08X) in extra data list.";
	static const UInt32 kRetnAddr = 0x004AF98C;
	static uint32_t kTemp;

	__asm
	{
		mov		kTemp, edx
		mov		edx, dword ptr ds : [ebp + 0x0C]
		push	edx
		mov		edx, kTemp
		push	edx
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_ChunkAbnormallyTerminatedMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Chunk %c%c%c%c abnormally terminated TESObjectREFR::Load for ref (%08X).\n";
	static const UInt32 kRetnAddr = 0x0064FF84;

	__asm
	{
		mov		ecx, dword ptr ds : [edi + 0x0C]
		push	ecx
		mov		ecx, eax
		shr		ecx, 0x18
		push	ecx
		mov		edx, eax
		mov		ecx, eax
		sar		edx, 0x10
		and edx, 0xFF
		push	edx
		sar		ecx, 0x08
		and ecx, 0xFF
		push	ecx
		and eax, 0xFF
		push	eax
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_OnPackageBeginScriptNotCompiledMsgHook()
{
	static const char* messageFormat = "MASTERFILE: %s (%08X)";
	static const UInt32 kCallAddr = 0x00407120;
	static const UInt32 kRetnAddr = 0x00496F3C;

	__asm
	{
		mov		eax, kCallAddr
		call	eax
		mov		ebx, dword ptr ds : [esi + 0x0C]
		push	ebx
		mov		ebx, dword ptr ss : [ebp + 0x58]
		push	ebx
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_OnPackageChangeScriptNotCompiledMsgHook()
{
	static const char* messageFormat = "MASTERFILE: %s (%08X)";
	static const UInt32 kCallAddr = 0x00407120;
	static const UInt32 kRetnAddr = 0x00496F79;

	__asm
	{
		mov		eax, kCallAddr
		call	eax
		mov		ebx, dword ptr ds : [esi + 0x0C]
		push	ebx
		mov		ebx, dword ptr ss : [ebp + 0x58]
		push	ebx
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_OnPackageEndScriptNotCompiledMsgHook()
{
	static const char* messageFormat = "MASTERFILE: %s (%08X)";
	static const UInt32 kCallAddr = 0x00407120;
	static const UInt32 kRetnAddr = 0x00496FB3;

	__asm
	{
		mov		eax, kCallAddr
		call	eax
		mov		ebx, dword ptr ds : [esi + 0x0C]
		push	ebx
		mov		ebx, dword ptr ss : [ebp + 0x58]
		push	ebx
		push	messageFormat
		jmp		kRetnAddr
	}
}


__declspec(naked) void hk_ScriptHasTextButNotCompiledMsgHook()
{
	static const char* messageFormat = "SCRIPTS: %s (%X08)";
	static const UInt32 kCallAddr = 0x00407120;
	static const UInt32 kRetnAddr = 0x005C36F1;

	__asm
	{
		mov		eax, kCallAddr
		call	eax
		mov		eax, [ebx + 0x0C]
		push	eax
		mov		ecx, dword ptr ss : [ebp - 0x20]
		push	ecx
		push	messageFormat
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_RemoveEmptyLitWaterMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Removing empty lit water extra data in ref (%08X).";
	static const UInt32 kRetnAddr = 0x004B044A;

	__asm
	{
		mov		ecx, [esp + 0x80]
		push	eax
		mov		eax, messageFormat
		push	eax
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_EnableStateParentLoopMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Enable state parent loop detected in ref (%08X). Parent removed.";
	static const UInt32 kRetnAddr = 0x004AFDB7;
	static uint32_t kTemp;

	__asm
	{
		mov		kTemp, ecx
		mov		ecx, [esp + 0x04]
		mov		ecx, [ecx + 0x0C]
		push	ecx
		mov		eax, messageFormat
		push	eax
		mov		ecx, kTemp
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_RemoveEmptyActivateParentMsgHook()
{
	static const char* messageFormat = "MASTERFILE: Removing empty activate parent extra data in ref (%08X).";
	static const UInt32 kRetnAddr = 0x004B0214;
	static uint32_t kTemp;

	__asm
	{
		mov		kTemp, ecx
		mov		ecx, [esp + 0x04]
		mov		ecx, [ecx + 0x0C]
		push	ecx
		mov		eax, messageFormat
		push	eax
		mov		ecx, kTemp
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_UnableToFindLeveledObjectMsgHook()
{
	static const char* messageFormat = "Unable to find Leveled Object Form (%08X) for owner object \"%s\" (%08X).";
	static const UInt32 kRetnAddr = 0x005004BB;
	static uint32_t kTemp;

	__asm
	{
		mov		kTemp, eax
		mov		eax, [ebx + 0x0C]
		push	eax
		mov		eax, kTemp
		push	eax
		push	ecx
		mov		eax, messageFormat
		push	eax
		jmp		kRetnAddr
	}
}

void DoScriptErrorWarning(char* errorMsg) {
	EditorUI_Log2(errorMsg);
	if (bScriptCompileWarningPopup) {
		MessageBoxExA(NULL, errorMsg, "Script Compile Error", MB_ICONWARNING, NULL);
	}
}

__declspec(naked) void hk_EnableScriptErrorsMsgHook()
{
	static const UInt32 kRetnAddr = 0x005C57E7;
	__asm
	{
		pushad

		push	eax
		call	DoScriptErrorWarning
		add		esp, 4

		popad
		mov		dword ptr ss : [esp + 0x0120] , 0xFFFFFFFF
		jmp		kRetnAddr
	}
}

__declspec(naked) void hk_UnableToFindPortalLinkedMsgHook()
{
	static const UInt32 retnAddr = 0x4B000E;
	static const char* msg = "MASTERFILE: Unable to find portal linked reference %08X for ref %08X. Portal linked reference data will be removed.";
	_asm
	{
		push [ebp + 0xC] // form->refID
		push msg
		jmp retnAddr
	}
}

__declspec(naked) void hk_RemovingEmptyReflectorWaterMsgHook()
{
	static const UInt32 retnAddr = 0x4B0482;
	static const char* msg = "MASTERFILE: Removing empty reflector water extra for ref %08X.";
	_asm
	{
		push [ebp + 0xC] // form->refID
		push msg
		jmp retnAddr
	}
}




void WriteErrorMessageHooks() {
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

	if (bNoFactionReactionMessage) {
		//	DEFAULT: (Faction Reaction Error) Faction '%s' (%08X) is a Friend or Ally of Faction '%s' (%08X), but Faction '%s' (%08X) is not a Friend or Ally of Faction '%s' (%08X). 
		XUtil::PatchMemoryNop(0x00568979, 5);
	}

	//	patch script editor messages - credit to roy
	WriteRelJump(0x005C57E2, (UInt32)hk_EnableScriptErrorsMsgHook);
	XUtil::PatchMemoryNop(0x005C57E7, 0x06);

	WriteRelJump(0x4B0009, UInt32(hk_UnableToFindPortalLinkedMsgHook));
	SafeWrite8(0x4B0019, 0xC); // add esp, 8 => add esp, 0xC for extra pushed arg

	WriteRelJump(0x4B047D, UInt32(hk_RemovingEmptyReflectorWaterMsgHook));
	SafeWrite8(0x4B0489, 0x8); // pop extra arg
}