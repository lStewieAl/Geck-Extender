#include <vector>
#include <algorithm>
#include <assert.h>
#include <CommCtrl.h>
#include "Editor.h"

bool g_UseDeferredDialogInsert;
HWND g_DeferredListView;
HWND g_DeferredComboBox;
uintptr_t g_DeferredStringLength;
bool g_AllowResize;
std::vector<std::pair<const char*, void*>> g_DeferredMenuItems;

void ResetUIDefer()
{
	g_UseDeferredDialogInsert = false;
	g_DeferredListView = nullptr;
	g_DeferredComboBox = nullptr;
	g_DeferredStringLength = 0;
	g_AllowResize = false;
	g_DeferredMenuItems.clear();
}

void BeginUIDefer()
{
	ResetUIDefer();
	g_UseDeferredDialogInsert = true;
}

void SuspendComboBoxUpdates(HWND ComboHandle, bool Suspend)
{
	COMBOBOXINFO info = {};
	info.cbSize = sizeof(COMBOBOXINFO);

	if (!GetComboBoxInfo(ComboHandle, &info))
		return;

	_declspec(thread) static WNDPROC originalWndProc;

	if (!Suspend)
	{
		SetWindowLongPtrW(info.hwndList, GWLP_WNDPROC, (LONG_PTR)originalWndProc);
		originalWndProc = nullptr;

		SendMessage(ComboHandle, CB_SHOWDROPDOWN, FALSE, 0);
		SendMessage(info.hwndList, WM_SETREDRAW, TRUE, 0);

		SendMessage(ComboHandle, CB_SETMINVISIBLE, 30, 0);
		SendMessage(ComboHandle, WM_SETREDRAW, TRUE, 0);

	}
	else
	{
		SendMessage(ComboHandle, WM_SETREDRAW, FALSE, 0);// Prevent repainting until finished
		SendMessage(ComboHandle, CB_SETMINVISIBLE, 1, 0);// Possible optimization for older libraries (source: MSDN forums)

		auto hackWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
		{
			switch (uMsg)
			{
			case WM_SETREDRAW:
			case LB_ADDSTRING:
			case LB_INSERTSTRING:
			case LB_DELETESTRING:
			case LB_SETTOPINDEX:
			case LB_SETITEMDATA:
				return originalWndProc(hwnd, uMsg, wParam, lParam);
			}

			return 0;
		};

		originalWndProc = (WNDPROC)GetWindowLongPtrW(info.hwndList, GWLP_WNDPROC);
		SetWindowLongPtrW(info.hwndList, GWLP_WNDPROC, (LONG_PTR)(WNDPROC)hackWndProc);

		SendMessage(info.hwndList, WM_SETREDRAW, FALSE, 0);
		SendMessage(ComboHandle, CB_SHOWDROPDOWN, TRUE, 0);
	}
}


