#pragma once

#include "GameData.h"
#include "GameObjects.h"
#include "GameTasks.h"
#include "NiObjects.h"
#include <GameSettings.h>
#include <array>
#include <algorithm>


namespace MultiBoundsAdder {

	namespace {
#define PRINT_FAILS 0
		static void RunGarbageCollection() {
			CdeclCall(0x9DEC60, false); // NiObjectGarbage::ClearAll
			ModelLoader::GetSingleton()->ClearUnusedModels();
			TES::GetSingleton()->CleanUpUnusedTextures(true);
			StdCall(0x81AF10); // NiGlobalStringTable::FreeUnusedStrings
		}

		static void LoadRefs(TESObjectCELL* apCell) {
			apCell->CellRefLockEnter();
			auto pIter = apCell->objectList.Head();
			while (pIter) {
				TESObjectREFR* pRef = pIter->data;
				pIter = pIter->next;

				if (!pRef)
					continue;

				if (pRef->IsActor())
					continue;

				if (pRef->Get3D())
					continue;

				// Deleted
				if ((pRef->flags & 0x20) != 0)
					continue;

				// Disabled
				if ((pRef->flags & 0x800) != 0)
					continue;

				if (pRef->IsMultiBoundOrRoomMarker())
					continue;

				const char* pPath = pRef->GetEquippableModelPath();
				if (!pPath)
					continue;

				//Console_Print("Loading 3D data for %08X - %s", pRef->refID, pPath);
				NiNode* pNode = ModelLoader::GetSingleton()->LoadFile(pPath, 0, false, 0, false, false);
				if (pNode) {
					pRef->Set3DSimple(pNode);
					pNode->m_kLocal.translate = *pRef->GetPos();
					pNode->m_kLocal.scale = pRef->scale;
					pRef->GetOrientation(pNode->m_kLocal.rotate);
					NiUpdateData kUpdateData;
					pNode->Update(kUpdateData);
				}
			}
			apCell->CellRefLockLeave();
		}

		static TESObjectCELL* LoadCell(SInt32 iX, SInt32 iY, TESWorldSpace* apWorld) {
			TESObjectCELL* pLoadedCell = DataHandler::GetSingleton()->GetCellFromCellCoord(iX, iY, apWorld, true);
			ThisCall(0x6656D0, apWorld, pLoadedCell);
			pLoadedCell->Load3D();
			pLoadedCell->AddObjectsToLoadedRefCollection();
			LoadRefs(pLoadedCell);
			return pLoadedCell;
		}

		static TESObjectCELL* LoadCell(TESObjectCELL* apCell) {
			apCell->Load3D();
			apCell->AddObjectsToLoadedRefCollection();
			LoadRefs(apCell);
			return apCell;
		}

		static void UnloadCell(TESObjectCELL* apCell) {
			DataHandler::GetSingleton()->UnloadCell(apCell);
		}

		void PrintFail(const char* apReason, const TESObjectREFR* apRef, const TESObjectREFR* apMBRef) {
#if PRINT_FAILS
			Console_Print("Can't add ref %s (%08X) to MultiBound %08X - %s", apRef->GetEditorID(), apRef->refID, apMBRef->refID, apReason);
#endif
		}

		void PrintFail(const char* apReason, const TESObjectREFR* apRef) {
#if PRINT_FAILS
			Console_Print("Ignoring ref %s (%08X) - %s", apRef->GetEditorID(), apRef->refID, apReason);
#endif
		}

		bool TestVertices(NiAVObject* apTestedObject, BSMultiBound* apMultiBound, BOOL& arResult, bool abFastExit) {
			return CdeclCall<bool>(0x643350, apTestedObject, apMultiBound, &arResult, abFastExit);
		}

