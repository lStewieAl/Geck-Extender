#pragma once
#include "NiObjects.h"
void(__cdecl* SaveWindowPositionToINI)(HWND, const char*) = ((void(__cdecl*)(HWND hWnd, const char* Src))(0x43E170));

struct RenderData
{
	NiWindow* niWindow;
	NiNode* pNode;
	NiCamera* pCamera;

	static RenderData* GetSingleton() { return *(RenderData**)0xED116C; };
};

// functions for getting and setting the render window camera
void GetCameraViewMatrix(NiMatrix33* out) {
	auto current = &RenderData::GetSingleton()->pNode->m_localRotate;
	*out = *current;
}

void SetCameraViewMatrix(NiMatrix33* newView) {
	auto current = &RenderData::GetSingleton()->pNode->m_localRotate;
	*current = *newView;
}

void GetCameraPos(NiPoint3* outPosition) {
	NiVector3* currentPos = &RenderData::GetSingleton()->pNode->m_localTranslate;
	outPosition->x = currentPos->x;
	outPosition->y = currentPos->y;
	outPosition->z = currentPos->z;
}

void SetCameraPos(NiPoint3* newPos) {
	NiVector3* currentPos = &RenderData::GetSingleton()->pNode->m_localTranslate;
	currentPos->x = newPos->x;
	currentPos->y = newPos->y;
	currentPos->z = newPos->z;
}

void GetCamera(NiPoint3* outPosition, NiMatrix33* outDirection) {
	GetCameraPos(outPosition);
	GetCameraViewMatrix(outDirection);
}

void SetCamera(NiPoint3* pos, NiMatrix33* direction) {
	SetCameraPos(pos);
	SetCameraViewMatrix(direction);
}

void SetTimeOfDay(float time) {
	float* unknownStruct = *(float**)(0xECF93C);
	if (!unknownStruct) return;

	float* timeStruct = *(float**)(unknownStruct + 0x1A);
	if (!timeStruct) return;

	ThisCall(0x44CD00, timeStruct, time);
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
	StdCall(0x4164D0);
}

bool GetIsShowWater() {
	return (*(bool*)(0xECEED4));
}

bool GetIsShowPortalsAndRooms() {
	return (*(bool*)(0xECEEF8));
}

void SetIsShowPortalsAndRooms(bool state) {
	(*(byte*)(0xECEEF8)) = state;
	StdCall(0x416590);
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
	StdCall(0x416490);
}

bool GetIsShowLightMarkers() {
	return (*(bool*)(0xECEEBC));
}

void SetIsShowLightMarkers(bool state) {
	(*(byte*)(0xECEEBC)) = state;
	RefreshLightMarkers();
}

void RefreshSoundMarkers() {
	StdCall(0x4165B0);
}

bool GetIsShowSoundMarkers() {
	return (*(bool*)(0xECEEA4));
}

void SetIsShowSoundMarkers(bool state) {
	(*(byte*)(0xECEEA4)) = state;
	RefreshSoundMarkers();
}

void SetFlycamMode(int state) {
	CdeclCall(0x451EF0, state);
}

int GetFlycamMode() {
	return (*(int*)(0xED11BC));
}

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

struct RenderWindow
{
	struct SelectedData
	{
		static SelectedData* GetSelected() { return *(SelectedData**)0xECFB8C; }
		static SelectedData* GetClipboard() { return *(SelectedData**)0xECFB90; }

		struct List
		{
			TESObjectREFR* ref;
			List* prev;
			List* next;
		};

		List* selectedForms;
		UInt32 numItems;
		NiPoint3 pos;
		float maxDistFromCenter;
	};

	static HWND GetWindow() { return *(HWND*)0xECFB40; };
	static bool GetMousePos(NiPoint3* aPosOut, NiPoint3* aRotOut = nullptr)
	{
		POINT point;
		if (!GetCursorPos(&point)) {
			return false;
		}
		ScreenToClient(GetWindow(), &point);

		NiPoint3 pos;
		NiPoint3 rot;
		if (!aPosOut)
		{
			aPosOut = &pos;
		}
		if (!aRotOut)
		{
			aRotOut = &rot;
		}

		return RenderData::GetSingleton()->pCamera->WindowPointToRay(point.x, point.y, aPosOut, aRotOut);
	}
};

struct ObjectsView
{
	static HWND GetWindow() { return *(HWND*)0xECFB70; };
	static void Refresh() { SendMessageA(GetWindow(), 1042, NULL, NULL); };
};

struct CellView
{
	static HWND GetWindow() { return *(HWND*)0xECFB78; };
};

struct HistoryManager
{
	enum UndoType
	{
		kUndoType_ColorLand = 0x5,
		kUndoType_PaintLand = 0x6,
	};

	struct Element
	{
		UInt32 count000;
		UInt32 unk004;
		TESForm* form;
		UInt32 undoType;
		NiPoint3 rotation;
		NiPoint3 refPos;
		TESObjectCELL* parentCell;
		UInt32 unk02C;
		UInt32 unk030;
		UInt32 unk034;
		UInt32 unk038;
		UInt32 unk03C;
		UInt32 unk040;
		UInt32 undoPaintLandData;
		void* portalRefData;
		void* occlusionPlaneData;
		NiPoint3* primitivePlaneBoundsX;
		UInt32 primitivePlaneBoundsY;
		UInt32 primitivePlaneBoundsZ;
		HistoryManager::Element* ptr06C;
		UInt32 prev;
	};

	HistoryManager::Element* element;
	HistoryManager::Element* current;

	static HistoryManager* GetSingleton() { return *(HistoryManager **)0xECFDF4; };
	void ClearHistoryForCurrentElement() { ThisCall(0x467CC0, this); };
	void AddAction(int aiActionType, RenderWindow::SelectedData* apSelectedForms) { ThisCall(0x465D90, this, aiActionType, apSelectedForms); };
};
