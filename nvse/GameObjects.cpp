#include "GameObjects.h"
#include "NiObjects.h"


// 0x6437A0
void TESObjectREFR::AddMultiBound(TESObjectREFR* apMultiBoundRef) {
	ThisCall(0x6437A0, this, apMultiBoundRef);
}

// 0x643070
bool TESObjectREFR::CheckBound(const BSMultiBound* apBound) const {
	return ThisCall<bool>(0x643070, this, apBound);
}

// 0x569620
bool TESObjectREFR::IsInMultiBound(const BSMultiBound* apBound) const {
	return apBound->GetPointWithin(*GetPos());
}

// 0x63D770
void TESObjectREFR::MarkAsModified(bool abModified) {
	ThisCall(0x63D770, this, abModified);
}

// 0x642830
bool TESObjectREFR::CanSetMultibound(const TESObjectREFR* apRef, const TESObjectREFR* apBound) {
	return CdeclCall<bool>(0x642830, apRef, apBound);
}

// 0x4BA240
bool TESObjectREFR::IsMultiBoundOrRoomMarker() const {
	return ThisCall<bool>(0x4BA240, this);
}

// 0x64C0C0
const char* TESObjectREFR::GetEquippableModelPath() const {
	return ThisCall<const char*>(0x64C0C0, this);
}

// 0x64C030
void TESObjectREFR::Set3DSimple(NiNode* apNode) {
	ThisCall(0x64C030, this, apNode);
}

NiMatrix33& TESObjectREFR::GetOrientation(NiMatrix33& arMatrix) const {
	if (IsActor())
		arMatrix.FromEulerAnglesXYZ(0, 0, rotZ);
	else
		arMatrix.FromEulerAnglesXYZ(rotX, rotY, rotZ);

	return arMatrix;
}
