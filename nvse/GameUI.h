#pragma once

#include "GameTiles.h"
#include "GameTypes.h"
#include "GameExtraData.h"

class Menu;
class SceneGraph;
class FOPipboyManager;
class NiObject;
class TESObjectREFR;
class NiNode;

typedef Menu * (* _TempMenuByType)(UInt32 menuType);
extern const _TempMenuByType TempMenuByType;

// 584
class InterfaceManager
{
public:
	InterfaceManager();
	~InterfaceManager();

	static InterfaceManager *	GetSingleton(void);
	static bool					IsMenuVisible(UInt32 menuType);
	static Menu *				GetMenuByType(UInt32 menuType);
	static Menu *				TempMenuByType(UInt32 menuType);
	static TileMenu *			GetMenuByPath(const char * componentPath, const char ** slashPos);
	static Tile::Value *		GetMenuComponentValue(const char * componentPath);
	static Tile *				GetMenuComponentTile(const char * componentPath);




	UInt32				flags;							// 000	bit 1 would be "not menu mode"
	SceneGraph			* sceneGraph004;				// 004
	SceneGraph			* sceneGraph008;				// 008
	UInt8				currentMode;					// 00C	1 = GameMode; 2 = MenuMode
	UInt8				pad00D[3];						// 00D
	UInt32				unk010[6];						// 010
	TileImage			*cursor;						// 028
	float				unk02C;							// 02C
	float				unk030;							// 030
	float				unk034;							// 034
	float				cursorX;						// 038
	float				unk03C;							// 03C
	float				cursorY;						// 040
	UInt32				unk044[11];						// 044
	tList<TESObjectREFR> debugSelections;				// 070
	UInt32				unk078;							// 078
	UInt32				unk07C;							// 07C
	NiNode				* niNode080;					// 080
	NiNode				* niNode084;					// 084
	UInt32				unk088[(0x09C - 0x088) >> 2];	// 088
	Tile				* menuRoot;						// 09C
	Tile				* unk0A0;						// 0A0 seen "Strings"
	void				* unk0A4;						// 0A4 saw Tile? seen NiNode
	UInt32				unk0A8;							// 0A8
	NiObject			* unk0AC;						// 0AC seen NiAlphaProperty
	UInt32				unk0B0[(0x0BC - 0x0B0) >> 2];	// 0B0
	Tile				*activeTileAlt;					// 0BC
	UInt32				unk0C0[3];						// 0B0
	Tile				* activeTile;					// 0CC
	Menu				* activeMenu;					// 0D0
	UInt32				unk0D4[(0x0E4 - 0x0D4) >> 2];	// 0D4
	UInt8				msgBoxButton;					// 0E4 -1 if no button pressed
	UInt8				pad0E5[3];
	UInt32				unk0E8;							// 0E8
	UInt32				unk0EC;							// 0EC
	TESObjectREFR		* debugSelection;				// 0F0	compared to activated object during Activate
	UInt32				unk0F4;							// 0F4
	UInt32				unk0F8;							// 0F8
	TESObjectREFR		* crosshairRef;					// 0FC
	UInt32				unk100[(0x114 - 0x100) >> 2];	// 100
	UInt32				menuStack[10];					// 114
	UInt32				unk13C[14];						// 13C
	FOPipboyManager		* pipboyManager;				// 174
	UInt32				* unk178;						// 178 could be camera pointer, 0054 is bFlyCam
	NiTArray<UInt32>	array17C;						// 17C NiTPrimitiveArray@PackedMenu@BGSMenuPacker
	UInt32				unk18C[(0x394 - 0x18C) >> 2];	// 18C
	NiObject			* unk394;						// 394 seen NiSourceTexture
	UInt32				unk398[(0x454 - 0x398) >> 2];	// 398
	NiTArray<UInt32>	array454;						// 454 NiTPrimitiveArray@TextureType@BSTextureManager
	NiTArray<UInt32>	array464;						// 464 NiTPrimitiveArray@FilterMode@NiTexturingProperty
	UInt32				unk468[(0x584 - 0x474) >> 2];	// 474
};
STATIC_ASSERT(sizeof(InterfaceManager) == 0x584);
STATIC_ASSERT(offsetof(InterfaceManager, debugSelections) == 0x070);
STATIC_ASSERT(offsetof(InterfaceManager, debugSelection) == 0x0F0);
STATIC_ASSERT(offsetof(InterfaceManager, crosshairRef) == 0x0FC);
STATIC_ASSERT(offsetof(InterfaceManager, pipboyManager) == 0x174);