		UInt32 TestMultibound(TESObjectREFR* apMBRef, BSMultiBound* apMultiBound, std::vector<TESObjectREFR*>& arRefList) {
			UInt32 uiCount = 0;
			for (TESObjectREFR* pTestedRef : arRefList) {
				if (!TESObjectREFR::CanSetMultibound(apMBRef, pTestedRef)) {
					PrintFail("Not allowed", pTestedRef, apMBRef);
					continue;
				}

				// Simple position test
				if (!pTestedRef->IsInMultiBound(apMultiBound)) {
					PrintFail("Position outside of the bound", pTestedRef, apMBRef);
					continue;
				}

				NiNode* pRoot = pTestedRef->Get3D();
				if (!pRoot) {
					PrintFail("No 3D data", pTestedRef, apMBRef);
					continue;
				}

#if 0
				// Simple bound test
				if (pTestedRef->CheckBound(apMultiBound)) {
					PrintFail("Bound outside of the bound", pTestedRef, apMBRef);
					continue;
				}
#endif

				// Vertex test
				BOOL uResult = TRUE;
				if (!TestVertices(pRoot, apMultiBound, uResult, true)) {
					PrintFail("Geometry outside of the bound", pTestedRef, apMBRef);
					continue;
				}

				pTestedRef->AddMultiBound(apMBRef);
				pTestedRef->MarkAsModified(true);
				apMBRef->MarkAsModified(true);

				Console_Print("Adding %s (%08X) to MultiBound %08X", pTestedRef->GetEditorID(), pTestedRef->refID, apMBRef->refID);

				pTestedRef->flags |= 0x80000000;

				uiCount++;
			}

			return uiCount;
		}

		void EvaluateReferences(const TESObjectREFR* apTestedRef, std::vector<TESObjectREFR*>& arRefList) {
			TESForm* pBase = apTestedRef->baseForm;
			if (!pBase)
				return;

			if (apTestedRef->refID == 0)
				return;

			switch (pBase->typeID) {
			case kFormType_TESNPC:
			case kFormType_TESCreature:
			case kFormType_TESLevCreature:
			case kFormType_TESLevCharacter:
			case kFormType_BGSProjectile:
			case kFormType_BGSExplosion:
				PrintFail("Disallowed form type", apTestedRef);
				return;
			}

			if (apTestedRef->IsMultiBoundOrRoomMarker()) {
				PrintFail("Is a MultiBound", apTestedRef);
				return;
			}

			if (!apTestedRef->Get3D()) {
				PrintFail("No 3D data", apTestedRef);
				return;
			}

			if (apTestedRef->extraDataList.GetMultiBoundRef()) {
				PrintFail("Already in a MultiBound", apTestedRef);
				return;
			}

			arRefList.push_back(const_cast<TESObjectREFR*>(apTestedRef));
		}

