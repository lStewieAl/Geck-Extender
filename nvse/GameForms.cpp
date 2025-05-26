#include "GameForms.h"

// 0x6662D0
TESObjectCELL* TESWorldSpace::GetCellAtCoord(SInt32 aiX, SInt32 aiY) const {
	return ThisCall<TESObjectCELL*>(0x6662D0, this, aiX, aiY);
}

// 0x6699B0
TESObjectCELL* TESWorldSpace::GetCellAtPos(const NiPoint3& arPos) const {
	return GetCellAtCoord(SInt32(arPos.x) >> 12, SInt32(arPos.y) >> 12);
}

// 0x6361C0
NiNode* TESObjectCELL::Load3D() {
	return ThisCall<NiNode*>(0x6361C0, this);
}

// 0x637610
void TESObjectCELL::AttachModels(bool abSetupMopp) {
	ThisCall(0x637610, this, abSetupMopp);
}

// 0x629120
void TESObjectCELL::AddObjectsToLoadedRefCollection() {
	ThisCall(0x629120, this);
}

// 0x62CEF0
BSMultiBoundNode* TESObjectCELL::AddMultiBoundRef(TESObjectREFR* apMultiBound) {
	return ThisCall<BSMultiBoundNode*>(0x62CEF0, this, apMultiBound);
}

void TESBipedModelForm::SetBipedMask(UInt32 mask)
{
	bipedFlags = mask & 0xFF;
}

void  TESBipedModelForm::SetPath(const char* newPath, UInt32 whichPath, bool bFemalePath)
{
	String* toSet = NULL;

	switch (whichPath)
	{
	case ePath_Biped:
		toSet = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		toSet = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		toSet = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	}

	if (toSet)
		toSet->Set(newPath);
}

const char* TESBipedModelForm::GetPath(UInt32 whichPath, bool bFemalePath)
{
	String* pathStr = NULL;

	switch (whichPath)
	{
	case ePath_Biped:
		pathStr = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		pathStr = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		pathStr = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	}

	if (pathStr)
		return pathStr->m_data;
	else
		return "";
}

SInt8 TESActorBaseData::GetFactionRank(TESFaction* faction)
{
	for (tList<FactionListData>::Iterator iter = factionList.Begin(); !iter.End(); ++iter)
	{
		FactionListData* data = iter.Get();
		if (data && (data->faction == faction))
			return data->rank;
	}

	return -1;
}
void TESActorBaseData::SetFactionRank(TESFaction* faction, SInt8 rank)
{
	bool found = false;
	int count = 0;
	TESActorBaseData::FactionListData* entry = NULL;
	for (auto iter = factionList.Begin(); !iter.End(); ++iter)
	{
		entry = iter.Get();
		if (entry->faction == faction)
		{
			found = true;
			break;
		}
		count++;	// oh, iterators...
	}
	if (found)
	{
		if (rank > -1) factionList.GetNthItem(count)->rank = rank;
		else
		{	// SetFactionRank deletes the faction if the rank is negative
			factionList.RemoveNth(count);
			FormHeap_Free(entry);
		}
	}
	else
	{
		FactionListData* data = (FactionListData*)FormHeap_Allocate(sizeof(FactionListData));
		data->faction = faction;
		data->rank = rank;
		factionList.AddAt(data, eListEnd);
	}
}

static const UInt8 kHandGripTable[] =
{
	TESObjectWEAP::eHandGrip_Default,
	TESObjectWEAP::eHandGrip_1,
	TESObjectWEAP::eHandGrip_2,
	TESObjectWEAP::eHandGrip_3,
	TESObjectWEAP::eHandGrip_4,
	TESObjectWEAP::eHandGrip_5,
	TESObjectWEAP::eHandGrip_6,
};

UInt8 TESObjectWEAP::HandGrip() const
{
	for (UInt32 i = 0; i < sizeof(kHandGripTable) / sizeof(kHandGripTable[0]); i++)
		if (handGrip == kHandGripTable[i])
			return i;

	return 0;
}

void TESObjectWEAP::SetHandGrip(UInt8 _handGrip)
{
	if (_handGrip < sizeof(kHandGripTable) / sizeof(kHandGripTable[0]))
		handGrip = kHandGripTable[_handGrip];
}

