#include "NiNodes.h"
void NiObjectNET::SetName(const char* newName)
{
	ThisStdCall(0x818130, this, &newName);
}