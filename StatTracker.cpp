namespace StatTracker
{
    static ULONGLONG uStartTick = 0;
    static UInt32 uSavedTotalSeconds;
    static ULONGLONG uLastSavedTick = 0;
    constexpr UInt32 OUR_TIMER = 0x1041;

    int ReadTime()
    {
        return GetPrivateProfileIntA("Stats", "uTime", 0, IniPath);
    }

    void WriteTime(UInt32 uiTimeSeconds)
    {
        char value[32];
        snprintf(value, sizeof(value), "%d", uiTimeSeconds);
        WritePrivateProfileStringA("Stats", "uTime", value, IniPath);
    }

    void IncreaseSessionCounter()
    {
        int uiSessions = GetPrivateProfileIntA("Stats", "uSessions", 0, IniPath);
        ++uiSessions;

        char value[0x10];
        snprintf(value, sizeof(value), "%d", uiSessions);
        WritePrivateProfileStringA("Stats", "uSessions", value, IniPath);
    }

    void SetupTimer(HWND hMainWindow)
    {
        uSavedTotalSeconds = ReadTime();

        uStartTick = GetTickCount64();
        uLastSavedTick = uStartTick;

        SetTimer(hMainWindow, OUR_TIMER, 1000, nullptr);
    }

    static bool bShowTotalTime = false;
    void UpdateStatusBarText(HWND hStatusBar)
    {
        ULONGLONG now = GetTickCount64();
        ULONGLONG sessionTime = (now - uStartTick) / 1000;
        ULONGLONG totalTime = uSavedTotalSeconds + sessionTime;

        ULONGLONG displayTime = (bShowTotalTime ^ config.bShowSessionTime) ? sessionTime : totalTime;

        char buf[0x10];
        snprintf(buf, sizeof(buf), "%02llu:%02llu:%02llu",
            displayTime / 3600,
            (displayTime % 3600) / 60,
            displayTime % 60);

        SendMessageA(hStatusBar, SB_SETTEXTA, 4, (LPARAM)buf);
    }

    WNDPROC OldStatusProc;
    LRESULT CALLBACK StatusBarProc(
        HWND hStatusBar,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam)
    {
        switch (msg)
        {
        case WM_MOUSEMOVE:
        {
            POINT pt;
            pt.x = (SHORT)LOWORD(lParam);
            pt.y = (SHORT)HIWORD(lParam);

            RECT rc;
            SendMessageA(hStatusBar, SB_GETRECT, 4, (LPARAM)&rc);

            bool bHoveringTime = PtInRect(&rc, pt);

            if (bHoveringTime != bShowTotalTime)
            {
                bShowTotalTime = bHoveringTime;
                UpdateStatusBarText(hStatusBar);
            }

            TRACKMOUSEEVENT tme = {};
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hStatusBar;
            TrackMouseEvent(&tme);
            break;
        }

        case WM_MOUSELEAVE:
            bShowTotalTime = false;
            UpdateStatusBarText(hStatusBar);
            break;
        }

        return CallWindowProcA(OldStatusProc, hStatusBar, msg, wParam, lParam);
    }

    void SetupStatusBar(HWND hStatusBar)
    {
        RECT rc;
        GetClientRect(hStatusBar, &rc);
        int kParts[] = { 150, 225, 500, rc.right - 100, -1 };
        SendMessageA(hStatusBar, SB_SETPARTS, 5, (LPARAM)kParts);

        if (!OldStatusProc)
        {
            OldStatusProc = (WNDPROC)SetWindowLongPtrA(hStatusBar, GWLP_WNDPROC, (LONG_PTR)StatusBarProc);
        }
    }

    void Callback(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        HWND hStatusBar = MainWindow::GetStatusBar();
        switch (Msg)
        {
        case WM_CREATE:
            IncreaseSessionCounter();
            SetupTimer(hMainWindow);
            SetupStatusBar(hStatusBar);
            break;
        case WM_SIZE:
            SetupStatusBar(hStatusBar);
            break;
        case WM_TIMER:
            if (wParam == OUR_TIMER)
            {
                ULONGLONG now = GetTickCount64();
                ULONGLONG sessionTime = (now - uStartTick) / 1000;
                ULONGLONG totalTime = uSavedTotalSeconds + sessionTime;

                // Save every minute.
                if (now - uLastSavedTick >= 60'000)
                {
                    uLastSavedTick += 60'000;
                    WriteTime(static_cast<UInt32>(totalTime));
                }

                UpdateStatusBarText(hStatusBar);
            }
            break;
        case WM_CLOSE:
        {
            ULONGLONG now = GetTickCount64();
            ULONGLONG sessionTime = (now - uStartTick) / 1000;
            WriteTime(static_cast<UInt32>(uSavedTotalSeconds + sessionTime));
            break;
        }
        }
    }
}