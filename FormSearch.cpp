#include "GameAPI.h"

#define IDD_INPUTDIALOG 101
#define IDC_INPUTEDIT 1001
extern HWND g_MainHwnd;

namespace FormSearch
{
    void LookupFormAndCloseMenu(HWND hwndDlg)
    {
        char formName[256];
        GetDlgItemText(hwndDlg, IDC_INPUTEDIT, formName, sizeof(formName));

        if (*formName)
        {
            auto form = GetFormByName(formName);
            if (!form)
            {
                if (int formID = strtoul(formName, NULL, 16))
                {
                    form = LookupFormByID(formID);
                }
            }

            if (form)
            {
                (*(void(__thiscall**)(__int32, HWND, __int32, __int32))(*(__int32*)form + 0x164))((__int32)form, g_MainHwnd, 0, 1);
            }
        }

        EndDialog(hwndDlg, IDOK);
    }

    BOOL CALLBACK InputDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case IDOK: {
                LookupFormAndCloseMenu(hwndDlg);
                return TRUE;
            }
            case IDCANCEL:
                EndDialog(hwndDlg, IDCANCEL);
                return TRUE;
            }
            break;
            // TODO:
            // handle Enter being pressed from within the edit control
        }
        return FALSE;
    }

    void ShowInputDialog(HINSTANCE hInstance, HWND hwndParent) {
        // Create the dialog window
        HWND hwndDlg = CreateWindowEx(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
            WC_DIALOG, "Lookup Form",
            WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, 200, 100,
            hwndParent, NULL, hInstance, NULL);

        // Create the input edit control
        HWND hwndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            10, 10, 180, 20,
            hwndDlg, (HMENU)IDC_INPUTEDIT, hInstance, NULL);

        // Create the OK button
        HWND hwndOK = CreateWindowEx(0, "BUTTON", "OK",
            WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            50, 40, 50, 20,
            hwndDlg, (HMENU)IDOK, hInstance, NULL);

        // Create the Cancel button
        HWND hwndCancel = CreateWindowEx(0, "BUTTON", "Cancel",
            WS_CHILD | WS_VISIBLE,
            110, 40, 50, 20,
            hwndDlg, (HMENU)IDCANCEL, hInstance, NULL);

        // Set the dialog procedure
        SetWindowLongPtr(hwndDlg, DWLP_DLGPROC, (LONG_PTR)InputDialogProc);

        // Show the dialog window
        ShowWindow(hwndDlg, SW_SHOW);
    }

    void Show()
    {
        HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);
        ShowInputDialog(hInstance, g_MainHwnd);
    }
}