#include "GECKUtility.h"

#include "GameForms.h"
#include "GameSettings.h"
#include "GameData.h"

void(__cdecl* SaveWindowPositionToINI)(HWND, const char*) = ((void(__cdecl*)(HWND hWnd, const char* Src))(0x43E170));
HWND g_mainWindowToolbar = (HWND)0xECFC14;

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
	if (auto sky = TES::GetSingleton()->sky)
	{
		ThisCall(0x44CD00, sky, time); // Sky::SetTime
	}
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

ObjectPalette* ObjectPalette::GetSingleton() { return (ObjectPalette*)0xECE248; };
HWND ObjectPalette::GetWindow() { return *(HWND*)0xECE23C; };

HWND RenderWindow::GetWindow() { return *(HWND*)0xECFB40; };
bool RenderWindow::GetMousePos(NiPoint3* aPosOut, NiPoint3* aRotOut) {
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

TESObjectCELL* RenderWindow::GetCurrentCell() { return *(TESObjectCELL**)0xED1174; };
bool RenderWindow::InLandscapeEditingMode() { return *(bool*)0xED142D; };

RenderWindow::SelectedData* RenderWindow::SelectedData::GetSelected() { return *(SelectedData**)0xECFB8C; }
RenderWindow::SelectedData* RenderWindow::SelectedData::GetClipboard() { return *(SelectedData**)0xECFB90; }

HWND ObjectsView::GetWindow() { return *(HWND*)0xECFB70; };
HWND ObjectsView::GetListView() { return *(HWND*)0xED1114; };
HWND ObjectsView::GetTreeView() { return *(HWND*)0xED1118; };
void ObjectsView::Refresh() { SendMessageA(GetWindow(), 1042, NULL, NULL); };

HWND CellView::GetWindow() { return *(HWND*)0xECFB78; };

HistoryManager* HistoryManager::GetSingleton() { return *(HistoryManager**)0xECFDF4; };
void HistoryManager::ClearHistoryForCurrentElement() { ThisCall(0x467CC0, this); };
void HistoryManager::Undo() { ThisCall(0x4665C0, this, 0); };
void HistoryManager::AddAction(int aiActionType, RenderWindow::SelectedData* apSelectedForms) { ThisCall(0x465D90, this, aiActionType, apSelectedForms); };

RenderData* RenderData::GetSingleton() { return *(RenderData**)0xED116C; };

NavMeshManager* NavMeshManager::GetSingleton() { return (NavMeshManager*)0xECEFF8; }

bool NavMeshManager::IsActive() { return *(bool**)0xED1412; }
void NavMeshManager::Undo() { ThisCall(0x4249F0, this); }

tList<HWND>* OpenWindows::GetWindowList() { return (tList<HWND>*)0xED033C; }

void AddFormsToListView(HWND listView, tList<TESForm>* forms, bool(__cdecl* filterFn)(TESForm*, void*), void* filterData)
{
	CdeclCall(0x47E410, listView, forms, filterFn, filterData);
}

void GetCellCoords(NiPoint3& pos, TESObjectCELL* cell)
{
	if (cell->IsInterior())
	{
		pos = NiPoint3::ZERO;
	}
	else
	{
		pos.x = 2048 + (cell->GetPosX() << 12);
		pos.y = 2048 + (cell->GetPosY() << 12);
		pos.z = 0;
	}
}

void OpenForm(TESForm* form, HWND parentHwnd)
{
	if (form)
	{
		if (form->typeID == kFormType_TESObjectCELL || form->typeID == kFormType_TESObjectLAND)
		{
			TESObjectCELL* cell = form->typeID == kFormType_TESObjectLAND
				? (TESObjectCELL*)(UInt32(form) + 0x34) : (TESObjectCELL*)form;

			NiPoint3 pos;
			GetCellCoords(pos, cell);
			CdeclCall(0x465490, &pos, cell);
			if (!cell->IsInterior())
			{
				cell->GetLandHeight(&pos, &pos.z);
				CdeclCall(0x4652D0, &pos);
			}
		}
		else
		{
			form->OpenDialog(parentHwnd, 0, 1);
		}
	}
}


void RunCallbackOnCell(TESObjectCELL* cell, void (*callback)(TESObjectREFR*))
{
	if (auto iter = cell->objectList.Head())
	{
		do
		{
			if (auto ref = iter->data)
			{
				callback(ref);
			}
		} while (iter = iter->next);
	}
}

void RunCallbackOnAllCellRefs(void (*callback)(TESObjectREFR*))
{
	auto tes = TES::GetSingleton();
	if (auto cell = tes->currentInterior)
	{
		RunCallbackOnCell(cell, callback);
	}
	else
	{
		Setting* uGridsToLoad = (Setting*)0xED6550;
		for (int x = 0, n = uGridsToLoad->data.uint; x < n; ++x)
		{
			for (int y = 0, n = uGridsToLoad->data.uint; y < n; ++y)
			{
				if (auto cell = tes->gridCellArray->GetCell(x, y))
				{
					if (cell->cellState == 6)
					{
						RunCallbackOnCell(cell, callback);
					}
				}
			}
		}
	}
}

BOOL __stdcall _CreateProcessA(LPSTR exePath, LPPROCESS_INFORMATION lpProcessInformation)
{
	STARTUPINFOA startupInfo;
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	return StdCall<BOOL>(*(UInt32*)0xD232D8, NULL, exePath, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, lpProcessInformation);
}

void* Window_GetExtraData(HWND hWnd, int code)
{
	return CdeclCall<void*>(0x47AB70, hWnd, code);
}

LRESULT __cdecl TESCSMain__WriteToStatusBar(unsigned int statusBarId, const char* msg)
{
	return CdeclCall<LRESULT>(0x4657A0, statusBarId, msg);
}