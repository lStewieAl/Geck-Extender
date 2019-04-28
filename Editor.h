#pragma once
#include <nvse\SafeWrite.h> 
#include "xutil.h"

void InsertListViewItem(HWND ListViewHandle, void* Parameter, bool UseImage, int ItemIndex);
void InsertComboBoxItem(HWND ComboBoxHandle, const char* DisplayText, void* Value, bool AllowResize);
void BeginUIDefer();
void EndUIDefer();