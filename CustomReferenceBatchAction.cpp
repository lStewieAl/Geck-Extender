#include "CustomReferenceBatchAction.h"

#include "GeckUtility.h"
#include "GameObjects.h"
#include "settings.h"

namespace CustomReferenceBatchAction
{
	/*
	 To add new actions: add to the enum and the actions list
	*/

	enum CtrlIDs
	{
		IDC_SET_REFLECTED_BY = 1033,
		IDC_SET_REFRACTED_BY = 1034,
		IDC_SET_ENABLE_PARENT = 1035,
		IDC_CUSTOM_BUTTON,
		IDC_SET_LINKED_REF = 1037,
		IDC_SET_ACTIVATE_REF = 1038,
		IDC_SET_MULTIBOUND = 1039,
		IDC_SELECTED_REFERENCES_LIST = 1018,
		IDC_SET_EXTERNAL_EMITTANCE = 1040,
		IDC_SET_OWNER = 1041,

		IDC_SELECT_REFERENCE_RENDER_WINDOW = 1982,
		IDC_CELL_COMBO = 2063,
		IDC_REFERENCE_COMBO = 1983,
		IDC_CUSTOM_DROPDOWN,

		IDC_SET_REFLECTED_BY_AUTOWATER = 5062,
		IDC_SET_REFRACTED_BY_AUTOWATER = 5063,

		IDC_TARGET_REFERENCE_LABEL = 5103,
		IDC_BOTTOM_COMBO = 5104,
		IDC_BOTTOM_COMBO_LABEL = 5105,
		IDC_TOP_COMBO_LABEL = 5106,
		IDC_TOP_COMBO = 5107,

		IDC_OPPOSITE_CHECK = 5137,

		IDC_DO = 1,
	};


	enum eCustomACTION : UInt32
	{
		MIN_COUNT = ReferenceBatchAction::eACTION::MAX_COUNT,

		SET_INITIALLY_DISABLED = MIN_COUNT,
		SET_ENCOUNTER_ZONE,
		SET_LEVEL_MODIFIER,

		MAX_COUNT
	};

	struct CustomActionDef
	{
		eCustomACTION id;
		const char* name;
		bool needsRef;

		bool(__cdecl* refComboboxFilter)(TESObjectREFR*, void*); // return true for refs that should be shown in the combobox
		bool (*execute)(TESObjectREFR*);
		void (*setupForm)();

		static const CustomActionDef* GetAction(eCustomACTION id);
	};

	static const CustomActionDef kActions[] =
	{
		{
			SET_INITIALLY_DISABLED,
			"Set Initially Disabled",
			false,

			nullptr,

			[](TESObjectREFR* pRef) -> bool
			{
				pRef->SetDisabled(true);
				Console_Print("SET_INITIALLY_DISABLED: %s | TRUE", pRef->GetEditorID());
				return true;
			},

			nullptr
		},

		{
			SET_ENCOUNTER_ZONE,
			"Set Encounter Zone",
			false,

			nullptr,

			[](TESObjectREFR* pRef) -> bool
			{
				Console_Print("CustomReferenceBatchActions: TODO Implement Set Encounter Zone");
				return false;
			},

			[]()
			{
				// TODO implement setup form
			}
		},

		{
			SET_LEVEL_MODIFIER,
			"Set Level Modifier",
			false,

			nullptr,

			[](TESObjectREFR* pRef) -> bool
			{
				if (!pRef->baseForm || !(pRef->baseForm->typeID == kFormType_TESNPC || pRef->baseForm->typeID == kFormType_Creature))
				{
					return false;
				}

				TESActorBase* actorBase = (TESActorBase*)pRef->baseForm;
				if (!ThisCall<bool>(0x4EAF20, &actorBase->baseData))
				{
					return false;
				}

				auto hWnd = ReferenceBatchAction::GetWindow();
				auto hTopCombo = GetDlgItem(hWnd, IDC_TOP_COMBO);
				int iSelectedLevel = TESComboBox::GetSelectedItemData(hTopCombo);

				pRef->SetLevCreaModifier(iSelectedLevel);

				auto ppActorLevelLabels = (char**)0xE92120;
				Console_Print("SET_LEVEL_MODIFIER: %s | %s", pRef->GetEditorID(), ppActorLevelLabels[iSelectedLevel]);
				return true;
			},

			[]()
			{
				auto hWnd = ReferenceBatchAction::GetWindow();
				auto hTopLabel = GetDlgItem(hWnd, IDC_TOP_COMBO_LABEL);
				auto hTopCombo = GetDlgItem(hWnd, IDC_TOP_COMBO);

				SetWindowTextA(hTopLabel, "Level Modifier");
				SendMessageA(hTopCombo, CB_RESETCONTENT, 0, 0);
				auto ppActorLevelLabels = (char**)0xE92120;
				for (int i = 0; i < 5; ++i)
				{
					TESComboBox::AddItem(hTopCombo, ppActorLevelLabels[i], i);
				}

				ShowWindow(hTopLabel, SW_SHOW);
				ShowWindow(hTopCombo, SW_SHOW);
			}
		}
	};

