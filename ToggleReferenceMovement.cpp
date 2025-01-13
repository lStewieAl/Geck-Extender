#include "Utilities.h"
namespace ToggleReferenceMovement
{
	bool bReferenceMovementAllowed = true;
	bool Toggle()
	{
		bReferenceMovementAllowed = !bReferenceMovementAllowed;
		return bReferenceMovementAllowed;
	}

	__HOOK OnCheckReferenceMovementHook()
	{
		_asm
		{
			cmp bReferenceMovementAllowed, 1
			jne done
			cmp byte ptr ds : [0xED1453], 0 // IsControlHeld
		done:
			ret
		}
	}

	void InitHooks()
	{
		WriteRelCall(0x459FDF, UInt32(OnCheckReferenceMovementHook));
		SafeWrite16(0x459FDF + 5, 0x9066);
	}
}