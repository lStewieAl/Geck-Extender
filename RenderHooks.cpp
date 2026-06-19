#include "GameData.h"
#include "GameObjects.h"
extern double havokAnimationRate;
namespace RenderHooks
{
	__HOOK FixMultiBounds()
	{
		__asm
		{
			cmp eax, 0x7F7FFFFF
			je AhhGary

			mov ecx, dword ptr ds : [eax + 0xC]
			mov dword ptr ds : [ecx + 0x8], 0
		AhhGary:
			ret
		}
	}

	float GetRefSpeedMultiplier()
	{
		float multiplier = 1.0F;
		if (GetAsyncKeyState(VK_SHIFT) < 0)
		{
			multiplier *= config.fMovementShiftMultiplier;
		}
		if (GetAsyncKeyState(VK_MENU) < 0)
		{
			multiplier *= config.fMovementAltMultiplier;
		}
		return multiplier;
	}

	char __cdecl hk_DoRenderPan(int a1, int a2, float a3)
	{
		if (GetAsyncKeyState(VK_MENU) < 0)
		{
			a3 *= config.fMovementAltMultiplier;
		}
		return CdeclCall<char>(0x464210, a1, a2, a3);
	}

	char __cdecl hk_DoRenderMousewheelScroll(int a1, int a2, float a3)
	{
		return CdeclCall<char>(0x464210, a1, a2, a3 * GetRefSpeedMultiplier());
	}

	double __fastcall hk_CalculateVectorMagnitude(float* vector)
	{
		return ThisCall<float>(0x40B3D0, vector) * GetRefSpeedMultiplier();
	}

	/* multiply the reference movement speed dependent */
	__HOOK RenderWindowReferenceMovementSpeedHook()
	{
		static const UInt32 retnAddr = 0x455398;
		_asm
		{
			pushad
			call GetRefSpeedMultiplier
			fmul
			popad

			//	originalCode:
			fmul dword ptr ds : [0xECFD1C]
			jmp retnAddr
		}
	}

	float GetOrthoSpeedMultiplier()
	{
		if (GetAsyncKeyState(VK_MENU) < 0)
		{
			return config.fMovementAltMultiplier;
		}
		return 1.0F;
	}

	__HOOK hk_OrthographicZoom()
	{
		static const UInt32 retnAddr = 0x45F667;
		_asm
		{
			pushad
			call GetOrthoSpeedMultiplier
			fmul
			popad

			//	originalCode
			fmul dword ptr ds : [0xD2E0D0]
			jmp retnAddr
		}
	}

	__HOOK hk_OrthographicZoom2()
	{
		static const UInt32 retnAddr = 0x4602DE;
		_asm
		{
			fild dword ptr ss : [esp + 0x24]
			fmul dword ptr ds : [eax]

			pushad
			call GetOrthoSpeedMultiplier
			fmul
			popad

			jmp retnAddr
		}
	}

	__HOOK hk_RefCameraRotation()
	{
		static const UInt32 retnAddr = 0x45F601;
		_asm {
			fld dword ptr[eax]

			pushad
			call GetOrthoSpeedMultiplier
			fmul
			popad

			push ecx
			fstp[esp]
			jmp retnAddr
		}
	}

	__HOOK RenderWindowHandlesRefRotationHook()
	{
		static const UInt32 retnAddr = 0x4523D0;
		_asm
		{
			call GetRefSpeedMultiplier
			fild dword ptr ss : [esp + 0x114]
			fmul
			test byte ptr ds : [0xECFCEC], 0x2
			jmp retnAddr
		}
	}


	const double M_TAU = 6.28318530717958647692528676;
	float __fastcall CalculateRefRotation(signed int amount)
	{
		const double pi_on_180 = *(double*)0xD2C320;;
		float rotationSpeedMult = *(float*)0xECFD10;
		float* currentRotation = (float*)0xED1164;
		int iSnapToAngle = *(int*)0xECFD04;

		double snapRadians = iSnapToAngle * pi_on_180;
		double addend = (amount * rotationSpeedMult / 10.0);
		*currentRotation += addend;

		// computes how many "SnapRadians" the rotation should be scaled to
		int truncatedSnapRadians = *currentRotation / snapRadians;

		double rotation = snapRadians * truncatedSnapRadians;
		if (rotation == 0.0)
		{
			return 0;
		}

		*currentRotation = 0;
		while (rotation >= M_TAU) rotation -= M_TAU;
		while (rotation < 0) rotation += M_TAU;

		return rotation;
	}