	const CustomActionDef* CustomActionDef::GetAction(eCustomACTION id)
	{
		for (auto& action : kActions)
		{
			if (action.id == id)
				return &action;
		}
		return nullptr;
	}

	constexpr size_t kActionCount = sizeof(kActions) / sizeof(kActions[0]);

	const bool CustomActionNeedsRef[MAX_COUNT - MIN_COUNT] = { false, false, false };

	void AddCustomReferenceBatchActionCombobox(HWND hDlg)
	{
		RECT rcBtn = { 224, 162, 224 + 9, 162 + 9 };

		MapDialogRect(hDlg, &rcBtn);

		CreateWindowEx(
			0,
			"BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			rcBtn.left,
			rcBtn.top,
			rcBtn.right - rcBtn.left,
			rcBtn.bottom - rcBtn.top,
			hDlg,
			(HMENU)IDC_CUSTOM_BUTTON,
			GetModuleHandle(NULL),
			NULL
		);

		RECT rc = { 238, 162, 238 + 110, 162 + 99 };

		MapDialogRect(hDlg, &rc);

		HWND hCombo = CreateWindowEx(
			0,
			"COMBOBOX",
			"",
			WS_CHILD | WS_VISIBLE |
			CBS_DROPDOWNLIST |
			CBS_SORT |
			WS_VSCROLL |
			WS_TABSTOP,
			rc.left,
			rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			hDlg,
			(HMENU)IDC_CUSTOM_DROPDOWN,
			GetModuleHandle(NULL),
			NULL
		);
		HFONT hFont = (HFONT)SendMessage(hDlg, WM_GETFONT, 0, 0);
		SendMessage(hCombo, WM_SETFONT, (WPARAM)hFont, TRUE);

		int idx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Set Initially Disabled");
		SendMessage(hCombo, CB_SETITEMDATA, idx, SET_INITIALLY_DISABLED);

		idx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Set Encounter Zone");
		SendMessage(hCombo, CB_SETITEMDATA, idx, SET_ENCOUNTER_ZONE);

		idx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Set Level Modifier");
		SendMessage(hCombo, CB_SETITEMDATA, idx, SET_LEVEL_MODIFIER);

		SendMessage(hCombo, CB_SETCURSEL, 0, 0);

		HWND hGroup = FindWindowEx(hDlg, NULL, "Button", "Action");
		RECT rcAction = { 215, 14, 215 + 138, 14 + 175 }; // increase height from 169 -> 175

		MapDialogRect(hDlg, &rcAction);

		SetWindowPos(
			hGroup,
			NULL,
			rcAction.left,
			rcAction.top,
			rcAction.right - rcAction.left,
			rcAction.bottom - rcAction.top,
			SWP_NOZORDER
		);
	}

	bool __cdecl CanSetInitiallyDisabled(TESObjectREFR* apRef, void* formal)
	{
		return true;
	}

	void __cdecl OnSetFormButtonsForAction(UInt32 auiActionID)
	{
		CdeclCall(0x4114D0, auiActionID); // enables controls, sets text and fills dropdowns for the selected action

		auto hWnd = ReferenceBatchAction::GetWindow();
		auto uiAction = ReferenceBatchAction::GetAction();
		auto pRefSelector = RefSelectControl::Get(hWnd, IDC_SELECT_REFERENCE_RENDER_WINDOW);

		auto def = CustomActionDef::GetAction((eCustomACTION)uiAction);
		if (def)
		{
			if (def->refComboboxFilter)
			{
				pRefSelector->SetComparator(def->refComboboxFilter);
			}

			if (def->setupForm)
			{
				def->setupForm();
			}
		}
	}

