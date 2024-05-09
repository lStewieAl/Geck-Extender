#include "LaunchGame.h"

#include "GameForms.h"
#include "Settings.h"
#include "GECKUtility.h"

#include <fstream>
#include <sstream>
#include <string>

namespace LaunchGame
{
	constexpr const char* GECK_LAUNCH_FILENAME = "GeckLaunchGame.txt";
	void WriteLaunchAtCellFile(UInt32 time, TESObjectCELL* cell)
	{
		std::ofstream outputFile(GECK_LAUNCH_FILENAME, std::ios::out);
		if (!outputFile)
		{
			Console_Print("Unable to open file for writing.");
			return;
		}

		auto editorId = cell->GetEditorID();
		if (editorId && *editorId)
		{
			outputFile << time << ',' << editorId;
		}
	}

	// Function to read the game launch data from the file
	bool ReadLaunchAtCellFile(LaunchGameData& data)
	{
		std::ifstream inputFile(GECK_LAUNCH_FILENAME);
		if (!inputFile)
		{
			return false;
		}

		// Read the entire line from the file
		std::string line;
		if (!std::getline(inputFile, line))
		{
			return false;
		}

		// Create a string stream from the read line to parse the values
		std::istringstream iss(line);
		std::string token;

		// Retrieve and convert the first token to DWORD (timestamp)
		if (std::getline(iss, token, ','))
		{
			data.timestamp = std::stoul(token);
		}
		else
		{
			return false;
		}

		// Retrieve the second token as the editor ID
		if (std::getline(iss, token, ','))
		{
			data.cellEditorId = token;
		}
		else
		{
			return false;
		}

		return true;
	}

	bool IsGeckRunning()
	{
		return FindWindowA("Garden of Eden Creation Kit", 0);
	}

	void OnGameMainMenu()
	{
		constexpr UInt32 TIMESTAMP_MAX_DELAY = 30 * 1000;
		LaunchGameData launchData;
		if (ReadLaunchAtCellFile(launchData))
		{
			if (IsGeckRunning() && (GetTickCount() - launchData.timestamp) < TIMESTAMP_MAX_DELAY)
			{
				ThisCall(0x93DB60, *(UInt32**)0x11DEA3C, launchData.cellEditorId.c_str(), 0); // CenterOnCell
			}
		}
	}

	void Launch()
	{
		char exePath[MAX_PATH];
		auto commandLineDir = (const char*)0xECFE30;
		_snprintf(exePath, sizeof(exePath), "%s\\%s", commandLineDir, config.sLaunchExeName);
		exePath[sizeof(exePath) - 1] = '\0'; // ensure exePath is null terminated

		PROCESS_INFORMATION processInfo;
		ZeroMemory(&processInfo, sizeof(processInfo));

		if (_CreateProcessA(exePath, &processInfo))
		{
			auto cell = RenderWindow::GetCurrentCell();
			if (cell && GetAsyncKeyState(VK_SHIFT) < 0)
			{
				Console_Print("Launched game - process id: %d");
				WriteLaunchAtCellFile(GetTickCount(), cell);
			}
		}

		if (processInfo.hProcess) CloseHandle(processInfo.hProcess);
		if (processInfo.hThread) CloseHandle(processInfo.hThread);
	}
}