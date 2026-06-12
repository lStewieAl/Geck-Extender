#include "GeckUtility.h"
#include "GameData.h"
#include "GameApi.h"

#include <dbghelp.h>
#include "libs/stb_sprintf.h"

namespace CrashHandler
{
	void __cdecl CrashSaveSetName(char* dst, size_t size, char* format, void* DEFAULT)
	{
		ModInfo* activeFile = DataHandler::GetSingleton()->activeFile;
		const char* modName = "DEFAULT.esp";
		if (activeFile)
		{
			modName = activeFile->name;
		}
		stbsp_sprintf(dst, "%s", modName);
	}

	LPTOP_LEVEL_EXCEPTION_FILTER s_originalFilter = nullptr;

	void CreateCrashSave()
	{
		// create a save in the Data//CrashSaves folder called %s.esp
		WriteRelCall(0x4DB07A, UInt32(CrashSaveSetName));

		CreateDirectoryA("Data\\CrashSaves", 0);
		static const char* path = "Data\\CrashSaves\\";
		SafeWrite32(0x4DB0AC, UInt32(path));

		ThisCall(0x4DB020, DataHandler::GetSingleton()); // DoAutosave

		// restore original path (Data\\Backup\\)
		SafeWrite32(0x4DB0AC, 0xD415C4);
	}

#define CENTERED_TEXT(size, name) (size/2) + sizeof(name) / 2, name, (size/2) - sizeof(name) / 2, ""
	namespace PDB {

		UInt32 __fastcall GetModuleBase(UInt32 eip, HANDLE process) {
			IMAGEHLP_MODULE module = { 0 };
			module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
			if (!SymGetModuleInfo(process, eip, &module)) return 0;

			return module.BaseOfImage;
		}

		bool __fastcall GetModule(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize) {
			IMAGEHLP_MODULE module = { 0 };
			module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
			if (!SymGetModuleInfo(process, eip, &module))
				return false;

			strcpy_s(buffer, bufferSize, module.ModuleName);
			return true;
		}

		bool __fastcall GetSymbol(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize) {
			char symbolBuffer[sizeof(SYMBOL_INFO) + 255];
			const auto symbol = (SYMBOL_INFO*)symbolBuffer;

			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = 254;
			DWORD64 offset = 0;

			if (!SymFromAddr(process, eip, &offset, symbol))
				return false;

			sprintf_s(buffer, bufferSize, "%s+0x%I64X", symbol->Name, offset);
			return true;
		}

		bool __fastcall GetLine(UInt32 eip, HANDLE process, char* buffer, size_t bufferSize) {
			char lineBuffer[sizeof(IMAGEHLP_LINE) + 255];
			const auto line = (IMAGEHLP_LINE*)lineBuffer;
			line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

			DWORD offset = 0;

			if (!SymGetLineFromAddr(process, eip, &offset, line))
				return false;

			sprintf_s(buffer, bufferSize, "%s:%d", line->FileName, line->LineNumber);
			return true;
		}
	}

	void __fastcall GetCalltraceFunction(UInt32 eip, UInt32 ebp, HANDLE process, char* buffer, SIZE_T bufferSize) {
		/*if (GetModuleFileName((HMODULE)frame.AddrPC.Offset, path, MAX_PATH)) {  //Do this work on non base addresses even on  Windows? Cal directly the LDR function?
		if (!SymLoadModule(process, NULL, path, NULL, 0, 0)) Log() << FormatString("Porcoddio %0X", GetLastError());
		}*/

		const auto moduleBase = PDB::GetModuleBase(eip, process);

		char begin[16];
		sprintf_s(begin, "0x%08X | ", ebp);

		char middle[MAX_PATH] = {};

		const auto moduleOffset = (moduleBase != 0x00400000) ? eip - moduleBase + 0x10000000 : eip;

		{
			char module[MAX_PATH] = {};
			char symbol[MAX_PATH] = {};
			if (!PDB::GetModule(eip, process, module, sizeof(module)) || !module[0])
				sprintf_s(middle, "%28s (0x%08X) | %-40s |", "-\\(°_o)/-", moduleOffset, "(Corrupt stack or heap?)");
			else if (!PDB::GetSymbol(eip, process, symbol, sizeof(symbol)) || !symbol[0])
				sprintf_s(middle, "%28s (0x%08X) | %-40s |", module, moduleOffset, "");
			else
				sprintf_s(middle, "%28s (0x%08X) | %-40s |", module, moduleOffset, symbol);
		}

		char end[MAX_PATH] = {};
		PDB::GetLine(eip, process, end, sizeof(end));

		sprintf_s(buffer, bufferSize, "%s%s %s", begin, middle, end);
	}

