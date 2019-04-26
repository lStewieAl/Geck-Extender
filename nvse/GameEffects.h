#pragma once

class ActiveEffect
{
public:
	ActiveEffect();	// args are caster, magicItem, effectItem
	virtual ~ActiveEffect();

	virtual ActiveEffect *	Clone(void);
	virtual void			Unk_02(UInt32 arg);
	virtual void			Unk_03(UInt32 arg);
	virtual void			SaveGame(UInt32 arg);
	virtual void			LoadGame(UInt32 arg);
	virtual void			Unk_06(UInt32 arg);
	virtual void			Unk_07(UInt32 arg);
	virtual void			Unk_08(UInt32 arg);
	virtual bool			UnregisterCaster(MagicCaster * _caster);	// returns 1 and clears caster if it matches the parameter, else returns 0
	virtual bool			Unk_0A(void);
	virtual void			CopyTo(ActiveEffect * target);
	virtual bool			Unk_0C(UInt32 arg);
	virtual bool			Unk_0D(UInt32 arg);
	virtual void			Unk_0E(UInt32 arg);		// update/add effect?
	virtual void			Terminate(void);	// update/add effect?
	virtual void			Unk_10(UInt32 arg);
	virtual void			Unk_11(UInt32 arg);
	virtual void			Unk_12(UInt32 arg);
	virtual void			Unk_13(UInt32 arg);
	virtual void			Unk_14(void);
	virtual void			Unk_15(void);
	virtual void			Unk_16(void);
//	void		** _vtbl;			// 00
	float		timeElapsed;		// 04
	MagicItem	*magicItem;			// 08
	EffectItem	* effectItem;		// 0C
	bool		bApplied;			// 10
	bool		bTerminated;		// 11 set to 1 when effect is to be removed
	UInt8		flags12;			// 12
	UInt8		pad13;				// 13
	UInt32		unk14;				// 14
	UInt32		unk18;				// 18 - flags
	float		magnitude;			// 1C - adjusted based on target?
	float		duration;			// 20 - adjusted based on target?
	MagicTarget	* target;			// 24
	MagicCaster	* caster;			// 28
	UInt32		spellType;			// 2C e.g. SpellItem::kType_Ability
	UInt32		unk30;				// 30
	UInt32		unk34;				// 34
	UInt32		unk38;				// 38
	TESForm		* enchantObject;	// 3C enchanted obj responsible for effect
	tList<TESForm>	data;			// 40 - in ScriptEffect this is a Script *

	void Remove(bool bRemoveImmediately);
};
STATIC_ASSERT(sizeof(ActiveEffect) == 0x048);

class ValueModifierEffect : public ActiveEffect
{
public:
	ValueModifierEffect();
	~ValueModifierEffect();

	UInt32 actorVal;	// 48
};
STATIC_ASSERT(offsetof(ValueModifierEffect, actorVal) == 0x48);
STATIC_ASSERT(sizeof(ValueModifierEffect) == 0x04C);

class AssociatedItemEffect : public ActiveEffect
{
public:
	AssociatedItemEffect();
	~AssociatedItemEffect();

	TESObject	* item;	// 48 - creature, armor, weapon
};

class CommandEffect : public ActiveEffect
{
public:
	CommandEffect();
	~CommandEffect();
	virtual void Unk_17(void);
};

class AbsorbEffect : public ValueModifierEffect
{
public:
	AbsorbEffect();
	~AbsorbEffect();
	virtual void Unk_1A(void);
	UInt32 unk04C[4];
};

class BoundItemEffect : public AssociatedItemEffect
{
public:
	BoundItemEffect();
	~BoundItemEffect();
	UInt32 unk04C[24];
};

class CalmEffect : public ValueModifierEffect
{
public:
	CalmEffect();
	~CalmEffect();
	virtual void Unk_17(void);
	UInt32 unk048;
};

class ChameleonEffect : public ValueModifierEffect
{
public:
	ChameleonEffect();
	~ChameleonEffect();
	virtual void Unk_1A(void);
};

class CommandCreatureEffect : public CommandEffect
{
public:
	CommandCreatureEffect();
	~CommandCreatureEffect();
};

class CommandHumanoidEffect : public CommandEffect
{
public:
	CommandHumanoidEffect();
	~CommandHumanoidEffect();
};

