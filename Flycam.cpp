#include "NiTypes.h"
#include "NiObjects.h"
namespace Flycam
{
	char bSkipNextRenderWindowUpdate;
	BOOL __stdcall OnRenderWindowSetCursorPos(int X, int Y)
	{
		// When the cursor reaches the edge of the render window, it is warped back
		// into the window with SetCursorPos(). The camera update hook executes
		// immediately after the warp using the correct mouse delta. However,
		// SetCursorPos() also generates a synthetic WM_MOUSEMOVE message whose
		// delta is based on the warped cursor position, resulting in unintended
		// camera movement. Set a flag so the immediate update proceeds normally
		// and the synthetic follow-up update is ignored.
		bSkipNextRenderWindowUpdate = 2;
		return SetCursorPos(X, Y);
	}

	void __fastcall RenderWindowOnMoveMouseSetTranslate(NiNode* apCameraRoot, void* edx, NiMatrix33* apMatrix)
	{
		if (!bSkipNextRenderWindowUpdate || --bSkipNextRenderWindowUpdate == 1)
		{
			apCameraRoot->m_localRotate = apMatrix;
		}
	}

	class NiMatrix3;
	NiMatrix3* __fastcall OnFlycamCalculateMatrixFromEuler(NiMatrix3* apMatrix, void* edx, float afYaw, float afPitch, float afRoll)
	{
		constexpr float maxPitch = 1.55334f; // ~89 degrees
		if (afRoll > maxPitch)
		{
			afRoll = maxPitch;
		}
		else if (afRoll < -maxPitch)
		{
			afRoll = -maxPitch;
		}

		return ThisCall<NiMatrix3*>(0x80A8F0, apMatrix, afYaw, afPitch, afRoll);
	}

	// hooks before movement speed is determined for flycam mode
	int FlycamMovementSpeedMultiplier()
	{
		Setting* fFlyMoveSpeed = (Setting*)0xED12BC;
		fFlyMoveSpeed->data.f = config.fFlycamNormalMovementSpeed;

		if (GetAsyncKeyState(VK_SHIFT) < 0)
		{
			fFlyMoveSpeed->data.f *= config.fFlycamShiftMovementSpeed;
		}

		if (GetAsyncKeyState(VK_MENU) < 0)
		{
			fFlyMoveSpeed->data.f *= config.fFlycamAltMovementSpeed;
		}

		/* fix flycam speed being dependent on framerate by slowing down movement if framerate is greater than 30fps (33ms/frame)*/
		static DWORD uiLastFlycamTime = 0;
		DWORD uiCurrentTick = GetTickCount();
		int iDelta = uiCurrentTick - uiLastFlycamTime;

		if (iDelta < 33)
		{
			fFlyMoveSpeed->data.f *= (float)iDelta / 33.0f;
		}
		uiLastFlycamTime = uiCurrentTick;

		return 0;
	}

	// edi       : difference in x pos, 
	// esp+0x324 : difference in y pos
	// Hook to make flycam rotation speed based on x/y position difference rather than 1.0 for any difference
	__HOOK FlycamRotationSpeedMultiplierHook()
	{
		static const UInt32 retnAddr = 0x45D3F8;
		_asm
		{
			mov dword ptr ds : [0x00ED140C] , ebx

			push edi
			fild dword ptr ss : [esp]
			add esp, 4

			fmul config.fFlycamRotationSpeed
			fstp dword ptr ss : [esp + 0xF0]

			fild dword ptr ss : [esp + 0x324]
			fmul config.fFlycamRotationSpeed
			fstp dword ptr ss : [esp + 0x18]

			jmp retnAddr
		}
	}

	
	/* check if Q or E are held and modify the Z movement speed (stored in esp+0x2C) before it is passed to the view transform */
	__HOOK RenderWindowFlycamPreTransformMovementHook()
	{
		static const UInt32 retnAddr = 0x455DB1;
		_asm
		{
			push 'Q'
			call esi
			test ax, 0x8000
			jz noQ

			fld  dword ptr ss : [esp + 0x2C]
			fsub dword ptr ds : [0xED12C0]
			fstp dword ptr ss : [esp + 0x2C]
		noQ:
			push 'E'
			call esi
			test ax, 0x8000
			jz noE

			fld dword ptr ss : [esp + 0x2C]
			fadd dword ptr ds : [0xED12C0]
			fstp dword ptr ss : [esp + 0x2C]
		noE:
			mov esi, dword ptr ds : [0xED116C]
			jmp retnAddr
		}
	}

	__HOOK RenderWindowFlycamPostTransformMovementHook()
	{
		static const UInt32 retnAddr = 0x455DCE;
		_asm
		{
			push eax
			push VK_LCONTROL
			call dword ptr ds : [0xD234D8] // GetAsyncKeyState
			test ax, 0x8000
			jz noDown

			pop eax
			fld  dword ptr ss : [eax + 8]
			fsub dword ptr ds : [0xED12C0]
			fstp dword ptr ss : [eax + 8]
			push eax

		noDown:
			push VK_SPACE
			call dword ptr ds : [0xD234D8] // GetAsyncKeyState
			test ax, 0x8000
			jz noUp

			pop eax
			fld  dword ptr ss : [eax + 8]
			fadd dword ptr ds : [0xED12C0]
			fstp dword ptr ss : [eax + 8]
			push eax

		noUp:
			pop eax
			mov edx, dword ptr ss : [eax]
			mov ecx, dword ptr ss : [eax + 4]
			jmp retnAddr
		}
	}

	void Init()
	{
		// make flycam mouse rotation grid smaller (1*1 instead of 2*2)
		// the game was checking if the difference in mouse position was > 1 rather than >= 1
		SafeWrite8(0x45D3AD, 0x7C);
		SafeWrite8(0x45D3BD, 0x7F);
		SafeWrite8(0x45D3D9, 0x7C);
		SafeWrite8(0x45D3EA, 0x7F);

		// ignore mouse movement after the call to SetCursorPos when reaching the edge of the window to prevent it flickering
		WriteRelCall(0x45D387, UInt32(OnRenderWindowSetCursorPos));
		SafeWrite8(0x45D387 + 5, 0x90);
		WriteRelCall(0x45D49D, UInt32(RenderWindowOnMoveMouseSetTranslate));

		// fix being able to look upside down by clamping the roll
		WriteRelCall(0x45D487, UInt32(OnFlycamCalculateMatrixFromEuler));

		// add shift and alt scaling flycam movement speed, and fix speed being framerate dependent
		WriteRelCall(0x455D12, UInt32(FlycamMovementSpeedMultiplier));

		if (config.bSmoothFlycamRotation)
		{
			WriteRelJump(0x45D3A3, UInt32(FlycamRotationSpeedMultiplierHook));
			WriteRelJump(0x456A9F, 0x456AE6); // skip call to SetCursorPos when changing to flycam mode
		}

		// add Q/E keys for up/down movement in flycam mode for render window
		// if the movment is applied Post transform, it will be relative to the world
		if (config.bFlycamUpDownRelativeToWorld)
		{
			WriteRelJump(0x455DC9, UInt32(RenderWindowFlycamPostTransformMovementHook));
		}
		else
		{
			WriteRelJump(0x455DAB, UInt32(RenderWindowFlycamPreTransformMovementHook));
		}
	}
}