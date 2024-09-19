#include "NiObjects.h"

void BSMultiBoundAABB::GetVertices(BoundVertices* apVerts) {
	NiPoint3 x = NiPoint3::UNIT_X * HalfExtents.x;
	NiPoint3 y = NiPoint3::UNIT_Y * HalfExtents.y;
	NiPoint3 z = NiPoint3::UNIT_Z * HalfExtents.z;

	NiPoint3 v2 = Center + x;
	NiPoint3 v10 = Center - x;

	NiPoint3 v3 = v2 + y;
	NiPoint3 v7 = v2 - y;

	NiPoint3 v11 = v10 + y;
	NiPoint3 v15 = v10 - y;

	apVerts->point[0] = v3 + z;
	apVerts->point[1] = v3 - z;

	apVerts->point[2] = v7 + z;
	apVerts->point[3] = v7 - z;

	apVerts->point[4] = v11 + z;
	apVerts->point[5] = v11 - z;

	apVerts->point[6] = v15 + z;
	apVerts->point[7] = v15 - z;
}

void BSMultiBoundOBB::GetVertices(BoundVertices* apPoints) {
	float v3; // xmm2_4
	float v4; // xmm3_4
	float v5; // xmm4_4
	float v7; // xmm13_4
	float v8; // xmm14_4
	float v9; // xmm7_4
	float v10; // xmm15_4
	float v12; // xmm8_4
	float v13; // xmm9_4
	float v14; // xmm1_4
	float v15; // xmm5_4
	float v16; // xmm6_4
	float v17; // xmm10_4
	float v18; // xmm11_4
	float v19; // xmm12_4
	float v20; // xmm2_4
	float v21; // xmm13_4
	float v22; // xmm3_4
	float v23; // xmm4_4
	float v24; // xmm14_4
	float v26; // xmm15_4

	NiPoint3 base;

	base.x = HalfExtents.x;
	base.y = HalfExtents.y;
	base.z = HalfExtents.z;

	v3 = Axis.GetEntry(0, 0) * base.x;
	v4 = Axis.GetEntry(0, 1) * base.x;
	v5 = Axis.GetEntry(0, 2) * base.x;

	v7 = Axis.GetEntry(1, 0) * base.y;
	v8 = Axis.GetEntry(1, 1) * base.y;
	v10 = Axis.GetEntry(1, 2) * base.y;

	v14 = Axis.GetEntry(2, 0) * base.z;
	v15 = Axis.GetEntry(2, 1) * base.z;
	v16 = Axis.GetEntry(2, 2) * base.z;

	v9 = Center.x + v3;
	v17 = Center.x - v3;

	v12 = Center.y + v4;
	v18 = Center.y - v4;

	v13 = Center.z + v5;
	v19 = Center.z - v5;

	v20 = v14 + v7;
	v21 = v7 - v14;
	v22 = v15 + v8;
	v23 = v16 + v10;

	v24 = v8 - v15;
	v26 = v10 - v16;
	apPoints->point[0].x = v20 + v9;
	apPoints->point[0].y = (v15 + v8) + v12;
	apPoints->point[0].z = (v16 + v10) + v13;

	apPoints->point[1].x = v21 + v9;
	apPoints->point[1].y = v24 + v12;
	apPoints->point[1].z = v26 + v13;

	apPoints->point[2].x = v9 - v21;
	apPoints->point[2].y = v12 - v24;
	apPoints->point[2].z = v13 - v26;

	apPoints->point[3].x = v9 - v20;
	apPoints->point[3].y = v12 - v22;
	apPoints->point[3].z = v13 - v23;

	apPoints->point[4].x = v20 + v17;
	apPoints->point[4].y = v22 + v18;
	apPoints->point[4].z = v23 + v19;

	apPoints->point[5].x = v21 + v17;
	apPoints->point[5].y = v24 + v18;
	apPoints->point[5].z = v26 + v19;

	apPoints->point[6].x = v17 - v21;
	apPoints->point[6].y = v18 - v24;
	apPoints->point[6].z = v19 - v26;

	apPoints->point[7].x = v17 - v20;
	apPoints->point[7].y = v18 - v22;
	apPoints->point[7].z = v19 - v23;
}

void NiAVObject::Update(NiUpdateData& arData){
	UpdateDownwardPass(arData, 0);
	if (m_parent)
		m_parent->UpdateUpwardPass();
};