UInt8 TESObjectWEAP::AttackAnimation() const
{
	switch (attackAnim) {
	case eAttackAnim_Default:		return 0;
	case eAttackAnim_Attack3:		return 1;
	case eAttackAnim_Attack4:		return 2;
	case eAttackAnim_Attack5:		return 3;
	case eAttackAnim_Attack6:		return 4;
	case eAttackAnim_Attack7:		return 5;
	case eAttackAnim_Attack8:		return 6;
	case eAttackAnim_AttackLeft:	return 7;
	case eAttackAnim_AttackLoop:	return 8;
	case eAttackAnim_AttackRight:	return 9;
	case eAttackAnim_AttackSpin:	return 10;
	case eAttackAnim_AttackSpin2:	return 11;
	case eAttackAnim_AttackThrow:	return 12;
	case eAttackAnim_AttackThrow2:	return 13;
	case eAttackAnim_AttackThrow3:	return 14;
	case eAttackAnim_AttackThrow4:	return 15;
	case eAttackAnim_AttackThrow5:	return 16;
	case eAttackAnim_PlaceMine:		return 17;
	case eAttackAnim_PlaceMine2:	return 18;

	default:
		return -1;
	}
}

void TESObjectWEAP::SetAttackAnimation(UInt8 _attackAnim)
{
	switch (_attackAnim) {
	case 0: {
		attackAnim = eAttackAnim_Default;
		break;
	}
	case 1: {
		attackAnim = eAttackAnim_Attack3;
		break;
	}
	case 2: {
		attackAnim = eAttackAnim_Attack4;
		break;
	}
	case 3: {
		attackAnim = eAttackAnim_Attack5;
		break;
	}
	case 4: {
		attackAnim = eAttackAnim_Attack6;
		break;
	}
	case 5: {
		attackAnim = eAttackAnim_Attack7;
		break;
	}
	case 6: {
		attackAnim = eAttackAnim_Attack8;
		break;
	}

	case 7: {
		attackAnim = eAttackAnim_AttackLeft;
		break;
	}
	case 8: {
		attackAnim = eAttackAnim_AttackLoop;
		break;
	}
	case 9: {
		attackAnim = eAttackAnim_AttackRight;
		break;
	}
	case 10: {
		attackAnim = eAttackAnim_AttackSpin;
		break;
	}
	case 11: {
		attackAnim = eAttackAnim_AttackSpin2;
		break;
	}
	case 12: {
		attackAnim = eAttackAnim_AttackThrow;
		break;
	}
	case 13: {
		eAttackAnim_AttackThrow2;
		break;
	}
	case 14: {
		attackAnim = eAttackAnim_AttackThrow3;
		break;
	}
	case 15: {
		attackAnim = eAttackAnim_AttackThrow4;
		break;
	}
	case 16: {
		attackAnim = eAttackAnim_AttackThrow5;
		break;
	}
	case 17: {
		attackAnim = eAttackAnim_PlaceMine;
		break;
	}
	case 18: {
		attackAnim = eAttackAnim_PlaceMine2;
		break;
	}
	default:
		break;
	}
}

TESObjectIMOD* TESObjectWEAP::GetItemMod(UInt8 which)
{
	TESObjectIMOD* pMod = NULL;
	switch (which) {
	case 1: pMod = itemMod1; break;
	case 2: pMod = itemMod2; break;
	case 3: pMod = itemMod3; break;
	}
	return pMod;
}

const char* TESPackage::TargetData::StringForTargetCode(UInt8 targetCode)
{
	switch (targetCode) {
	case TESPackage::kTargetType_Refr:
		return "Reference";
	case TESPackage::kTargetType_BaseObject:
		return "Object";
	case TESPackage::kTargetType_TypeCode:
		return "ObjectType";
	default:
		return NULL;
	}
}

UInt8 TESPackage::TargetData::TargetCodeForString(const char* targetStr)
{
	if (!_stricmp(targetStr, "REFERENCE"))
		return TESPackage::kTargetType_Refr;
	else if (!_stricmp(targetStr, "OBJECT"))
		return TESPackage::kTargetType_BaseObject;
	else if (!_stricmp(targetStr, "OBJECTTYPE"))
		return TESPackage::kTargetType_TypeCode;
	else
		return 0xFF;
}

