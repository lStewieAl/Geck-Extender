#include "GameData.h"

DataHandler* DataHandler::GetSingleton() {
	DataHandler** g_dataHandler = (DataHandler**)0xED3B0C;
	return *g_dataHandler;
}

class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo)
	{
		return _stricmp(modInfo->name, m_stringToFind) == 0;
	}
};

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	LoadedModFinder finder = LoadedModFinder(modName);
	return modList.modInfoList.Find(finder);
}

const ModInfo ** DataHandler::GetActiveModList()
{
	static const ModInfo* activeModList[0x100] = { 0 };

	if (!(*activeModList))
	{
		UInt16 index = 0;
		for (index = 0  ; index < DataHandler::GetSingleton()->modList.modInfoList.Count() ; index++)
		{
			ModInfo* entry = DataHandler::GetSingleton()->modList.modInfoList.GetNthItem(index);
			if (entry->IsLoaded())
				activeModList[index] = entry;
		}
	}

	return activeModList;
}

UInt8 DataHandler::GetModIndex(const char* modName)
{
	LoadedModFinder finder = LoadedModFinder(modName);
	return modList.modInfoList.GetIndexOf(finder);
}

const char* DataHandler::GetNthModName(UInt32 modIndex)
{
	const ModInfo** activeModList = GetActiveModList();
	if (modIndex < GetActiveModCount() && activeModList[modIndex])
		return activeModList[modIndex]->name;
	else
		return "";
}

ModInfo* DataHandler::GetNthFile(int n)
{
	return ThisCall<ModInfo*>(0x4CF380, DataHandler::GetSingleton(), n);
}

struct IsModLoaded
{
	bool Accept(ModInfo* pModInfo) const {
		return pModInfo->IsLoaded();
	}
};

UInt8 DataHandler::GetActiveModCount() const
{
	IsModLoaded isLoaded = IsModLoaded();
	UInt32 count = modList.modInfoList.CountIf(isLoaded);
	return count;
}

ModInfo::ModInfo() {
	//
};

ModInfo::~ModInfo() {
	//
};

TESObjectREFR* DataHandler::CreateReferenceAtLocation(TESBoundObject* object, const NiPoint3* aPos, const NiPoint3* aRot, float radius)
{
	return ThisCall<TESObjectREFR*>(0x4D0940, this, object, aPos, aRot, radius, 0, 0);
}