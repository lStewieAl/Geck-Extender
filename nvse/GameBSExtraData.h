#pragma once

// Added to remove a cyclic dependency between GameForms.h and GameExtraData.h

class TESForm;
class TESObjectREFR;

// C+?
class BSExtraData
{
public:
	BSExtraData();
	virtual ~BSExtraData();

	virtual bool Differs(BSExtraData* extra);	// 001

	static BSExtraData* Create(UInt8 xType, UInt32 size, UInt32 vtbl);

//	void		** _vtbl;	// 000
	UInt8		type;		// 004
	UInt8		pad[3];		// 005
	BSExtraData	* next;		// 008
};

// 020
struct BaseExtraList
{
	bool			HasType(UInt32 type) const;
	BSExtraData *	GetByType(UInt32 type) const;

	void			MarkType(UInt32 type, bool bCleared);
	bool			Remove(BSExtraData* toRemove, bool free = false);
	bool			RemoveByType(UInt32 type, bool free = false);
	bool			Add(BSExtraData* toAdd);
	void			RemoveAll();

	bool			MarkScriptEvent(UInt32 eventMask, TESForm* eventTarget);

	void			Copy(BaseExtraList* from);

	void			DebugDump() const;

	bool			IsWorn();

	void		** m_vtbl;					// 000
	BSExtraData	* m_data;					// 004
	UInt8		m_presenceBitfield[0x15];	// 008 - if a bit is set, then the extralist should contain that extradata
											// bits are numbered starting from the lsb
	UInt8		pad1D[3];					// 01D
};

struct ExtraDataList : public BaseExtraList
{
	static ExtraDataList * Create(BSExtraData* xBSData = NULL);

	TESObjectREFR* GetMultiBoundRef() const;
};

STATIC_ASSERT(offsetof(BaseExtraList, m_presenceBitfield) == 0x008);
STATIC_ASSERT(sizeof(ExtraDataList) == 0x020);

class DialogExtraSubWindow : BSExtraData
{
	struct ExtraSubWindow
	{
		UInt32 unk00;
		UInt32 unk04;
		HWND parent;
		UInt32 _hInstance;
		UInt32 posX;
		UInt32 posY;
		HWND unkDlgItem;
		HWND dialogWindow;
	};

public:
	ExtraSubWindow* subWindow;
	UInt32 menuID;
};

enum MenuExtraData
{
	kMenuExtra_DialogExtraParam = 0x0,
	kMenuExtra_DialogExtraLocalCopy = 0x1,
	kMenuExtra_DialogExtraColorControl = 0x2,
	kMenuExtra_DialogExtraSubWindow = 0x4,
	kMenuExtra_DialogExtraCurrCondItem = 0x5,
	kMenuExtra_DialogExtraWorkingData = 0x6,
	kMenuExtra_DialogExtraFactionData = 0x7,
	kMenuExtra_DialogExtraPopupMenu = 0x8,
	kMenuExtra_DialogExtraRefSelectControl = 0x9,
	kMenuExtra_DialogExtraReactionData = 0xB,
	kMenuExtra_DialogExtraPreviewControl = 0xC,
	kMenuExtra_DialogExtraFreeformFaceControl = 0xD,
	kMenuExtra_DialogExtraDialogData = 0xE,
	kMenuExtra_DialogExtraNotifyInfo = 0xF,
	kMenuExtra_DialogExtraQuestStageData = 0x10,
	kMenuExtra_DialogExtraTimer = 0x11,
	kMenuExtra_DialogExtraQuestFilter = 0x12,
	kMenuExtra_DialogExtraRects = 0x13,
	kMenuExtra_ConversationEditorExtraData = 0x15,
};