TESPackage::TargetData* TESPackage::TargetData::Create()
{
	TargetData* data = (TargetData*)FormHeap_Allocate(sizeof(TargetData));

	// fill out with same defaults as editor uses
	data->count = 0;
	data->target.objectCode = TESPackage::kObjectType_Activators;
	data->targetType = TESPackage::kTargetType_TypeCode;

	return data;
}

TESPackage::TargetData* TESPackage::GetTargetData()
{
	if (!target)
		target = TargetData::Create();

	return target;
}

void TESPackage::SetTarget(TESObjectREFR* refr)
{
	TargetData* tdata = GetTargetData();
	tdata->targetType = kTargetType_Refr;
	tdata->target.refr = refr;
	tdata->count = 150;	//DefaultDistance
}

void TESPackage::SetCount(UInt32 aCount)
{
	if (target) {
		TargetData* tdata = GetTargetData();
		tdata->count = aCount;
	}
}

void TESPackage::SetTarget(TESForm* baseForm, UInt32 count)
{
	TargetData* tdata = GetTargetData();
	tdata->targetType = kTargetType_BaseObject;
	tdata->count = count;
	tdata->target.form = baseForm;
}

void TESPackage::SetTarget(UInt8 typeCode, UInt32 count)
{
	if (typeCode > 0 && typeCode < kObjectType_Max)
	{
		TargetData* tdata = GetTargetData();
		tdata->targetType = kTargetType_TypeCode;
		tdata->target.objectCode = typeCode;
		tdata->count = count;
	}
}

TESPackage::LocationData* TESPackage::LocationData::Create()
{
	LocationData* data = (LocationData*)FormHeap_Allocate(sizeof(LocationData));

	data->locationType = kPackLocation_CurrentLocation;
	data->object.form = NULL;
	data->radius = 0;

	return data;
}

TESPackage::LocationData* TESPackage::GetLocationData()
{
	if (!location)
		location = LocationData::Create();

	return location;
}

bool TESPackage::IsFlagSet(UInt32 flag)
{
	return (packageFlags & flag) == flag;
}

void TESPackage::SetFlag(UInt32 flag, bool bSet)
{
	if (bSet)
		packageFlags |= flag;
	else
		packageFlags &= ~flag;

	// handle either-or flags
	switch (flag)
	{
	case kPackageFlag_LockDoorsAtStart:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtStart) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtStart, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtStart:
		if (IsFlagSet(kPackageFlag_LockDoorsAtStart) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtStart, !bSet);
		break;
	case kPackageFlag_LockDoorsAtEnd:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtEnd) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtEnd, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtEnd:
		if (IsFlagSet(kPackageFlag_LockDoorsAtEnd) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtEnd, !bSet);
		break;
	case kPackageFlag_LockDoorsAtLocation:
		if (IsFlagSet(kPackageFlag_UnlockDoorsAtLocation) == bSet)
			SetFlag(kPackageFlag_UnlockDoorsAtLocation, !bSet);
		break;
	case kPackageFlag_UnlockDoorsAtLocation:
		if (IsFlagSet(kPackageFlag_LockDoorsAtLocation) == bSet)
			SetFlag(kPackageFlag_LockDoorsAtLocation, !bSet);
		break;
	}
}

static const char* TESPackage_ObjectTypeStrings[TESPackage::kObjectType_Max] =
{
	"NONE", "Activators", "Armors", "Books", "Clothing", "Containers", "Doors", "Ingredients", "Lights", "Miscellaneous", "Flora", "Furniture",
	"Weapons: Any", "Ammo", "NPCs", "Creatures", "Keys", "Alchemy", "Food", "All: Combat Wearable", "All: Wearable", "Weapons: Ranged", "Weapons: Melee",
	"Weapons: NONE", "Actor Effects: Any", "Actor Effects: Range Target", "Actor Effects: Range Touch", "Actor Effects: Range Self"
};

// add 1 to code before indexing
static const char* TESPackage_DayStrings[] = {
	"Any", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Weekdays", "Weekends", "MWF", "TT"
};

// add 1
static const char* TESPackage_MonthString[] = {
	"Any", "January", "February", "March", "April", "May", "June", "July", "August", "September",
	"October", "November", "December", "Spring", "Summer", "Autumn", "Winter"
};

