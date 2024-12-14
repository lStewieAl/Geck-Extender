#pragma once

class MemoryManager {
protected:
	bool	bInitialized;
	UInt16	usNumHeaps;
	UInt16	usNumPhysicalHeaps;
	void**	ppHeaps;
	void*	pHeapsByContext[66];
	// Rest of the class is omitted since we don't need it for now

	void InitializePools();
	void SpecifyMemoryLayout();
	void SpecifyPools();
	void CreatePool(UInt32 auiPoolNumber, UInt32 auiSize, const char* apName);

public:
	void Initialize();

	static MemoryManager* GetSingleton();
};