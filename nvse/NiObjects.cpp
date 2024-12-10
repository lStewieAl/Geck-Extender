#include "NiObjects.h"

void NiAVObject::Update(NiUpdateData& arData){
	UpdateDownwardPass(arData, 0);
	if (m_parent)
		m_parent->UpdateUpwardPass();
};