	LONG WINAPI DoCrashSave(EXCEPTION_POINTERS* info)
	{
		CreateCrashSave();

		char buf[0x1000];

		auto contextRecord = info->ContextRecord;

		stbsp_sprintf(buf, "The geck has quit unexpectedly. Please check the Data//CrashSaves folder for a crash save and verify it in xEdit.\r\nDebug Information:\r\nREGISTERS\r\neip: %08X", contextRecord->Eip);
		stbsp_sprintf(buf, "%s\r\neax: %08X", buf, contextRecord->Eax);
		stbsp_sprintf(buf, "%s\r\nebx: %08X", buf, contextRecord->Ebx);
		stbsp_sprintf(buf, "%s\r\necx: %08X", buf, contextRecord->Ecx);
		stbsp_sprintf(buf, "%s\r\nedx: %08X", buf, contextRecord->Edx);
		stbsp_sprintf(buf, "%s\r\nedi: %08X", buf, contextRecord->Edi);
		stbsp_sprintf(buf, "%s\r\nesi: %08X", buf, contextRecord->Esi);
		stbsp_sprintf(buf, "%s\r\nebp: %08X", buf, contextRecord->Ebp);
		stbsp_sprintf(buf, "%s\r\nesp: %08X", buf, contextRecord->Esp);

		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		DWORD machine = IMAGE_FILE_MACHINE_I386;
		CONTEXT context = {};
		memcpy(&context, info->ContextRecord, sizeof(CONTEXT));

		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_ALLOW_ABSOLUTE_SYMBOLS);

		char buffer[2048] = {};
		{
			char pathBuffer[MAX_PATH];
			GetCurrentDirectory(sizeof(pathBuffer), pathBuffer);
			sprintf_s(buffer, "%s;%s\\Data\\NVSE\\Plugins", pathBuffer, pathBuffer);
			GetEnvironmentVariable("_NT_SYMBOL_PATH", pathBuffer, sizeof(pathBuffer));
			sprintf_s(buffer, "%s;%s", buffer, pathBuffer);
			GetEnvironmentVariable("_NT_ALTERNATE_SYMBOL_PATH ", pathBuffer, sizeof(pathBuffer));
			sprintf_s(buffer, "%s;%s", buffer, pathBuffer);

			if (!SymInitialize(process, buffer, true))
				_MESSAGE("Error initializing symbol store");
		}

		STACKFRAME frame = {};
		frame.AddrPC.Offset = info->ContextRecord->Eip;
		if (!frame.AddrPC.Offset && info->ContextRecord->Esp)
		{
			// if EIP was null, we likely hit a `call 0` so use the return address from [esp] instead
			frame.AddrPC.Offset = *(int*)info->ContextRecord->Esp;
		}

		frame.AddrPC.Mode = AddrModeFlat;
		frame.AddrFrame.Offset = info->ContextRecord->Ebp;
		frame.AddrFrame.Mode = AddrModeFlat;
		frame.AddrStack.Offset = info->ContextRecord->Esp;
		frame.AddrStack.Mode = AddrModeFlat;
		DWORD eip = 0;

		_MESSAGE("\nCalltrace:\n %*s%*s |  %*s%*s | %*s%*s |  Source", CENTERED_TEXT(10, "ebp"), CENTERED_TEXT(40, "Address"), CENTERED_TEXT(40, "Function"));

		while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) {
			if (frame.AddrPC.Offset == eip)
				break;
			eip = frame.AddrPC.Offset;
			GetCalltraceFunction(frame.AddrPC.Offset, frame.AddrFrame.Offset, process, buffer, sizeof(buffer));
			_MESSAGE(buffer);
		}

		UInt32* esp = (UInt32*)contextRecord->Esp;
		stbsp_sprintf(buf, "%s\r\n\r\nSTACK", buf);

		char stack[0x1000];
		*stack = 0;
		__try
		{
			for (int row = 0; row < 16; ++row)
			{
				for (int col = 0; col < 4; ++col)
				{
					stbsp_sprintf(stack, "%s%08X |", stack, esp[row * 4 + col]);
				}
				strcat(stack, "\r\n");
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {};

		stbsp_sprintf(buf, "%s\r\n%s", buf, stack);
		_MESSAGE("%s", buf);
		MessageBoxA(nullptr, buf, "Error", MB_ICONERROR | MB_OK);

		return s_originalFilter && s_originalFilter(info);
	};

	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI FakeSetUnhandledExceptionFilter(__in LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
	{
		s_originalFilter = lpTopLevelExceptionFilter;
		return nullptr;
	}

	void SetCrashSaveHandler()
	{
		SafeWrite32(0xD2326C, UInt32(FakeSetUnhandledExceptionFilter));
		SetUnhandledExceptionFilter(&DoCrashSave);
	}


	void Init()
	{
		SetCrashSaveHandler();
	}
}