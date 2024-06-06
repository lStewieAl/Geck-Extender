#pragma once

#include "nvse/GameData.h"
#include "nvse/GameForms.h"

namespace ONAMFix 
{
	static NiTLargeArray<TESForm*>** const pModifiedForms = (NiTLargeArray<TESForm*>**)0xED56D0;

	static bool IsMaster(ModInfo* apFile) 
	{
		// If file is newly created, the master flag would not be yet set, that's why checking the extension takes priority
		// (The flag is set after this patch is called)
		const char* pFileName = apFile->name;
		if (!pFileName)
		{
			return false;
		}

		const char* pDot = strrchr(pFileName, '.');
		// This is not possible to happen, but just in case
		if (!pDot)
		{
			return false;
		}

		const char* pExt = pDot + 1;
		if (!_stricmp(pExt, "esm"))
		{
			return true;
		}

		// ESP plugin could be flagged as master - flag is more important here
		return apFile->IsMaster();
	}

	static void CreateONAM(ModInfo* apFile) 
	{
		// Only master files have ONAM
		// All forms in ESPs are persistent, so they don't need it
		// Technically we could add ONAM to ESPs, but it would just be a waste of space
		if (!IsMaster(apFile))
		{
			return;
		}

		if (apFile->pONAM_Data) 
		{
			FormHeap_Free(apFile->pONAM_Data);
			apFile->pONAM_Data = nullptr;
			apFile->uiONAM_Size = 0;
		}

		auto pModified = *pModifiedForms;
		if (!pModified->numObjs)
		{
			return;
		}

		std::vector<UInt32> kFormIDs;
		for (UInt32 i = 0; i < pModified->firstFreeEntry; i++) 
		{
			const TESForm* pForm = pModified->Get(i);

			if (!pForm)
			{
				continue;
			}

			UInt32 uiTypeID = pForm->typeID;
			bool bAdd = false;
			switch (uiTypeID) 
			{
			case kFormType_TESObjectREFR:
			case kFormType_MissileProjectile:
			case kFormType_GrenadeProjectile:
			case kFormType_BeamProjectile:
			case kFormType_FlameProjectile:
			case kFormType_ContinuousBeamProjectile:
				// Only temporary references are added to ONAM
				if (!pForm->IsPersistent())
				{
					bAdd = true;
				}
				break;
			case kFormType_TESObjectLAND:
			case kFormType_NavMesh:
				bAdd = true;
				break;
			}

			if (!bAdd)
			{
				continue;
			}

			// Skip references that come from our file
			// Only overridden references are added to ONAM
			if (pForm->mods.Head()->item == apFile)
			{
				continue;
			}


#if 0
			Console_Print("Adding form %08X to %s's ONAM", pForm->refID, apFile->name);
#endif
			kFormIDs.push_back(pForm->refID);
		}

		const UInt32 uiFormCount = kFormIDs.size();
		if (uiFormCount)
		{
			UInt32 uiDataSize = uiFormCount * sizeof(UInt32);
			UInt32* pONAM = (UInt32*)FormHeap_Allocate(uiDataSize);
			memcpy(pONAM, kFormIDs.data(), uiDataSize);

			apFile->pONAM_Data = pONAM;
			apFile->uiONAM_Size = uiFormCount;
		}
		else
		{
			apFile->pONAM_Data = nullptr;
			apFile->uiONAM_Size = 0;
		}
	}


	static UInt32 __fastcall SavePlugin(ModInfo* apThis) {
		CreateONAM(apThis);
		return ThisCall<UInt32>(0x4E1450, apThis);
	}

	static void InitHooks() {
		WriteRelCall(0x4D9CEA, UInt32(SavePlugin));
	}
}