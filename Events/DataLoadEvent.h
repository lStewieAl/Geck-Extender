#pragma once
#include <functional>
#include <vector>
namespace DataLoadEvent
{
	void InitHooks();

	using Callback = std::function<void()>;
	void RegisterCallback(Callback listener);
	void RemoveCallback(Callback listener);
}