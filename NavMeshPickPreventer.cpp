#include "BetterFloatingFormList.h"
#include "GameTypes.h"
#include "GameObjects.h"
#include "GameAPI.h"
#include "GeckUtility.h"

#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

extern HWND g_MainHwnd;

namespace NavMeshPickPreventer
{
	char IniPath[MAX_PATH];

	void WriteINI();

	tList<UInt32> ignoredRefs;
	std::vector<std::string> failedToResolveIds; // editorIDs not found in the loaded plugins
	UInt32 lastPickedRefID;
	UInt32 lastPickedBaseFormID;
	bool __fastcall OnPickNavMeshNodeGetShouldSkip(TESObjectREFR* ref)
	{
		if (ThisCall<bool>(0x44FEB0, ref) || (ref->baseForm && ref->baseForm->typeID == kFormType_Activator)) return true;

		bool bIsIgnoredRef = ignoredRefs.IsInList((UInt32*)ref->refID);
		bool bIsIgnoredBaseForm = ref->baseForm && ignoredRefs.IsInList((UInt32*)ref->baseForm->refID);
		if (bIsIgnoredRef || bIsIgnoredBaseForm)
		{
#ifdef _DEBUG
			EditorUI_Log("Ignored %s (%08X)", ref->GetEditorID(), ref->refID);
#endif
			return true;
		}

		if (lastPickedRefID != ref->refID)
		{
#ifdef _DEBUG
			EditorUI_Log("Picked %s (%08X)", ref->GetEditorID(), ref->refID);
#endif
			lastPickedRefID = ref->refID;
			lastPickedBaseFormID = ref->baseForm ? ref->baseForm->refID : 0;
		}
		return false;
	}

	bool AddFormToIgnoredList(TESForm* form)
	{
		UInt32* refID = (UInt32*)form->refID;
		if (!ignoredRefs.IsInList(refID))
		{
			ignoredRefs.Insert(refID);
			return true;
		}
		return false;
	}

	bool AddAllSelectedRefsToIgnoredList()
	{
		bool bItemsAdded = false;

		auto selected = RenderWindow::SelectedData::GetSelected();
		if (auto selectedFormIter = selected->selectedForms)
		{
			do
			{
				if (auto form = selectedFormIter->ref)
				{
					if (form->baseForm)
					{
						AddFormToIgnoredList(form->baseForm);
						bItemsAdded = true;
					}
				}
			} while (selectedFormIter = selectedFormIter->next);
		}
		return bItemsAdded;
	}

	bool AddLastRefToIgnoredList()
	{
		if (auto ref = LookupFormByID(lastPickedBaseFormID))
		{
			if (!ignoredRefs.IsInList((UInt32*)lastPickedRefID))
			{
				ignoredRefs.Insert((UInt32*)lastPickedRefID);
				Console_Print("Added %s (%08X) to ignored base forms list", ref->GetEditorID(), ref->refID);
				return true;
			}
		}
		else if (auto ref = LookupFormByID(lastPickedRefID))
		{
			if (!ignoredRefs.IsInList((UInt32*)lastPickedRefID))
			{
				ignoredRefs.Insert((UInt32*)lastPickedRefID);
				Console_Print("Added %s (%08X) to ignored refs list", ref->GetEditorID(), ref->refID);
				return true;
			}
		}
		return false;
	}

