#pragma once

void SafeWrite8(UInt32 addr, UInt32 data);
void SafeWrite16(UInt32 addr, UInt32 data);
void SafeWrite32(UInt32 addr, UInt32 data);
void SafeWrite64(UInt32 addr, UInt32 data);
void SafeWriteBuf(UInt32 addr, void * data, UInt32 len);
void SafeWriteBuf(UInt32 addr, const char* data, UInt32 len);
void SafeWriteFloat(UInt32 addr, float data);

// 5 bytes
void WriteRelJump(UInt32 jumpSrc, UInt32 jumpTgt);
void WriteRelCall(UInt32 jumpSrc, UInt32 jumpTgt);

// 6 bytes
void WriteRelJnz(UInt32 jumpSrc, UInt32 jumpTgt);
void WriteRelJz(UInt32 jumpSrc, UInt32 jumpTgt);
void WriteRelJle(UInt32 jumpSrc, UInt32 jumpTgt);

[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourVtable(UInt32 addr, UInt32 dst);
[[nodiscard]] __declspec(noinline) UInt32 __stdcall DetourRelCall(UInt32 jumpSrc, UInt32 jumpTgt);
UInt32 __stdcall GetRelJumpAddr(UInt32 jumpSrc);