void Debug_DumpMenus(void);

enum
{
	// ###

	kMenuType_Min = 0x3E9,
	kMenuType_Inventory,
	kMenuType_Stats,
	kMenuType_HUDMain,
	kMenuType_Loading =			0x3EF,
	kMenuType_Container,
	kMenuType_Dialog,
	kMenuType_SleepWait =		0x3F4,
	kMenuType_Pause,
	kMenuType_LockPick,
	kMenuType_Quantity =		0x3F8,
	kMenuType_PipboyData =		0x3FF,
	kMenuType_Book =			0x402,
	kMenuType_LevelUp,
	kMenuType_PipboyRepair =	0x40B,
	kMenuType_RaceSex,
	kMenuType_Credits =			0x417,
	kMenuType_CharGen,
	kMenuType_TextEdit =		0x41B,
	kMenuType_Barter =			0x41D,
	kMenuType_Surgery,
	kMenuType_Hacking,
	kMenuType_VATS,
	kMenuType_Computers,
	kMenuType_VendorRepair,
	kMenuType_Tutorial,
	kMenuType_SPECIAL,
	kMenuType_ItemMod,
	kMenuType_LoveTester =		0x432,
	kMenuType_CompanionWheel,
	kMenuType_TraitSelect,
	kMenuType_Recipe,
	kMenuType_SlotMachine =		0x438,
	kMenuType_Blackjack,
	kMenuType_Roulette,
	kMenuType_Caravan,
	kMenuType_Max = 0x43C,
};

class Menu
{
public:
	Menu();
	~Menu();

