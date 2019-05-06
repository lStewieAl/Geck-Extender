#pragma once
#include "ZeLogWindow.h"
struct NiPoint3
{
	float x, y, z;
};

struct posEtc {
	void* unk0[0x16];
	NiPoint3 pos; // 58
	void* unk64[0xC];
	NiPoint3 lastPos; // 8C
};

struct lpMem {
	void* unk0;
	posEtc* unk4;
	posEtc* unk8;
};


lpMem* GetLPMem() {
	return *(lpMem**)(0xED116C);
}

// functions for getting and setting the render window camera
int* __cdecl GetCamera(NiPoint3* outPosition, NiPoint3* outDirection) {
	return ((int* (__cdecl*)(NiPoint3* outPosition, NiPoint3* outDirection))(0x8B2F40))(outPosition, outDirection);
}

void __cdecl SetCamera(NiPoint3* pos, NiPoint3* direction) {
	((void(__cdecl*)(NiPoint3 * a1, NiPoint3 * a2))(0x8B2FE0))(pos, direction);
}

void GetCameraPos(NiPoint3* outPosition) {
	NiPoint3* currentPos = &GetLPMem()->unk4->pos;
	outPosition->x = currentPos->x;
	outPosition->y = currentPos->y;
	outPosition->z = currentPos->z;
}

void SetCameraPos(NiPoint3* newPos) {
	NiPoint3* currentPos = &GetLPMem()->unk4->pos;
	currentPos->x = newPos->x;
	currentPos->y = newPos->y;
	currentPos->z = newPos->z;
}

void __cdecl SetCameraDirection(NiPoint3* direction) {
	SetCamera((NiPoint3*)(0xF238EC), direction);
}
