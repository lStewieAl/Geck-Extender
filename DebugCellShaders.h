#pragma once

#include "NiNodes.h"

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
#define BSBatchRenderer_pCurrentPass (*(BSShaderProperty::RenderPass**)0xF23C30)



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

static const NiColorAlpha PassCountDebugColors[] = {
	NiColorAlpha(0.0f, 1.0f, 0.0f, 1.0f),
	NiColorAlpha(0.0f, 0.875f, 0.125f, 1.0f),
	NiColorAlpha(0.0f, 0.75f, 0.25f, 1.0f),
	NiColorAlpha(0.0f, 0.625f, 0.375f, 1.0f),
	NiColorAlpha(0.0f, 0.5f, 0.5f, 1.0f),
	NiColorAlpha(0.0f, 0.375f, 0.625f, 1.0f),
	NiColorAlpha(0.0f, 0.25f, 0.75f, 1.0f),
	NiColorAlpha(0.0f, 0.125f, 0.875f, 1.0f),
	NiColorAlpha(0.0f, 0.0f, 1.0f, 1.0f),
	NiColorAlpha(0.125f, 0.0f, 0.875f, 1.0f),
	NiColorAlpha(0.25f, 0.0f, 0.75f, 1.0f),
	NiColorAlpha(0.375f, 0.0f, 0.625f, 1.0f),
	NiColorAlpha(0.5f, 0.0f, 0.5f, 1.0f),
	NiColorAlpha(0.625f, 0.0f, 0.375f, 1.0f),
	NiColorAlpha(0.75f, 0.0f, 0.25f, 1.0f),
	NiColorAlpha(0.875f, 0.0f, 0.125f, 1.0f),
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

// Returns false if debug mode is active
bool SetDebugColors(NiColorAlpha* apAmbient, BSShaderProperty* apShaderProp, NiGeometry* apGeometry) {
	UInt32 uiLightCount = ThisStdCall(0x925830, apShaderProp); // GetActiveLightCount

	bool bAutoColor = config.bAutoLightWarnings && (uiLightCount > 6) && eDebugRenderMode != DEBUG_LIGHT_COUNT;

	if (eLastRenderMode != eDebugRenderMode) {
		bRenderModeChanged = true;
		eLastRenderMode = eDebugRenderMode;
	}

	if (eDebugRenderMode > 0 || bAutoColor) {
		if (eDebugRenderMode == DEBUG_LIGHT_COUNT || bAutoColor) {
			uiLightCount = max(min(uiLightCount, 10), 0);
			*apAmbient = LightCountDebugColors[uiLightCount];

			if (bAutoColor) {
				apAmbient->Scale(1.5f);
			}
		}
		else if (eDebugRenderMode == DEBUG_PASS_COUNT) {
			UInt16 usPasses = apShaderProp->GetNumberofPasses(apGeometry);
			*apAmbient = PassCountDebugColors[min(16, max(0, usPasses))];
		}

		if (bRenderModeChanged) {
			LightingFlags = 1;
			bRenderModeChanged = false;
		}

		return false;
	}
	else {
		return true;
	}
}

void inline HandleDebugRender(DWORD* apShader, BSShaderProperty* apShaderProperty, DWORD* apRenderPass, NiGeometry* apGeometry, NiMaterialProperty* apMaterialProperty, bool abIs30Shader) {
	static NiColorAlpha* pAmbientColor = abIs30Shader ? &ShaderConstant_AmbientColor30 : &ShaderConstant_AmbientColor;

	if (SetDebugColors(pAmbientColor, apShaderProperty, apGeometry)) {
		if (abIs30Shader) {
			StdCall(0x9661C0, apMaterialProperty, apShaderProperty); // Lighting30Shader::UpdateAmbientColor
		}
		else {
			StdCall(0x94DA90, apShaderProperty, apRenderPass, apGeometry, apMaterialProperty); // ShadowLightShader::UpdateAmbientColor
		}
		if (bRenderModeChanged) {
			LightingFlags |= (1 | 2 | 8 | 20);
			bRenderModeChanged = false;
		}
	}
}

void __stdcall ShadowLightShader_UpdateAmbientColorEx(BSShaderProperty* apShaderProperty, DWORD* apRenderPass, NiGeometry* apGeometry, NiMaterialProperty* apMaterialProperty) {
	HandleDebugRender(nullptr, apShaderProperty, apRenderPass, apGeometry, apMaterialProperty, false);
}

void __stdcall Lighting30Shader_UpdateAmbientColor30Ex(NiMaterialProperty* apMaterialProperty, BSShaderProperty* apShaderProperty) {
	HandleDebugRender(nullptr, apShaderProperty, nullptr, nullptr, apMaterialProperty, true);
}


void __fastcall BSShaderPPLightingProperty__AddPass_Opt(void* apThis, void*, void* apGeometry, void* apLight0, UInt16* apusPassCount, BOOL abAddPass, bool* abEnable, bool abSkinned, bool abGlowMap, bool abSpecular, bool bFacegen, bool abHasLights) {
	if (abAddPass == 0) {
		(*apusPassCount)++;
		return;
	}

	ThisStdCall(0x97FDD0, apThis, apGeometry, apLight0, apusPassCount, abAddPass, abEnable, abSkinned, abGlowMap, abSpecular, bFacegen, abHasLights);
}

void __fastcall ShadowLightShader__SetAmbientColor(void* apThis) {
	static NiColorAlpha* pAmbientColor = &ShaderConstant_AmbientColor;

	NiGeometry* pGeometry = BSBatchRenderer_pCurrentPass->pGeometry;
	BSShaderProperty* pShaderProperty = (BSShaderProperty*)pGeometry->m_kProperties.m_aspProps[NiPropertyState::SHADE];

	if (SetDebugColors(pAmbientColor, pShaderProperty, pGeometry)) {
		ThisStdCall(0x94E5A0, apThis);
	}
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

	WriteRelCall(0x94FD81, UInt32(ShadowLightShader__SetAmbientColor));
	WriteRelCall(0x94FFC1, UInt32(ShadowLightShader__SetAmbientColor));


	// Crash fix
	WriteRelCall(0x98158C, UInt32(BSShaderPPLightingProperty__AddPass_Opt));

	// Prevent shader package destruction (allows shader reloading)
	SafeWrite8(0x9001F8, 0xEB);
	SafeWrite8(0x9002AA, 0xEB);
}