#include "GeckUtility.h"
#include "GameObjects.h"
#include "settings.h"

namespace CustomReferenceBatchAction
{
	enum eCustomACTION : UInt32
	{
		MIN_COUNT = ReferenceBatchAction::eACTION::MAX_COUNT,
		SET_INITIALLY_DISABLED = MIN_COUNT,
		SET_ENCOUNTER_ZONE,
		SET_LEVEL_MODIFIER,
		MAX_COUNT
	};

	const bool CustomActionNeedsRef[MAX_COUNT - MIN_COUNT] = { false, false, false };

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
		IDC_EXTERIOR_LIGHT_COMBO = 5104,
		IDC_EXTERIOR_LIGHT_LABEL = 5105,
		IDC_INTERIOR_LIGHT_LABEL = 5106,
		IDC_INTERIOR_LIGHT_COMBO = 5107,

		IDC_OPPOSITE_CHECK = 5137,

		IDC_DO = 1,
	};

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

		int idx;
		idx = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Set Initially Disabled");
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
		switch (uiAction)
		{
		case eCustomACTION::SET_INITIALLY_DISABLED:
			pRefSelector->SetComparator(CanSetInitiallyDisabled);
			break;
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

		bool bEnableDoButton = pSelectedRef != nullptr || !CustomActionNeedsRef[auiAction - eCustomACTION::MIN_COUNT];
		auto hDoButton = GetDlgItem(hWnd, IDC_DO);
		EnableWindow(hDoButton, bEnableDoButton);
	}

	void OnDoAction()
	{
		eCustomACTION auiAction = (eCustomACTION)ReferenceBatchAction::GetAction();
		if (auiAction >= eCustomACTION::MIN_COUNT)
		{
			MessageHandler::DisableWarnings();

			auto selected = RenderWindow::SelectedData::GetSelected();
			for (auto ref : *selected)
			{
				if (ref && ref->Unk_43() && ref->Unk_4E())
				{
					ref->MarkAsModified();
					switch (auiAction)
					{
					case SET_INITIALLY_DISABLED:
						Console_Print("CustomReferenceBatchActions: TODO Implement Set Initially Disabled");
						break;
					case SET_ENCOUNTER_ZONE:
						Console_Print("CustomReferenceBatchActions: TODO Implement Set Encounter Zone");
						break;
					case SET_LEVEL_MODIFIER:
						Console_Print("CustomReferenceBatchActions: TODO Implement Set Level Modifier");
						break;
					}
				}
			}

			MessageHandler::EnableWarnings();
		}

		CdeclCall(0x4110D0); // ReferenceBatchAction::DoAction
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
					eCustomACTION action =
						(eCustomACTION)(eCustomACTION::MIN_COUNT + sel);

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