void EndUIDefer()
{
	if (!g_UseDeferredDialogInsert)
		return;

	if (g_DeferredListView)
	{
		SendMessage(g_DeferredListView, WM_SETREDRAW, TRUE, 0);
		RedrawWindow(g_DeferredListView, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
	}

	if (!g_DeferredMenuItems.empty())
	{
		const HWND control = g_DeferredComboBox;

		// Sort alphabetically if requested to try and speed up inserts
		int finalWidth = 0;
		LONG_PTR style = GetWindowLongPtr(control, GWL_STYLE);

		if ((style & CBS_SORT) == CBS_SORT)
		{
			std::sort(g_DeferredMenuItems.begin(), g_DeferredMenuItems.end(),
				[](const std::pair<const char*, void*>& a, const std::pair<const char*, void*>& b) -> bool
				{
					return _stricmp(a.first, b.first) > 0;
				});
		}

		SendMessage(control, CB_INITSTORAGE, g_DeferredMenuItems.size(), g_DeferredStringLength * sizeof(char));
		HDC hdc = GetDC(control);
		if (hdc)
		{
			SuspendComboBoxUpdates(control, true);

			// Pre-calculate font widths for resizing, starting with TrueType
			int fontWidths[UCHAR_MAX];
			ABC trueTypeFontWidths[UCHAR_MAX];

			if (!GetCharABCWidthsA(hdc, 0, ARRAYSIZE(trueTypeFontWidths) - 1, trueTypeFontWidths))
			{
				BOOL result = GetCharWidthA(hdc, 0, ARRAYSIZE(fontWidths) - 1, fontWidths);
				AssertMsg(result, "Failed to determine any font widths");
			}
			else
			{
				for (int i = 0; i < ARRAYSIZE(fontWidths); i++)
					fontWidths[i] = trueTypeFontWidths[i].abcB;
			}

			// Insert everything all at once
			for (auto& pair : g_DeferredMenuItems)
			{
				auto& display = pair.first;
				auto& value = pair.second;

				LRESULT index = SendMessageA(control, CB_ADDSTRING, 0, (LPARAM)display);
				int lineSize = 0;

				if (index != CB_ERR && index != CB_ERRSPACE)
					SendMessageA(control, CB_SETITEMDATA, index, (LPARAM)value);

				for (const char* c = display; *c != '\0'; c++)
					lineSize += fontWidths[*c];

				finalWidth = std::max<int>(finalWidth, lineSize);

				free((void*)display);
			}

			SuspendComboBoxUpdates(control, false);
			ReleaseDC(control, hdc);
		}

		// Resize to fit
		if (g_AllowResize)
		{
			LRESULT currentWidth = SendMessage(control, CB_GETDROPPEDWIDTH, 0, 0);

			if (finalWidth > currentWidth)
				SendMessage(control, CB_SETDROPPEDWIDTH, finalWidth, 0);
		}
	}

	ResetUIDefer();
}

void InsertComboBoxItem(HWND ComboBoxHandle, const char* DisplayText, void* Value, bool AllowResize)
{
	if (!ComboBoxHandle)
		return;

	if (!DisplayText)
		DisplayText = "NONE";

	if (g_UseDeferredDialogInsert)
	{
		AssertMsg(!g_DeferredComboBox || (g_DeferredComboBox == ComboBoxHandle), "Got handles to different combo boxes? Reset probably wasn't called.");

		g_DeferredComboBox = ComboBoxHandle;
		g_DeferredStringLength += strlen(DisplayText) + 1;
		g_AllowResize |= AllowResize;

		// A copy must be created since lifetime isn't guaranteed after this function returns
		g_DeferredMenuItems.emplace_back(_strdup(DisplayText), Value);
	}
	else
	{
		if (AllowResize)
		{
			HDC hdc = GetDC(ComboBoxHandle);
			if (hdc)
			{
				SIZE size;
				if (GetTextExtentPoint32A(hdc, DisplayText, (int)strlen(DisplayText), &size))
				{
					LRESULT currentWidth = SendMessageA(ComboBoxHandle, CB_GETDROPPEDWIDTH, 0, 0);

					if (size.cx > currentWidth)
						SendMessageA(ComboBoxHandle, CB_SETDROPPEDWIDTH, size.cx, 0);
				}

				ReleaseDC(ComboBoxHandle, hdc);
			}
		}

		LRESULT index = SendMessageA(ComboBoxHandle, CB_ADDSTRING, 0, (LPARAM)DisplayText);

		if (index != CB_ERR && index != CB_ERRSPACE)
			SendMessageA(ComboBoxHandle, CB_SETITEMDATA, index, (LPARAM)Value);
	}
}

void InsertListViewItem(HWND ListViewHandle, void* Parameter, bool UseImage, int ItemIndex)
{
	if (ItemIndex == -1)
		ItemIndex = INT_MAX;

	LVITEMA item;
	memset(&item, 0, sizeof(item));

	item.mask = LVIF_PARAM | LVIF_TEXT;
	item.iItem = ItemIndex;
	item.lParam = (LPARAM)Parameter;
	item.pszText = LPSTR_TEXTCALLBACK;

	if (UseImage)
	{
		item.mask |= LVIF_IMAGE;
		item.iImage = I_IMAGECALLBACK;
	}

	if (g_UseDeferredDialogInsert)
	{
		AssertMsg(!g_DeferredListView || (g_DeferredListView == ListViewHandle), "Got handles to different list views? Reset probably wasn't called.");
		if (!g_DeferredListView)
		{
			g_DeferredListView = ListViewHandle;
			SendMessage(ListViewHandle, WM_SETREDRAW, FALSE, 0);
		}
	}

	ListView_InsertItem(ListViewHandle, &item);
}