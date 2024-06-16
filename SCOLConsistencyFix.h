#pragma once

#include "Utilities.h"
#include "SafeWrite.h"

namespace SCOLConsistencyFix 
{
	class NiTriStrips;
	class NiPoint3;
	class NiPoint2;
	class NiColorA;

	NiTriStrips* __fastcall CreateNiTriStrips(NiTriStrips* apThis, void*, UInt16 ausVertices, NiPoint3* apVertex, NiPoint3* apNormal, NiColorA* apColor, NiPoint2* apTexture, UInt16 ausNumTextureSets, UInt32 aeNBTMethod, UInt16 ausTriangles, UInt16 ausStrips, UInt16* apusStripLengths, UInt16* apusStripLists) 
	{
		NiTriStrips* pStrips = ThisCall<NiTriStrips*>(0x82A0F0, apThis, ausVertices, apVertex, apNormal, apColor, apTexture, ausNumTextureSets, aeNBTMethod, ausTriangles, ausStrips, apusStripLengths, apusStripLists);
		// Calculate consistency
		// Static by default, mutable if vertex controller is used (Geometry morphs for example)
		ThisCall(0x8315B0, pStrips, false);
		return pStrips;
	}

	void InitHooks() 
	{
		WriteRelCall(0x5DA4B6, UInt32(CreateNiTriStrips));
	}

}