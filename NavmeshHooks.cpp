#include "GameData.h"
namespace NavmeshHooks
{
	const char* NavmeshConfirmMessage = "Are you sure you want to find cover edges?";
	__HOOK RenderWindowNavMeshConfirmFindCover()
	{
		static const UInt32 skipAddr = 0x462C7A;
		_asm
		{
			push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
			push 0xD2D1F4 // "Navmesh: Find Cover"
			push NavmeshConfirmMessage
			push 0
			call MessageBoxA
			cmp eax, IDNO
			je skip

			push 0
			mov ecx, 0xF073F8
			push 0x456F2E // retnAddr
			mov eax, 0x6F51B0 // Navmesh::FindCoverEdges
			jmp eax

		skip:
			jmp skipAddr
		}
	}

	__HOOK MainWindowNavMeshConfirmFindCover()
	{
		static const UInt32 skipAddr = 0x44382F;
		_asm
		{
			push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
			push 0xD2D1F4 // "Navmesh: Find Cover"
			push NavmeshConfirmMessage
			push 0
			call MessageBoxA
			cmp eax, IDNO
			je skip

			push 0
			mov ecx, 0xF073F8
			push 0x44451D // retnAddr
			mov eax, 0x6F51B0 // Navmesh::FindCoverEdges
			jmp eax

		skip:
			jmp skipAddr
		}
	}

	__HOOK NavMeshToolbarConfirmFindCover()
	{
		static const UInt32 skipAddr = 0x40AC87;
		_asm
		{
			push MB_TASKMODAL | MB_ICONWARNING | MB_YESNO
			push 0xD2D1F4 // "Navmesh: Find Cover"
			push NavmeshConfirmMessage
			push 0
			call MessageBoxA
			cmp eax, IDNO
			je skip

			//	doNavmesh
			push 0
			mov ecx, 0xF073F8
			push 0x40AC7B // retnAddr
			mov eax, 0x6F51B0 // Navmesh::FindCoverEdges
			jmp eax

		skip:
			jmp skipAddr
		}
	}

	void __fastcall OnMainWindowUndo(TESUndo* apUndo, void* edx, int formal)
	{
		if (NavMeshManager::IsActive())
		{
			NavMeshManager::GetSingleton()->Undo();
		}
		else
		{
			apUndo->Undo();
		}
	}

	void __stdcall OnTAllocZeroMemory(int* pMemory, int size, int numAllocs, void* initFn, void* destroyFn)
	{
		memset(pMemory, 0, 4 * numAllocs);
	}
	
	void __fastcall NavMeshManager__OnMergeVertices(NavMeshManager* navMeshManager)
	{
		if (navMeshManager->HasMultipleNavmeshesSelected())
		{
			if (MessageBoxA(MainWindow::GetWindow(), "You are about to merge Navmesh records which will cause one to be deleted, do you wish to proceed?", "Geck Extender", MB_YESNOCANCEL) != IDYES)
			{
				return;
			}
		}

		ThisCall(0x4267B0, navMeshManager);
	}

	void __fastcall NavMeshManager__OnCreateTriangle(NavMeshManager* navMeshManager)
	{
		if (navMeshManager->HasMultipleNavmeshesSelected())
		{
			if (MessageBoxA(MainWindow::GetWindow(), "You are about to merge Navmesh records which will cause one to be deleted, do you wish to proceed?", "Geck Extender", MB_YESNOCANCEL) != IDYES)
			{
				return;
			}
		}

		ThisCall(0x427650, navMeshManager);
	}

	void __fastcall NavMeshManager__OnCreateQuad(NavMeshManager* navMeshManager, void* edx, char a2)
	{
		if (navMeshManager->HasMultipleNavmeshesSelected())
		{
			if (MessageBoxA(MainWindow::GetWindow(), "You are about to merge Navmesh records which will cause one to be deleted, do you wish to proceed?", "Geck Extender", MB_YESNOCANCEL) != IDYES)
			{
				return;
			}
		}

		ThisCall(0x427770, navMeshManager, a2);
	}

	__HOOK NavMeshManager__PostRenderCellClearPrintHook()
	{
		static const char* EmptyString = "";
		_asm
		{
			push EmptyString
			push 0x3
			mov eax, 0x4657A0 // TESCSMain::WriteToStatusBar
			call eax
			add esp, 0x14
			ret 0xC
		}
	}

	__HOOK NavMeshInfoMap__CheckInfosHook()
	{
		_asm
		{
			test edx, edx
			je skip

			cmp byte ptr ds : [edx + 0x04] , bl
			jne skip
		
			inc esi

		skip:
			mov edx, 0x6ED488
			jmp edx
		}
	}