static const char* TESPackage_LocationStrings[] = {
	"Reference", "Cell", "Current", "Editor", "Object", "ObjectType"
};

static const char* TESPackage_TypeStrings[] = {
	"Find", "Follow", "Escort", "Eat", "Sleep", "Wander", "Travel", "Accompany", "UseItemAt", "Ambush",
	"FleeNotCombat", "Sandbox", "Patrol", "Guard", "Dialogue", "UseWeapon"
};

static const char* TESPackage_ProcedureStrings[] = {
	"TRAVEL", "ACTIVATE", "ACQUIRE", "WAIT", "DIALOGUE", "GREET", "GREET DEAD", "WANDER", "SLEEP",
	"OBSERVE COMBAT", "EAT", "FOLLOW", "ESCORT", "COMBAT", "ALARM", "PURSUE", "FLEE", "DONE", "YELD",
	"TRAVEL TARGET", "CREATE FOLLOW", "GET UP", "MOUNT HORSE", "DISMOUNT HORSE", "DO NOTHING", "UNKNOWN 019", "UNKNOWN 01B",
	"ACCOMPANY", "USE ITEM AT", "AIM", "NOTIFY", "SANDMAN", "WAIT AMBUSH", "SURFACE", "WAIT FOR SPELL", "CHOOSE CAST",
	"FLEE NON COMBAT", "REMOVE WORN ITEMS", "SEARCH", "CLEAR MOUNT POSITION", "SUMMON CREATURE DEFEND", "AVOID AREA",
	"UNEQUIP ARMOR", "PATROL", "USE WEAPON", "DIALOGUE ACTIVATE", "GUARD", "SANDBOX", "USE IDLE MARKER", "TAKE BACK ITEM",
	"SITTING", "MOVEMENT BLOCKED", "CANIBAL FEED",
};

const char* TESPackage::StringForPackageType(UInt32 pkgType)
{
	if (pkgType < kPackType_MAX) {
		return TESPackage_TypeStrings[pkgType];
	}
	else {
		return "";
	}
}

const char* TESPackage::StringForObjectCode(UInt8 objCode)
{
	if (objCode < kObjectType_Max)
		return TESPackage_ObjectTypeStrings[objCode];

	return "";
}

UInt8 TESPackage::ObjectCodeForString(const char* objString)
{
	for (UInt32 i = 0; i < kObjectType_Max; i++) {
		if (!_stricmp(objString, TESPackage_ObjectTypeStrings[i]))
			return i;
	}

	return kObjectType_Max;
}

#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525 || RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
static const char** s_procNames = (const char**)0x011A3CC0;
#elif EDITOR
#else
#error unsupported Fallout version
#endif

const char* TESPackage::StringForProcedureCode(eProcedure proc)
{
	if (proc < kProcedure_MAX)
		return TESPackage_ProcedureStrings[proc];

	return "";
}

//const char* TESPackage::StringForProcedureCode(eProcedure proc, bool bRemovePrefix)
//{
//	static size_t prefixLen = strlen("PROCEDURE_");
//
//	const char* name = NULL;
//	// special-case "AQUIRE" (sic) to fix typo in game executable
//	if (proc == TESPackage::kProcedure_ACQUIRE) {
//		name = "PROCEDURE_ACQUIRE";
//	}
//	else {
//		name = (proc <= TESPackage::kProcedure_MAX) ? s_procNames[proc] : NULL;
//	}
//
//	if (name && bRemovePrefix) {
//		name += prefixLen;
//	}
//
//	return name;
//}

const char* TESPackage::PackageTime::DayForCode(UInt8 dayCode)
{
	dayCode += 1;
	if (dayCode >= sizeof(TESPackage_DayStrings))
		return "";
	return TESPackage_DayStrings[dayCode];
}

const char* TESPackage::PackageTime::MonthForCode(UInt8 monthCode)
{
	monthCode += 1;
	if (monthCode >= sizeof(TESPackage_MonthString))
		return "";
	return TESPackage_MonthString[monthCode];
}

