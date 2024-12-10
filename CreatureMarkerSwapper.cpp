#include "nvse/GameForms.h"
#include "nvse/GameObjects.h"
#include "Utilities.h"

#include "libs/stb_sprintf.h"
#include "libs/nlohmann/json.hpp"
#include <fstream>
#include <string>
using json = nlohmann::json;

namespace CreatureMarkerSwapper
{
	std::unordered_map<std::string, std::string> idToMeshPath;

	bool SwapMarkerModel(TESActorBase* actorBase, TESObjectREFR* ref, char* dst, size_t sizeInBytes)
	{
#if _DEBUG
		Console_Print("ActorBase: %s (%08X) - Ref: %s (%08X)",
			actorBase->GetEditorID(),
			actorBase->refID,
			ref ? ref->GetEditorID() : "NULL",
			ref ? ref->refID : 0);
#endif
		std::string editorID = actorBase->GetEditorID();
		auto iter = idToMeshPath.find(editorID);
		if (iter != idToMeshPath.end())
		{
#if _DEBUG
			Console_Print("Swapped %s with %s", actorBase->GetEditorID(), iter._Ptr->_Myval.second);
#endif
			strncpy(dst, iter._Ptr->_Myval.second.c_str(), sizeInBytes - 1);
			dst[sizeInBytes - 1] = '\0';
			return true;
		}
		return false;
	}

	void __fastcall LoadCreatureMarker(TESActorBase* actorBase, TESObjectREFR* ref, char* dst, size_t sizeInBytes, const char* format, const char* sMeshes)
	{
		if (!SwapMarkerModel(actorBase, ref, dst, sizeInBytes))
		{
			stbsp_snprintf(dst, sizeInBytes, format, sMeshes);
		}
	}

	__HOOK LoadCreatureMarkerHook()
	{
		_asm
		{
			mov ecx, edi // creature
			mov edx, ebp // ref
			jmp LoadCreatureMarker
		}
	}

	__HOOK LoadNPCMarkerHook()
	{
		_asm
		{
			mov ecx, ebp // npc
			mov edx, esi // ref
			jmp LoadCreatureMarker
		}
	}

	void ReadJSON(const char* path)
	{
		try
		{
			std::ifstream i(path);
			json j;
			i >> j;

			if (j.is_array())
			{
				for (auto& elem : j)
				{
					if (!elem.is_object())
					{
						Console_Print("JSON error: expected object with EditorID and MeshPath fields.");
						continue;
					}

					idToMeshPath.emplace(elem.contains("EditorID") ? elem["EditorID"].get<std::string>() : "",
										 elem.contains("MeshPath") ? elem["MeshPath"].get<std::string>() : "");
				}
			}
		}
		catch (nlohmann::json::exception& e)
		{
			Console_Print("The JSON is incorrectly formatted! It will not be applied.");
			Console_Print("JSON error: %s", e.what());
		}
	}

	void Init()
	{
		WriteRelCall(0x564FD9, UInt32(LoadCreatureMarkerHook));
		SafeWriteBuf(0x564FD9 + 5, "\x0F\x1F\x00", 3);
		WriteRelCall(0x5728E9, UInt32(LoadNPCMarkerHook));
		SafeWriteBuf(0x5728E9 + 5, "\x0F\x1F\x00", 3);

		ForEachFileInPath("Data\\NVSE\\Plugins\\GeckExtender\\CreatureSwapper\\", "*.json", ReadJSON);
	}
}