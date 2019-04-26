#pragma once

#include "GameForms.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"

class TESObjectCELL;
struct ScriptEventList;
class ActiveEffect;
class NiNode;
class Animation;

// 008
class TESChildCell
{
public:
	TESChildCell();
	~TESChildCell();

	// no virtual destructor
	virtual TESObjectCELL *	GetPersistentCell(void);		// 000

//	void	** vtbl;	// 00
};

// 68
class TESObjectREFR : public TESForm
{
public:
	MEMBER_FN_PREFIX(TESObjectREFR);

	TESObjectREFR();
	~TESObjectREFR();

	virtual void		Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void		Unk_4F(void);
	virtual void		Unk_50(void);
	virtual void		Unk_51(void);
	virtual bool		CastShadows(void);
	virtual void		Unk_53(void);
	virtual void		Unk_54(void);
	virtual void		Unk_55(void);
	virtual void		Unk_56(void);
	virtual void		Unk_57(void);
	virtual void		Unk_58(void);
	virtual void		Unk_59(void);
	virtual void		Unk_5A(void);
	virtual void		Unk_5B(void);
	virtual void		Unk_5C(void);
	virtual void		Unk_5D(void);
	virtual void		Unk_5E(void);
	virtual TESObjectREFR*		RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 count, UInt32 unk3, UInt32 unk4, TESObjectREFR* destRef, 
		UInt32 unk6, UInt32 unk7, UInt32 unk8, UInt8 unk9);	// 40 unk2 quantity? Returns the reference assigned to the removed item.
	virtual void		Unk_60(void);
	virtual bool		EquipArmor(TESForm *item, UInt32 count, ExtraDataList *xData, UInt32 arg4);
	virtual void		Unk_62(void);	// Linked to Unequip (and or equip maybe)
	virtual void		Unk_63(void);
	virtual void		AddItem(TESForm *item, ExtraDataList *xDataList, UInt32 quantity);
	virtual void		Unk_65(void);
	virtual void		Unk_66(void);
	virtual void		Unk_67(void);						// Actor: GetMagicEffectList
	virtual bool		GetIsChildSize(bool checkHeight);	// 068 Actor: GetIsChildSize
	virtual UInt32		GetActorUnk0148(void);				// result can be interchanged with baseForm, so TESForm* ?
	virtual void		SetActorUnk0148(UInt32 arg0);
	virtual void		Unk_6B(void);
	virtual void		Unk_6C(void);	// REFR: GetBSFaceGenNiNodeSkinned
	virtual void		Unk_6D(void);	// REFR: calls 006C
	virtual void		Unk_6E(void);	// MobileActor: calls 006D then NiNode::Func0040
	virtual void		Unk_6F(void);
	virtual void		Unload3D(void);
	virtual void		AnimateNiNode(void);					// same in FOSE ! identical to Func0052 in OBSE which says (inits animation-related data, and more)
	virtual void		GenerateNiNode(bool arg0);				// same in FOSE !
	virtual void		Set3D(NiNode* niNode, bool unloadArt);	// same in FOSE !
	virtual NiNode *	GetNiNode(void);						// same in FOSE !
	virtual void		Unk_75(void);
	virtual void		Unk_76(void);
	virtual void		Unk_77(void);
	virtual void		Unk_78(void);
	virtual Animation *	GetAnimation(void);			// 0079
	virtual ValidBip01Names * GetValidBip01Names(void);	// 007A	Character only
	virtual ValidBip01Names * CallGetValidBip01Names(void);
	virtual void		SetValidBip01Names(ValidBip01Names validBip01Names);
	virtual void		GetPos(void);				// GetPos or GetDistance
	virtual void		Unk_7E(UInt32 arg0);
	virtual void		Unk_7F(void);
	virtual void		Unk_80(UInt32 arg0);
	virtual void		Unk_81(UInt32 arg0);
	virtual void		Unk_82(void);
	virtual UInt32		Unk_83(void);
	virtual void		Unk_84(UInt32 arg0);
	virtual UInt32		Unk_85(void);			// 0 or GetActor::Unk01AC
	virtual bool		Unk_86(void);			// return false for Projectile, Actor and Creature, true for character and PlayerCharacter
	virtual bool		Unk_87(void);			// seems to always return 0
	virtual bool		Unk_88(void);			// seems to always return 0
	virtual bool		IsProjectile();
	virtual void		Unk_8A(void);			// SetParentCell (Interior only ?)
	virtual bool		IsDead(bool arg0);			// IsDead = HasNoHealth (baseForm health <= 0 or Flags bit23 set)
	virtual bool		Unk_8C(void);
	virtual bool		Unk_8D(void);
	virtual void		Unk_8E(void);
	virtual void		Unk_8F(void);

	enum {
		kFlags_Unk00000002			= 0x00000002,
		kFlags_Deleted				= 0x00000020,		// refr removed from .esp or savegame
		kFlags_Taken				= kFlags_Deleted | kFlags_Unk00000002,
		kFlags_Persistent			= 0x00000400,		//shared bit with kFormFlags_QuestItem
		kFlags_Temporary			= 0x00004000,
		kFlags_IgnoreFriendlyHits	= 0x00100000,
		kFlags_Destroyed			= 0x00800000,
		kChanged_Inventory			= 0x08000000,
	};

	struct RenderState
	{
		UInt32	unk00;
		UInt32	unk04;
		float	waterLevel;		// 08 //
		float	unk0C;
		UInt32	unk10;		// flags most likely
		NiNode	* niNode;	// same in FOSE
		//NiNode	* niNode2;
		// possibly more, need to find alloc
	};

	struct EditorData {
		UInt32	unk00;	// 00
	};
	// 0C