	UInt32 forceFinalizeNavMeshModIndex = -1;
	void __fastcall NavMeshManager__ShowFinalizeAllNavMeshesPopup(NavMeshManager* navMeshManager)
	{
		if (*config.sForceFinalizeNavMeshModName)
		{
			auto mod = DataHandler::GetSingleton()->LookupModByName(config.sForceFinalizeNavMeshModName);
			if (mod)
			{
				forceFinalizeNavMeshModIndex = mod->modIndex;
			}
			else
			{
				Console_Print("Force Finalize NavMesh: Unable to find mod %s", config.sForceFinalizeNavMeshModName);
			}
		}

		ThisCall(0x426160, navMeshManager);

		forceFinalizeNavMeshModIndex = -1;
	}

	void* __fastcall OnForceFinalizeShouldProcessCell(TESObjectCELL* cell)
	{
		if (forceFinalizeNavMeshModIndex != -1)
		{
			if (cell->modIndex != forceFinalizeNavMeshModIndex)
			{
				return nullptr;
			}
		}
		return cell->navMeshArray;
	}

	int __cdecl OnForceFinalizeShowMessageBox(char* DstBuf, size_t SizeInBytes, char* Format, UInt32 numCellsToFinalize)
	{
		if (forceFinalizeNavMeshModIndex != -1)
		{
			return CdeclCall<int>(0x401190, DstBuf, SizeInBytes, "About to finalize %d cells with navmeshes for file %s. Finalizing and saving the entire worldspace could take a larger amount of time. Continue?", numCellsToFinalize, config.sForceFinalizeNavMeshModName);
		}
		return CdeclCall<int>(0x401190, DstBuf, SizeInBytes, Format, numCellsToFinalize);
	}

	void Init()
	{
		if (config.bNavmeshFindCoverConfirmPrompt)
		{
			WriteRelJump(0x456F22, UInt32(RenderWindowNavMeshConfirmFindCover));
			WriteRelJump(0x444511, UInt32(MainWindowNavMeshConfirmFindCover));
			WriteRelJump(0x40AC6F, UInt32(NavMeshToolbarConfirmFindCover)); // skip 0x40AC87, retn 0x40AC7B
		}

		// fix the undo menu button for NavMesh
		WriteRelCall(0x44104A, UInt32(OnMainWindowUndo));

		SafeWrite16(0x6C07D8, 0x07EB); // fix unnecessary duplicate function call in NavMeshRender::AttachEdge
		WriteRelCall(0x6778C2, UInt32(OnTAllocZeroMemory)); // microoptimize TAlloc::Allocate

		// fix NavMeshRenderer creating a new child slot whenever attaching children
		SafeWrite8(0x6BF799, 1); // sets bFirstAvail to true
		SafeWrite8(0x6C06F7, 1);
		SafeWrite8(0x6C20CF, 1);

		// prevent x/y/z keys moving the mouse when pressed during navmesh mode
		SafeWriteBuf(0x41F3A0, "\xC2\x04\x00", 3);

		// show a warning when merging vertices from different navmeshes
		WriteRelCall(0x40AAA7, UInt32(NavMeshManager__OnMergeVertices));
		WriteRelCall(0x457E23, UInt32(NavMeshManager__OnMergeVertices));
		WriteRelCall(0x40A9FF, UInt32(NavMeshManager__OnCreateTriangle));
		WriteRelCall(0x4288AC, UInt32(NavMeshManager__OnCreateTriangle));
		WriteRelCall(0x4298CE, UInt32(NavMeshManager__OnCreateTriangle));
		WriteRelCall(0x456FBF, UInt32(NavMeshManager__OnCreateTriangle));
		WriteRelCall(0x4288BF, UInt32(NavMeshManager__OnCreateQuad));
		WriteRelCall(0x40AA20, UInt32(NavMeshManager__OnCreateQuad));
		WriteRelCall(0x456FD9, UInt32(NavMeshManager__OnCreateQuad));

		// clear the 'Creating Editor NavMeshes' print when it's done
		WriteRelJump(0x42216C, UInt32(NavMeshManager__PostRenderCellClearPrintHook));
		WriteRelJump(0x4220E2, UInt32(NavMeshManager__PostRenderCellClearPrintHook));

		// fix crash when saving with a single vertex in a cell with no navmeshes
		WriteRelJump(0x6ED482, UInt32(NavMeshInfoMap__CheckInfosHook));

		// add option for only finalizing navmeshes for cells from a specified esm
		WriteRelCall(0x4446E1, UInt32(NavMeshManager__ShowFinalizeAllNavMeshesPopup));
		WriteRelCall(0x426248, UInt32(OnForceFinalizeShouldProcessCell));
		WriteRelCall(0x4262CF, UInt32(OnForceFinalizeShowMessageBox));
	}
}