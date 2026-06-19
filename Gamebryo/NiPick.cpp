#include "NiPick.hpp"

void NiPick::SetTarget(NiAVObject* apObject)
{
    ThisCall(0x40BB90, this, apObject);
}

bool NiPick::PickObjects(const NiPoint3& kOrigin, const NiPoint3& kDir, bool bAppend)
{
    return ThisCall<bool>(0xC34C30, this, &kOrigin, &kDir, bAppend);
}