	virtual void	Destructor(bool arg0);		// pass false to free memory
	virtual void	SetField(UInt32 idx, Tile* value);
	virtual void	Unk_02(UInt32 arg0, UInt32 arg1);	// show menu?
	virtual void	HandleClick(UInt32 buttonID, Tile* clickedButton); // buttonID = <id> trait defined in XML
	virtual void	HandleMouseover(UInt32 arg0, Tile * activeTile);	//called on mouseover, activeTile is moused-over Tile
	virtual void	Unk_05(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_06(UInt32 arg0, UInt32 arg1, UInt32 arg2);
	virtual void	Unk_07(UInt32 arg0, UInt32 arg1, UInt32 arg2);
	virtual void	Unk_08(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_09(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_0A(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_0B(void);
	virtual bool	HandleKeyboardInput(char inputChar);	//for keyboard shortcuts, return true if handled
	virtual UInt32	GetID(void);
	virtual bool	Unk_0E(UInt32 arg0, UInt32 arg1);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);

	TileMenu		*tile;		// 04
	UInt32			unk08;		// 08
	UInt32			unk0C;		// 0C
	UInt32			unk10;		// 10
	UInt32			unk14;		// 14
	UInt32			unk18;		// 18
	UInt32			unk1C;		// 1C
	UInt32			id;			// 20
	UInt32			unk24;		// 24
	TESObjectREFR *GetTargetRef();
};
class TESNPC;
class RaceSexMenu : public Menu
{
public:
	UInt32		unk028[44]; // 028
	TESNPC*		unknpc;		// 0D8

	void UpdatePlayerHead(void);
};

template <typename Item> struct ListBoxItem
{
	Tile	*tile;
	Item	*object;
};

template <typename Item> class ListBox : public BSSimpleList<ListBoxItem<Item>>
{
public:
	Item *Find(Tile *tile)
	{
		for (tList<ListBoxItem<Item>>::Iterator iter(list); !iter.End(); iter.Next())
			if (iter->tile == tile) return iter->object;
		return NULL;
	}
};

typedef ListBox<ExtraContainerChanges::EntryData> MenuItemEntryList;

// 124
class InventoryMenu : public Menu		// 1002
{
public:
	InventoryMenu();
	~InventoryMenu();

	TileRect			*tile028;		// 028
	TileRect			*tile02C;		// 02C
	TileRect			*tile030;		// 030
	TileRect			*tile034;		// 034
	TileImage			*tile038;		// 038
	TileRect			*tile03C;		// 03C
	TileImage			*tile040;		// 040
	TileImage			*tile044;		// 044
	TileImage			*tile048;		// 048
	TileImage			*tile04C;		// 04C
	TileImage			*tile050;		// 050
	TileImage			*tile054;		// 054
	TileRect			*tile058;		// 058
	TileRect			*tile05C;		// 05C
	TileRect			*tile060;		// 060
	TileRect			*tile064;		// 064
	TileRect			*tile068;		// 068
	TileRect			*tile06C;		// 06C
	TileRect			*tile070;		// 070
	TileImage			*tile074;		// 074
	TileImage			*tile078;		// 078
	TileRect			*tile07C;		// 07C
	TileText			*tile080;		// 080
	UInt32				filter;			// 084
	UInt32				unk088[12];		// 088
	MenuItemEntryList	itemList;		// 0B8
	TileImage			*tile0C4;		// 0C4
	TileRect			*selected;		// 0C8
	TileImage			*tile0CC;		// 0CC
	UInt32				unk0D0[6];		// 0D0
	TileRect			*tile0E8;		// 0E8
	TileRect			*tile0EC;		// 0EC
	TileRect			*tile0F0;		// 0F0
	TileRect			*tile0F4;		// 0F4
	TileRect			*tile0F8;		// 0F8
	TileRect			*tile0FC;		// 0FC
	TileRect			*tile100;		// 100
	TileRect			*tile104;		// 104
	TileRect			*tile108;		// 108
	UInt32				unk10C[6];		// 10C
};

// 50
class Tile3D : public Tile
{
public:
	Tile3D();
	~Tile3D();

	UInt32			unk38[6];
};

// 278
class HUDMainMenu : public Menu			// 1004
{
public:
	HUDMainMenu();
	~HUDMainMenu();

	UInt32					unk028;			// 028
	TileImage				*tile02C;		// 02C
	TileText				*tile030;		// 030
	TileRect				*tile034;		// 034
	TileRect				*tile038;		// 038
	TileRect				*tile03C;		// 03C
	TileImage				*tile040;		// 040
	TileImage				*tile044;		// 044
	TileText				*tile048;		// 048
	TileText				*tile04C;		// 04C
	TileImage				*tile050;		// 050
	TileImage				*tile054;		// 054
	TileText				*tile058;		// 058
	TileRect				*tile05C;		// 05C
	TileRect				*tile060;		// 060
	TileText				*tile064;		// 064
	TileText				*tile068;		// 068
	TileImage				*tile06C;		// 06C
	TileImage				*tile070;		// 070
	TileText				*tile074;		// 074
	TileText				*tile078;		// 078
	TileImage				*tile07C;		// 07C
	TileImage				*tile080;		// 080
	TileText				*tile084;		// 084
	TileText				*sneakLabel;	// 088
	TileImage				*tile08C;		// 08C
	TileText				*tile090;		// 090
	TileImage				*tile094;		// 094
	TileText				*tile098;		// 098
	TileRect				*tile09C;		// 09C
	TileText				*tile0A0;		// 0A0
	TileImage				*tile0A4;		// 0A4
	TileText				*tile0A8;		// 0A8
	TileText				*tile0AC;		// 0AC
	TileText				*tile0B0;		// 0B0
	TileText				*tile0B4;		// 0B4
	TileText				*tile0B8;		// 0B8
	TileText				*tile0BC;		// 0BC
	TileText				*tile0C0;		// 0C0
	TileImage				*tile0C4;		// 0C4
	TileRect				*tile0C8;		// 0C8
	TileText				*tile0CC;		// 0CC
	TileImage				*tile0D0;		// 0D0
	TileImage				*tile0D4;		// 0D4
	TileImage				*tile0D8;		// 0D8
	TileText				*tile0DC;		// 0DC
	TileText				*tile0E0;		// 0E0
	TileImage				*tile0E4;		// 0E4
	TileText				*tile0E8;		// 0E8
	TileText				*tile0EC;		// 0EC
	TileText				*tile0F0;		// 0F0
	TileImage				*tile0F4;		// 0F4
	TileImage				*tile0F8;		// 0F8
	TileImage				*tile0FC;		// 0FC
	TileImage				*tile100;		// 100
	TileImage				*tile104;		// 104
	TileImage				*tile108;		// 108
	TileImage				*tile10C;		// 10C
	TileImage				*tile110;		// 110
	TileRect				*tile114;		// 114
	TileRect				*tile118;		// 118
	TileRect				*tile11C;		// 11C
	TileRect				*tile120;		// 120
	TileRect				*tile124;		// 124
	TileRect				*tile128;		// 128
	TileRect				*tile12C;		// 12C
	TileRect				*tile130;		// 130
	TileRect				*tile134;		// 134
	TileRect				*tile138;		// 138
	TileRect				*tile13C;		// 13C
	TileRect				*tile140;		// 140
	TileRect				*tile144;		// 144
	Tile3D					*tile148;		// 148
	TileRect				*tile14C;		// 14C
	TileRect				*tile150;		// 150
	TileImage				*tile154;		// 154
	TileImage				*tile158;		// 158
	TileText				*tile15C;		// 15C
	TileRect				*tile160;		// 160
	TileText				*tile164;		// 164
	TileText				*tile168;		// 168
	TileText				*tile16C;		// 16C
	TileImage				*tile170;		// 170
	TileImage				*tile174;		// 174
	TileText				*tile178;		// 178
	TileText				*tile17C;		// 17C
	TileImage				*tile180;		// 180
	UInt32					unk184[2];		// 184
	NiNode					*node18C;		// 18C
	UInt32					unk190[2];		// 190
	BSSimpleArray<void>		array198;		// 198
	UInt32					unk1A8[4];		// 1A8
	TESObjectREFR			*refr1B8;		// 1B8
	UInt32					unk1BC[3];		// 1BC
	TileRect				*tile1C8;		// 1C8
	TileRect				*tile1CC;		// 1CC
	UInt32					unk1D0;			// 1D0
	TileRect				*tile1D4;		// 1D4
	TileRect				*tile1D8;		// 1D8
	TileRect				*tile1DC;		// 1DC
	TileRect				*tile1E0;		// 1E0
	TileRect				*tile1E4;		// 1E4
	TileRect				*tile1E8;		// 1E8
	UInt32					unk1EC[5];		// 1EC
	NiControllerSequence	*niContSeq;		// 200
	UInt32					unk204[11];		// 204
	void					*ptr230;		// 230
	void					*ptr234;		// 234
	UInt32					unk238[16];		// 238
};

// 10C
class ContainerMenu : public Menu		// 1008
{
public:
	ContainerMenu();
	~ContainerMenu();

	TileImage			*tile028;		// 028
	TileText			*tile02C;		// 02C
	TileImage			*tile030;		// 030
	TileText			*tile034;		// 034
	TileImage			*tile038;		// 038
	TileImage			*tile03C;		// 03C
	TileText			*tile040;		// 040
	TileImage			*tile044;		// 044
	TileImage			*tile048;		// 048
	TileImage			*tile04C;		// 04C
	TileImage			*tile050;		// 050
	TileImage			*tile054;		// 054
	TileImage			*tile058;		// 058
	TileRect			*tile05C;		// 05C
	TileRect			*tile060;		// 060
	TileRect			*tile064;		// 064
	TileRect			*tile068;		// 068
	TileRect			*tile06C;		// 06C
	TileRect			*tile070;		// 070
	TESObjectREFR		*containerRef;	// 074
	UInt32				unk078[5];		// 078
	UInt32				leftFilter;		// 08C
	UInt32				rightFilter;	// 090
	UInt32				unk094;			// 094
	MenuItemEntryList	leftItems;		// 098
	TileImage			*tile0A4;		// 0A4
	TileImage			*leftActive;	// 0A8
	TileImage			*tile0AC;		// 0AC
	UInt32				unk0B0;			// 0B0
	UInt16				leftCount;		// 0B4
	UInt16				unk0B6;			// 0B6
	UInt32				unk0B8[4];		// 0B8
	MenuItemEntryList	rightItems;		// 0C8
	TileImage			*tile0D4;		// 0D4
	TileImage			*rightActive;	// 0D8
	TileImage			*tile0DC;		// 0DC
	UInt32				unk0E0;			// 0E0
	UInt16				rightCount;		// 0E4
	UInt16				unk0E6;			// 0E6
	UInt32				unk0E8[4];		// 0E8
	MenuItemEntryList	*currentItems;	// 0F8
	UInt32				unk0FC[4];		// 0FC
};

// 13C
class DialogMenu : public Menu			// 1009
{
public:
	DialogMenu();
	~DialogMenu();

	UInt32				unk028[2];		// 028
	TileImage			*tile030;		// 030
	TileText			*tile034;		// 034
	TileText			*tile038;		// 038
	TileImage			*tile03C;		// 03C
	ListBox<int>		unk040;			// 040
	TileImage			*tile04C;		// 04C
	TileImage			*selected;		// 050
	TileImage			*tile054;		// 054
	UInt32				unk058[6];		// 058
	void				*unk070;		// 070
	UInt32				unk074;			// 074
	TESTopicInfo		*info078;		// 078
	UInt32				unk07C;			// 07C
	TESObjectREFR		*partnerRef;	// 080
	float				unk084[2];		// 084
	void				*unk08C;		// 08C
	void				*unk090;		// 090
	TESTopicInfo		*infos094[16];	// 094
	void				*unk0D4;		// 0D4
	UInt32				unk0D8[3];		// 0D8
	TESTopicInfo		*infos0E4[8];	// 0E4
	UInt32				unk104[3];		// 104
	TESTopicInfo		*infos110[6];	// 110
	float				unk128;			// 128
	void				*unk12C;		// 12C	ImageSpaceModifierInstanceDOF
	UInt32				unk130[3];		// 130
};

// E4
class LockPickMenu : public Menu		// 1014
{
public:
	LockPickMenu();
	~LockPickMenu();

	UInt32				unk28[17];		// 28
	TESObjectREFR		*targetRef;		// 6C
	UInt32				unk70[29];		// 70
};

// 44
class QuantityMenu : public Menu		// 1016
{
public:
	QuantityMenu();
	~QuantityMenu();

	virtual void		Unk_12(void);

	TileRect			*tile28;		// 28
	TileImage			*tile2C;		// 2C
	TileImage			*tile30;		// 30
	TileText			*tile34;		// 34
	TileImage			*tile38;		// 38
	TileImage			*tile3C;		// 3C
	float				currentQtt;		// 40
};

class PlayerCharacter;
// 230
class MapMenu : public Menu				// 1023
{
public:
	MapMenu();
	~MapMenu();

	TileText						*tile028;		// 028
	TileText						*tile02C;		// 02C
	TileImage						*tile030;		// 030
	TileImage						*tile034;		// 034
	TileImage						*tile038;		// 038
	TileImage						*tile03C;		// 03C
	TileRect						*tile040;		// 040
	TileImage						*tile044;		// 044
	TileImage						*tile048;		// 048
	TileImage						*tile04C;		// 04C
	TileImage						*tile050;		// 050
	TileImage						*tile054;		// 054
	TileImage						*tile058;		// 058
	TileRect						*tile05C;		// 05C
	TileImage						*tile060;		// 060
	TileImage						*tile064;		// 064
	TileImage						*tile068;		// 068
	TileRect						*tile06C;		// 06C
	TileImage						*tile070;		// 070
	TileImage						*tile074;		// 074
	TileImage						*tile078;		// 078
	TileText						*tile07C;		// 07C
	UInt8							currentTab;		// 080
	UInt8							pad081[3];		// 081
	TileImage						*tile084;		// 084
	UInt32							unk088[8];		// 088
	BSSimpleArray<String>			arr0A8;			// 0A8
	UInt32							unk0B8[7];		// 0B8
	TileImage						*mapMarker;		// 0D4
	tList<TESObjectREFR>			mapMarkerList;	// 0D8
	tList<TESObjectREFR>			doorList;		// 0E0
	UInt32							unk0E8[8];		// 0E8
	TESObjectCELL					*cell108;		// 108
	TESWorldSpace					*wspc10C;		// 10C
	UInt32							unk110;			// 110
	PlayerCharacter					*pc;			// 114
	TESObjectREFR					*selectedMarker;// 118
	TESObjectCELL					*cell11C;		// 11C
	UInt32							unk120[2];		// 120
	UInt32							unk128;			// 128
	UInt32							unk12C;			// 12C
	ListBox<TESQuest>				questList;		// 130
	TileImage						*tile13C;		// 13C
	UInt32							unk140;			// 140
	TileImage						*tile144;		// 144
	UInt32							unk148[6];		// 148
	ListBox<BGSNote>				noteList;		// 160
	TileImage						*tile16C;		// 16C
	UInt32							unk170;			// 170
	TileImage						*tile174;		// 174
	UInt32							unk178[6];		// 178
	ListBox<TESObjectREFR>			radioRefList;	// 190
	TileImage						*tile19C;		// 19C
	UInt32							unk1A0;			// 1A0
	TileImage						*tile1A4;		// 1A4
	UInt32							unk1A8[6];		// 1A8
	ListBox<BGSQuestObjective>		objectiveList;	// 1C0
	TileImage						*tile1CC;		// 1CC
	UInt32							unk1D0;			// 1D0
	TileImage						*tile1D4;		// 1D4
	UInt32							unk1D8[6];		// 1D8
	ListBox<TESChallenge>			challengeList;	// 1F0
	TileImage						*tile1FC;		// 1FC
	UInt32							unk200;			// 200
	TileImage						*tile204;		// 204
	UInt32							unk208[6];		// 208
	BSSimpleArray<Tile>				arr220;			// 220
};

// 8C
class RepairMenu : public Menu			// 1035
{
public:
	RepairMenu();
	~RepairMenu();

	TileRect				*tile28;		// 28
	TileImage				*tile2C;		// 2C
	TileRect				*tile30;		// 30
	TileImage				*tile34;		// 34
	TileRect				*tile38;		// 38
	TileRect				*tile3C;		// 3C
	TileText				*tile40;		// 40
	TileText				*tile44;		// 44
	TileText				*tile48;		// 48
	TileRect				*tile4C;		// 4C
	TileRect				*tile50;		// 50
	TileImage				*tile54;		// 54
	TileImage				*tile58;		// 58
	MenuItemEntryList		repairItems;	// 5C
	TileImage				*tile68;		// 68
	TileImage				*selected;		// 6C
	TileImage				*tile70;		// 70
	UInt32					unk74[6];		// 74
};

// 5C
class TextEditMenu : public Menu		// 1051
{
public:
	TextEditMenu();
	~TextEditMenu();

	virtual void		Unk_12(void);

	TileText			*currTextTile;		// 28
	TileImage			*okButton;			// 2C
	TileText			*messageTitle;		// 30
	String				currTextStr;		// 34
	String				str3C;				// 3C
	UInt16				strLength;			// 44
	UInt8				pad46[2];			// 46
	UInt32				maxPixelLength;		// 48
	UInt32				unk4C[3];			// 4C
	bool				(*callback)(char*);	// 58
};

typedef tList<ExtraContainerChanges::EntryData> BarterItemList;

// 120
class BarterMenu : public Menu			// 1053
{
public:
	BarterMenu();
	~BarterMenu();

	TileImage			*tile028;		// 028
	TileImage			*tile02C;		// 02C
	TileImage			*tile030;		// 030
	TileText			*tile034;		// 034
	TileImage			*tile038;		// 038
	TileText			*tile03C;		// 03C
	TileImage			*tile040;		// 040
	TileImage			*tile044;		// 044
	TileText			*tile048;		// 048
	TileImage			*tile04C;		// 04C
	TileText			*tile050;		// 050
	TileImage			*tile054;		// 054
	TileRect			*tile058;		// 058
	TileImage			*tile05C;		// 05C
	TileRect			*tile060;		// 060
	TileRect			*tile064;		// 064
	TileRect			*tile068;		// 068
	TileRect			*tile06C;		// 06C
	TileRect			*tile070;		// 070
	TileImage			*tile074;		// 074
	TileImage			*tile078;		// 078
	TileRect			*tile07C;		// 07C
	TESObjectREFR		*merchantRef;	// 080
	float				barterTotalSum;	// 084
	UInt32				unk088;			// 084
	UInt32				playerGold;		// 08C
	UInt32				merchantGold;	// 090
	UInt32				unk094[2];		// 094
	UInt32				leftFilter;		// 09C
	UInt32				rightFilter;	// 0A0
	UInt32				unk0A4;			// 0A4
	MenuItemEntryList	leftItems;		// 0A8
	TileImage			*tile0B4;		// 0B4
	TileImage			*leftActive;	// 0B8
	TileImage			*tile0BC;		// 0BC
	UInt32				unk0C0;			// 0C0
	UInt16				leftCount;		// 0C4
	UInt16				unk0C6;			// 0C6
	UInt32				unk0C8[4];		// 0C8
	MenuItemEntryList	rightItems;		// 0D8
	TileImage			*tile0E4;		// 0E4
	TileImage			*rightActive;	// 0E8
	TileImage			*tile0EC;		// 0EC
	UInt32				unk0F0;			// 0F0
	UInt16				rightCount;		// 0F4
	UInt16				unk0F6;			// 0F6
	UInt32				unk0F8[4];		// 0F8
	MenuItemEntryList	*currentItems;	// 108
	BarterItemList		leftBarter;		// 10C
	BarterItemList		rightBarter;	// 114
	UInt32				unk11C;			// 11C
};

// 1DC
class HackingMenu : public Menu			// 1055
{
public:
	HackingMenu();
	~HackingMenu();

	UInt32				unk028[92];		// 028
	TESObjectREFR		*targetRef;		// 198
	UInt32				unk19C[16];		// 19C
};

// 144
class VATSMenu : public Menu			// 1056
{
public:
	VATSMenu();
	~VATSMenu();

	virtual void		Unk_12(void);

	UInt32				unk028;			// 028
	TileImage			*tile02C;		// 02C
	TileImage			*tile030;		// 030
	TileImage			*tile034;		// 034
	TileImage			*tile038;		// 038
	TileImage			*tile03C;		// 03C
	TileImage			*tile040;		// 040
	TileImage			*tile044;		// 044
	TileImage			*tile048;		// 048
	TileImage			*tile04C;		// 04C
	TileImage			*tile050;		// 050
	TileText			*tile054;		// 054
	TileText			*tile058;		// 058
	TileText			*tile05C;		// 05C
	TileText			*tile060;		// 060
	TileImage			*tile064;		// 064
	TileImage			*tile068;		// 068
	TileImage			*tile06C;		// 06C
	TileImage			*tile070;		// 070
	TileText			*tile074;		// 074
	TileRect			*tile078;		// 078
	TileRect			*tile07C;		// 07C
	TileRect			*tile080;		// 080
	TileImage			*tile084;		// 084
	TileRect			*tile088;		// 088
	TileImage			*tile08C;		// 08C
	TileImage			*tile090;		// 090
	TileImage			*tile094;		// 094
	TileImage			*tile098;		// 098
	TileText			*tile09C;		// 09C
	TileImage			*tile0A0;		// 0A0
	TileImage			*tile0A4;		// 0A4
	UInt32				unk0A8[2];		// 0A8
	ListBox<UInt32>		queuedActions;	// 0B0
	TileImage			*tile0BC;		// 0BC
	TileImage			*tile0C0;		// 0C0
	TileImage			*tile0C4;		// 0C4
	UInt32				unk0C8[24];		// 0C8
	TESObjectREFR		*targetRef;		// 128
	UInt32				unk12C;			// 12C
	void				*ptr130;		// 130
	UInt32				unk134[4];		// 134
};

// FC
class ComputersMenu : public Menu		// 1057
{
public:
	ComputersMenu();
	~ComputersMenu();

	UInt32				unk28[33];		// 28
	TESObjectREFR		*targetRef;		// AC
	UInt32				unkB0[19];		// B0
};

// A0
class RepairServicesMenu : public Menu	// 1058
{
public:
	RepairServicesMenu();
	~RepairServicesMenu();

	TileText			*tile28;		// 28
	TileText			*tile2C;		// 2C
	TileImage			*tile30;		// 30
	TileImage			*tile34;		// 34
	TileRect			*tile38;		// 38
	TileRect			*tile3C;		// 3C
	TileRect			*tile40;		// 40
	TileText			*tile44;		// 44
	TileText			*tile48;		// 48
	TileText			*tile4C;		// 4C
	TileRect			*tile50;		// 50
	TileRect			*tile54;		// 54
	TileText			*tile58;		// 58
	TileImage			*tile5C;		// 5C
	TileImage			*tile60;		// 60
	TileImage			*tile64;		// 64
	MenuItemEntryList	itemList;		// 68
	TileImage			*tile74;		// 74
	TileImage			*selected;		// 78
	TileImage			*tile7C;		// 7C
	UInt32				unk80[2];		// 80
	float				unk88;			// 88
	UInt32				unk8C[2];		// 8C
	float				unk94;			// 94
	UInt32				unk98;			// 98
	UInt8				skill;			// 9C
	UInt8				pad9D[3];		// 9D
};

// 90
class ItemModMenu : public Menu			// 1061
{
public:
	ItemModMenu();
	~ItemModMenu();

	TileRect			*tile28;		// 28
	TileImage			*tile2C;		// 2C
	TileRect			*tile30;		// 30
	TileImage			*tile34;		// 34
	TileRect			*tile38;		// 38
	TileRect			*tile3C;		// 3C
	TileText			*tile40;		// 40
	TileText			*tile44;		// 44
	TileText			*tile48;		// 48
	TileRect			*tile4C;		// 4C
	TileRect			*tile50;		// 50
	TileImage			*tile54;		// 54
	TileImage			*tile58;		// 58
	TileText			*tile5C;		// 5C
	MenuItemEntryList	itemModList;	// 60
	TileImage			*tile6C;		// 6C
	TileImage			*selected;		// 70
	TileImage			*tile74;		// 74
	UInt32				unk78[6];		// 78
};

// 88
class CompanionWheelMenu : public Menu	// 1075
{
public:
	CompanionWheelMenu();
	~CompanionWheelMenu();

	virtual void		Unk_12(void);

	UInt32				unk28[16];		// 28
	Actor				*companionRef;	// 68
	UInt32				unk6C[7];		// 6C
};

// 104
class RecipeMenu : public Menu			// 1077
{
public:
	RecipeMenu();
	~RecipeMenu();

	TileImage					*tile028;		// 028
	TileText					*tile02C;		// 02C
	TileImage					*tile030;		// 030
	TileImage					*tile034;		// 034
	TileText					*tile038;		// 038
	TileText					*tile03C;		// 03C
	TileImage					*tile040;		// 040
	TileImage					*tile044;		// 044
	TileImage					*tile048;		// 048
	TileImage					*tile04C;		// 04C
	TileRect					*tile050;		// 050
	TileText					*tile054;		// 054
	TileText					*tile058;		// 058
	TileText					*tile05C;		// 05C
	TESObjectREFR				*sourceRef;		// 060
	TESRecipeCategory			*category;		// 064
	UInt32						unk068;			// 068
	ListBox<TESRecipe>			recipeList;		// 06C
	TileImage					*tile078;		// 078
	TileImage					*tile07C;		// 07C
	TileImage					*tile080;		// 080
	UInt32						unk084;			// 084
	UInt16						count;			// 088
	UInt16						unk08A;			// 08A
	UInt32						unk08C[4];		// 084
	ListBox<TESRecipe>			*unk09C;		// 09C
	ListBox<TESRecipe::Entry>	componentList;	// 0A0
	TileImage					*tile0AC;		// 0AC
	TileImage					*tile0B0;		// 0B0
	TileImage					*tile0B4;		// 0B4
	UInt32						unk0B8[6];		// 0B8
	ListBox<Condition>			conditionList;	// 0D0
	UInt32						unk0DC[10];		// 0DC
};

// E88
class CaravanMenu : public Menu			// 1083
{
public:
	CaravanMenu();
	~CaravanMenu();

	UInt32					unk028[54];		// 028
	TESObjectREFR			*opponentRef;	// 100
	UInt32					unk104[865];	// 104
};

