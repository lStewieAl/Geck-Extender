#include "SafeWrite.h"

void SafeWrite8(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt8 *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void SafeWrite16(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt16 *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void SafeWrite32(UInt32 addr, UInt32 data)
{
	UInt32	oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt32 *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void SafeWrite64(UInt32 addr, UInt64 data)
{
	UInt32	oldProtect;

	VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((UInt64*)addr) = data;
	VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
}

void SafeWriteBuf(UInt32 addr, void * data, UInt32 len)
{
	UInt32	oldProtect;

	VirtualProtect((void *)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void *)addr, data, len);
	VirtualProtect((void *)addr, len, oldProtect, &oldProtect);
}

void SafeWriteBuf(UInt32 addr, const char* data, UInt32 len) {
	SafeWriteBuf(addr, (void*)data, len);
}

void SafeWriteFloat(UInt32 addr, float data)
{
	UInt32 oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((float *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jmp rel32
	SafeWrite8(jumpSrc, 0xE9);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// call rel32
	SafeWrite8(jumpSrc, 0xE8);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void WriteRelJnz(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jnz rel32
	SafeWrite16(jumpSrc, 0x850F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

void WriteRelJz(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jz rel32
	SafeWrite16(jumpSrc, 0x840F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

void WriteRelJle(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// jle rel32
	SafeWrite16(jumpSrc, 0x8E0F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

void WriteRelJa(UInt32 jumpSrc, UInt32 jumpTgt)
{
	// ja rel32
	SafeWrite16(jumpSrc, 0x870F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	UInt32 originalFunction = *(UInt32*)(jumpSrc + 1) + jumpSrc + 5;
	WriteRelCall(jumpSrc, jumpTgt);
	return originalFunction;
}

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst)
{
	UInt32 originalFunction = *(UInt32*)addr;
	SafeWrite32(addr, dst);
	return originalFunction;
}

UInt32 __stdcall GetRelJumpAddr(UInt32 jumpSrc)
{
	return *(UInt32*)(jumpSrc + 1) + jumpSrc + 5;
}

void ReplaceCall(UInt32 jumpSrc, UInt32 jumpTgt)
{
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}
