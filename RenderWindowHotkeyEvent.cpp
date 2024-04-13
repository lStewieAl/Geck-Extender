#include "RenderWindowHotkeyEvent.h"
#include "Utilities.h"

namespace RenderWindowHotkeyEvent
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

	bool HandleEvent(UInt32 hotkey)
	{

	}
}