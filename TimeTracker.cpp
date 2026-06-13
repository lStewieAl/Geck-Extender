namespace TimeTracker
{
    static ULONGLONG uStartTick = 0;

    void Callback(HWND hMainWindow, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        enum { OUR_TIMER = 0x1041 };

        HWND hStatusBar = MainWindow::GetStatusBar();
        if (Msg == WM_CREATE)
        {
            uStartTick = GetTickCount64();

            RECT rc;
            GetClientRect(hStatusBar, &rc);
            int kParts[] = { 150, 225, 500, rc.right - 100, -1 };
            SendMessageA(hStatusBar, SB_SETPARTS, 5, (LPARAM)kParts);

            SetTimer(hMainWindow, OUR_TIMER, 1000, nullptr);
        }
        else if (Msg == WM_SIZE)
        {
            RECT rc;
            GetClientRect(hStatusBar, &rc);
            int kParts[] = { 150, 225, 500, rc.right - 100, -1 };
            SendMessageA(hStatusBar, SB_SETPARTS, 5, (LPARAM)kParts);
        }
        else if (Msg == WM_TIMER && wParam == OUR_TIMER)
        {
            ULONGLONG elapsed = (GetTickCount64() - uStartTick) / 1000;

            int hours = static_cast<int>(elapsed / 3600);
            int minutes = static_cast<int>((elapsed % 3600) / 60);
            int seconds = static_cast<int>(elapsed % 60);

            char kBuf[16];
            snprintf(kBuf, sizeof(kBuf), "%02d:%02d:%02d", hours, minutes, seconds);
            SendMessageA(hStatusBar, SB_SETTEXTA, 4, (LPARAM)kBuf);
        }
    }
}