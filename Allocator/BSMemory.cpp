#include "BSMemory.hpp"

// -------------------------------------------------------------------------
// Internal globals and functions
// -------------------------------------------------------------------------
void*	pMemoryManager = nullptr;
void*	(__thiscall* Allocate)(void* pThis, size_t stSize) = nullptr;
void*	(__thiscall* ReAllocate)(void* pThis, void* pvMem, size_t stSize) = nullptr;
void	(__thiscall* Deallocate)(void* pThis, void* pvMem) = nullptr;
size_t	(__thiscall* Size)(void* pThis, void* pvMem) = nullptr;

void	BSAllocatorInitializer();
void	CreateHeapIfNotExisting(UInt32 auiHeapAddress, UInt32 auiCallAddress);
int		(__cdecl* CreateHeap)(UInt32 aeSerialize);
int		CreateHeapStub(UInt32 aeSerialize) { return 1; }
bool	bInitialized = false;

// -------------------------------------------------------------------------
// Functions made to be used by the user
// These functions use game's memory manager to manage memory
// They are used to replace new and delete operators
// -------------------------------------------------------------------------
void* BSNew(size_t stSize) {
	if (!bInitialized)
		BSAllocatorInitializer();

	return Allocate(pMemoryManager, stSize);
}

void* BSNewAligned(size_t stAlign, size_t stSize) {
	char* pMemory = static_cast<char*>(BSNew(stSize + stAlign));
	UInt32 uiAlignment = stAlign - (reinterpret_cast<UInt8>(pMemory) & (stAlign - 1));
	pMemory[UInt8(uiAlignment) - 1] = uiAlignment;
	return &pMemory[UInt8(uiAlignment)];
}

void* BSReallocate(void* pvMem, size_t stSize) {
	return ReAllocate(pMemoryManager, pvMem, stSize);
}

void BSFree(void* pvMem) {
	Deallocate(pMemoryManager, pvMem);
}

SIZE_T BSSize(void* pvMem) {
	return Size(pMemoryManager, pvMem);
}

// -------------------------------------------------------------------------
// Functions made to initialize the allocator
// Compatible with both game and GECK
// -------------------------------------------------------------------------

// This function is used to create game's heap if it doesn't exist
// It's possible to load the plugin before game is even initialized
// In those cases, malloc fails due to lack of heap - that's why we need to create it manually
_declspec(noinline) void CreateHeapIfNotExisting(UInt32 auiHeapAddress, UInt32 auiCallAddress) {
	if (*(HANDLE*)auiHeapAddress)
		return;

	CreateHeap(true);

	bInitialized = true;

	UInt32 oldProtect;
	VirtualProtect((void*)auiCallAddress, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*(UInt32*)(auiCallAddress + 1) = UInt32(CreateHeapStub) - auiCallAddress - 5;
	VirtualProtect((void*)auiCallAddress, 4, oldProtect, &oldProtect);
}

// This function sets up correct addresses based on the program
_declspec(noinline) void BSAllocatorInitializer() {
	if (*(UInt8*)0x401190 != 0x55) {
		// Is GECK
		pMemoryManager	= (void*)0xF21B5C;
		Allocate		= (void* (__thiscall*)(void*, size_t))0x8540A0;
		ReAllocate		= (void* (__thiscall*)(void*, void*, size_t))0x8543B0;
		Deallocate		= (void(__thiscall*)(void*, void*))0x8542C0;
		Size			= (size_t(__thiscall*)(void*, void*))0x854720;
		CreateHeap		= (int(__cdecl*)(UInt32))0xC770C3;

		CreateHeapIfNotExisting(0xF9907C, 0xC62B21);
	}
	else {
		pMemoryManager	= (void*)0x11F6238;
		Allocate		= (void* (__thiscall*)(void*, size_t))0xAA3E40;
		ReAllocate		= (void* (__thiscall*)(void*, void*, size_t))0xAA4150;
		Deallocate		= (void(__thiscall*)(void*, void*))0xAA4060;
		Size			= (size_t(__thiscall*)(void*, void*))0xAA44C0;
		CreateHeap		= (int(__cdecl*)(UInt32))0xEDDB6A;

		CreateHeapIfNotExisting(0x12705BC, 0xECC3CB);
	}
}
