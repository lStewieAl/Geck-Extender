#pragma once

#include "libs/stb_sprintf.h"

namespace PrintReplacer {
	void InitHooks() {
		WriteRelJump(0x401190, UInt32(stbsp_snprintf));
		WriteRelJump(0xC5D348, UInt32(stbsp_snprintf));
		WriteRelJump(0xC5BD5E, UInt32(stbsp_vsnprintf));
		WriteRelJump(0xC5D2CB, UInt32(stbsp_sprintf));
	}
}