#include "MemoryManager.hpp"
#include "BSMemory.hpp"
#include "GameData.h"
#include "Settings.h"
#include "Utilities.h"

	// KiB from bytes
#define B_KiB(x) (x << 10)

	// MiB from bytes
#define B_MiB(x) (x << 20)

	// GiB from bytes
#define B_GiB(x) (x << 30)

// 0x857DC0
void MemoryManager::InitializePools() {
	ThisCall(0x857DC0, this);
}

// 0x4010C0
void MemoryManager::SpecifyMemoryLayout() {
	ZeroMemory(pHeapsByContext, sizeof(pHeapsByContext));
	usNumHeaps = 1;
	ppHeaps = BSNew<void*>(usNumHeaps);

	// MemoryHeap constructor
	ppHeaps[0] = BSNew(0x20F4);
	ThisCall(0x854ED0, ppHeaps[0], B_MiB(config.uiHeapMaxSizeMB), B_MiB(config.uiHeapInitialSizeMB), "Default Heap", 0);
	pHeapsByContext[65] = ppHeaps[0];
}

void MemoryManager::SpecifyPools() {
	CreatePool(8,	B_MiB(48),	"Pool 8");
	CreatePool(12,	B_MiB(12),	"Pool 12");
	CreatePool(16,	B_MiB(12),	"Pool 16");
	CreatePool(20,	B_MiB(8),	"Pool 20");
	CreatePool(24,	B_MiB(8),	"Pool 24");
	CreatePool(28,	B_MiB(10),	"Pool 28");
	CreatePool(32,	B_MiB(8),	"Pool 32");
	CreatePool(36,	B_MiB(6),	"Pool 36");
	CreatePool(40,	B_MiB(6),	"Pool 40");
	CreatePool(44,	B_MiB(4),	"Pool 44");
	CreatePool(48,	B_MiB(6),	"Pool 48");
	CreatePool(52,	B_MiB(3),	"Pool 52");
	CreatePool(56,	B_MiB(8),	"Pool 56");
	CreatePool(64,	B_MiB(16),	"Pool 64");
	CreatePool(72,	B_MiB(32),	"Pool 72");
	CreatePool(76,	B_MiB(1),	"Pool 76");
	CreatePool(80,	B_MiB(8),	"Pool 80");
	CreatePool(92,	B_MiB(2),	"Pool 92");
	CreatePool(96,	B_MiB(32),	"Pool 96");
	CreatePool(100, B_MiB(1),	"Pool 100");
	CreatePool(104, B_MiB(4),	"Pool 104");
	CreatePool(120, B_MiB(12),	"Pool 120");
	CreatePool(128, B_MiB(4),	"Pool 128");
	CreatePool(160, B_MiB(10),	"Pool 160");
	CreatePool(192, B_MiB(2),	"Pool 192");
	CreatePool(200, B_MiB(4),	"Pool 200");
	CreatePool(208, B_MiB(8),	"Pool 208");
	CreatePool(240, B_MiB(3),	"Pool 240");
	CreatePool(256, B_MiB(32),	"Pool 256");
	CreatePool(272, B_MiB(8),	"Pool 272");
	CreatePool(288, B_MiB(24),	"Pool 288");
	CreatePool(400, B_MiB(16),	"Pool 400");
	CreatePool(448, B_MiB(2),	"Pool 448");
	CreatePool(480, B_MiB(4),	"Pool 480");
	CreatePool(508, B_MiB(4),	"Pool 508");
	CreatePool(576, B_MiB(12),	"Pool 576");
	CreatePool(596, B_MiB(1),	"Pool 596");
}

// 0x853F20
void MemoryManager::CreatePool(UInt32 auiPoolNumber, UInt32 auiSize, const char* apName) {
	ThisCall(0x853F20, this, auiPoolNumber, auiSize, apName);
}

// 0x853C20
void MemoryManager::Initialize() {
	InitializePools();
	SpecifyMemoryLayout();
	bInitialized = true;
	SpecifyPools();
	TLSData::SetHeapIndex(0);
}

MemoryManager* MemoryManager::GetSingleton() {
	return (MemoryManager*)0xF21B5C;
}