#pragma once
#include "MainWindowLoadEvent.h"
#include "Utilities.h"

namespace MainWindowLoadEvent
{
	using Callback = std::function<void()>;
	void RegisterCallback(Callback listener);
	void RemoveCallback(Callback listener);
	void InitHooks();
}