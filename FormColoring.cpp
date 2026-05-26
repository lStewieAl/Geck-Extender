#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "GameForms.h"
#include "GameApi.h"
#include "settings.h"
#include "Events/DataLoadEvent.h"
#include "FormColoring.h"

namespace FormColoring
{
	/*
	Add customizable context actions to the Object Window for coloring forms in the ListView
	e.g. [Object Window] sCustomFormColors=Done:00FF00;In Progress:ADD8E6;Rejected:FF0000;Needs Review:FFFF00
	Saves form colors in GeckExtenderFormColors.ini
	*/

	char IniPath[MAX_PATH];

	struct CustomColorEntry
	{
		std::string Label;
		COLORREF Color;
	};

	std::vector<CustomColorEntry> g_labelColors;
	std::unordered_map<UInt32, COLORREF> g_formColors;
	std::unordered_map<std::string, COLORREF> g_unresolvedFormColors;

	COLORREF ParseHexColor(const std::string& hex)
	{
		unsigned int value = std::stoul(hex, nullptr, 16);

		BYTE r = (value >> 16) & 0xFF;
		BYTE g = (value >> 8) & 0xFF;
		BYTE b = value & 0xFF;

		return RGB(r, g, b);
	}

	void ParseCustomColorLabels(const char* apConfig)
	{
		g_labelColors.clear();
		if (!apConfig || !*apConfig)
		{
			return;
		}

		std::string config(apConfig);

		UInt32 iNumEntries = 0;
		size_t start = 0;
		while (start < config.size())
		{
			size_t semi = config.find(';', start);
			if (semi == std::string::npos)
			{
				semi = config.size();
			}

			std::string entry = config.substr(start, semi - start);

			size_t colon = entry.find(':');

			if (colon != std::string::npos)
			{
				std::string label = entry.substr(0, colon);
				std::string colorHex = entry.substr(colon + 1);
				try
				{
					g_labelColors.push_back(
						{
							label,
							ParseHexColor(colorHex)
						});
					++iNumEntries;
					if (iNumEntries >= NUM_MAX_COLORS)
					{
						break;
					}
				}
				catch (...)
				{
					// ignore malformed entries
				}
			}

			start = semi + 1;
		}
	}

	int RGBToBrightness(COLORREF rgb)
	{
		BYTE r = GetRValue(rgb);
		BYTE g = GetGValue(rgb);
		BYTE b = GetBValue(rgb);
		return (r * 299 + g * 587 + b * 114) / 1000;
	}

	LRESULT HandleCustomDraw(LPARAM lParam)
	{
		LPNMLVCUSTOMDRAW lvcd = (LPNMLVCUSTOMDRAW)lParam;

		switch (lvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
		{
			return CDRF_NOTIFYITEMDRAW;
		}

		case CDDS_ITEMPREPAINT:
		{
			auto form = reinterpret_cast<TESForm*>(lvcd->nmcd.lItemlParam);
			if (form)
			{
				// User-selected color takes priority
				auto it = g_formColors.find(form->refID);
				if (it != g_formColors.end())
				{
					COLORREF bg = it->second;
					lvcd->clrTextBk = bg;
					lvcd->clrText = RGBToBrightness(bg) < 128
						? RGB(255, 255, 255)
						: RGB(0, 0, 0);
					return CDRF_NEWFONT;
				}
			}
			return CDRF_DODEFAULT;
		}
		}

		return CDRF_DODEFAULT;
	}

	COLORREF PickColor(HWND owner, COLORREF initial = RGB(255, 255, 255))
	{
		static COLORREF customColors[16] = {};

		CHOOSECOLOR cc{};
		cc.lStructSize = sizeof(cc);
		cc.hwndOwner = owner;
		cc.rgbResult = initial;
		cc.lpCustColors = customColors;
		cc.Flags = CC_FULLOPEN | CC_RGBINIT;

		if (ChooseColor(&cc))
		{
			return cc.rgbResult;
		}
		return CLR_INVALID; // user cancelled
	}

	HMENU CreateSubMenu(HWND listView)
	{
		HMENU hMenu = CreatePopupMenu();

		COLORREF currentColor = CLR_INVALID;
		int index = -1;
		while ((index = ListView_GetNextItem(listView, index, LVNI_SELECTED)) != -1)
		{
			LVITEM item {};
			item.mask = LVIF_PARAM;
			item.iItem = index;

			if (ListView_GetItem(listView, &item))
			{
				if (auto form = reinterpret_cast<TESForm*>(item.lParam))
				{
					auto it = g_formColors.find(form->refID);
					if (it != g_formColors.end())
					{
						if (currentColor != CLR_INVALID && currentColor != it->second)
						{
							// if there are multiple different colors, don't checkmark anything
							currentColor = CLR_INVALID - 1;
							break;
						}
						currentColor = it->second;
					}
					else if (currentColor != CLR_INVALID)
					{
						// if there's a mix of colored and non-coloreds, don't checkmark anything
						currentColor = CLR_INVALID - 1;
						break;
					}
				}
			}
		}

		for (size_t i = 0; i < g_labelColors.size(); i++)
		{
			UINT flags = MF_BYPOSITION;
			if (g_labelColors[i].Color == currentColor)
			{
				flags |= MF_CHECKED;
			}
			InsertMenuA(hMenu, 0xFFFFFFFF, flags, COLOR_ITEM_CONFIG_BASE + i, g_labelColors[i].Label.c_str());
		}
		InsertMenuA(hMenu, 0xFFFFFFFF, MF_BYPOSITION, COLOR_ITEM_CUSTOM, "Custom");
		if (currentColor != CLR_INVALID)
		{
			InsertMenuA(hMenu, 0xFFFFFFFF, MF_SEPARATOR, 0, nullptr);
			InsertMenuA(hMenu, 0xFFFFFFFF, MF_BYPOSITION, COLOR_ITEM_CLEAR, "Clear Color");
		}
		
		return hMenu;
	}

