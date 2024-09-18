#include "GameBSExtraData.h"
#include "utilities.h"

// 0x421E10
TESObjectREFR* ExtraDataList::GetMultiBoundRef() const {
	return ThisCall<TESObjectREFR*>(0x4ABA10, this);
}