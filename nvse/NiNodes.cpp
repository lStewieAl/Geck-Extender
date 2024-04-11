#include "NiNodes.h"
void NiObjectNET::SetName(const char* newName)
{
	ThisCall(0x818130, this, &newName);
}