#include <CommCtrl.h>
#include <GECKUtility.h>
namespace ObjectWindowTreeHooks
{
	struct ObjectWindowTreeViewItem
	{
		UInt32 nodeId;
		UInt32 rootNodeId;
		char fullPath[MAX_PATH];
		char nodeName[MAX_PATH];
	};

	enum ObjectWindow__eNode
	{
		kObjWndNode_Activator = 0x0,
		kObjWndNode_Armor = 0x1,
		kObjWndNode_Book = 0x2,
		kObjWndNode_Container = 0x3,
		kObjWndNode_Door = 0x4,
		kObjWndNode_Ingredient = 0x5,
		kObjWndNode_Light = 0x6,
		kObjWndNode_MiscItem = 0x7,
		kObjWndNode_Static = 0x8,
		kObjWndNode_Grass = 0x9,
		kObjWndNode_Tree = 0xA,
		kObjWndNode_Weapon = 0xB,
		kObjWndNode_NPC = 0xC,
		kObjWndNode_Creature = 0xD,
		kObjWndNode_LeveledCreature = 0xE,
		kObjWndNode_LeveledCharacter = 0xF,
		kObjWndNode_ActorEffect = 0x10,
		kObjWndNode_ObjectEffect = 0x11,
		kObjWndNode_Ingestible = 0x12,
		kObjWndNode_LeveledItem = 0x13,
		kObjWndNode_Key = 0x14,
		kObjWndNode_Ammo = 0x15,
		kObjWndNode_Flora = 0x16,
		kObjWndNode_Furniture = 0x17,
		kObjWndNode_Sound = 0x18,
		kObjWndNode_LandTexture = 0x19,
		kObjWndNode_CombatStyle = 0x1A,
		kObjWndNode_LoadScreen = 0x1B,
		kObjWndNode_LeveledSpell = 0x1C,
		kObjWndNode_AnimObject = 0x1D,
		kObjWndNode_WaterType = 0x1E,
		kObjWndNode_IdleMarker = 0x1F,
		kObjWndNode_EffectShader = 0x20,
		kObjWndNode_Projectile = 0x21,
		kObjWndNode_TalkingActivator = 0x22,
		kObjWndNode_Explosion = 0x23,
		kObjWndNode_TextureSet = 0x24,
		kObjWndNode_Debris = 0x25,
		kObjWndNode_MenuIcon = 0x26,
		kObjWndNode_FormList = 0x27,
		kObjWndNode_Perk = 0x28,
		kObjWndNode_BodyPartData = 0x29,
		kObjWndNode_AddOnNode = 0x2A,
		kObjWndNode_MovableStatic = 0x2B,
		kObjWndNode_Terminal = 0x2C,
		kObjWndNode_PlaceableWater = 0x2D,
		kObjWndNode_CameraShot = 0x2E,
		kObjWndNode_ImpactData = 0x2F,
		kObjWndNode_ImpactDataSet = 0x30,
		kObjWndNode_Quest = 0x31,
		kObjWndNode_Package = 0x32,
		kObjWndNode_VoiceType = 0x33,
		kObjWndNode_Class = 0x34,
		kObjWndNode_Race = 0x35,
		kObjWndNode_Eyes = 0x36,
		kObjWndNode_HeadPart = 0x37,
		kObjWndNode_Faction = 0x38,
		kObjWndNode_Note = 0x39,
		kObjWndNode_Weather = 0x3A,
		kObjWndNode_Climate = 0x3B,
		kObjWndNode_ArmorAddon = 0x3C,
		kObjWndNode_Global = 0x3D,
		kObjWndNode_Imagespace = 0x3E,
		kObjWndNode_ImagespaceModifier = 0x3F,
		kObjWndNode_EncounterZone = 0x40,
		kObjWndNode_Message = 0x41,
		kObjWndNode_ConstructibleObject = 0x42,
		kObjWndNode_AcousticSpace = 0x43,
		kObjWndNode_Ragdoll = 0x44,
		kObjWndNode_Script = 0x45,
		kObjWndNode_BaseEffect = 0x46,
		kObjWndNode_MusicType = 0x47,
		kObjWndNode_StaticCollection = 0x48,
		kObjWndNode_ItemMod = 0x49,
		kObjWndNode_Reputation = 0x4A,
		kObjWndNode_Recipe = 0x4B,
		kObjWndNode_RecipeCategory = 0x4C,
		kObjWndNode_CasinoChip = 0x4D,
		kObjWndNode_Casino = 0x4E,
		kObjWndNode_LoadScreenType = 0x4F,
		kObjWndNode_MediaSet = 0x50,
		kObjWndNode_MediaLocationController = 0x51,
		kObjWndNode_Challenge = 0x52,
		kObjWndNode_AmmoEffect = 0x53,
		kObjWndNode_CaravanCard = 0x54,
		kObjWndNode_CaravanMoney = 0x55,
		kObjWndNode_CaravanDeck = 0x56,
		kObjWndNode_ALL = 0x57,
		kObjWndNode_NavMesh_Unimplemented = 0x59,
	};