class ConcussionEffect : public ActiveEffect
{
public:
	ConcussionEffect();
	~ConcussionEffect();

	float	unk48;		//  48
};
STATIC_ASSERT(sizeof(ConcussionEffect) == 0x4C);

class CureEffect : public ActiveEffect
{
public:
	CureEffect();
	~CureEffect();
	virtual void Unk_17(void);
	UInt32 unk048[2];
};

class DarknessEffect : public ValueModifierEffect
{
public:
	DarknessEffect();
	~DarknessEffect();
	virtual void Unk_1A(void);
};

class DemoralizeEffect : public ActiveEffect
{
public:
	DemoralizeEffect();
	~DemoralizeEffect();
	virtual void Unk_17(void);
	UInt32 unk048;
};

class DetectLifeEffect : public ValueModifierEffect
{
public:
	DetectLifeEffect();
	~DetectLifeEffect();
	virtual void Unk_1A(void);
};

class DisintegrateArmorEffect : public ActiveEffect
{
public:
	DisintegrateArmorEffect();
	~DisintegrateArmorEffect();
	UInt32 unk048;
};

class DisintegrateWeaponEffect : public ActiveEffect
{
public:
	DisintegrateWeaponEffect();
	~DisintegrateWeaponEffect();
};

class DispelEffect : public ActiveEffect
{
public:
	DispelEffect();
	~DispelEffect();
	virtual void Unk_17(void);
};

class FrenzyEffect : public ValueModifierEffect
{
public:
	FrenzyEffect();
	~FrenzyEffect();
	virtual void Unk_1A(void);
	UInt32 unk04C;
};

class InvisibilityEffect : public ValueModifierEffect
{
public:
	InvisibilityEffect();
	~InvisibilityEffect();
	virtual void Unk_1A(void);
};

class LightEffect : public ActiveEffect
{
public:
	LightEffect();
	~LightEffect();
	UInt32 unk048;
};

class LimbConditionEffect : public ValueModifierEffect
{
public:
	LimbConditionEffect();
	~LimbConditionEffect();
	virtual void Unk_1A(void);
	UInt32 unk04C;
};

class LockEffect : public ActiveEffect
{
public:
	LockEffect();
	~LockEffect();
	virtual void Unk_17(void);
};

class NightEyeEffect : public ValueModifierEffect
{
public:
	NightEyeEffect();
	~NightEyeEffect();
	virtual void Unk_1A(void);
};

class OpenEffect : public ActiveEffect
{
public:
	OpenEffect();
	~OpenEffect();
	virtual void Unk_17(void);
};

class ParalysisEffect : public ValueModifierEffect
{
public:
	ParalysisEffect();
	~ParalysisEffect();
	virtual void Unk_1A(void);
};

class ReanimateEffect : public ActiveEffect
{
public:
	ReanimateEffect();
	~ReanimateEffect();
	UInt32 unk048[10];
};

class ScriptEffect : public ActiveEffect
{
public:
	ScriptEffect();
	~ScriptEffect();
	UInt32	unk048[2];
};

class ShieldEffect : public ValueModifierEffect
{
public:
	ShieldEffect();
	~ShieldEffect();

	virtual void Unk_1A(void);
	UInt32	unk04C;		// 4C
};

class SummonCreatureEffect : public AssociatedItemEffect
{
public:
	SummonCreatureEffect();
	~SummonCreatureEffect();
	UInt32 unk04C[10];
};

class SunDamageEffect : public ActiveEffect
{
public:
	SunDamageEffect();
	~SunDamageEffect();
	virtual void Unk_17(void);
	UInt32 unk048[2];
};

class TelekinesisEffect : public ValueModifierEffect
{
public:
	TelekinesisEffect();
	~TelekinesisEffect();
	virtual void Unk_1A(void);
	UInt32 unk04C[5];
};

class TurnUndeadEffect : public ActiveEffect
{
public:
	TurnUndeadEffect();
	~TurnUndeadEffect();
	virtual void Unk_17(void);
	UInt32 unk048;
};

class ValueAndConditionsEffect : public ValueModifierEffect
{
public:
	ValueAndConditionsEffect();
	~ValueAndConditionsEffect();
	virtual void Unk_1A(void);
	UInt32 unk04C;
};

class VampirismEffect : public ActiveEffect
{
public:
	VampirismEffect();
	~VampirismEffect();
	virtual void Unk_17(void);
};
