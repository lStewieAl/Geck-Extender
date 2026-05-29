#include "BetterFloatingFormList.h"
#include "GameTypes.h"
#include "GameObjects.h"
#include "GameAPI.h"
#include "GeckUtility.h"
#include <unordered_set>

#include "Events/DataLoadEvent.h"
#include "libs/stb_sprintf.h"
#include "libs/nlohmann/json.hpp"
#include <fstream>
#include <string>
using json = nlohmann::json;

extern HWND g_MainHwnd;
namespace RecentlyOpenedForms
{
	const char* kRecentFile = "Data\\NVSE\\Plugins\\GeckExtender\\RecentForms.json";
	constexpr size_t kMaxRecentForms = 50;

	struct RecentFormEntry
	{
		std::string editorID;
		std::time_t lastOpened;
	};
	std::vector<RecentFormEntry> g_RecentForms;

	inline void to_json(json& j, const RecentFormEntry& entry)
	{
		j =
		{
			{ "editorID", entry.editorID },
			{ "lastOpened", entry.lastOpened }
		};
	}

	inline void from_json(const json& j, RecentFormEntry& entry)
	{
		j.at("editorID").get_to(entry.editorID);
		j.at("lastOpened").get_to(entry.lastOpened);
	}

	void SaveRecentForms()
	{
		try
		{
			json j = g_RecentForms;

			std::ofstream file(kRecentFile);

			if (file.is_open())
			{
				file << j.dump(1, '\t');
			}
			else
			{
				Console_Print("Failed to open recent forms file for writing");
			}
		}
	catch (std::exception& e)
	{
		Console_Print("SaveRecentForms exception: %s", e.what());
	}
	catch (...)
	{
		Console_Print("SaveRecentForms unknown exception");
	}
	}

	void SortRecentForms()
	{
		std::sort(
			g_RecentForms.begin(),
			g_RecentForms.end(),
			[](const RecentFormEntry& a, const RecentFormEntry& b)
			{
				return a.lastOpened < b.lastOpened;
			});
	}

	void LoadRecentForms()
	{
		try
		{
			std::ifstream file(kRecentFile);

			if (!file.is_open())
				return;

			json j;
			file >> j;

			g_RecentForms = j.get<std::vector<RecentFormEntry>>();

			SortRecentForms();

			if (g_RecentForms.size() > kMaxRecentForms)
			{
				g_RecentForms.resize(kMaxRecentForms);
			}
		}
		catch (std::exception& e)
		{
			Console_Print("LoadRecentForms exception: %s", e.what());
		}
		catch (...)
		{
			Console_Print("LoadRecentForms unknown exception");
		}
	}

	LRESULT CALLBACK WindowCallback(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (Message == BSMsg_AcceptsDropType)
		{
			SetWindowLong(Hwnd, DWL_MSGRESULT, false);
			return TRUE;
		}
		else if (Message == WM_INITDIALOG)
		{
			SendMessageA(Hwnd, BetterFloatingFormList::BFL_SET_FILTER, 0, 0);
			SendMessageA(Hwnd, BetterFloatingFormList::BFL_SET_INITIAL_SORT, 0, 0);
		}

		auto result = BetterFloatingFormList::BaseWindowCallback(Hwnd, Message, wParam, lParam);

		if (Message == WM_INITDIALOG)
		{
			DragAcceptFiles(Hwnd, FALSE);
		}
		else if (Message == BetterFloatingFormList::BFL_DELETED_ITEMS)
		{
			auto formList = (tList<TESForm>*)wParam;

			if (formList && formList->Head())
			{
				std::unordered_set<std::string> deletedIDs;

				auto iter = formList->Head();

				do
				{
					if (auto form = iter->data)
					{
						if (auto edid = form->GetEditorID())
						{
							deletedIDs.insert(edid);
						}
					}
				} while (iter = iter->next);

				g_RecentForms.erase(
					std::remove_if(
						g_RecentForms.begin(),
						g_RecentForms.end(),
						[&deletedIDs](const RecentFormEntry& entry)
						{
							return deletedIDs.contains(entry.editorID);
						}),
					g_RecentForms.end());

				SaveRecentForms();
			}

		}
		return result;
	}

	void ShowList()
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(nullptr);

		static tList<TESForm> recentForms;
		recentForms.RemoveAll();

		for (const auto& entry : g_RecentForms)
		{
			TESForm* form = LookupFormByName(entry.editorID.c_str());

			if (form)
			{
				recentForms.Append(form);
			}
		}

		auto hWnd = CreateDialogParamA(
			hInstance,
			(LPCSTR)189,
			g_MainHwnd,
			(DLGPROC)WindowCallback,
			(LPARAM)((char*)(&recentForms)) - 0xC);

		SendMessageA(hWnd, WM_SETTEXT, 0, (LPARAM)"Recently Opened");
	}

	static std::time_t GetCurrentTimestamp()
	{
		using namespace std::chrono;

		return system_clock::to_time_t(system_clock::now());
	}

	void StoreForm(const char* sFormName)
	{
		if (!sFormName || !sFormName[0])
			return;

		// Remove existing entry if already present
		g_RecentForms.erase(
			std::remove_if(
				g_RecentForms.begin(),
				g_RecentForms.end(),
				[sFormName](const RecentFormEntry& entry)
				{
					return _stricmp(entry.editorID.c_str(), sFormName) == 0;
				}),
			g_RecentForms.end());

		g_RecentForms.push_back(
			{
				sFormName,
				GetCurrentTimestamp()
			});

		// Trim history
		if (g_RecentForms.size() > kMaxRecentForms)
		{
			g_RecentForms.resize(kMaxRecentForms);
		}

		SaveRecentForms();
	}

	void PostLoadPlugins()
	{
		LoadRecentForms();
	}

	HWND __stdcall OnOpenObjectWindowForm(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
	{
		auto pForm = ((TESForm**)(dwInitParam))[1];
		if (pForm)
		{
			StoreForm(pForm->GetEditorID());
		}
		return CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	}

	void InitHooks()
	{
		DataLoadEvent::RegisterCallback(PostLoadPlugins);
		WriteRelCall(0x4376A6, UInt32(OnOpenObjectWindowForm));
		SafeWrite8(0x4376A6 + 5, 0x90);
	}
}