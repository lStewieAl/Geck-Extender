#include "DataLoadEvent.h"
#include "Utilities.h"
#include "GECKUtility.h"

namespace DataLoadEvent
{
	using Callback = std::function<void()>;
	std::vector<Callback> listeners;

	void RegisterCallback(Callback listener)
	{
		listeners.push_back(listener);
	}

	void RemoveCallback(Callback listener)
	{
		listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
			[&listener](const Callback& item) { return item.target<void()>() == listener.target<void()>(); }),
			listeners.end());
	}

	void __cdecl OnEndLoading(int statusBarId, const char* msg)
	{
		TESCSMain__WriteToStatusBar(statusBarId, msg);

		for (auto& listener : listeners) {
			listener();
		}
	}

	void InitHooks()
	{
		WriteRelCall(0x464D2D, UInt32(OnEndLoading));
	}
}