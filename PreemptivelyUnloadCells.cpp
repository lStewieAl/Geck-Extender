#include "GECKUtility.h"
#include "GameData.h"
#include "Settings.h"

namespace PreemptivelyUnloadCells
{
	bool __fastcall OnLoadInterior_HasBufferedInteriorCell(TES* apTES, void* edx, TESObjectCELL* apCell)
	{
		bool bHasBufferedInteriorCell = ThisCall<bool>(0x4C7DF0, apTES, apCell);
		if (bHasBufferedInteriorCell)
		{
			return true;
		}

		if (config.bPreemptivelyUnloadCells)
		{
			if (apTES->pInteriorCell)
			{
				apTES->DetachGridCells();
			}
			if (apTES->FreeExteriorBuffer(true, nullptr))
			{
				apTES->CleanUpUnusedTextures();
			}
		}
		return false;
	}

	void __fastcall OnSetWorldSpace_DetachCells(TES* apTES, TESWorldSpace* apNewWorldSpace)
	{
		apTES->DetachGridCells();

		if (config.bPreemptivelyUnloadCells)
		{
			int iFreed = apTES->FreeExteriorBuffer(true, apTES->pWorldSpace);
			if (!apTES->pInteriorCell)
			{
				iFreed += apTES->FreeInteriorBuffer(true);
			}
			if (iFreed)
			{
				apTES->CleanUpUnusedTextures();
			}
		}

		apTES->pWorldSpace = apNewWorldSpace;
	}

	__HOOK OnSetWorldSpace_DetachCellsHook()
	{
		_asm
		{
			mov edx, edi
			jmp OnSetWorldSpace_DetachCells
		}
	}

	void OnUpdateCurrentGridCell(TES* apTES)
	{
		if (config.bPreemptivelyUnloadCells)
		{
			if (apTES->FreeInteriorBuffer(true))
			{
				apTES->CleanUpUnusedTextures();
			}
		}
	}

	__HOOK OnUpdateCurrentGridCellHook()
	{
		_asm
		{
			mov ecx, esi
			call OnUpdateCurrentGridCell
			cmp dword ptr ds : [esi + 0x88], 0
			ret
		}
	}

	void Init()
	{
		SafeWriteBuf(0x4C9F01, "\x90\x90\x90\x90\x90\x90", 6); // set the worldspace in our hook instead so we keep the pointer to the previous one
		WriteRelCall(0x4C9F01 + 6, UInt32(OnSetWorldSpace_DetachCellsHook));// TES::SetWorldSpace
		
		// TES::UpdateCurrentGridCell (0x4CCC5C) - we likely don't need to hook here, it's related to BackgroundCellLoads
		WriteRelCall(0x4CCD49, UInt32(OnUpdateCurrentGridCellHook)); // TES::UpdateCurrentGridCell
		SafeWrite16(0x4CCD49 + 5, 0x9066);

		WriteRelCall(0x4CB0E3, UInt32(OnLoadInterior_HasBufferedInteriorCell)); // TES::LoadInterior
	}
}