		void GatherReferences(TESObjectREFR* apMBRef, BSMultiBound* apMultiBound, TESWorldSpace* apWorld, std::vector<TESObjectREFR*>& arRefList) {
			if (!apWorld)
				return;

			double dRadius = apMultiBound->spShape->GetRadius();

			NiPoint3 kWorldPos = *apMBRef->GetPos();

			Console_Print("Testing MultiBound %s (%08X) at %f,%f", apMBRef->GetEditorID(), apMBRef->refID, kWorldPos.x, kWorldPos.y);

			// Create a square around the MultiBound
			struct IntPoint2 {
				SInt32 x;
				SInt32 y;
			};

			IntPoint2 kPoints[4];
			kPoints[0].x = SInt32(kWorldPos.x - dRadius) >> 12;
			kPoints[0].y = SInt32(kWorldPos.y - dRadius) >> 12;
			kPoints[1].x = SInt32(kWorldPos.x + dRadius) >> 12;
			kPoints[1].y = SInt32(kWorldPos.y - dRadius) >> 12;
			kPoints[2].x = SInt32(kWorldPos.x + dRadius) >> 12;
			kPoints[2].y = SInt32(kWorldPos.y + dRadius) >> 12;
			kPoints[3].x = SInt32(kWorldPos.x - dRadius) >> 12;
			kPoints[3].y = SInt32(kWorldPos.y + dRadius) >> 12;

			// Calculate the size of the square
			SInt32 iMinX = kPoints[0].x;
			SInt32 iMaxX = kPoints[0].x;
			SInt32 iMinY = kPoints[0].y;
			SInt32 iMaxY = kPoints[0].y;

			for (UInt32 i = 1; i < 4; i++) {
				if (kPoints[i].x < iMinX)
					iMinX = kPoints[i].x;
				if (kPoints[i].x > iMaxX)
					iMaxX = kPoints[i].x;

				if (kPoints[i].y < iMinY)
					iMinY = kPoints[i].y;
				if (kPoints[i].y > iMaxY)
					iMaxY = kPoints[i].y;
			}

			Console_Print("Testing range (%d, %d) x (%d, %d)", iMinX, iMaxX, iMinY, iMaxY);

			// Get the cells in the square
			std::vector<TESObjectCELL*> kCells;
			for (SInt32 iX = iMinX; iX <= iMaxX; iX++) {
				for (SInt32 iY = iMinY; iY <= iMaxY; iY++) {
					TESObjectCELL* pCell = DataHandler::GetSingleton()->GetCellFromCellCoord(iX, iY, apWorld, true);
					if (pCell)
						kCells.push_back(pCell);
				}
			}


			std::sort(kCells.begin(), kCells.end());
			auto new_end = std::unique(kCells.begin(), kCells.end());
			kCells.erase(new_end, kCells.end());

			for (TESObjectCELL* pCell : kCells) {
				if (!pCell)
					continue;

				//Console_Print("> Testing cell %08X", pCell->refID);

				bool bLoaded = true;
				if (!TES::GetSingleton()->IsCellLoaded(pCell, false)) {
					SInt32 iX = pCell->GetPosX();
					SInt32 iY = pCell->GetPosY();
					pCell = LoadCell(iX, iY, apWorld);
					bLoaded = false;
				}

				if (pCell->objectList.IsEmpty() || !pCell->renderData->multiboundRefMap.numItems) {
					if (!bLoaded)
						UnloadCell(pCell);

					continue;
				}

				pCell->CellRefLockEnter();

				auto pIter = pCell->objectList.Head();
				while (pIter) {
					TESObjectREFR* pRef = pIter->data;
					pIter = pIter->next;

					EvaluateReferences(pRef, arRefList);
				}

				if (!bLoaded)
					UnloadCell(pCell);

				pCell->CellRefLockLeave();
			}
		}

		void TestCell(TESObjectCELL* apCell, TESWorldSpace* apWorld) {
			if (!apCell->renderData) {
				Console_Print("No data in the cell!");
				return;
			}

			if (!apCell->renderData->multiboundRefMap.numItems) {
				return;
			}

			if (apCell->objectList.IsEmpty()) {
				return;
			}

			Console_Print("Testing cell %s (%08X)", apCell->GetEditorID(), apCell->refID);

			bool bModified = false;

			apCell->CellRefLockEnter();
			std::vector<TESObjectREFR*> kValidRefs;
			UInt32 uiCount = apCell->objectList.Count();
			kValidRefs.reserve(uiCount);


			if (!apWorld) {
				Console_Print("Evaluating %d objects", uiCount);
				auto pIter = apCell->objectList.Head();
				while (pIter) {
					TESObjectREFR* pRef = pIter->data;
					pIter = pIter->next;

					EvaluateReferences(pRef, kValidRefs);
				}
				Console_Print("Found %d candidates", kValidRefs.size());

				if (kValidRefs.empty()) {
					apCell->CellRefLockLeave();
					return;
				}
			}


			UInt32 uiAdded = 0;
			NiTMapIterator kIter = apCell->renderData->multiboundRefMap.GetFirstPos();
			if (apWorld) {
				while (kIter) {
					TESObjectREFR* pMBRef = nullptr;
					NiPointer<BSMultiBoundNode> spMBNode = nullptr;
					apCell->renderData->multiboundRefMap.GetNext(kIter, pMBRef, spMBNode);

					GatherReferences(pMBRef, spMBNode->spMultiBound.data, apWorld, kValidRefs);
				}
				Console_Print("Found %d candidates", kValidRefs.size());

				Console_Print("Evaluating them against %d MultiBounds", apCell->renderData->multiboundRefMap.numItems);
				kIter = apCell->renderData->multiboundRefMap.GetFirstPos();
				while (kIter) {
					TESObjectREFR* pMBRef = nullptr;
					NiPointer<BSMultiBoundNode> spMBNode = nullptr;
					apCell->renderData->multiboundRefMap.GetNext(kIter, pMBRef, spMBNode);

					uiAdded += TestMultibound(pMBRef, spMBNode->spMultiBound, kValidRefs);
				}
			}
			else {
				Console_Print("Evaluating them against %d MultiBounds", apCell->renderData->multiboundRefMap.numItems);
				while (kIter) {
					TESObjectREFR* pMBRef = nullptr;
					NiPointer<BSMultiBoundNode> spMBNode = nullptr;
					apCell->renderData->multiboundRefMap.GetNext(kIter, pMBRef, spMBNode);

					uiAdded += TestMultibound(pMBRef, spMBNode->spMultiBound, kValidRefs);
				}
			}


			if (uiAdded) {
				apCell->MarkAsModified(true);
				Console_Print("Added %d objects to MultiBounds", uiAdded);
			}

			apCell->CellRefLockLeave();
		}

