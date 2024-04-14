#include "GameTypes.h"
#include "GameObjects.h"
#include "GameAPI.h"

#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

namespace NavMeshPickPreventer
{
	char IniPath[MAX_PATH];

	tList<UInt32> ignoredRefs;
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

	void AddLastRefToIgnoredList()
	{
		if (auto ref = LookupFormByID(lastPickedBaseFormID))
		{
			if (!ignoredRefs.IsInList((UInt32*)lastPickedRefID))
			{
				ignoredRefs.AddAt((UInt32*)lastPickedRefID, 0);
				Console_Print("Added %s (%08X) to ignored base forms list", ref->GetEditorID(), ref->refID);
			}
		}
		else if (auto ref = LookupFormByID(lastPickedRefID))
		{
			if (!ignoredRefs.IsInList((UInt32*)lastPickedRefID))
			{
				ignoredRefs.AddAt((UInt32*)lastPickedRefID, 0);
				Console_Print("Added %s (%08X) to ignored refs list", ref->GetEditorID(), ref->refID);
			}
		}
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

		return concatenatedIDs.str();
	}

	constexpr const char* INI_SECTION = "NavMesh";
	constexpr const char* INI_KEY = "sIgnoredPickEditorIDs";
	std::vector<std::string> ReadIDs()
	{
		char buffer[0x1000];
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

		auto ids = ReadIDs();
		for (const std::string& id : ids)
		{
			if (auto form = LookupFormByName(id.c_str()))
			{
				ignoredRefs.AddAt((UInt32*)form->refID, 0);
			}
			else
			{
				Console_Print("NavMeshPickPreventer: Failed to find form: %s", id.c_str());
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

	void OnKeyDown()
	{
		AddLastRefToIgnoredList();
		WriteINI();
	}
}