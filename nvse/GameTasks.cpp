#include "GameTasks.h"

ModelLoader* ModelLoader::GetSingleton()
{
	return *(ModelLoader**)0xED36A0;
}

// 0x4BF5A0
void ModelLoader::QueueReference(TESObjectREFR* apRef, UInt32 aePriority, bool abAllowQueueReferenceQueuing) {
	ThisCall(0x4BF5A0, this, apRef, aePriority, 0);
}

// 0x4C0040
NiNode* ModelLoader::LoadFile(const char* apPath, UInt32 aeLODFadeMult, bool abAssignShaders, int unused, bool abKeepUV, bool abNoUseCountIncrease) {
	return ThisCall<NiNode*>(0x4C0040, this, apPath, aeLODFadeMult, abAssignShaders, unused, abKeepUV, abNoUseCountIncrease);
}

