#pragma once

enum DebugRenderModes {
	DEBUG_NONE = 0,
	DEBUG_LIGHT_COUNT = 1,
	DEBUG_PASS_COUNT = 2,
	DEBUG_TEXTURE_COUNT = 4,
};

#define LightingFlags *(UInt32*)0xF23C28
#define eDebugRenderMode *(DebugRenderModes*)0xF23C04
#define ShaderConstant_AmbientColor (*(NiColorAlpha*)0xF244E0)
#define ShaderConstant_AmbientColor30 (*(NiColorAlpha*)0xF2B988)



static const NiColorAlpha LightCountDebugColors[] = {
	NiColorAlpha(0.0f, 1.0f, 0.0f, 1.0f),
	NiColorAlpha(0.0f, 0.8f, 0.2f, 1.0f),
	NiColorAlpha(0.0f, 0.6f, 0.4f, 1.0f),
	NiColorAlpha(0.0f, 0.4f, 0.6f, 1.0f),
	NiColorAlpha(0.0f, 0.2f, 0.8f, 1.0f),
	NiColorAlpha(0.0f, 0.0f, 1.0f, 1.0f),
	NiColorAlpha(0.2f, 0.0f, 0.8f, 1.0f),
	NiColorAlpha(0.4f, 0.0f, 0.6f, 1.0f),
	NiColorAlpha(0.6f, 0.0f, 0.4f, 1.0f),
	NiColorAlpha(0.8f, 0.0f, 0.2f, 1.0f),
	NiColorAlpha(1.0f, 0.0f, 0.0f, 1.0f),
};


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


// TODO - Wall
// Attach bAutoLightWarnings to some button or a hotkey
// Finish Lighting30Shader variant

static bool bRenderModeChanged = false;
static DebugRenderModes eLastRenderMode = DEBUG_NONE;

void inline HandleDebugRender(DWORD* pShader, DWORD* pShaderProperty, DWORD* pRenderPass, DWORD* pGeometry, DWORD* pMaterialProperty, bool bIs30Shader) {
	UInt32 uiLightCount = ThisStdCall(0x925830, pShaderProperty); // GetActiveLightCount

	bool bAutoColor = bAutoLightWarnings && (uiLightCount > 6) && eDebugRenderMode != DEBUG_LIGHT_COUNT;

	if (eLastRenderMode != eDebugRenderMode) {
		bRenderModeChanged = true;
		eLastRenderMode = eDebugRenderMode;
	}

	if (eDebugRenderMode == 1 || bAutoColor) {
		uiLightCount = max(min(uiLightCount, 10), 0);
		static NiColorAlpha* pAmbientColor = bIs30Shader ? &ShaderConstant_AmbientColor30 : &ShaderConstant_AmbientColor;
		*pAmbientColor = LightCountDebugColors[uiLightCount];

		if (bAutoColor) {
			pAmbientColor->Scale(3.0f);
		}

		if (eDebugRenderMode == 1 && bRenderModeChanged) {
			LightingFlags = 1;
			bRenderModeChanged = false;
		}
	}
	else {
		if (bIs30Shader) {
			StdCall(0x9661C0, pMaterialProperty, pShaderProperty); // Lighting30Shader::UpdateAmbientColor
		}
		else {
			StdCall(0x94DA90, pShaderProperty, pRenderPass, pGeometry, pMaterialProperty); // ShadowLightShader::UpdateAmbientColor
		}
		if (bRenderModeChanged) {
			LightingFlags |= (1 | 2 | 8 | 20);
			bRenderModeChanged = false;
		}
	}
}

void __stdcall ShadowLightShader_UpdateAmbientColorEx(DWORD* pShaderProperty, DWORD* pRenderPass, DWORD* pGeometry, DWORD* pMaterialProperty) {
	HandleDebugRender(nullptr, pShaderProperty, pRenderPass, pGeometry, pMaterialProperty, false);
}

void __stdcall Lighting30Shader_UpdateAmbientColor30Ex(DWORD* pMaterialProperty, DWORD* pShaderProperty) {
	HandleDebugRender(nullptr, pShaderProperty, nullptr, nullptr, pMaterialProperty, true);
}

void RestoreRenderWindowDebugShaders()
{
	// Needs more decoding. They are for 3.0 shaders which are not that important for now - Wall
	//WriteRelJump(0x925EC8, UInt32(DebugShaderHook925E80));
	//WriteRelJump(0x926223, UInt32(DebugShaderHook9261D0));
	//WriteRelJump(0x953744, UInt32(DebugShaderHook952F60));
	//WriteRelJump(0x965B6C, UInt32(DebugShaderHook965B40));

	WriteRelCall(0x94FBAB, UInt32(ShadowLightShader_UpdateAmbientColorEx));
	//WriteRelCall(0x966625, UInt32(Lighting30Shader_UpdateAmbientColor30Ex));

#define RES_HACKER_ADDR_TO_ACTUAL 0x4CF800

	// Set WS_VISIBLE on debug shader buttons - Stewie
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x30, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x60, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x90, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0xC0, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x100, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x134, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x16C, 0x50);
	SafeWrite8(RES_HACKER_ADDR_TO_ACTUAL + 0xC2DD97 + 0x1A4, 0x50);
}