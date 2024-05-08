#include "MainWindowLoadEvent.h"
#include "Utilities.h"

namespace MainWindowLoadEvent
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

	void __cdecl TESCSMain__InitializeCSWindows()
	{
		CdeclCall(0x43F200);

		for (auto& listener : listeners) {
			listener();
		}
	}

	void InitHooks()
	{
		WriteRelCall(0x446581, UInt32(TESCSMain__InitializeCSWindows));
	}
}