#ifdef EDITOR
	EditorData	editorData;			// +04
#endif

	TESChildCell	childCell;				// 018
	TESSound		* loopSound;			// 01C
	TESForm			* baseForm;				// 020
	
	float			rotX, rotY, rotZ;		// 024 - either public or accessed via simple inline accessor common to all child classes
	float			posX, posY, posZ;		// 030 - seems to be private
	float			scale;					// 03C 

	TESObjectCELL	* parentCell;			// 040
	ExtraDataList	extraDataList;			// 044
	RenderState		* renderState;			// 064	- (05C in FOSE)

	ScriptEventList *	GetEventList() const;

	bool IsTaken() const { return (flags & kFlags_Taken) == kFlags_Taken; } // Need to implement
	bool IsPersistent() const { return (flags & kFlags_Persistent) != 0; }
	bool IsTemporary() { return (flags & kFlags_Temporary) ? true : false; }
	bool IsDeleted() { return (flags & kFlags_Deleted) ? true : false; }
	bool IsDestroyed() { return (flags & kFlags_Destroyed) ? true : false; }

	bool Update3D();
	TESContainer* GetContainer();
	bool IsMapMarker();

	ExtraContainerChanges::EntryData *GetItemEntry(TESForm *item);
	SInt32 GetItemCount(TESForm *form);
	void AddItemAlt(TESForm *item, UInt32 count, float condition);
	void RemoveItemTarget(TESForm *form, TESObjectREFR *target, SInt32 quantity, bool clrOwner);
	void DropItemAlt(TESForm *form, SInt32 quantity, bool clrOwner);
	bool GetInventoryItems(UInt8 typeID);
	TESObjectCELL *GetParentCell();
	float GetDistance(TESObjectREFR *target);
	bool MoveToCell(TESForm *worldOrCell, float *posVector);
	TESObjectREFR *GetMerchantContainer();
	bool SetLinkedRef(TESObjectREFR *linkObj, UInt8 modIdx);

	static TESObjectREFR* Create(bool bTemp = false);

	MEMBER_FN_PREFIX(TESObjectREFR);
#if RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525
	DEFINE_MEMBER_FN(Activate, bool, 0x00573170, TESObjectREFR*, UInt32, UInt32, UInt32);	// Usage Activate(actionRef, 0, 0, 1); found inside Cmd_Activate_Execute as the last call (190 bytes)
	DEFINE_MEMBER_FN(Set3D, void, 0x0094EB40, NiNode*, bool);	// Usage Set3D(niNode, true); virtual func 0073
#elif RUNTIME_VERSION == RUNTIME_VERSION_1_4_0_525ng
	DEFINE_MEMBER_FN(Activate, bool, 0x00573430, TESObjectREFR*, UInt32, UInt32, UInt32);	// Usage Activate(actionRef, 0, 0, 1); found inside Cmd_Activate_Execute
	DEFINE_MEMBER_FN(Set3D, void, 0x005705A0, NiNode*, bool);	// Usage Set3D(niNode, true); virtual func 0073
#elif EDITOR
#else
#error
#endif
};

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

STATIC_ASSERT(offsetof(TESObjectREFR, baseForm) == 0x020);
STATIC_ASSERT(offsetof(TESObjectREFR, extraDataList) == 0x044);
STATIC_ASSERT(sizeof(TESObjectREFR) == 0x068);

class BaseProcess;

