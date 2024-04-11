namespace EasterEggs
{
	bool IsAprilFoolsDay()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		return st.wMonth == 4 && st.wDay == 1;
	}

	void Init()
	{
		WriteRelCall(0x43DC55, UInt32(IsAprilFoolsDay));
		SafeWriteBuf(0x43DC75, "\x0F\x1F\x44\x00\x00", 5);
	}
}