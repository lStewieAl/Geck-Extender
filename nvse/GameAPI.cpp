#include "GameAPI.h"
#include "GameRTTI.h"
#include "GameForms.h"
#include "GameObjects.h"
#include "GameTypes.h"
#include "CommandTable.h"
#include "GameScript.h"
#include "StringVar.h"

#if NVSE_CORE
#include "Hooks_Script.h"
#include "ScriptUtils.h"
#endif

static NVSEStringVarInterface* s_StringVarInterface = NULL;
bool extraTraces = false;

// arg1 = 1, ignored if canCreateNew is false, passed to 'init' function if a new object is created
typedef void * (* _GetSingleton)(bool canCreateNew);

const _LookupFormByID LookupFormByID = (_LookupFormByID)0x004F9620;	// Call between third reference to RTTI_TESWorldspace and RuntimeDynamicCast
const _LookupFormByName LookupFormByName = (_LookupFormByName)(0x004F9650); // Search for aNonPersistentR and aPlayer (third call below aPlayer, second is LookupFomrByID)
const _FormHeap_Allocate FormHeap_Allocate = (_FormHeap_Allocate)0x00401000;
const _FormHeap_Free FormHeap_Free = (_FormHeap_Free)0x0000401180;
const _ShowCompilerError ShowCompilerError = (_ShowCompilerError)0x005C5730;	// Called with aNonPersistentR (still same sub as the other one)