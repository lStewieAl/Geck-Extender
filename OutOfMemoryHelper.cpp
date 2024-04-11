#include "Utilities.h"

void CreateCrashSave();
namespace OutOfMemoryHelper
{
	namespace EmergencyBuffer
	{
		constexpr UInt32 BUFFER_SIZE = 0x2000;
		constexpr UInt32 NUM_BUFFERS = 0x80;
		void* pBuffer[NUM_BUFFERS];

		void Allocate()
		{
			if (!pBuffer)
			{
				for (int i = 0; i < NUM_BUFFERS; ++i)
				{
					pBuffer[i] = malloc(BUFFER_SIZE);
				}
			}
		}

		void Free()
		{
			for (int i = 0; i < NUM_BUFFERS; ++i)
			{
				if (pBuffer[i])
				{
					free(pBuffer[i]);
					pBuffer[i] = nullptr;
				}
			}
		}
	}

	void* __stdcall OnMemoryAllocationFailure(int size)
	{
		EmergencyBuffer::Free();

		MessageBoxA(nullptr, "Geck was unable to allocate requested memory, attempting to clear buffers...", "Out of memory", MB_ICONERROR);
		
		// PurgeCellBuffers - the problem is without knowing where the crash was, we can't really safely free the cells...
		CdeclCall(0x44F180);

		CreateCrashSave();

		EmergencyBuffer::Allocate();
		return malloc(size);
	}

	void Init()
	{
		// allocate some memory to be freed in emergencies to provide a small buffer for handling issues once out of memory
		EmergencyBuffer::Allocate();

		// try and free up memory if the geck runs out
		SafeWriteBuf(0x854283, "\x85\xC0\x74\x03\xC2\x04\x00", 7);
		WriteRelJump(0x854283 + 7, UInt32(OnMemoryAllocationFailure));
	}
}