// 088
class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();

	virtual void		StartHighProcess(void);		// 090
	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Unk_94(void);
	virtual void		Unk_95(void);
	virtual void		Unk_96(void);
	virtual void		Unk_97(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		Unk_A0(void);	// StartConversation(targetActor, subjectLocationData, targetLocationData, headTrack, allowMovement, arg, topicID, arg, arg
	virtual void		Unk_A1(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		Unk_AA(void);
	virtual void		Unk_AB(void);
	virtual void		Unk_AC(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual void		Unk_AF(void);
	virtual void		Unk_B0(void);
	virtual void		Unk_B1(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		Unk_B4(void);
	virtual void		Unk_B5(void);
	virtual void		Unk_B6(void);
	virtual void		Unk_B7(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		Unk_BA(void);
	virtual void		Unk_BB(void);
	virtual void		Unk_BC(void);
	virtual void		Unk_BD(void);
	virtual void		Unk_BE(void);
	virtual void		Unk_BF(void);
	virtual void		Unk_C0(void);
	
	BaseProcess		*baseProcess;	// 068
	UInt32			unk6C;			// 06C - loaded
	TESObjectREFR	*unk70;			// 070 - loaded
	UInt32			unk74;			// 074 - loaded
	UInt32			unk78;			// 078 - loaded
	UInt8			unk7C;			// 07C - loaded
	UInt8			unk7D;			// 07D - loaded
	UInt8			unk7E;			// 07E - loaded
	UInt8			unk7F;			// 07F - loaded
	UInt8			unk80;			// 080 - loaded
	UInt8			unk81;			// 081 - loaded
	UInt8			unk082;			// 082 - cleared when starting combat on player
	UInt8			unk83;			// 083 - loaded
	UInt8			unk84;			// 084 - loaded
	UInt8			unk85;			// 085 - loaded
	UInt8			unk86;			// 086 - loaded
	UInt8			unk87;			// 087	Init'd to the inverse of NoLowLevelProcessing
};
STATIC_ASSERT(offsetof(MobileObject, baseProcess) == 0x068);
STATIC_ASSERT(sizeof(MobileObject) == 0x088);

class hkpRigidBody;

// 150
class Projectile : public MobileObject
{
public:
	Projectile();
	~Projectile();

	virtual void	Unk_C1(void);
	virtual void	Unk_C2(void);
	virtual void	Unk_C3(void);
	virtual void	Unk_C4(void);
	virtual void	Unk_C5(void);
	virtual void	Unk_C6(void);
	virtual void	Unk_C7(void);
	virtual bool	DisarmPlacedExplosives(TESObjectREFR *refr, bool unk);
	virtual void	Unk_C9(void);
	virtual void	Unk_CA(void);
	virtual void	Unk_CB(void);

	struct Struct088
	{
		TESObjectREFR	*refr;		// 00
		UInt32			unk04[2];	// 04
		float			unk0C;		// 0C
		UInt32			unk10[2];	// 10
		float			unk18;		// 18
		hkpRigidBody	*rigidBody;	// 1C
		UInt32			unk20[4];	// 20
	};

	Struct088		*unk088;		// 088
	UInt32			unk08C;			// 08C
	float			unk090[14];		// 090
	UInt32			flags;			// 0C8	0x200 - Disarmed
	float			speedMult1;		// 0CC
	float			speedMult2;		// 0D0
	float			unk0D4;			// 0D4
	float			elapsedTime;	// 0D8
	float			hitDamage;		// 0DC
	float			unk0E0;			// 0E0
	float			detonationTime;	// 0E4
	float			unk0E8[4];		// 0E8
	TESObjectWEAP	*sourceWeap;	// 0F8
	TESObjectREFR	*sourceRef;		// 0FC
	float			unk100[4];		// 100
	float			distTravelled;	// 110
	UInt32			unk114[10];		// 114
	UInt8			status;			// 13C	0 - Not triggered, 1 - Triggered, 2 - Disarmed
	UInt8			unk13D[3];		// 13D
	UInt32			unk140[4];		// 140
};

// 154
class BeamProjectile : public Projectile
{
public:
	BeamProjectile();
	~BeamProjectile();

	UInt32		unk150;		// 150
};

// 158
class ContinuousBeamProjectile : public Projectile
{
public:
	ContinuousBeamProjectile();
	~ContinuousBeamProjectile();

	UInt32		unk150[2];	// 150
};

// 158
class FlameProjectile : public Projectile
{
public:
	FlameProjectile();
	~FlameProjectile();

	virtual void	Unk_CC(void);

	UInt32		unk150[2];	// 150
};

// 154
class GrenadeProjectile : public Projectile
{
public:
	GrenadeProjectile();
	~GrenadeProjectile();

	virtual void	Unk_CC(void);

	UInt32		unk150;		// 150
};

// 160
class MissileProjectile : public Projectile
{
public:
	MissileProjectile();
	~MissileProjectile();

	virtual void	Unk_CC(void);

	UInt32		unk150[4];	// 150
};

// 00C
class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual Actor	*GetActor();
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	
	UInt32 unk004;	// 004
	UInt32 unk008;	// 008
};

STATIC_ASSERT(sizeof(MagicCaster) == 0x00C);

typedef tList<ActiveEffect> ActiveEffectList;
// 010
class MagicTarget
{
public:
	MagicTarget();
	~MagicTarget();

	virtual void	Destructor(void);
	virtual TESObjectREFR* GetParent(void);
	virtual ActiveEffectList* GetEffectList(void);

	UInt8			byt004;		// 004 
	UInt8			byt005;		// 005 
	UInt8			byt006[2];	// 006-7
	tList<ActiveEffect>	activeEffects;		// 008

	void RemoveEffect(EffectItem *effItem);

	void StopEffect(void *arg0, bool arg1)
	{
		ThisStdCall(0x8248E0, this, arg0, arg1);
	}
};

STATIC_ASSERT(sizeof(MagicTarget) == 0x010);

class hkaRaycastInterface
{
public:
	hkaRaycastInterface();
	~hkaRaycastInterface();
	virtual hkaRaycastInterface*	Destroy(bool doFree);
	virtual void					Unk_01(void* arg0);
	virtual void					Unk_02(void);

	// Assumed to be 0x010 bytes due to context where the vftable is used
	UInt32	unk000[(0x010 - 0x004) >> 2];	// 0000
};
STATIC_ASSERT(sizeof(hkaRaycastInterface) == 0x010);

class bhkRagdollController : public hkaRaycastInterface
{
public:
	bhkRagdollController();
	~bhkRagdollController();

	UInt32	unk000[(0x021C - 0x010) >> 2];	// 0010
	UInt8	fill021C[3];					// 021C
	bool	bool021F;						// 021F	when null assume FIK status is false
	bool	fikStatus;						// 0220
	UInt8	fill0221[3];					// 0221
};
STATIC_ASSERT(sizeof(bhkRagdollController) == 0x0224);

class bhkRagdollPenetrationUtil;
class ActorMover;
class PlayerMover;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;

class PathingRequest;
class PathingSolution;
class DetailedActorPathHandler;
class ActorPathingMessageQueue;
class CombatProcedure;
class CombatAction;
class CombatGoal;
class PathingLocation;
class PathingCoverLocation;
struct UnreachableLocation;
struct UnreachableCoverLocation;

// 28
class PathingLocation
{
public:
	virtual void	Unk_00(void);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);

	UInt32			unk04[9];	// 04
};

class ActorMover	// I need to call Func008
{
public:
	virtual void		Unk_00(void);
	virtual void		Unk_01(void);
	virtual void		Unk_02(void);
	virtual void		Unk_03(void);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		Unk_06(void);
	virtual void		Unk_07(void);
	virtual UInt32		GetMovementFlags();
		// bit 11 = swimming 
		// bit 9 = sneaking
		// bit 8 = run
		// bit 7 = walk
		// bit 0 = keep moving (Q)
	UInt32						unk04[6];			// 04
	PathingRequest				*pathingRequest;	// 1C
	PathingSolution				*pathingSolution;	// 20
	DetailedActorPathHandler	*pathHandler;		// 24
	Actor						*actor;				// 28
	UInt32						unk2C;				// 2C
	ActorPathingMessageQueue	*pathingMsgQueue;	// 30
	UInt32						movementFlags1;		// 34
	UInt32						unk38;				// 38
	UInt32						movementFlags2;		// 3C
	UInt32						unk40;				// 40
	PathingLocation				pathingLocation;	// 44
	UInt32						unk6C;				// 6C
	UInt8						unk70[4];			// 70
	UInt32						unk74;				// 74
	UInt32						unk78;				// 78
	UInt32						unk7C;				// 7C
	UInt32						unk80;				// 80
	UInt32						unk84;				// 84
};


struct CombatActors
{
	struct CombatTarget
	{
		Actor*	target;
		UInt32	unk004[25];
	};
	struct CombatAlly
	{
		Actor* ally;
		UInt32	unk004[4];
	};
	UInt32						unk000[2];
	BSSimpleArray<CombatTarget>	targets;
	BSSimpleArray<CombatAlly>	allies;
};

// 188
class CombatController : public TESPackage
{
public:
	CombatController();
	~CombatController();

	struct Unk09C
	{
		UInt32									unk000[4];	// 000
		TESObjectWEAP							*weapon1;	// 010
		TESObjectWEAP							*weapon2;	// 014
		TESObjectWEAP							*weapon3;	// 018
		TESObjectWEAP							*weapon4;	// 01C
		UInt32									unk020;		// 020
		BSSimpleArray<TESObjectWEAP>			arr024;		// 024
		UInt32									unk034[36];	// 034
		void									*ptr0C4;	// 0C4
		UInt32									unk0C8[17];	// 0C8
		BSSimpleArray<PathingCoverLocation>		arr10C;		// 10C
		UInt32									unk11C[11];	// 11C
		BSSimpleArray<PathingCoverLocation>		arr148;		// 148
		UInt32									unk158[3];	// 158
		BSSimpleArray<UnreachableCoverLocation>	arr164;		// 164
		BSSimpleArray<UnreachableLocation>		arr174;		// 174
		UInt32									unk184[15];	// 184
		Actor									*actor1C0;	// 1C0
		CombatController						*cmbtCtrl;	// 1C4
		UInt32									unk1C8[22];	// 1C8
	};

	CombatActors					*combatActors;		// 080
	CombatProcedure					*combatProcedure1;	// 084
	CombatProcedure					*combatProcedure2;	// 088
	BSSimpleArray<CombatProcedure>	combatProcedures;	// 08C
	Unk09C							*struct09C;			// 09C
	void							*ptr0A0;			// 0A0
	UInt32							unk0A4;				// 0A4
	CombatAction					*combatAction;		// 0A8
	CombatGoal						*combatGoal;		// 0AC
	UInt32							unk0B0[3];			// 0B0
	Actor							*packageOwner;		// 0BC
	Actor							*packageTarget;		// 0C0
	UInt32							unk0C4[11];			// 0C4
	TESObjectWEAP					*weapon;			// 0F0
	TESCombatStyle					*combatStyle;		// 0F4
	UInt32							unk0F8[11];			// 0F8
	UInt8							unk124;				// 124
	bool							stopCombat;			// 125
	UInt8							unk126[2];			// 126
	UInt32							unk128[24];			// 128
};

typedef std::vector<TESForm*> EquippedItemsList;

class Actor : public MobileObject
{
public:
	Actor();
	~Actor();

	virtual void		Unk_C1(void);
	virtual void		Unk_C2(void);
	virtual void		Unk_C3(void);
	virtual void		Unk_C4(void);
	virtual void		Unk_C5(void);
	virtual void		Unk_C6(void);
	virtual void		SetIgnoreCrime(bool ignoreCrime);
	virtual bool		GetIgnoreCrime();
	virtual void		Unk_C9(void);
	virtual void		Unk_CA(void);
	virtual void		Unk_CB(void);
	virtual void		Unk_CC(void);
	virtual void		Unk_CD(void);
	virtual void		Unk_CE(void);
	virtual void		Unk_CF(void);
	virtual void		Unk_D0(void);
	virtual void		Unk_D1(void);
	virtual void		Unk_D2(void);
	virtual void		Unk_D3(void);
	virtual void		Unk_D4(void);
	virtual void		Unk_D5(void);
	virtual void		Unk_D6(void);
	virtual void		Unk_D7(void);
	virtual bool		IsPlayerRef();
	virtual void		Unk_D9(void);
	virtual void		Unk_DA(void);
	virtual void		Unk_DB(void);
	virtual void		Unk_DC(void);
	virtual void		Unk_DD(void);
	virtual void		Unk_DE(void);
	virtual void		Unk_DF(void);
	virtual void		Unk_E0(void);
	virtual void		Unk_E1(void);
	virtual void		Unk_E2(void);
	virtual void		Unk_E3(void);
	virtual UInt32		GetActorType();	// Creature = 0, Character = 1, PlayerCharacter = 2
	virtual void		Unk_E5(void);
	virtual void		Unk_E6(void);
	virtual void		Unk_E7(void);
	virtual void		Unk_E8(void);
	virtual void		Unk_E9(void);
	virtual void		Unk_EA(void);
	virtual void		DamageActorValue(UInt32 avCode, float damage, Actor *attacker);
	virtual void		Unk_EC(void);
	virtual void		Unk_ED(void);
	virtual void		Unk_EE(void);
	virtual ExtraContainerChanges::EntryData *GetPreferedWeapon(UInt32 unk);
	virtual void		Unk_F0(void);
	virtual void		Unk_F1(void);
	virtual void		Unk_F2(void);
	virtual void		Unk_F3(void);
	virtual void		Unk_F4(void);
	virtual void		Unk_F5(void);
	virtual void		Unk_F6(void);
	virtual void		Unk_F7(void);
	virtual void		Unk_F8(void);	// 0F8 bool AddSpell(spell)
	virtual void		Unk_F9(void);
	virtual void		Unk_FA(void);
	virtual void		Unk_FB(void);
	virtual void		Unk_FC(void);
	virtual void		Unk_FD(void);
	virtual void		Unk_FE(void);
	virtual void		Unk_FF(void);
	virtual void		Unk_100(void);
	virtual void		Unk_101(void);
	virtual void		Unk_102(void);
	virtual void		Unk_103(void);
	virtual void		Unk_104(void);
	virtual void		Unk_105(void);
	virtual void		Unk_106(void);
	virtual void		Unk_107(void);
	virtual void		Unk_108(void);
	virtual void		Unk_109(void);
	virtual CombatController	*GetCombatController();
	virtual Actor		*GetCombatTarget();
	virtual void		Unk_10C(void);
	virtual void		Unk_10D(void);
	virtual void		Unk_10E(void);
	virtual void		Unk_10F(void);
	virtual void		Unk_110(void);
	virtual void		Unk_111(void);
	virtual void		Unk_112(void);
	virtual void		Unk_113(void);
	virtual void		Unk_114(void);
	virtual void		Unk_115(void);
	virtual void		Unk_116(void);
	virtual void		Unk_117(void);
	virtual void		Unk_118(void);
	virtual void		Unk_119(void);
	virtual void		Unk_11A(void);
	virtual void		Unk_11B(void);
	virtual void		Unk_11C(void);
	virtual void		Unk_11D(void);
	virtual void		Unk_11E(void);
	virtual void		Unk_11F(void);
	virtual void		Unk_120(void);
	virtual void		Unk_121(void);
	virtual void		Unk_122(void);	//	0122	Check for LevelUp
	virtual void		Unk_123(void);
	virtual void		Unk_124(void);
	virtual void		Unk_125(void);
	virtual void		SetPerkRank(BGSPerk* perk, UInt8 rank, bool alt);
	virtual void		Unk_127(void);
	virtual UInt8		GetPerkRank(BGSPerk* perk, bool alt);
	virtual void		Unk_129(void);
	virtual void		Unk_12A(void);
	virtual void		Unk_12B(void);
	virtual void		Unk_12C(void);
	virtual void		Unk_12D(void);
	virtual void		DoHealthDamage(Actor *attacker, float damage);
	virtual void		Unk_12F(void);
	virtual void		Unk_130(void);
	virtual void		Unk_131(void);
	virtual void		Unk_132(void);
	virtual void		Unk_133(void);
	virtual void		Unk_134(void);
	virtual void		Unk_135(void);
	virtual void		Unk_136(void);
	
	MagicCaster			magicCaster;			// 088
	MagicTarget			magicTarget;			// 094
	ActorValueOwner		avOwner;				// 0A4
	CachedValuesOwner	cvOwner;				// 0A8

	bhkRagdollController				* ragDollController;			// 0AC
	bhkRagdollPenetrationUtil			* ragDollPentrationUtil;		// 0B0
	UInt32								unk0B4;						// 0B4-
	float								unk0B8;						// 0B8
	UInt8								unk0BC;					// 0BC-
	UInt8								unk0BD;					// 0BD
	UInt8								unk0BE;					// 0BE
	UInt8								unk0BF;					// 0BF
	Actor								*killer;					// 0C0
	UInt8								unk0C4;					// 0C4-
	UInt8								unk0C5;					// 0C5
	UInt8								unk0C6;					// 0C6
	UInt8								unk0C7;					// 0C7
	float								unk0C8;						// 0C8
	float								unk0CC;						// 0CC
	tList<void>							unk0D0;					// 0D0
	UInt8								unk0D8;					// 0D8
	UInt8								unk0D9;					// 0D9
	UInt8								unk0DA;					// 0DA
	UInt8								unk0DB;					// 0DB
	UInt32								unk0DC;						// 0DC
	tList<void>							unk0E0;					// 0E0
	UInt8								unk0E8;					// 0E8	const 1
	UInt8								unk0E9;					// 0E9
	UInt8								unk0EA;					// 0EA
	UInt8								unk0EB;					// 0EB
	UInt32								unk0EC;						// 0EC
	UInt8								unk0F0;					// 0F0-
	UInt8								unk0F1;					// 0F1-
	UInt8								unk0F2;					// 0F2
	UInt8								unk0F3;					// 0F3
	tList<void>							unk0F4;					// 0F4
	tList<void>							unk0FC;					// 0FC
	bool								isInCombat;					// 104
	UInt8								pad105[3];				// 105
	UInt32								lifeState;						// 108 saved as byte HasHealth = 1 or 2, optionally 6
	UInt32								criticalStage;				// 10C
	UInt32								unk110;						// 110-
	float								unk114;						// 114
	UInt8								unk118;					// 118-
	UInt8								unk119;					// 119+
	UInt16								unk11A;						// 11A+
	UInt32								unk11C;						// 11C-
	UInt32								unk120;						// 120-
	bool								forceRun;					// 124
	bool								forceSneak;					// 125
	UInt8								unk126;					// 126-
	UInt8								unk127;					// 127-
	Actor								*combatTarget;				// 128
	BSSimpleArray<Actor*>				*combatTargets;				// 12C
	BSSimpleArray<Actor*>				*combatAllies;				// 130
	UInt8								unk134;					// 134-
	UInt8								unk135;					// 135+
	UInt16								unk136;						// 136+
	UInt32								unk138;						// 138-
	UInt32								unk13C;						// 13C-
	UInt32								actorFlags;					// 140	0x80000000 - IsEssential
	bool								ignoreCrime;				// 144
	UInt8								unk145;					// 145	Has to do with package evaluation
	UInt8								unk146;					// 146	Has to do with package evaluation
	UInt8								unk147;					// 147
	UInt32								unk148;							// 148
	UInt8								inWater;					// 14C
	UInt8								isSwimming;						// 14D
	UInt16								unk14E;						// 14E+
	UInt32								unk150;						// 150-
	float								unk154;						// 154
	float								unk158;						// 158
	UInt8								unk15C;					// 15C-
	UInt8								unk15D;					// 15D-
	UInt16								unk15E;						// 15E+
	float								startingPos[3];				// 160
	float								unk16C;						// 16C
	TESForm								*startingWorldOrCell;		// 170
	UInt8								unk174;					// 174-
	UInt8								unk175;					// 175-
	UInt16								unk176;						// 176+
	float								unk178;						// 178
	float								unk17C;						// 17C
	float								unk180;						// 180
	float								unk184;						// 184
	float								unk188;						// 188
	UInt8								unk18C;					// 18C-
	bool								isTeammate;					// 18D
	UInt8								unk18E;					// 18E-
	UInt8								unk18F;					// 18F
	ActorMover							* actorMover;					// 190
	UInt32								unk194;						// 194-
	UInt32								unk198;						// 198-
	float								unk19C;						// 19C
	UInt32								unk1A0;						// 1A0-
	UInt32								unk1A4;						// 1A4-
	UInt32								unk1A8;						// 1A8-
	UInt32								unk1AC;						// 1AC-
	UInt8								unk1B0;					// 1B0-
	UInt8								unk1B1;					// 1B1-
	UInt8								unk1B2;					// 1B2
	UInt8								unk1B3;					// 1B3

	// OBSE: unk1 looks like quantity, usu. 1; ignored for ammo (equips entire stack). In NVSE, pretty much always forced internally to 1 
	// OBSE: itemExtraList is NULL as the container changes entry is not resolved before the call
	// NVSE: Default values are those used by the vanilla script functions.
	void EquipItem(TESForm * objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);	// unk3 apply enchantment on player differently
	void UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = NULL, UInt32 unk3 = 1, bool lockEquip = false, UInt32 unk5 = 1);

	EquippedItemsList	GetEquippedItems();
	ExtraContainerDataArray	GetEquippedEntryDataList();
	ExtraContainerExtendDataArray GetEquippedExtendDataList();

	bool GetDead() { return (lifeState == 1) || (lifeState == 2); }
	bool GetRestrained() { return lifeState == 5; }

	bool GetLOS(Actor *target);
	SInt8 GetCurrentAIPackage();
	SInt8 GetCurrentAIProcedure();
	TESObjectWEAP *GetEquippedWeapon();
	bool IsItemEquipped(TESForm *item);
	UInt8 EquippedWeaponHasMod(UInt8 modID);
	bool IsSneaking();
	void StopCombat();
	bool IsInCombatWith(Actor *target);
	SInt32 GetDetectedValue(Actor *detected);
	TESPackage *GetStablePackage();
	TESObjectREFR *GetPackageTarget();
	TESCombatStyle *GetCombatStyle();
	bool GetKnockedState();
	bool IsWeaponOut();
	bool GetIsGhost();
	float GetRadiationLevel();
	void TurnToFacePoint(float *point);
};

STATIC_ASSERT(offsetof(Actor, magicCaster) == 0x088);

class Character : public Actor
{
public:
	Character();
	~Character();

	virtual void		Unk_137(void);
	virtual void		Unk_138(void);

	ValidBip01Names	* validBip01Names;	// 1B4
	float			flt1B8;				// 1B8
	float			flt1BC;				// 1BC
	UInt8			byt1C0;				// 1C0
	UInt8			byt1C1;				// 1C1
	UInt16			unk1C2;				// 1C2
	float			flt1C4;				// 1C4
};



class ItemChange;
class bhkRigidBody;
struct ParentSpaceNode;
struct TeleportLink;


typedef tList<BGSQuestObjective::Target> QuestObjectiveTargets;
// 9BC
class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	// used to flag controls as disabled in disabledControlFlags
	enum
	{
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};

	virtual void		Unk_139(void);
	virtual void		Unk_13A(void);

	struct PerkRank
	{
		BGSPerk* perk;
		UInt8 rank;
		UInt8 pad[3];
	};

	// lotsa data
	
	//UInt32								unk1C8[(0x244-0x1C8) >> 2];		// 1C8	0224 is a package of type 1C, 208 could be a DialogPackage, 206 questObjectiveTargets is valid
																			// 0AC is bhkRagdollController, 0B0 is bhkRagdollPenetrationUtil, 190 is PlayerMover

	UInt32								unk1C8[(0x208 - 0x1C8) / 4];	// 1C8
	TESPackage							* package208;					// 208	// dialogue?
	void								* unk20C;						// 20C
	tList<ActiveEffect>					* activeEffects;				// 210
	UInt32								unk214[2];						// 214
	void								* unk21C;						// 21C
	UInt32								unk220;							// 220
	TESPackage							* package224;					// 224	// type 1C?
	UInt32								unk228[(0x240 - 0x228) / 4];	// 228
	UInt8								showQuestItems;					// 240
	UInt8								unk241[3];						// 241
	float								actorValues1[77];				// 244
	float								actorValues2[77];				// 378
	UInt32								unk4AC;							// 4AC
	float								actorValues3[77];				// 4B0
	tList<BGSNote>						notes;							// 5E4
	ImageSpaceModifierInstanceDOF		* imodDOF1;						// 5EC
	ImageSpaceModifierInstanceDOF		* imodDOF2;						// 5F0
	ImageSpaceModifierInstanceDRB		* imodDRB;						// 5F4
	UInt32								unk5F8;							// 5F8
	tList<Actor>						teammates;						// 5FC
	TESObjectREFR						* lastDoor;						// 604
	UInt32								unk608[(0x614 - 0x608) / 4];	// 608
	tList<TESCaravanCard>				* caravanCards1;				// 614	// who the fuck decided to use a pointer to a tList?
	tList<TESCaravanCard>				* caravanCards2;				// 618	// these Obsidian devs...
	UInt32								unk61C[(0x634 - 0x61C) / 4];	// 61C
	UInt32								unk634;							// 634  // *bhkMouseSpringAction  (with grabbed ref)
	TESObjectREFR						* grabbedRef;					// 638
	UInt32								unk63C[(0x648 - 0x63C) / 4];	// 63C
	UInt8								unk648;							// 648
	UInt8								unk649;							// 649
	bool								bNotFirstPerson;				// 64A	= not FirstPerson	...ThirdPerson?
	UInt8								unk64B;							// 64B
	bool								bThirdPerson;					// 64C
	UInt8								unk64D;							// 64D
	UInt8								unk64E;							// 64E
	bool								isUsingScope;					// 64F
	UInt32								unk650[(0x680 - 0x650) / 4];	// 650 
	UInt8								disabledControlFlags;			// 680	kControlFlag_xxx
	UInt8								pad681[3];						// 681
	UInt32								unk684[(0x68C - 0x684) / 4];	// 684
	ValidBip01Names						* playerVB01N;					// 68C
	ExtraAnim::Animation				* extraAnimation;				// 690 ExtraDataAnim::Data
	NiNode								* playerNode;					// 694 used as node if unk6A is true (not first-person)
	UInt32								unk698;							// 698
	NiNode								* playerNode2;					// 69C // 1st person?
	UInt32								unk6A0[2];						// 6A0
	TESTopic							* topic;						// 6A8 // this+6AC are probably tList<TESTopic>
	tList<TESTopic>						* topicList;					// 6AC // again with the pointers to linked lists...
	tList<void*>						unk6B0;							// 6B0 // log entries
	TESQuest							* quest;						// 6B8 // active quest
	tList<BGSQuestObjective>			questObjectiveList;				// 6BC // current quest objectives
	tList<BGSQuestObjective::Target>	questTargetList;				// 6C4 // current quest targets
	UInt32								unk6CC[(0x6F8 - 0x6CC) / 4];	// 6CC
	BSSimpleArray<ParentSpaceNode>		parentSpaceNodes;				// 6F8
	BSSimpleArray<TeleportLink>			teleportLinks;					// 708
	UInt32								unk718[(0x760 - 0x718) / 4];	// 718
	TESRegion							* currentRegion;				// 760
	TESRegionList						regionsList;					// 764
	UInt32								unk770[(0x7AC - 0x770) / 4];	// 770
	TESForm								* playerCellOrWorldSpace;		// 7AC // TESObjectREFR::parentCell?
	UInt32								unk7B0;							// 7B0
	BGSMusicType						* musicType7B4;					// 7B4
	UInt8								gameDifficulty;					// 7B8
	UInt8								pad7B9[3];						// 7B9
	bool								isHardcore;						// 7BC
	UInt8								pad7Bd[3];						// 7BD
	UInt32								unk7C0;							// 7C0
	UInt8								unk7C4[2];						// 7C4 // maybe other player flags? inCharGen?
	bool								isToddler;						// 7C6
	bool								canUsePA;						// 7C7

	UInt32								unk7C8[(0x7D0 - 0x7C8) / 4];	// 7C8
	TESWorldSpace						* worldspace7D0;				// 7D0
	UInt32								unk7D4[(0x7E0 - 0x7D4) / 4];	// 7D4
	float								cameraRotationZ;				// 7E0
	float								cameraRotationX;				// 7E4
	float								cameraPosition1;				// 7E8 // need to find order of axis
	float								cameraPosition2;				// 7EC
	float								cameraPosition3;				// 7F0 // (adjusted by scaledHeight)
	UInt32								unk7F4[(0x86C - 0x7F4) / 4];
	UInt32								unk86C;							// 86C // cleared after equipment change?
	UInt32								unk870[(0x87C - 0x870) / 4];	// 870
	tList<PerkRank>						playerPerks;					// 87C
	tList<BGSEntryPointPerkEntry>		playerPerkEntries[74];			// 884 // unk884[(0xAD4 - 0x884) / 4];
	tList<PerkRank>						followerPerks;					// AD4
	tList<BGSEntryPointPerkEntry>		followerPerkEntries[74];		// ADC // unkADC[(0xD68 - 0xADC) / 4];
	UInt32								unkD2C[(0xD64 - 0xD2C) / 4];	// D2C
	CombatActors						* combatActors;					// D64
	UInt32								teammateCount;					// D68
	UInt32								unkD6C[(0xD80 - 0xD6C) / 4];	// D6C
	NiNode								* nodeD80;						// D80
	UInt32								unkD84[(0xDB4 - 0xD84) / 4];	// D84
	NiNode								* nodeDB4;						// DB4
	UInt32								unkDB8[(0xDD4 - 0xDB8) / 4];	// DB8
	float								unkDD4[6];						// DD4
	bhkRigidBody						* rigidBody;					// DEC
	bool								playerIsInCombat;				// DF0 // double-check this
	bool								playerUndetected;				// DF1
	UInt8								padDF2[2];						// DF2
	BSSimpleArray<ItemChange>			itemChangesDF4;					// DF4 // wtf is this?
	UInt32                              unkE04[(0xE34 - 0xE04) / 4];	// E0C
	NiNode								* ironSightNode;				// E34
	UInt32								unkE38;
	BSSimpleArray<TESAmmo*>				ammoE3C;						// E3C
	UInt32								unkE4C;							// E4C
		// 7C4 is a byte used during Combat evaluation (Player is targetted ?), 
		// BSSimpleArray<ItemChange> at DF4...   ItemChange?
		// BSSimpleArray<TESAmmo> at E3C
		// 7C6 is a boolean meaning toddler, 
		// 7C7 byte bool PCCanUsePowerArmor, Byt0E39 referenced during LoadGame
		// Used when entering FlyCam : 7E8/7EC/7F0 stores Pos, 7F0 adjusted by scaledHeight multiplied by 0698 , 7E0 stores RotZ, 7E4 RotX
		// Perks forms a list at 87C and AD4. Caravan Cards at 614 and 618. Quest Stage LogEntry at 6B0. Quest Objectives at 6BC.
		// Hardcore flag would be E38. Byte at DF0 seems to be PlayerIsInCombat
		// tList at 6C4 is cleared when there is no current quest. There is another NiNode at 069C
		// list of perk and perkRank are at 0x087C and 0x0AD4 (alt perks). 086C is cleared after equipement change.
		// D68 counts the Teammates.
		// D74: 96 bytes that are cleared when the 3D is cleared.

	bool IsThirdPerson() { return bThirdPerson ? true : false; }
	UInt32 GetMovementFlags() { return actorMover->GetMovementFlags(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool IsPlayerSwimming() { return (GetMovementFlags()  >> 11) & 1; }

	static PlayerCharacter*	GetSingleton();
	bool SetSkeletonPath(const char* newPath);
	static void UpdateHead(void);
	QuestObjectiveTargets* GetCurrentQuestObjectiveTargets();
};

STATIC_ASSERT(offsetof(PlayerCharacter, ragDollController) == 0x0AC);
STATIC_ASSERT(offsetof(PlayerCharacter, questObjectiveList) == 0x6BC);
STATIC_ASSERT(offsetof(PlayerCharacter, bThirdPerson) == 0x64C);
STATIC_ASSERT(offsetof(PlayerCharacter, actorMover) == 0x190);
STATIC_ASSERT(sizeof(PlayerCharacter) == 0xE50);
