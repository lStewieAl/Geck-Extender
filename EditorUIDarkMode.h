#pragma once

#include <Uxtheme.h>

#pragma comment(lib, "uxtheme.lib")

namespace EditorUIDarkMode
{
	void Initialize();

	LRESULT CALLBACK CallWndProcCallback(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK DialogWindowSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK ListViewSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK MDIClientSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	DWORD WINAPI Comctl32GetSysColor(int nIndex);
	HBRUSH WINAPI Comctl32GetSysColorBrush(int nIndex);
	HRESULT WINAPI Comctl32DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int cchText, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);
	HRESULT WINAPI Comctl32DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, LPCRECT pClipRect);
}