	void __cdecl ObjectWindowTreeView__ExpandIfExpandedInINI(HWND hWnd, HTREEITEM ahTreeItem)
	{
		CdeclCall(0x448F80, hWnd, ahTreeItem);
	}

	char** ObjectWindow__kNodeNames = (char**)0xEA6DB8;
	char** ObjectWindow__kRootNodeNames = (char**)0xEA6D94;
	HTREEITEM __cdecl ObjectWindowTreeView__FindItemRecurse(
		HWND hWnd,
		HTREEITEM apItem,
		int aiNodeID,
		int aiRootNodeID,
		char* apSrc)
	{
		HTREEITEM next = apItem;
		HTREEITEM result = NULL;
		ObjectWindowTreeViewItem* item = nullptr;
		if (apItem && hWnd)
		{
			while (1)
			{
				if (result)
					return result;
				if (!next)
					break;

				tagTVITEMA lParam;
				lParam.hItem = next;
				lParam.mask = TVIF_HANDLE;
				if (!SendMessageA(hWnd, TVM_GETITEMA, 0, (LPARAM)&lParam) || !lParam.lParam)
				{
					break;
				}

				item = (ObjectWindowTreeViewItem*)lParam.lParam;
				if (item->nodeId != aiNodeID || item->rootNodeId != aiRootNodeID)
				{
					break;
				}

				if (apSrc ? _stricmp(apSrc, item->fullPath) : item->fullPath[0])
				{
					break;
				}

				return next;

			LABEL_16:
				next = (HTREEITEM)SendMessageA(hWnd, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)next);
				if (!next)
					return result;
			}
			auto child = (HTREEITEM)SendMessageA(hWnd, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)next);
			if (child)
			{
				result = ObjectWindowTreeView__FindItemRecurse(hWnd, child, aiNodeID, aiRootNodeID, apSrc);
				if (result)
				{
					return result;
				}
			}
			goto LABEL_16;
		}
		return result;
	}

	HTREEITEM __cdecl ObjectWindowTreeView__FindItemRecurseCached(HWND hWnd, HTREEITEM apItem, int aiNodeID, int aiRootNodeID, char* apName)
	{
		// Cache Size -> Cache Hits
		// 1 -> 879
		// 2 -> 1183
		// 3 -> 1376
		// 4 -> 1500
		// 8 -> 1752
		// 16 -> 1884
		// 32 -> 1967
		// 64 -> 2001
		constexpr int CACHE_SIZE = 8;

		static struct
		{
			HTREEITEM pItem;
			int iNodeID;
			int iRootNodeID;
			char kName[0x80];
			HTREEITEM result;

			bool Equals(HTREEITEM apItem, int aiNodeID, int aiRootNodeID, char* apName)
			{
				return apItem == pItem && aiNodeID == iNodeID && aiRootNodeID == iRootNodeID && apName && !strcmp(apName, kName);
			}

			void Set(HTREEITEM apItem, int aiNodeID, int aiRootNodeID, char* apName, HTREEITEM ahResult)
			{
				result = ahResult;
				iRootNodeID = aiRootNodeID;
				iNodeID = aiNodeID;
				pItem = apItem;
				if (apName)
				{
					strncpy(kName, apName, sizeof(kName));
				}
				else
				{
					*kName = '\xFF';
					kName[1] = '\0';
				}
			}
		} LAST[CACHE_SIZE];
		static int lastIndex;

		for (UInt32 i = 0; i < CACHE_SIZE; ++i)
		{
			UInt32 index = (lastIndex - i) % CACHE_SIZE;
			if (LAST[index].Equals(apItem, aiNodeID, aiRootNodeID, apName))
			{
				return LAST[index].result;
			}
		}

		auto result = ObjectWindowTreeView__FindItemRecurse(hWnd, apItem, aiNodeID, aiRootNodeID, apName);
		if (result)
		{
			LAST[lastIndex].Set(apItem, aiNodeID, aiRootNodeID, apName, result);
			lastIndex = (lastIndex + 1) % CACHE_SIZE;
		}
		return result;
	}

	HTREEITEM __cdecl ObjectWindowTreeView__SetupNodeNameRecurse(HWND hWnd, int nodeId, int rootNodeId, char* Src)
	{
		HTREEITEM rootNode = (HTREEITEM)SendMessageA(hWnd, TVM_GETNEXTITEM, 0, 0);

		auto result = ObjectWindowTreeView__FindItemRecurseCached(hWnd, rootNode, nodeId, rootNodeId, Src);
		if (result)
		{
			return result;
		}

		ObjectWindowTreeViewItem* item = (ObjectWindowTreeViewItem*)FormHeap_Allocate(sizeof(ObjectWindowTreeViewItem));
		item->fullPath[0] = '\0';
		item->nodeName[0] = '\0';
		item->nodeId = nodeId;
		item->rootNodeId = rootNodeId;

		const char* nodeName = nullptr;
		if (nodeId != -1)
		{
			if (Src)
			{
				strncpy(item->fullPath, Src, sizeof(item->fullPath));
				auto lastSlash = strrchr(Src, '\\');
				char* tail = lastSlash ? lastSlash + 1 : Src;
				strncpy(item->nodeName, tail, sizeof(item->nodeName));
				goto LABEL_14;
			}
			if (nodeId >= 87)
			{
				strcpy(item->nodeName, "*All");
			LABEL_14:
				HTREEITEM hParent = TVI_ROOT;

				if (nodeId != -1)
				{
					char replacedChar;
					char* lastSlash = nullptr;

					int otherNodeId = -1;
					if (item->fullPath[0])
					{
						lastSlash = strrchr(item->fullPath, '\\');
						if (!lastSlash)
						{
							lastSlash = item->fullPath;
						}
						replacedChar = *lastSlash;
						*lastSlash = '\0';

						otherNodeId = nodeId;
					}

					hParent = ObjectWindowTreeView__SetupNodeNameRecurse(hWnd, otherNodeId, rootNodeId, item->fullPath);
					if (lastSlash)
					{
						*lastSlash = replacedChar;
					}

					if (!hParent)
						hParent = TVI_ROOT;
				}

				tagTVINSERTSTRUCTW lParam;
				lParam.itemex.mask = TVIF_TEXT | TVIF_PARAM;
				lParam.hInsertAfter = nodeId == kObjWndNode_ALL ? TVI_LAST : TVI_SORT;
				lParam.hParent = hParent;
				lParam.itemex.lParam = (LPARAM)item;
				lParam.itemex.pszText = LPSTR_TEXTCALLBACKW;
				HTREEITEM result = (HTREEITEM)SendMessageA(hWnd, TVM_INSERTITEM, 0, (LPARAM)&lParam);
				if (nodeId != -1)
				{
					if (result)
					{
						ObjectWindowTreeView__ExpandIfExpandedInINI(hWnd, result);
					}
				}
				return result;
			}
			nodeName = ObjectWindow__kNodeNames[nodeId];
		LABEL_13:
			strncpy(item->nodeName, nodeName, sizeof(item->nodeName));
			goto LABEL_14;
		}
		if (rootNodeId < 8)
		{
			nodeName = ObjectWindow__kRootNodeNames[rootNodeId];
			goto LABEL_13;
		}
		return 0;
	}

	void DisableObjectWindowTreeRedraw()
	{
		auto objectWindowTree = ObjectsView::GetTreeView();
		SendMessage(objectWindowTree, WM_SETREDRAW, FALSE, 0);
	}

	__HOOK OnSetupObjectWindowTreeViewHook()
	{
		_asm
		{
			push 0x44C5AF
			jmp DisableObjectWindowTreeRedraw
		}
	}

	ObjectWindowTreeViewItem* __cdecl PostSetupObjectWindowTreeView(HWND objectWindowTree, HTREEITEM hTreeItem)
	{
		SendMessage(objectWindowTree, WM_SETREDRAW, TRUE, 0);
		RedrawWindow(objectWindowTree, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
		return CdeclCall<ObjectWindowTreeViewItem*>(0x448E00, objectWindowTree, hTreeItem);
	}

	void InitHooks()
	{
		// speed up the object window insertions (~20%) by caching some results and skipping unnecessary strcpys
		WriteRelJump(0x4490E0, UInt32(ObjectWindowTreeView__SetupNodeNameRecurse));
		WriteRelCall(0x44C9D0, UInt32(ObjectWindowTreeView__SetupNodeNameRecurse));
		WriteRelCall(0x4499C5, UInt32(ObjectWindowTreeView__SetupNodeNameRecurse));
		WriteRelCall(0x4499A6, UInt32(ObjectWindowTreeView__SetupNodeNameRecurse));

		// speed up the object window tree view
		WriteRelJz(0x44C1C9, UInt32(OnSetupObjectWindowTreeViewHook));
		WriteRelCall(0x44C5F5, UInt32(PostSetupObjectWindowTreeView));
	}
}