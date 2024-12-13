#pragma once

#include "SafeWrite.h"
#include <xutil.h>

namespace UnserializedIO 
{
	namespace ToReplace 
	{
		constexpr static UInt32 fseekAddr = 0x85207B;
		constexpr static UInt32 freadAddr[] = { 0x851EE3, 0x85210F, 0x85212F };
		constexpr static UInt32 fwriteAddr[] = { 0x851F2D, 0x8521E1 };
	}

	namespace FunctionsAddr 
	{
		constexpr static UInt32 fseek = 0xC63400;
		constexpr static UInt32 fread = 0xC62A6D;
		constexpr static UInt32 fwrite = 0xC62686;
	}

	// Undoes Obsidian's serialization of the I/O functions by replacing them with the original ones
	// Technically we patched this in GECK.exe itself - this serves as a documentation
	void InitHooks() 
	{
		// Remove the initializator for the serialized I/O functions
		XUtil::PatchMemoryNop(0x8532CA, 5);

		WriteRelCall(ToReplace::fseekAddr, FunctionsAddr::fseek);

		for (UInt32 addr : ToReplace::freadAddr)
			WriteRelCall(addr, FunctionsAddr::fread);

		for (UInt32 addr : ToReplace::fwriteAddr)
			WriteRelCall(addr, FunctionsAddr::fwrite);
	}
}