	void OnSetCustomAction(eCustomACTION auiAction)
	{
		auto hWnd = ReferenceBatchAction::GetWindow();
		auto pRefSelector = RefSelectControl::Get(hWnd, IDC_SELECT_REFERENCE_RENDER_WINDOW);
		pRefSelector->SetEnabled(true);

		OnSetFormButtonsForAction(auiAction);

		auto pSelectedRef = pRefSelector->GetSelectedRef();
		if (!pRefSelector->AllowsPickRef(pSelectedRef))
		{
			ThisCall(0x48C190, pRefSelector, nullptr, nullptr);
		}
		ReferenceBatchAction::SetRef(pSelectedRef);

		auto def = CustomActionDef::GetAction(auiAction);
		bool bEnableDoButton = pSelectedRef != nullptr || (def && !def->needsRef);
		auto hDoButton = GetDlgItem(hWnd, IDC_DO);
		EnableWindow(hDoButton, bEnableDoButton);
	}

	void OnDoAction()
	{
		eCustomACTION auiAction = (eCustomACTION)ReferenceBatchAction::GetAction();
		if (auiAction < eCustomACTION::MIN_COUNT)
		{
			CdeclCall(0x4110D0); // ReferenceBatchAction::DoAction
		}
		else
		{
			auto def = CustomActionDef::GetAction(auiAction);
			if (def && def->execute)
			{
				MessageHandler::DisableWarnings();
				auto selected = RenderWindow::SelectedData::GetSelected();
				for (auto ref : *selected)
				{
					if (ref)
					{
						// vanilla calls Unk_4E, but that marks as modified - just check the temporary flag instead as it does.
						// the behaviour is slightly different for VC, since vanilla also checks some method using the Fud and Fvd arrays
						if (ref->Unk_43() && !ref->IsTemporary())
						{
							if (def->execute(ref))
							{
								ref->MarkAsModified(true);
							}
						}
					}
				}
				MessageHandler::EnableWarnings();
			}
		}
	}

	WNDPROC originalReferenceBatchActionFn;
	BOOL CALLBACK ReferenceBatchActionCallback(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_INITDIALOG)
		{
			auto result = CallWindowProc(originalReferenceBatchActionFn, hDlg, msg, wParam, lParam);

			if (config.bCustomReferenceBatchActions)
			{
				AddCustomReferenceBatchActionCombobox(hDlg);
			}

			// make the "Do" button not grayed out if an action is already selected when initialising the dialog
			auto currentAction = ReferenceBatchAction::GetAction();
			auto hDoButton = GetDlgItem(hDlg, IDC_DO);
			EnableWindow(hDoButton, currentAction != 0);
			return result;
		}
		else if (msg == WM_COMMAND)
		{
			UINT controlId = LOWORD(wParam);
			UINT notifyCode = HIWORD(wParam);

			if (controlId == IDC_CUSTOM_BUTTON)
			{
				HWND hCombo = GetDlgItem(hDlg, IDC_CUSTOM_DROPDOWN);
				int sel = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);

				if (sel != CB_ERR)
				{
					eCustomACTION action = (eCustomACTION)SendMessage(hCombo, CB_GETITEMDATA, sel, 0);
					OnSetCustomAction(action);
				}
			}

			// Changing dropdown selection
			else if (controlId == IDC_CUSTOM_DROPDOWN &&
				notifyCode == CBN_SELCHANGE)
			{
				// Automatically select custom radio button
				CheckRadioButton(hDlg, IDC_SET_REFLECTED_BY, IDC_SET_OWNER, IDC_CUSTOM_BUTTON);

				HWND hCombo = GetDlgItem(hDlg, IDC_CUSTOM_DROPDOWN);
				int sel = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);

				if (sel != CB_ERR)
				{
					eCustomACTION action = (eCustomACTION)SendMessage(hCombo, CB_GETITEMDATA, sel, 0);
					OnSetCustomAction(action);
				}

				return TRUE;
			}
		}
		return CallWindowProc(originalReferenceBatchActionFn, hDlg, msg, wParam, lParam);
	}

	void Init()
	{
		originalReferenceBatchActionFn = (WNDPROC)DetourVtable(0x456B3E, UInt32(ReferenceBatchActionCallback));
		if (config.bCustomReferenceBatchActions)
		{
			WriteRelCall(0x411B75, UInt32(OnSetFormButtonsForAction));
			WriteRelCall(0x411C8D, UInt32(OnSetFormButtonsForAction));
			WriteRelCall(0x41195F, UInt32(OnDoAction));
		}
	}
}