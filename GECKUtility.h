#pragma once
#include "NiObjects.h"
extern void(__cdecl* SaveWindowPositionToINI)(HWND, const char*);
extern HWND g_mainWindowToolbar;
extern HWND g_objectWindowHwnd;

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

	static ObjectPalette* GetSingleton();
};

struct RenderWindow
{
	struct SelectedData
	{
		static SelectedData* GetSelected();
		static SelectedData* GetClipboard();

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

	static HWND GetWindow();
	static bool GetMousePos(NiPoint3* aPosOut, NiPoint3* aRotOut = nullptr);
};

struct ObjectsView
{
	static HWND GetWindow();
	static void Refresh();
};

struct CellView
{
	static HWND GetWindow();
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

	static HistoryManager* GetSingleton();
	void ClearHistoryForCurrentElement();
	void AddAction(int aiActionType, RenderWindow::SelectedData* apSelectedForms);
};

struct RenderData
{
	NiWindow* niWindow;
	NiNode* pNode;
	NiCamera* pCamera;

	static RenderData* GetSingleton();
};

// functions for getting and setting the render window camera
void GetCameraViewMatrix(NiMatrix33* out);
void SetCameraViewMatrix(NiMatrix33* newView);
void GetCameraPos(NiPoint3* outPosition);
void SetCameraPos(NiPoint3* newPos);
void GetCamera(NiPoint3* outPosition, NiMatrix33* outDirection);
void SetCamera(NiPoint3* pos, NiMatrix33* direction);
void SetTimeOfDay(float time);
void ToggleRenderWindowAllowCellLoads(bool toggle);
bool GetIsRenderWindowAllowCellLoads();
void SetIsShowWater(bool state);
bool GetIsShowWater();
bool GetIsShowPortalsAndRooms();
void SetIsShowPortalsAndRooms(bool state);
long GetPreviousRenderWindowSize();
int GetPreviousRenderWindowWidth();
int GetPreviousRenderWindowHeight();
void RefreshLightMarkers();
bool GetIsShowLightMarkers();
void SetIsShowLightMarkers(bool state);
void RefreshSoundMarkers();
bool GetIsShowSoundMarkers();
void SetIsShowSoundMarkers(bool state);
void SetFlycamMode(int state);
int GetFlycamMode();