	LRESULT CALLBACK SubclassedListViewProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		if (Message == WM_KEYDOWN)
		{
			if (wParam == 'A' && GetAsyncKeyState(VK_CONTROL) < 0)
			{
				SelectAllItemsInListView(hWnd);
			}
			else if (wParam == VK_DELETE)
			{
				bool bFormsDeleted = false;
				int iSelected = -1;
				while ((iSelected = ListView_GetNextItem(hWnd, iSelected, LVNI_SELECTED)) != -1)
				{
					if (auto form = GetNthListForm(hWnd, iSelected))
					{
						ignoredRefs.Remove((UInt32*)form->refID);
						bFormsDeleted = true;
					}
					ListView_DeleteItem(hWnd, iSelected);
					iSelected--; // because the indices shift after deletion, decrement iSelected to ensure no item is skipped
				}

				if (bFormsDeleted)
				{
					WriteINI();
				}

				return true;
			}
		}
		else if (Message == BetterFloatingFormList::BFL_ADDED_ITEMS)
		{
			auto formList = (tList<TESForm>*)wParam;
			if (formList && formList->Head())
			{
				auto iter = formList->Head();
				do
				{
					if (auto form = iter->item)
					{
						AddFormToIgnoredList(form);
					}
				} while (iter = iter->next);

				WriteINI();
			}
		}
		return DefSubclassProc(hWnd, Message, wParam, lParam);
	}

	void SubclassListView(HWND hListView)
	{
		SetWindowSubclass(hListView, SubclassedListViewProc, 0, 0);
	}

	WNDPROC originalWindowCallback;
	LRESULT CALLBACK WindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		auto result = CallWindowProc(originalWindowCallback, Hwnd, Message, wParam, lParam);
		if (Message == WM_INITDIALOG)
		{
			// prevent the delete button deleting items from the list as it's misleading if users expect it to actually remove modified forms
			auto listView = GetDlgItem(Hwnd, 1018);
			SubclassListView(listView);
		}
		return result;
	}

	void ShowList()
	{
		originalWindowCallback = *(WNDPROC*)0x44BB19;
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);

		tList<TESForm> ignoredForms;
		ignoredForms.Init();

		auto iter = ignoredRefs.Head();
		do
		{
			if (auto form = LookupFormByID((UInt32)iter->item))
			{
				ignoredForms.Insert(form);
			}
		} while (iter = iter->next);

		auto hWnd = CreateDialogParamA(hInstance, (LPCSTR)189, g_MainHwnd, (DLGPROC)WindowCallback, (LPARAM)((char*)(&ignoredForms)) - 0xC);
		SendMessageA(hWnd, WM_SETTEXT, 0, (LPARAM)"Ignored Forms");
	}

	std::string ConcatenateIDs()
	{
		std::stringstream concatenatedIDs;

		auto iter = ignoredRefs.Head();
		bool isFirstItem = true;
		do
		{
			if (auto form = LookupFormByID((UInt32)iter->item))
			{
				if (!isFirstItem)
				{
					concatenatedIDs << ',';
				}
				isFirstItem = false;
				concatenatedIDs << (form->GetEditorID());
			}
		} while (iter = iter->next);

		for (const std::string& id : failedToResolveIds)
		{
			if (!isFirstItem)
			{
				concatenatedIDs << ',';
			}
			isFirstItem = false;
			concatenatedIDs << id;
		}

		return concatenatedIDs.str();
	}

	constexpr const char* INI_SECTION = "NavMesh";
	constexpr const char* INI_KEY = "sIgnoredPickEditorIDs";
	std::vector<std::string> ReadIDs()
	{
		char buffer[0x10000];
		int len = GetPrivateProfileStringA(INI_SECTION, INI_KEY, "", buffer, sizeof(buffer), IniPath);

		std::vector<std::string> ids;
		std::string idsStr = std::string(buffer);
		std::stringstream ss(idsStr);
		std::string id;
		while (getline(ss, id, ',')) {
			ids.push_back(id);
		}

		return ids;
	}

	void WriteINI()
	{
		WritePrivateProfileStringA(INI_SECTION, INI_KEY, ConcatenateIDs().c_str(), IniPath);
	}

	void ReadINI()
	{
		ignoredRefs.RemoveAll();
		failedToResolveIds.clear();

		auto ids = ReadIDs();
		for (const std::string& id : ids)
		{
			if (auto form = LookupFormByName(id.c_str()))
			{
				ignoredRefs.AddAt((UInt32*)form->refID, 0);
			}
			else
			{
				failedToResolveIds.push_back(id);
#ifdef _DEBUG
				Console_Print("NavMeshPickPreventer: Failed to find form: %s", id.c_str());
#endif
			}
		}
	}

	void Init()
	{
		WriteRelCall(0x45C5B7, UInt32(OnPickNavMeshNodeGetShouldSkip));

		GetModuleFileNameA(NULL, IniPath, MAX_PATH);
		strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\GeckExtender\\NavMeshIgnore\\config.ini");
	}

	void PostLoadPlugins()
	{
		ReadINI();
	}

	void OnKeyDown(bool bShiftHeld)
	{
		bool bWriteINI = false;
		if (bShiftHeld)
		{
			bWriteINI = AddAllSelectedRefsToIgnoredList();
		}
		else
		{
			bWriteINI = AddLastRefToIgnoredList();
		}

		if (bWriteINI)
		{
			WriteINI();
		}
	}
}