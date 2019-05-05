#pragma once
struct NiPoint3
{
	float x, y, z;
};

// functions for getting and setting the render window camera
int* __cdecl GetCamera(NiPoint3* outPosition, NiPoint3* outDirection) {
	return ((int* (__cdecl*)(NiPoint3* outPosition, NiPoint3* outDirection))(0x8B2F40))(outPosition, outDirection);
}

void __cdecl SetCamera(NiPoint3* pos, NiPoint3* direction) {
	((void(__cdecl*)(NiPoint3 * a1, NiPoint3 * a2))(0x8B2FE0))(pos, direction);
}

/* helper function for set camera, using current value for direction */
void __cdecl SetCameraPos(NiPoint3* pos) {
	SetCamera(pos, (NiPoint3*)(0xF238FC));
}

void __cdecl SetCameraDirection(NiPoint3* direction) {
	SetCamera((NiPoint3*)(0xF238EC), direction);
}

struct lpMem {
	void* unk0;
	void* unk4;
	float* unk8;
};

void SetCamera2(NiPoint3* pos, NiPoint3* direction) {
	lpMem* lpMems = (lpMem*)(0xED116C);
	float* posMaybe = lpMems->unk8;

}