		void RemoveMultiBounds(TESObjectCELL* apCell) {
			if (apCell->objectList.IsEmpty())
				return;

			apCell->CellRefLockEnter();

			auto pIter = apCell->objectList.Head();
			while (pIter) {
				TESObjectREFR* pRef = pIter->data;
				pIter = pIter->next;
				if (!pRef)
					continue;

				if (pRef->extraDataList.GetMultiBoundRef()) {
					pRef->SetMultiBound(nullptr);
					pRef->MarkAsModified(true);
				}
			}

			apCell->CellRefLockLeave();
		}
		
		TESObjectREFR* __fastcall TESDataHandler__CreateReferenceAtLocation_(DataHandler* apThis, void*, TESBoundObject* apForm, NiPoint3* aPos, NiPoint3* aRot, float radius, void* apPrimitive, int a7) {
			TESObjectREFR* pRet = ThisCall<TESObjectREFR*>(0x4D0940, apThis, apForm, aPos, aRot, radius, apPrimitive, a7);
			pRet->parentCell->AddMultiBoundRef(pRet);
			return pRet;
		}
	}

	static void TestObjects(bool abReattach) {
		TESObjectCELL* pCell = TES::GetSingleton()->currentInterior;
		if (pCell) {
			if (abReattach)
				RemoveMultiBounds(pCell);
			TestCell(pCell, nullptr);

			RunGarbageCollection();
		}
		else if (TES::GetSingleton()->currentWrldspc) {
#if 0
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

			UInt32 uiCellsProcessed = 0;
			// Remove all MultiBounds in a separate loop, since if we were doing it in one pass, chances are we could remove MBs we have set ourselves
			if (abReattach) {
				NiTMapIterator kIter = pWorld->cellMap->GetFirstPos();
				while (kIter) {
					UInt32 uiCoords = 0;
					TESObjectCELL* pCell = nullptr;
					pWorld->cellMap->GetNext(kIter, uiCoords, pCell);
					if (!pCell)
						continue;

					RemoveMultiBounds(pCell);
				}
			}

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
					pLoadedCell = LoadCell(iX, iY, pWorld);
					pCell = pLoadedCell;
				}

				if (abReattach)
					RemoveMultiBounds(pCell);

				TestCell(pCell, pWorld);

				if (pLoadedCell) {
					UnloadCell(pLoadedCell);
				}

				uiCellsProcessed++;

				if (uiCellsProcessed >= 8) {
					uiCellsProcessed = 0;
					RunGarbageCollection();
				}
			}
#endif
		}
		else {
			UInt32 uiCellsProcessed = 0;
			for (UInt32 i = 0; i < DataHandler::GetSingleton()->cellArray.numObjs; i++) {
				TESObjectCELL* pCell = DataHandler::GetSingleton()->cellArray.Get(i);
				if (!pCell)
					continue;

				LoadCell(pCell);

				TestCell(pCell, nullptr);

				UnloadCell(pCell);

				if (uiCellsProcessed >= 8) {
					uiCellsProcessed = 0;
					RunGarbageCollection();
				}
			}
		}
	}

	inline void InitHooks() {
		// Register the multibound ref in the cell during its creation (don't ask...)
		WriteRelCall(0x45BC2A, UInt32(TESDataHandler__CreateReferenceAtLocation_));
	}
}