#pragma once

#include "GameData.h"
#include "GameObjects.h"
#include "NiObjects.h"
#include <GameSettings.h>


namespace MultiBoundsAdder {

#define PRINT_FAILS 1

	void PrintFail(const char* apReason, TESObjectREFR* apRef, TESObjectREFR* apMBRef) {
#if PRINT_FAILS
		Console_Print("Can't add ref %08X to MultiBound %08X - %s", apRef->refID, apMBRef->refID, apReason);
#endif
	}
	
	bool TestVertices(NiAVObject* apTestedObject, BSMultiBound* apMultiBound, BOOL& arResult, bool abFastExit) {
		return CdeclCall<bool>(0x643350, apTestedObject, apMultiBound, &arResult, abFastExit);
	}

	void TestCell(TESObjectCELL* apCell) {
		Console_Print("Testing cell %08X", apCell->refID);

		const TESBoundObject* const pMultiBoundMarker = *(TESBoundObject**)0xEDDA40;
		const TESBoundObject* const pPortalMarker = *(TESBoundObject**)0xEDDA4C;
		const TESBoundObject* const pRoomMarker = *(TESBoundObject**)0xEDDA48;

		if (!apCell->objectList.Count()) {
			Console_Print("No loaded objects in the cell!");
			return;
		}

		bool bModified = false;

		apCell->CellRefLockEnter();
		std::vector<TESObjectREFR*> kValidRefs;
		kValidRefs.reserve(apCell->objectList.Count());

		Console_Print("Evaluating %d objects", apCell->objectList.Count());
		auto pIter = apCell->objectList.Head();
		while (pIter) {
			TESObjectREFR* pRef = pIter->data;
			pIter = pIter->next;

			TESForm* pBase = pRef->baseForm;
			if (!pBase)
				continue;

			if (pRef->refID == 0)
				break;

			bool bCanBeAttached = true;
			switch (pBase->typeID) {
			case kFormType_TESNPC:
			case kFormType_TESCreature:
			case kFormType_TESLevCreature:
			case kFormType_TESLevCharacter:
			case kFormType_BGSProjectile:
			case kFormType_BGSExplosion:
				bCanBeAttached = false;
				break;
			}

			if (bCanBeAttached)
				bCanBeAttached = pBase != pPortalMarker && pBase != pRoomMarker && pBase != pMultiBoundMarker;

			if (bCanBeAttached)
				bCanBeAttached = pRef->Get3D() != nullptr;

			if (bCanBeAttached)
				bCanBeAttached = pRef->extraDataList.GetMultiBoundRef() == nullptr;

			if (!bCanBeAttached)
				continue;

			kValidRefs.push_back(pRef);
		}

		Console_Print("Found %d candidates", kValidRefs.size());
		Console_Print("Evaluating them against %d MultiBounds", apCell->renderData->multiboundRefMap.numItems);
		UInt32 uiAdded = 0;
		for (TESObjectREFR* pTestedRef : kValidRefs) {
			NiTMapIterator kIter = apCell->renderData->multiboundRefMap.GetFirstPos();
			while (kIter) {
				TESObjectREFR* pMBRef = nullptr;
				NiPointer<BSMultiBoundNode> spMBNode = nullptr;
				apCell->renderData->multiboundRefMap.GetNext(kIter, pMBRef, spMBNode);

				if (!pMBRef || !spMBNode)
					continue;

				if (!TESObjectREFR::CanSetMultibound(pMBRef, pTestedRef)) {
					PrintFail("Not allowed", pTestedRef, pMBRef);
					continue;
				}

				// Simple position test
				if (!pTestedRef->IsInMultiBound(spMBNode->spMultiBound)) {
					PrintFail("Position outside of the bound", pTestedRef, pMBRef);
					continue;
				}

				NiNode* pRoot = pTestedRef->Get3D();
				if (!pRoot) {
					PrintFail("No 3D data", pTestedRef, pMBRef);
					continue;
				}

				// Simple bound test
				if (pTestedRef->CheckBound(spMBNode->spMultiBound)) {
					PrintFail("Bound outside of the bound", pTestedRef, pMBRef);
					continue;
				}

				// Vertex test
				BOOL uResult = TRUE;
				if (!TestVertices(pRoot, spMBNode->spMultiBound, uResult, true)) {
					PrintFail("Geometry outside of the bound", pTestedRef, pMBRef);
					continue;
				}

				pTestedRef->AddMultiBound(pMBRef);
				pTestedRef->MarkAsModified(true);
				pMBRef->MarkAsModified(true);
				bModified = true;
				Console_Print("Adding ref %08X to MultiBound %08X", pTestedRef->refID, pMBRef->refID);

				pTestedRef->flags |= 0x80000000;

				uiAdded++;
				break;
			}
		}

		if (bModified)
			apCell->MarkAsModified(true);

		if (uiAdded)
			Console_Print("Added %d objects to MultiBounds", uiAdded);
		
		apCell->CellRefLockLeave();
	}

	void TestObjects() {
		TESObjectCELL* pCell = TES::GetSingleton()->currentInterior;
		if (pCell) {
			TestCell(pCell);
		}
		else if (TES::GetSingleton()->currentWrldspc) {
#if 1
			Setting* uGridsToLoad = (Setting*)0xED6550;
			for (UInt32 x = 0; x < uGridsToLoad->data.i; x++) {
				for (UInt32 y = 0; y < uGridsToLoad->data.i; y++) {
					TESObjectCELL* pCell = TES::GetSingleton()->gridCellArray->GetCell(x, y);
					if (!pCell || pCell->cellState != TESObjectCELL::CS_ATTACHED)
						continue;
					
					TestCell(pCell);
				}
			}
#else
			TESWorldSpace* pWorld = TES::GetSingleton()->currentWrldspc;
			NiTMapIterator kIter = pWorld->cellMap->GetFirstPos();
			while (kIter) {
				UInt32 uiCoords = 0;
				TESObjectCELL* pCell = nullptr;
				pWorld->cellMap->GetNext(kIter, uiCoords, pCell);
				if (!pCell)
					continue;

				TESObjectCELL* pLoadedCell = nullptr;
				if (!TES::GetSingleton()->IsCellLoaded(pCell, false)) {
					SInt32 iX = pCell->GetPosX();
					SInt32 iY = pCell->GetPosY();
					pLoadedCell = DataHandler::GetSingleton()->GetCellFromCellCoord(iX, iY, pWorld, true);
					ThisCall(0x6656D0, pWorld, pLoadedCell);
				}

				TestCell(pCell);

				if (pLoadedCell) {
					DataHandler::GetSingleton()->UnloadCell(pLoadedCell);
				}
			}
#endif
		}
	}

}