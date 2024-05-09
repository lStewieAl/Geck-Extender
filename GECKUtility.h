#pragma once
#include "NiObjects.h"
extern void(__cdecl* SaveWindowPositionToINI)(HWND, const char*);
extern HWND g_mainWindowToolbar;

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
	static TESObjectCELL* GetCurrentCell();
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
	void Undo();
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


class PathingSolution;
class NavMesh;
class NavMeshEditVertex;
class NavMeshEditTriangle;
class NavMeshEditEdge;
struct NavMeshManager
{
	struct UndoStack
	{
		struct NavMeshUndoData
		{
			BSSimpleArray<void> navMeshModifyUndoArray;
			BSSimpleArray<void> navMeshMoveUndoArray;
			BSSimpleArray<NavMesh> navMeshArray;
			UInt8 byte30;
		};

		NavMeshUndoData* items[30];
		UInt32 numItems;
	};

	struct VertexTriangleEdgeArrays
	{
		BSSimpleArray<NavMeshEditVertex> vertices;
		BSSimpleArray<NavMeshEditTriangle> triangles;
		BSSimpleArray<NavMeshEditEdge> edges;
	};


	UInt32 unk000;
	UInt8 isSelectVertices;
	UInt8 isSelectTriangles;
	UInt8 isSelectEdges;
	UInt8 byte007;
	VertexTriangleEdgeArrays arrayData;
	UInt32 unk038;
	UInt32 unk03C;
	NiPoint3 pt040;
	NiPoint3 pt04C;
	NavMeshManager::UndoStack undoStack;
	UInt32 unk0D4;
	UInt32 unk0D8;
	UInt32 unk0DC;
	UInt32 unk0E0;
	UInt32 unk0E4;
	UInt32 unk0E8;
	UInt32 unk0EC;
	UInt32 unk0F0;
	UInt32 unk0F4;
	UInt32 unk0F8;
	UInt32 unk0FC;
	UInt32 unk100;
	UInt32 unk104;
	UInt32 unk108;
	UInt32 unk10C;
	UInt32 unk110;
	UInt32 unk114;
	UInt32 unk118;
	UInt32 unk11C;
	UInt32 unk120;
	PathingSolution* pPathingSolution;
	UInt32 unk128;
	UInt32 flags;
	UInt32 unk130;
	UInt8 byte134;
	UInt8 byte135;
	UInt8 byte136;
	UInt8 bSingleCellViewMode;
	UInt32 unk138;
	UInt32 unk13C;
	BSSimpleArray<void> pointArray140;
	BSSimpleArray<void> navMeshWarningDataArray;
	UInt32 unk160;
	UInt8 byte164;

	void Undo();

	static NavMeshManager* GetSingleton();
	static bool IsActive();
};

enum WindowMessages
{
	BSMsg_HandleDrop = 0x407,
	BSMsg_AcceptsDropType = 0x408,
};

struct OpenWindows
{
	static tList<HWND>* GetWindowList();
};