UInt8 TESPackage::PackageTime::CodeForDay(const char* dayStr)
{
	for (UInt8 i = 0; i < sizeof(TESPackage_DayStrings); i++) {
		if (!_stricmp(dayStr, TESPackage_DayStrings[i])) {
			return i - 1;
		}
	}

	return kWeekday_Any;
}

UInt8 TESPackage::PackageTime::CodeForMonth(const char* monthStr)
{
	for (UInt8 i = 0; i < sizeof(TESPackage_MonthString); i++) {
		if (!_stricmp(monthStr, TESPackage_MonthString[i])) {
			return i - 1;
		}
	}

	return kMonth_Any;
}

const char* TESPackage::LocationData::StringForLocationCode(UInt8 locCode)
{
	if (locCode < kPackLocation_Max)
		return TESPackage_LocationStrings[locCode];
	return "";
}

const char* TESPackage::LocationData::StringForLocationCodeAndData(void)
{
#define resultSize 256
	static char result[resultSize];
	if (locationType < kPackLocation_Max) {
		switch (locationType) {
		case kPackLocation_NearReference:
		case kPackLocation_InCell:
		case kPackLocation_ObjectID:
			if (object.form)
				stbsp_snprintf(result, resultSize, "%s \"%s\" [%08X] with a radius of %u", TESPackage_LocationStrings[locationType], object.form->GetTheName(),
					object.form->refID, radius);
			else
				stbsp_snprintf(result, resultSize, "%s \"\" [%08X] with a radius of %u", TESPackage_LocationStrings[locationType], 0, radius);
			break;
		case kPackLocation_ObjectType:
			stbsp_snprintf(result, resultSize, "%s \"%s\" [%04X] with a radius of %u", TESPackage_LocationStrings[locationType], StringForObjectCode(object.objectCode),
				object.objectCode, radius);
			break;
		default:
			stbsp_snprintf(result, resultSize, "%s with a radius of %u", TESPackage_LocationStrings[locationType], radius);
			break;
		}
		return result;
	}
	return "";
}

UInt8 TESPackage::LocationData::LocationCodeForString(const char* locStr)
{
	for (UInt32 i = 0; i < kPackLocation_Max; i++)
		if (!_stricmp(locStr, TESPackage_LocationStrings[i]))
			return i;
	return kPackLocation_Max;
}

const char* TESFaction::GetNthRankName(UInt32 whichRank, bool bFemale)
{
	TESFaction::Rank* rank = ranks.GetNthItem(whichRank);
	if (!rank)
		return NULL;
	else
		return bFemale ? rank->femaleName.CStr() : rank->name.CStr();
}

void TESFaction::SetNthRankName(const char* newName, UInt32 whichRank, bool bFemale)
{
	TESFaction::Rank* rank = ranks.GetNthItem(whichRank);
	if (rank)
	{
		if (bFemale)
			rank->femaleName.Set(newName);
		else
			rank->name.Set(newName);
	}
}

UInt32 EffectItemList::CountItems() const
{
	return list.Count();
}

EffectItem* EffectItemList::ItemAt(UInt32 whichItem)
{
	return list.GetNthItem(whichItem);
}

Script* EffectSetting::SetScript(Script* newScript)
{
	Script* oldScript = NULL;
	if (1 == archtype)
	{
		oldScript = (Script*)associatedItem;
		associatedItem = (TESForm*)newScript;
	};
	return oldScript;
};

Script* EffectSetting::RemoveScript()
{
	return SetScript(NULL);
};

TESContainer* TESContainer::Create()
{
	TESContainer* container = (TESContainer*)FormHeap_Allocate(sizeof(TESContainer));
	ThisCall(0x4F4440, container); // TESContainer::TESContainer
	return container;
}

void TESContainer::Destroy()
{
	ThisCall(0x4F5760, this);
}

void TESLeveledList::CalculateCurrentFormList(signed int ausLevel, int ausCount, TESContainer* apOut, int aeAllBelowForce)
{
	ThisCall(0x4FF6B0, this, ausLevel, ausCount, apOut, aeAllBelowForce);
}

void TESForm::SetDisabled(bool abDisabled)
{
	if (abDisabled)
	{
		flags |= kFormFlags_InitiallyDisabled;
	}
	else
	{
		flags &= ~kFormFlags_InitiallyDisabled;
	}
}