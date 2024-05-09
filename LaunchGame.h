#pragma once
#include "nvse/PluginAPI.h"
#include <string>
class TESObjectCELL;
namespace LaunchGame
{
	struct LaunchGameData
	{
		UInt32 timestamp;
		std::string cellEditorId;
	};

	void WriteLaunchAtCellFile(UInt32 time, TESObjectCELL* cell);
	void Launch();
	void OnMainGameLoop();
	bool OnGamePluginLoad(const NVSEInterface* nvse);
}