struct RenderWindowHotkey
{
	enum eKeys
	{
		kRenderHotkey_ToggleFlyCam = 0x0,
		kRenderHotkey_ToggleBatchDialog = 0x1,
		kRenderHotkey_ToggleOrthographicMode = 0x2,
		kRenderHotkey_Navmesh_FinalizeCell = 0x3,
		kRenderHotkey_ShowHideSelectedObjectS = 0x4,
		kRenderHotkey_ShowHideEnableStateParents = 0x5,
		kRenderHotkey_ShowHideEnableStateOppositeParents = 0x6,
		kRenderHotkey_Navmesh_FindCover = 0x7,
		kRenderHotkey_Navmesh_OpenStandardGenerationDialog = 0x8,
		kRenderHotkey_FreezeCamera = 0x9,
		kRenderHotkey_Navmesh_OpenAdvancedGenerationDialog = 0xA,
		kRenderHotkey_ToggleDebugGeometry = 0xB,
		kRenderHotkey_Navmesh_AddATriangleOrQuad = 0xC,
		kRenderHotkey_ToggleBrightLights = 0xD,
		kRenderHotkey_FindBadBound = 0xE,
		kRenderHotkey_CreateMultiboundRef = 0xF,
		kRenderHotkey_ShowLandBorders = 0x10,
		kRenderHotkey_CycleThroughViews = 0x11,
		kRenderHotkey_ToggleSelectionCulling = 0x12,
		kRenderHotkey_ToggleNonLandCulling = 0x13,
		kRenderHotkey_Copy = 0x14,
		kRenderHotkey_DropCamera = 0x15,
		kRenderHotkey_Navmesh_ClearSelection = 0x16,
		kRenderHotkey_CloneCopyAndPaste = 0x17,
		kRenderHotkey_ClearSelection = 0x18,
		kRenderHotkey_ToggleEditColors = 0x19,
		kRenderHotkey_ToggleNavmeshEdit = 0x1A,
		kRenderHotkey_Navmesh_OpenEdgePropertiesDialog = 0x1B,
		kRenderHotkey_Navmesh_SelectTriangleByIndex = 0x1C,
		kRenderHotkey_Navmesh_DropVertices = 0x1D,
		kRenderHotkey_SearchAndReplace = 0x1E,
		kRenderHotkey_DropSelection = 0x1F,
		kRenderHotkey_RaiseSelection = 0x20,
		kRenderHotkey_Navmesh_ToggleEdgeSelectMode = 0x21,
		kRenderHotkey_Navmesh_UnhideAllNavmeshes = 0x22,
		kRenderHotkey_Navmesh_HideSelectedNavmeshes = 0x23,
		kRenderHotkey_ToggleMeshEditingMode = 0x24,
		kRenderHotkey_Navmesh_InverseFloodFill = 0x25,
		kRenderHotkey_OpenTextureUseDialog = 0x26,
		kRenderHotkey_GetObjectData = 0x27,
		kRenderHotkey_LoadCellForSelectedObject = 0x28,
		kRenderHotkey_ToggleLandCulling = 0x29,
		kRenderHotkey_ToggleLightRadius = 0x2A,
		kRenderHotkey_Navmesh_MergeSelectedVertices = 0x2B,
		kRenderHotkey_Navmesh_MergeSelectedVerticesAlt = 0x2C,
		kRenderHotkey_MoveClipboardReferences = 0x2D,
		kRenderHotkey_ToggleMarkers = 0x2E,
		kRenderHotkey_Navmesh_ToggleSingleCellViewMode = 0x2F,
		kRenderHotkey_Navmesh_ToggleWater = 0x30,
		kRenderHotkey_Navmesh_ToggleNoLargeCreature = 0x31,
		kRenderHotkey_Navmesh_TogglePreferred = 0x32,
		kRenderHotkey_PortalMode = 0x33,
		kRenderHotkey_SaveLocalMapSnapshot = 0x34,
		kRenderHotkey_ToggleLocationSnap = 0x35,
		kRenderHotkey_ToggleRotationSnap = 0x36,
		kRenderHotkey_ToggleReferenceSnap = 0x37,
		kRenderHotkey_Navmesh_DeleteSelection = 0x38,
		kRenderHotkey_RenameSoundFiles = 0x39,
		kRenderHotkey_ScaleSelected = 0x3A,
		kRenderHotkey_Navmesh_SwapEdge = 0x3B,
		kRenderHotkey_Navmesh_ToggleSelectableTriangles = 0x3C,
		kRenderHotkey_PlaceCameraAboveSelection = 0x3D,
		kRenderHotkey_ToggleOcclusionPlanes = 0x3E,
		kRenderHotkey_Navmesh_ToggleSelectableVertices = 0x3F,
		kRenderHotkey_Paste = 0x40,
		kRenderHotkey_PasteInPlace = 0x41,
		kRenderHotkey_Navmesh_ChangeDrawMode = 0x42,
		kRenderHotkey_ToggleWaterCulling = 0x43,
		kRenderHotkey_ToggleWireframe = 0x44,
		kRenderHotkey_SwapEditorWater = 0x45,
		kRenderHotkey_GroupStaticRefs = 0x46,
		kRenderHotkey_COUNT = 0x47,
	};

	enum eRenderWindowHotkeyCategory : UInt32
	{
		kRenderHotkeyCategory_General = 0x0,
		kRenderHotkeyCategory_Navmesh = 0x1,
		kRenderHotkeyCategory_Camera = 0x2,
		kRenderHotkeyCategory_Visibility = 0x3,
		kRenderHotkeyCategory_Clipboard = 0x4,
	};

	enum RenderWindowHotkeyFlag : unsigned __int8
	{
		kRenderWindowPreferenceFlag_NONE = 0,
		kRenderWindowPreferenceFlag_Shift = 0x1,
		kRenderWindowPreferenceFlag_Ctrl = 0x2,
		kRenderWindowPreferenceFlag_Alt = 0x4,
		kRenderWindowPreferenceFlag_Z = 0x8,
		kRenderWindowPreferenceFlag_Navmesh = 0x40,
	};

	struct __declspec(align(2)) KeyCombo
	{
		union
		{
			struct
			{
				UInt8 key;
				RenderWindowHotkeyFlag flags;
			};
			unsigned short data;
		};

		KeyCombo(UInt8 key, RenderWindowHotkeyFlag flags) : key(key), flags(flags) {};
	};

	const char* sName;
	const char* sInternalName;
	eRenderWindowHotkeyCategory uiCategory;
	KeyCombo kDefaultCombo;
	KeyCombo kCombo;
	UInt32 bIsEditAllowed;
	RenderWindowHotkey(const char* name,
		const char* internalName = "",
		UInt8 defaultKey = 0,
		RenderWindowHotkeyFlag defaultFlags = kRenderWindowPreferenceFlag_NONE,
		eRenderWindowHotkeyCategory category = kRenderHotkeyCategory_General,
		UInt32 bIsEditAllowed = 1)
		: sName(name), sInternalName(internalName), uiCategory(category),
		bIsEditAllowed(bIsEditAllowed), kDefaultCombo(defaultKey, defaultFlags), kCombo(defaultKey, defaultFlags) {};
};
STATIC_ASSERT(sizeof(RenderWindowHotkey) == 0x14);

void AddFormsToListView(HWND listView, tList<TESForm>* forms, bool(__cdecl* filterFn)(TESForm*, void*) = nullptr, void* filterData = nullptr);

void OpenForm(TESForm* form, HWND parentHwnd = nullptr);

void RunCallbackOnAllCellRefs(void (*callback)(TESObjectREFR*));

BOOL __stdcall _CreateProcessA(LPSTR exePath, LPPROCESS_INFORMATION lpProcessInformation);