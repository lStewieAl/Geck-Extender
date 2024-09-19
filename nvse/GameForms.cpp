#include "GameForms.h"

// 0x6662D0
TESObjectCELL* TESWorldSpace::GetCellAtCoord(SInt32 aiX, SInt32 aiY) const {
	return ThisCall<TESObjectCELL*>(0x6662D0, this, aiX, aiY);
}

// 0x6699B0
TESObjectCELL* TESWorldSpace::GetCellAtPos(const NiPoint3& arPos) const {
	return GetCellAtCoord(SInt32(arPos.x) >> 12, SInt32(arPos.y) >> 12);
}

// 0x6361C0
NiNode* TESObjectCELL::Load3D() {
	return ThisCall<NiNode*>(0x6361C0, this);
}

// 0x637610
void TESObjectCELL::AttachModels(bool abSetupMopp) {
	ThisCall(0x637610, this, abSetupMopp);
}

// 0x629120
void TESObjectCELL::AddObjectsToLoadedRefCollection() {
	ThisCall(0x629120, this);
}

// 0x62CEF0
BSMultiBoundNode* TESObjectCELL::AddMultiBoundRef(TESObjectREFR* apMultiBound) {
	return ThisCall<BSMultiBoundNode*>(0x62CEF0, this, apMultiBound);
}