	void WriteColorHex(std::ostream& file, COLORREF color)
	{
		file << std::uppercase
			<< std::hex
			<< std::setfill('0')
			<< std::setw(2)
			<< std::setw(2) << static_cast<int>(GetRValue(color))
			<< std::setw(2) << static_cast<int>(GetGValue(color))
			<< std::setw(2) << static_cast<int>(GetBValue(color));
	}

	void WriteFormColorsToFile()
	{
		std::ofstream file(IniPath, std::ios::trunc);
		if (!file.is_open())
		{
			Console_Print("Form Coloring: Failed to save file: %s", IniPath);
			return;
		}

		// track which editor IDs we've written so we don't duplicate
		std::unordered_set<std::string> writtenIDs;

		// first write currently resolved forms
		for (const auto& [refID, color] : g_formColors)
		{
			TESForm* form = LookupFormByID(refID);
			if (!form)
			{
				continue;
			}

			const char* editorID = form->GetEditorID();
			if (!editorID || !*editorID)
			{
				continue;
			}

			file << editorID << "=";
			WriteColorHex(file, color);
			file << "\n";

			writtenIDs.insert(editorID);
		}

		// write unresolved entries that weren't replaced
		for (const auto& [editorID, color] : g_unresolvedFormColors)
		{
			if (writtenIDs.contains(editorID))
			{
				continue;
			}

			file << editorID << "=";
			WriteColorHex(file, color);
			file << "\n";
		}
	}

	void ReadFormColorsFromFile()
	{
		g_formColors.clear();

		std::ifstream file(IniPath);
		if (!file.is_open())
		{
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty())
			{
				continue;
			}

			size_t eq = line.find('=');
			if (eq == std::string::npos)
			{
				continue;
			}

			std::string editorID = line.substr(0, eq);
			std::string colorHex = line.substr(eq + 1);

			try
			{
				COLORREF color = ParseHexColor(colorHex);

				TESForm* form = LookupFormByName(editorID.c_str());
				if (form)
				{
					g_formColors[form->refID] = color;
				}
				else
				{
					// Preserve unresolved entries (e.g. if loading a different ESM)
					g_unresolvedFormColors[editorID] = color;
				}
			}
			catch (...)
			{
				// Ignore malformed entries
			}
		}
	}

	bool HandlePopupMenuCommand(HWND listView, UInt32 commandID)
	{
		if (commandID >= COLOR_ITEM_CLEAR && commandID < COLOR_ITEM_CONFIG_BASE + g_labelColors.size())
		{
			COLORREF color = 0;
			if (commandID == COLOR_ITEM_CUSTOM)
			{
				color = PickColor(GetParent(listView));
				if (color == CLR_INVALID)
				{
					return true; // cancelled
				}
			}
			else if (commandID != COLOR_ITEM_CLEAR)
			{
				size_t index = commandID - COLOR_ITEM_CONFIG_BASE;
				color = g_labelColors[index].Color;
			}

			int index = -1;
			while ((index = ListView_GetNextItem(listView, index, LVNI_SELECTED)) != -1)
			{
				LVITEM item {};
				item.mask = LVIF_PARAM;
				item.iItem = index;

				if (ListView_GetItem(listView, &item))
				{
					auto form = reinterpret_cast<TESForm*>(item.lParam);
					if (!form)
					{
						continue;
					}

					if (commandID == COLOR_ITEM_CLEAR)
					{
						g_formColors.erase(form->refID);
					}
					else
					{
						g_formColors[form->refID] = color;
					}
				}
			}

			WriteFormColorsToFile();
			InvalidateRect(listView, nullptr, TRUE);
			return true;
		}
		return false;
	}

	void PostLoadPlugins()
	{
		ReadFormColorsFromFile();
	}

	void Init()
	{
		ParseCustomColorLabels(config.sCustomFormColors);
		DataLoadEvent::RegisterCallback(PostLoadPlugins);

		GetModuleFileNameA(NULL, IniPath, MAX_PATH);
		strcpy((char*)(strrchr(IniPath, '\\') + 1), "Data\\nvse\\plugins\\GeckExtender\\FormColoring\\config.ini");
	}
}