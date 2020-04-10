#pragma once

_declspec(naked) void DebugShaderHook925E80()
{
	static const UInt32 debugAddr = 0x925E8D;
	static const UInt32 retnAddr = 0x925ECE;
	_asm
	{
		mov eax, [0xF23C04] // g_renderWindowShaderDebugFlags
		cmp eax, 1
		jl noDebugFlags
		cmp eax, 6
		jg noDebugFlags
		jmp debugAddr
		noDebugFlags :
		mov ecx, [0xF23C30]
			jmp retnAddr
	}
}

_declspec(naked) void DebugShaderHook9261D0()
{
	static const UInt32 debugAddr = 0x9261EB;
	static const UInt32 retnAddr = 0x926229;
	_asm
	{
		mov eax, [0xF23C04] // g_renderWindowShaderDebugFlags
		cmp eax, 1
		jl noDebugFlags
		cmp eax, 6
		jg noDebugFlags
		jmp debugAddr
		noDebugFlags :
		mov esi, [edi + 0xF8]
			jmp retnAddr
	}
}

_declspec(naked) void DebugShaderHook952F60()
{
	static const UInt32 debugAddr = 0x9538D5;
	static const UInt32 retnAddr = 0x95374A;
	_asm
	{
		je debugFlags
		mov eax, [0xF23C04] // g_renderWindowShaderDebugFlags
		test eax, eax
		jne debugFlags
		jmp retnAddr
		debugFlags :
		jmp debugAddr
	}
}


_declspec(naked) void DebugShaderHook965B40()
{
	static const UInt32 retnAddr = 0x965B78;
	_asm
	{
		mov eax, [0xF23C04] // g_renderWindowShaderDebugFlags
		cmp eax, 6
		ja debugFlags
		cmp edi, 0x1BD
		jb noDebug
		cmp edi, 0x1E1
		jbe debugFlags

	noDebug:
		mov ecx, [0xF983E4]
		mov edx, [ecx + 0x24]
		mov eax, [edx]
		test eax, eax
		je jmp1
		mov ecx, [0xF23F80]
		mov[eax + 8], ecx
	jmp1:
		mov edx, [0xF983E4]
		mov eax, [edx + 0x24]
		mov eax, [eax + 4]
		test eax, eax
		je done
		mov ecx, [0xF23F60]
		mov[eax + 8], ecx
		jmp done

	debugFlags:
		mov edx, [esi]
		mov eax, [edx + 0x13C]
		push ebx
		push edi
		call eax
	done:
		jmp retnAddr
	}
}

_declspec(naked) void DebugShaderHook9661C0()
{
	_asm
	{
		mov eax, [0xF23C04] // g_renderWindowShaderDebugFlags
		cmp eax, 1
		je shader1
		cmp eax, 2
		je shader2
		cmp eax, 4
		je shader4
		cmp eax, 5
		je shader5
	shader1:

	shader2:

	shader4:

	shader5:

		//	done
		add esp, 0x10
		ret 8
	}
}

void RestoreRenderWindowDebugShaders()
{
	WriteRelJump(0x925EC8, UInt32(DebugShaderHook925E80));
	WriteRelJump(0x926223, UInt32(DebugShaderHook9261D0));
	WriteRelJump(0x953744, UInt32(DebugShaderHook952F60));
	WriteRelJump(0x965B6C, UInt32(DebugShaderHook965B40));
	WriteRelJump(0x965B6C, UInt32(DebugShaderHook965B40));

	// add in switch statements
	WriteRelJump(0x9663C0, UInt32(DebugShaderHook9661C0));
	WriteRelJump(0x9663D0, UInt32(DebugShaderHook9661C0));
	WriteRelJump(0x966336, UInt32(DebugShaderHook9661C0));
	// still need to add the two switch statements in sub_94DA90 and sub_9661C0
}

