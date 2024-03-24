#include "GameObjects.h"
#include "GameRTTI.h"
#include "GameExtraData.h"
#include "GameTasks.h"
#include "GameUI.h"
#include "SafeWrite.h"
#include "NiObjects.h"

#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
	static const UInt32 s_TESObject_REFR_init	= 0x0055A2F0;	// TESObject_REFR initialization routine (first reference to s_TESObject_REFR_vtbl)
	static const UInt32	s_Actor_EquipItem		= 0x0088C650;	// maybe, also, would be: 007198E0 for FOSE	4th call from the end of TESObjectREFR::RemoveItem (func5F)
	static const UInt32	s_Actor_UnequipItem		= 0x0088C790;	// maybe, also, would be: 007133E0 for FOSE next sub after EquipItem
	static const UInt32 s_TESObjectREFR__GetContainer	= 0x0055D310;	// First call in REFR::RemoveItem
	static const UInt32 s_TESObjectREFR_Set3D	= 0x005702E0;	// void : (const char*)
	static const UInt32 s_PlayerCharacter_GetCurrentQuestTargets	= 0x00952BA0;	// BuildedQuestObjectiveTargets* : (void)
	static const UInt32 s_PlayerCharacter_GenerateNiNode	= 0x0094E1D0; // Func0072
	static const UInt32 kPlayerUpdate3Dpatch = 0x0094EB7A;
	static const UInt32 TESObjectREFR_Set3D = 0x0094EB40;
	static const UInt32 ValidBip01Names_Destroy = 0x00418E00;
	static const UInt32 ExtraAnimAnimation_Destroy = 0x00418D20;
	static const UInt32 RefNiRefObject_ReplaceNiRefObject = 0x0066B0D0;
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
	static const UInt32 s_TESObject_REFR_init	= 0x0055a6b0;	// TESObject_REFR initialization routine (first reference to s_TESObject_REFR_vtbl)
	static const UInt32	s_Actor_EquipItem		= 0x0088c2b0;	// maybe, also, would be: 007198E0 for FOSE	4th call from the end of TESObjectREFR::RemoveItem (func5F)
	static const UInt32	s_Actor_UnequipItem		= 0x0088c3f0;	// maybe, also, would be: 007133E0 for FOSE next sub after EquipItem
	static const UInt32 s_TESObjectREFR__GetContainer	= 0x0055D6B0;	// First call in REFR::RemoveItem
	static const UInt32 s_TESObjectREFR_Set3D	= 0x004E0F80;	// void : (const char*)
	static const UInt32 s_PlayerCharacter_GetCurrentQuestTargets	= 0x00952450;	// BuildedQuestObjectiveTargets* : (void)
	static const UInt32 s_PlayerCharacter_GenerateNiNode	= 0x0094DA80; // Func0072
	static const UInt32 kPlayerUpdate3Dpatch = 0x0094E42A;
	static const UInt32 TESObjectREFR_Set3D = 0x005705A0;
	static const UInt32 ValidBip01Names_Destroy = 0x00418C60;
	static const UInt32 ExtraAnimAnimation_Destroy = 0x00418B80;
	static const UInt32 RefNiRefObject_ReplaceNiRefObject = 0x009CA4F0;
#elif EDITOR
#else
#error
#endif

ScriptEventList* TESObjectREFR::GetEventList() const
{
	BSExtraData* xData = extraDataList.GetByType(kExtraData_Script);
	if (xData)
	{
		ExtraScript* xScript = DYNAMIC_CAST(xData, BSExtraData, ExtraScript);
		if (xScript)
			return xScript->eventList;
	}

	return 0;
}

TESContainer* TESObjectREFR::GetContainer()
{
	return (TESContainer*)ThisStdCall(s_TESObjectREFR__GetContainer, this);	// First call in REFR::RemoveItem
}

TESObjectCELL* TESObjectREFR::GetParentCell()
{
	if(parentCell) return parentCell;
	ExtraPersistentCell* xcell = (ExtraPersistentCell*)extraDataList.GetByType(kExtraData_PersistentCell);
	return xcell ? xcell->persistentCell : NULL;
}

bool TESObjectREFR::IsMapMarker()
{
	if (baseForm) {
		return baseForm->refID == 0x010;	// Read from the geck. Why is OBSE looking for a pointer ?
	}
	else {
		return false;
	}
}

bool TESObjectREFR::Update3D()
{
	Set3D(NULL, true);
	ModelLoader::GetSingleton()->QueueReference(this, 1, false);
	return true;
}

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj)	// For LevelledForm, find real baseForm, not temporary one.
{
	ExtraLeveledCreature * pXCreatureData = NULL;

	if (thisObj) {
		pXCreatureData = GetByTypeCast(thisObj->extraDataList, LeveledCreature);
		if (pXCreatureData && pXCreatureData->baseForm) {
			return pXCreatureData->baseForm;
		}
	}
	if (thisObj && thisObj->baseForm) {
		return thisObj->baseForm;
	}
	return NULL;
}

void Actor::EquipItem(TESForm * objType, UInt32 equipCount, ExtraDataList* itemExtraList, UInt32 unk3, bool lockEquip, UInt32 unk5)
{
	ThisStdCall(s_Actor_EquipItem, this, objType, equipCount, itemExtraList, unk3, lockEquip, unk5);
}

void Actor::UnequipItem(TESForm* objType, UInt32 unk1, ExtraDataList* itemExtraList, UInt32 unk3, bool lockUnequip, UInt32 unk5)
{
	ThisStdCall(s_Actor_UnequipItem, this, objType, unk1, itemExtraList, unk3, lockUnequip, unk5);
}

EquippedItemsList Actor::GetEquippedItems()
{
	EquippedItemsList itemList;
	ExtraContainerDataArray outEntryData;
	ExtraContainerExtendDataArray outExtendData;

	ExtraContainerChanges	* xChanges = static_cast <ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges));
	if(xChanges) {
		UInt32 count = xChanges->GetAllEquipped(outEntryData, outExtendData);
		for (UInt32 i = 0; i < count ; i++)
			itemList.push_back(outEntryData[i]->type);

	}

	return itemList;
}

ExtraContainerDataArray	Actor::GetEquippedEntryDataList()
{
	ExtraContainerDataArray itemArray;
	ExtraContainerExtendDataArray outExtendData;

	ExtraContainerChanges	* xChanges = static_cast <ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges));
	if(xChanges)
		xChanges->GetAllEquipped(itemArray, outExtendData);

	return itemArray;
}

ExtraContainerExtendDataArray	Actor::GetEquippedExtendDataList()
{
	ExtraContainerDataArray itemArray;
	ExtraContainerExtendDataArray outExtendData;

	ExtraContainerChanges	* xChanges = static_cast <ExtraContainerChanges *>(extraDataList.GetByType(kExtraData_ContainerChanges));
	if(xChanges)
		xChanges->GetAllEquipped(itemArray, outExtendData);

	return outExtendData;
}

