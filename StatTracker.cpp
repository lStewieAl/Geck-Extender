namespace StatTracker
{
    static ULONGLONG uStartTick = 0;
    static ULONGLONG uLastSavedTick = 0;
    constexpr UInt32 uiKey = 0x5A17C3E9;
    constexpr UInt32 OUR_TIMER = 0x1041;

    int ReadTime()
    {
        return GetPrivateProfileIntA("Stats", "uTime", 0, IniPath) ^ uiKey;
    }

    void WriteTime(UInt32 uiTimeSeconds)
    {
        char value[32];
        snprintf(value, sizeof(value), "%d", uiTimeSeconds ^ uiKey);
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
        uStartTick = GetTickCount64();
        uLastSavedTick = uStartTick;
        uStartTick -= static_cast<ULONGLONG>(ReadTime()) * 1'000;
        SetTimer(hMainWindow, OUR_TIMER, 1000, nullptr);
    }

    void SetupStatusBar(HWND hStatusBar)
    {
        RECT rc;
        GetClientRect(hStatusBar, &rc);
        int kParts[] = { 150, 225, 500, rc.right - 100, -1 };
        SendMessageA(hStatusBar, SB_SETPARTS, 5, (LPARAM)kParts);
    }

    void Callback(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        HWND hStatusBar = MainWindow::GetStatusBar();
        if (Msg == WM_CREATE)
        {
            IncreaseSessionCounter();
            SetupTimer(hMainWindow);
            SetupStatusBar(hStatusBar);
        }
        else if (Msg == WM_SIZE)
        {
            SetupStatusBar(hStatusBar);
        }
        else if (Msg == WM_TIMER && wParam == OUR_TIMER)
        {
            ULONGLONG now = GetTickCount64();
            ULONGLONG elapsed = (now - uStartTick) / 1000;

            // save the timer every minute
            if (now - uLastSavedTick >= 60'000)
            {
                uLastSavedTick += 60'000;
                WriteTime(elapsed);
            }

            int hours = static_cast<int>(elapsed / 3600);
            int minutes = static_cast<int>((elapsed % 3600) / 60);
            int seconds = static_cast<int>(elapsed % 60);

            char kBuf[16];
            snprintf(kBuf, sizeof(kBuf), "%02d:%02d:%02d", hours, minutes, seconds);
            SendMessageA(hStatusBar, SB_SETTEXTA, 4, (LPARAM)kBuf);
        }
        else if (Msg == WM_CLOSE)
        {
            ULONGLONG now = GetTickCount64();
            ULONGLONG elapsed = (now - uStartTick) / 1000;
            WriteTime(elapsed);
        }
    }
}