	__HOOK RenderWindowHandleRefRotationHook()
	{
		static const UInt32 retnAddr = 0x4524A2;
		_asm
		{
			mov ecx, [esp + 0x114] // rotation amount
			call CalculateRefRotation
			fstp[esp + 0x14]
			jmp retnAddr
		}
	}

	void HideRenderWindow(HWND hWnd)
	{
		ShowWindow(hWnd, SW_HIDE);
		CheckMenuItem(MainWindow::GetMenu(), 0x9D06, MF_UNCHECKED);
		WritePrivateProfileString("Windows", "bHideRenderWindow", " 1", IniPath);
	}

	WNDPROC originalRenderWindowCallback;
	BOOL CALLBACK RenderWindowCallbackHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE || wParam == VK_LWIN) {
				SetFlycamMode(0);
			}
			break;
		case WM_RBUTTONDOWN:
			SetFlycamMode(0);
			break;
		case 0x416:
			// Disabling landscape editing
			if (wParam == 0) {
				RenderWindow::ResetLandscapePainting();
			}
			break;
		case WM_INITDIALOG:
		{
			AddMinimizeAndCloseButtons(hWnd);
			break;
		}
		case WM_CLOSE:
			HideRenderWindow(hWnd);
			return TRUE;
		case WM_SYSCOMMAND:
			if ((wParam & 0xFFF0) == SC_MINIMIZE)
			{
				HideRenderWindow(hWnd);
				return TRUE;
			}
			break;
		}

		LRESULT res = CallWindowProc(originalRenderWindowCallback, hWnd, msg, wParam, lParam);

		if (msg == 0x416u && wParam == 0 && config.bRenderWindowRefreshAfterLandscapeEdit) {
			// Refresh render window after landscape editing disabled.
			CallWindowProc(originalRenderWindowCallback, hWnd, WM_KEYDOWN, VK_F5, lParam);
		}

		return res;
	}

	UInt32 originalCreateReferenceAtLocationFn_leveled;
	TESObjectREFR* __fastcall OnRenderWindowDragDropSupportLeveled(DataHandler* apDataHandler, void* edx, TESBoundObject* apObject, NiPoint3* arLocation, NiPoint3* arDirection, BOOL abCalcLocation, struct BGSPrimitive* apAddPrimitive, void* apAdditionalData)
	{
		if (apObject->typeID == kFormType_TESLevItem || apObject->typeID == kFormType_TESLevCharacter || apObject->typeID == kFormType_TESLevCreature)
		{
			auto levItem = (TESLevItem*)apObject;
			TESBoundObject* newForm = nullptr;

			// pick a random item from the list, this will be null if it's a nested list
			if (auto item = levItem->list.list.GetRandomItem())
			{
				if (item->form)
				{
					if (auto boundObj = CdeclCall<TESBoundObject*>(0xC5D114, item->form, 0, 0xE8C57C, 0xE8C7D4, 0)) // DYNAMIC_CAST(TESForm, TESBoundObject))
					{
						newForm = boundObj;
					}
				}
			}

			// use calculate formlist if it's a nested list - note this can return no new form since it's chance based...
			if (!newForm)
			{
				auto container = TESContainer::Create();

				// try finding a random item 100 times... jank workaround for when the chance sometimes returns null
				int attempts = 0;
				do
				{
					levItem->list.CalculateCurrentFormList(100, 1, container, false);
					if (auto data = container->formCountList.Head()->data)
					{
						if (data->form)
						{
							if (auto boundObj = CdeclCall<TESBoundObject*>(0xC5D114, data->form, 0, 0xE8C57C, 0xE8C7D4, 0)) // DYNAMIC_CAST(TESForm, TESBoundObject))
							{
								newForm = boundObj;
							}
						}
					}
					container->Destroy();
				} while (!newForm && ++attempts < 100);


				FormHeap_Free(container);
			}

			if (newForm)
			{
				apObject = newForm;
			}
		}
		return ThisCall<TESObjectREFR*>(originalCreateReferenceAtLocationFn_leveled, apDataHandler, apObject, arLocation, arDirection, abCalcLocation, apAddPrimitive, apAdditionalData);
	}

	UInt32 originalCreateReferenceAtLocationFn_coplanar;
	TESObjectREFR* __fastcall OnRenderWindowDragDropSupportCoplanar(DataHandler* apDataHandler, void* edx, TESBoundObject* apObject, NiPoint3& arLocation, NiPoint3& arDirection, BOOL abCalcLocation, struct BGSPrimitive* apAddPrimitive, void* apAdditionalData)
	{
		if (config.bCoplanarRefDrops)
		{
			auto pTES = TES::GetSingleton();
			auto pPick = RenderWindow::GetPick();
			pPick->SetTarget(pTES->sceneGraphObjectRoot);
			if (pPick->PickObjects(arLocation, arDirection))
			{
				if (pPick->m_pickResults.numObjs > 0)
				{
					abCalcLocation = false;
					const auto& pIntersectionPoint = pPick->m_pickResults.data[0]->m_intersect;
					if (pTES->pInteriorCell == nullptr)
					{
						TESWorldSpace* pWorldSpace = pTES->pWorldSpace;
						if (pWorldSpace == nullptr || DataHandler::GetSingleton()->GetCellFromCellCoord(arLocation.x, arLocation.y, pWorldSpace) == nullptr)
						{
							// invalid cell coords, use the original position
							abCalcLocation = true;
						}
					}

					if (!abCalcLocation)
					{
						arLocation = pIntersectionPoint;
						arDirection *= 0.f;
					}
				}
			}
		}
		return ThisCall<TESObjectREFR*>(originalCreateReferenceAtLocationFn_coplanar, apDataHandler, apObject, &arLocation, &arDirection, abCalcLocation, apAddPrimitive, apAdditionalData);
	}

	bool OnMoveRefCheckXYZHeld()
	{
		bool* isXHeld = (bool*)0xED1413;
		bool* isYHeld = (bool*)0xED1411;
		bool* isZHeld = (bool*)0xED1410;
		if (GetAsyncKeyState(VK_CONTROL) >= 0)
		{
			if (!*isXHeld)
			{
				if (*isXHeld = GetAsyncKeyState('X') < 0)
				{
					*(float*)0xED1164 = 0;
				}
			}
			if (!*isYHeld)
			{
				if (*isYHeld = GetAsyncKeyState(config.bSwapRenderCYKeys ? 'C' : 'Y') < 0)
				{
					*(float*)0xED1164 = 0;
				}
			}
			if (!*isZHeld)
			{
				if (*isZHeld = GetAsyncKeyState('Z') < 0)
				{
					*(float*)0xED1164 = 0;
				}
			}
		}
		return *isXHeld;
	}

	__HOOK EditLandCheckLandIsNull()
	{
		static const UInt32 retnAddr = 0x61CA60;
		_asm
		{
			mov dword ptr ss : [esp + 0x4], ebx
			test eax, eax
			je done

			cmp dword ptr ds : [eax], 0
		done:
			jmp retnAddr
		}
	}

	UInt32 __fastcall PlaceOPALObjectHook(ObjectPalette::Object* current, void* edx, float* point1, float* point2)
	{
		ObjectPalette::Object* toPlace = current;

		UInt32 randomIndex = -1;
		if (auto hWnd = ObjectPalette::GetWindow())
		{
			auto listView = GetDlgItem(hWnd, ObjectPalette::kListView);
			int selectedCount = ListView_GetSelectedCount(listView);
			if (selectedCount > 1)
			{
				randomIndex = rand() % selectedCount;
				unsigned int index = SendMessageA(listView, LVM_GETNEXTITEM, 0xFFFFFFFF, LVNI_SELECTED);

				if (randomIndex)
				{
					do
					{
						auto startIndex = index;
						index = SendMessageA(listView, LVM_GETNEXTITEM, startIndex, LVNI_SELECTED);
					} while (index != -1 && --randomIndex);
				}

				auto randomItem = (ObjectPalette::Object*)GetNthListItem(listView, index);
				if (randomIndex == 0 && randomItem) // if we've successfully looped and found an item
				{
					toPlace = randomItem;
				}
			}
		}

		if (randomIndex != 0 && config.bObjectPaletteAllowRandom)
		{
			if ((GetAsyncKeyState(VK_CAPITAL) < 0) ^ config.bObjectPaletteRandomByDefault)
			{
				ObjectPalette* opal = ObjectPalette::GetSingleton();

				randomIndex = rand() % opal->list.Count();
				toPlace = opal->list.GetNthItem(randomIndex);
			}
		}

		return ThisCall(0x40BF90, toPlace, point1, point2);
	}

	__HOOK NiTreeCtrl_CreateTreeRecursiveHook()
	{
		_asm
		{
		loopIter:
			mov ecx, [eax + 8]
			test ecx, ecx
			je skip

			ret

		skip:
			mov eax, [eax]
			test eax, eax
			jne loopIter

			pop eax
			mov eax, 0x53013E
			jmp eax
		}
	}

	__HOOK NiTreeCtrl_CreateTreeRecursiveHook2()
	{
		_asm
		{
			test edi, edi
			je skip

			mov eax, [edi]
			ret

		skip:
			pop eax
			mov ebp, [ebp]
			mov eax, 0x5304FD
			jmp eax
		}
	}

	__HOOK NiAVObject_GetViewerStringsHook()
	{
		_asm
		{
			test edi, edi
			je skip

			mov eax, [edi]
			ret

		skip:
			pop eax
			mov ebp, [ebp]
			mov eax, 0x80F13A
			jmp eax
		}
	}

	class TESRenderControl;
	UInt32 TESPreviewControl__SetTimeAddr;
	void __fastcall TESPreviewControl__SetTime(TESRenderControl* renderControl, void* edx, float afTime, char abIsPaused)
	{
		if (renderControl == *(TESRenderControl**)0xECECE4) // if this is the havok preview window
		{
			if (auto pRef = *(TESObjectREFR**)0xECECE0)
			{
				if (auto pNode = pRef->Get3D())
				{
					if (CdeclCall<bool>(0x40F8A0, 0xF2CD84, pNode)) // NiIsKindOf(BSMasterParticleSystem::ms_RTTI, pNode)
					{
						// For most object types, afTime represents the animation timeline offset (i.e. an absolute position).
						// For BSMasterParticleSystem however, it is treated as a delta time (time elapsed since the last frame).
						// Since the render window runs at a fixed 60fps, use a constant 16.67ms delta instead.
						afTime = (1000.0F / 60) / havokAnimationRate;
					}
				}
			}
		}
		ThisCall(TESPreviewControl__SetTimeAddr, renderControl, afTime, abIsPaused);
	}

	void __cdecl OnHavokPreviewSetup(TESForm* form)
	{
		if (form)
		{
			if (form->IsBoundObject())
			{
				// open the havok preview directly on the TESBoundObject
				CdeclCall(0x4102F0, form);
			}
			else
			{
				const char* modelPath = CdeclCall<const char*>(0x501B20, form);
				if (modelPath && *modelPath)
				{
					// call a method that creates a TESObjectSTAT and sets the mesh path
					CdeclCall(0x4105C0, modelPath);
				}
			}
		}
	}

	bool __fastcall IsBoundObjectOrHasModelPath(TESForm* form)
	{
		if (form->IsBoundObject())
		{
			return true;
		}

		const char* modelPath = CdeclCall<const char*>(0x501B20, form);
		return modelPath && *modelPath;
	}

	TESForm* __cdecl OnSelectObjectCheckIsBoundObject(TESForm* form)
	{
		if (IsBoundObjectOrHasModelPath(form))
		{
			return form;
		}
		return nullptr;
	}

	void Init()
	{
		//	stop geck crash with bUseMultibounds = 0 in exterior cells with multibounds - credit to roy
		WriteRelCall(0x4CA48F, (UInt32)FixMultiBounds); // add missing null check to TES::ResetAllMultiBoundNodes
		XUtil::PatchMemoryNop(0x4CA494, 0x5);

		// uncap framerate on render window - credit to shademe
		if (config.bRenderWindowUncap)
		{
			SafeWrite8(0x0045911B, 0x0A);
		}

		// uncap framerate on preview window - credit to shademe
		if (config.bPreviewWindowUncap)
		{
			SafeWrite8(0x004100E5, 0x0A);
		}

		if (config.bSwapRenderCYKeys)
		{
			// set C as hotkey for restricting movement to Y direction
			SafeWrite8(0x462D8B, 0x6); // (patch a switch table offset)
			SafeWrite8(0x462F5B, 0xF); // (patch a switch table offset)

			// remove Y as hotkey for Y movement
			SafeWrite8(0x462DA1, 0x8);
			SafeWrite8(0x462F71, 0x17);

			// allow Y as a hotkey in render window preferences
			SafeWrite8(0x4136C1, 'Z');
		}

		if (config.bUseAltShiftMultipliers)
		{
			// Scroll wheel and pan speed affected by shift/alt
			WriteRelCall(0x48B8C5, UInt32(hk_DoRenderMousewheelScroll)); // preview window
			WriteRelCall(0x48B74C, UInt32(hk_DoRenderMousewheelScroll)); // preview window
			WriteRelCall(0x48B7AC, UInt32(hk_DoRenderMousewheelScroll)); // preview window pan
			WriteRelCall(0x46040E, UInt32(hk_DoRenderMousewheelScroll)); // render window scroll

			// reference movement speed affected by shift/alt
			WriteRelJump(0x455392, UInt32(RenderWindowReferenceMovementSpeedHook));

			// render window panning, doesn't apply shift multiplier as it conflicts with keybinding
			WriteRelCall(0x45F7E5, UInt32(hk_DoRenderPan));
			WriteRelCall(0x45F846, UInt32(hk_DoRenderPan));

			// orthographic zoom 
			WriteRelJump(0x45F661, UInt32(hk_OrthographicZoom));
			WriteRelJump(0x4602D8, UInt32(hk_OrthographicZoom2));

			// camera rotation with a selected ref
			WriteRelJump(0x45F5FB, UInt32(hk_RefCameraRotation));

			// ref rotation
			WriteRelJump(0x4523C2, UInt32(RenderWindowHandlesRefRotationHook));
		}

		if (config.bSnapToGridRotationUseDoublePrecision)
		{
			// use double precision when calculating reference rotation to fix floating point errors
			WriteRelJump(0x4523E2, UInt32(RenderWindowHandleRefRotationHook));
		}

		// add support for dragging leveled items onto render window
		SafeWrite8(0x463282 + kFormType_TESLevItem, 1);
		SafeWrite8(0x463282 + kFormType_TESLevCreature, 1);
		SafeWrite8(0x463282 + kFormType_TESLevCharacter, 1);
		originalCreateReferenceAtLocationFn_leveled = DetourRelCall(0x46193B, UInt32(OnRenderWindowDragDropSupportLeveled));
		originalRenderWindowCallback = (WNDPROC)DetourVtable(0x4411A1, UInt32(RenderWindowCallbackHook));

		// add coplanar support
		originalCreateReferenceAtLocationFn_coplanar = DetourRelCall(0x46193B, UInt32(OnRenderWindowDragDropSupportCoplanar));

		// check if x/y/z are held while you move refs to fix the axis lock keys not working after closing a menu
		WriteRelCall(0x45F490, UInt32(OnMoveRefCheckXYZHeld));

		// fix null pointer with checking edges when using landscape editor
		WriteRelJump(0x61CA59, UInt32(EditLandCheckLandIsNull));

		if (config.bAllowEditLandEdges)
		{
			// changes: for(int i = 1; i < uGridsToLoad - 1; ++i);
			// to :		for(int i = 0; i < uGridsToLoad; ++i);

			// north
			SafeWrite8(0x459B93, 0x90);
			SafeWrite8(0x45DADC, 0x90); // flattening 
			SafeWrite8(0x45DEFB, 0); // texture painter

			// east 
			SafeWrite8(0x459B79, 0x90);
			SafeWrite8(0x45DAB9, 0x90); // flattening 
			SafeWrite8(0x45DF84, 0x90);	// texture painter

			// south
			SafeWrite8(0x459AF5, 0);
			SafeWrite8(0x45DA30, 0); // flattening
			SafeWrite8(0x45DF21, 0); // texture painter

			// west
			SafeWrite8(0x459B16, 0);
			SafeWrite8(0x45DA56, 0); // flattening
			SafeWrite8(0x45DFA5, 0x90); // texture painter
		}

		// add modifier CAPSLOCK for placing a random object from the objects palette, or select randomly if multiple items are highlighted
		WriteRelCall(0x45A6B8, UInt32(PlaceOPALObjectHook));

		// always show imposters
		SafeWrite8(0x65688D, 0xEB);

		// add null checks to fix the F10 ShowSceneGraph
		WriteRelCall(0x530126, UInt32(NiTreeCtrl_CreateTreeRecursiveHook));
		WriteRelCall(0x530508, UInt32(NiTreeCtrl_CreateTreeRecursiveHook2));
		WriteRelCall(0x80F145, UInt32(NiAVObject_GetViewerStringsHook));

		// fix havok preview anim timer increasing for non-anims
		TESPreviewControl__SetTimeAddr = DetourVtable(0xD38DE0, UInt32(TESPreviewControl__SetTime));

		// allow opening any form that has a model in the preview (e.g. Impact Datas)
		SafeWrite8(0x44B243, 0xB8);
		SafeWrite32(0x44B243 + 1, UInt32(IsBoundObjectOrHasModelPath));
		WriteRelCall(0x44A782, UInt32(OnSelectObjectCheckIsBoundObject));
		SafeWrite8(0x44B243 + 1 + 4, 0x90);
		WriteRelCall(0x44BB8E, UInt32(OnHavokPreviewSetup));
		WriteRelCall(0x44A793, UInt32(OnHavokPreviewSetup));

		// fix lights not updating when dragged unless a multibound is in the cell (thanks Pr0bability)
		SafeWrite16(0x90C136, 0x9066); // remove check that the node is the root in ShadowSceneNode::UpdateQueuedLight 
	}
}