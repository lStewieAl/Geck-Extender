#pragma once
void(__cdecl* SaveWindowPositionToINI)(HWND, char*) = ((void(__cdecl*)(HWND hWnd, char* Src))(0x43E170));

struct NiPoint3
{
	float x, y, z;
};

struct ViewMatrix
{
	float x1, x2, x3, x4, x5, x6, x7, x8, x9;
};

struct posEtc {
	void* unk0[0xD];
	ViewMatrix viewMatrix; // 38
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
	return *(lpMem * *)(0xED116C);
}

// functions for getting and setting the render window camera
void GetCameraViewMatrix(ViewMatrix* out) {
	ViewMatrix* current = &GetLPMem()->unk4->viewMatrix;
	out->x1 = current->x1;
	out->x2 = current->x2;
	out->x3 = current->x3;
	out->x4 = current->x4;
	out->x5 = current->x5;
	out->x6 = current->x6;
	out->x7 = current->x7;
	out->x8 = current->x8;
	out->x9 = current->x9;
}

void SetCameraViewMatrix(ViewMatrix* newView) {
	ViewMatrix* current = &GetLPMem()->unk4->viewMatrix;
	current->x1 = newView->x1;
	current->x2 = newView->x2;
	current->x3 = newView->x3;
	current->x4 = newView->x4;
	current->x5 = newView->x5;
	current->x6 = newView->x6;
	current->x7 = newView->x7;
	current->x8 = newView->x8;
	current->x9 = newView->x9;

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

void GetCamera(NiPoint3* outPosition, ViewMatrix* outDirection) {
	GetCameraPos(outPosition);
	GetCameraViewMatrix(outDirection);
}

void SetCamera(NiPoint3* pos, ViewMatrix* direction) {
	SetCameraPos(pos);
	SetCameraViewMatrix(direction);
}

void SetTimeOfDay(float time) {
	float* unknownStruct = *(float**)(0xECF93C);
	if (!unknownStruct) return;

	float* timeStruct = *(float**)(unknownStruct + 0x1A);
	if (!timeStruct) return;

	((void(__thiscall*)(float* timeStruct, float newTime))(0x44CD00))(timeStruct, time);
}

void ToggleRenderWindowAllowCellLoads(bool toggle) {
	/* toggle the third bit */
	if (toggle)
	{
		*(byte*)(0xECFCEC) |= (1 << 2);
	}
	else
	{
		*(byte*)(0xECFCEC) &= ~(1 << 2);
	}
}

bool GetIsRenderWindowAllowCellLoads() {
	return (*(byte*)(0xECFCEC) >> 2) & 1;
}

void SetIsShowWater(bool state) {
	(*(byte*)(0xECEED4)) = state;
	((void(*)(void))(0x4164D0))();
}

bool GetIsShowWater() {
	return (*(bool*)(0xECEED4));
}

bool GetIsShowPortalsAndRooms() {
	return (*(bool*)(0xECEEF8));
}

void SetIsShowPortalsAndRooms(bool state) {
	(*(byte*)(0xECEEF8)) = state;
	((void(*)(void))(0x416590))();
}

long GetPreviousRenderWindowSize() {
	return (*(long*)(0xED1444));
}

int GetPreviousRenderWindowWidth() {
	return (*(int*)(0xED1444));
}

int GetPreviousRenderWindowHeight() {
	return (*(int*)(0xED1448));
}

void RefreshLightMarkers() {
	((void(*)(void))(0x416490))();
}

bool GetIsShowLightMarkers() {
	return (*(bool*)(0xECEEBC));
}

void SetIsShowLightMarkers(bool state) {
	(*(byte*)(0xECEEBC)) = state;
	RefreshLightMarkers();
}

void RefreshSoundMarkers() {
	((void(*)(void))(0x4165B0))();
}

bool GetIsShowSoundMarkers() {
	return (*(bool*)(0xECEEA4));
}

void SetIsShowSoundMarkers(bool state) {
	(*(byte*)(0xECEEA4)) = state;
	RefreshSoundMarkers();
}

void SetFlycamMode(int state) {
	((void(__cdecl*)(int state))(0x451EF0))(state);
}

int GetFlycamMode() {
	return (*(int*)(0xED11BC));
}

HWND g_renderWindowHwnd = (HWND)0xE0C1AA;
HWND g_mainWindowToolbar = (HWND)0xECFC14;
HWND g_objectWindowHwnd = (HWND)0xECFB70;

struct ObjectPalette
{
	struct Object
	{
		char* name;			// 00
		UInt16	unk04;			// 04
		UInt16	unk06;			// 06
		int		formID;			// 08
		int		conformToSlope;	// 0C
		float	sinkMin;		// 10
		float	sinkMax;		// 14
		float	scaleMin;		// 18
		float	scaleMax;		// 1C
		float	xMin;			// 20
		float	xMax;			// 24
		float	yMin;			// 28
		float	yMax;			// 2C
		float	zMin;			// 30
		float	zMax;			// 34
	};

	ObjectPalette::Object* current;		// 00
	tList<ObjectPalette::Object> list;	// 04
	UInt32 sPaletteFileNameSetting;		// 0C		
	char* paletteName;					// 10

	static ObjectPalette* GetSingleton() { return (ObjectPalette*)0xECE248; };
};