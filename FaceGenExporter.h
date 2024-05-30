#pragma once

#include <string>
#include <format>
#include <filesystem>
#include "DirectXTex/DirectXTex/DirectXTex.h"
#include <d3d9.h>

#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <NiNodes.h>

namespace fs = std::filesystem;

extern bool g_bPauseLogging;

LRESULT (__cdecl* FooterPrint)(WPARAM, LPARAM) = (LRESULT(__cdecl*)(WPARAM, LPARAM))(0x4657A0);

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

#pragma comment(lib, "DirectXTex/DirectXTex/Lib/DirectXTex.lib")

namespace FaceGenExporter {
	DXGI_FORMAT D3DToDXGI(D3DFORMAT aeFormat) {
		switch (aeFormat) {
		case D3DFMT_A8B8G8R8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case D3DFMT_A2B10G10R10:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case D3DFMT_A8R8G8B8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case D3DFMT_X8R8G8B8:
			return DXGI_FORMAT_B8G8R8X8_UNORM;
		case D3DFMT_A16B16G16R16:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case D3DFMT_A16B16G16R16F:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case D3DFMT_DXT1:
			return DXGI_FORMAT_BC1_UNORM;
		case D3DFMT_DXT3:
			return DXGI_FORMAT_BC2_UNORM;
		case D3DFMT_DXT5:
			return DXGI_FORMAT_BC3_UNORM;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	};

	// Saves the texture as a BGR DDS file
	static int __fastcall SaveTextureImage(void* apThis, void*, NiTexture* apTexture, const char* apPath, UInt32 aeFormat) {
		if (apTexture == nullptr) {
			_MESSAGE("NiTexture is null");
			return D3DERR_INVALIDCALL;
		}

		NiDX9TextureData* pData = (NiDX9TextureData*)apTexture->rendererData;
		ComPtr<IDirect3DTexture9> spSourceTexture = (LPDIRECT3DTEXTURE9)pData->m_pkD3DTexture;

		auto pDevice = NiDX9Renderer::GetSingleton()->GetD3DDevice();

		if (spSourceTexture.Get() == nullptr) {
			_MESSAGE("D3D Texture is null");
			return D3DERR_INVALIDCALL;
		}


		// Get texture description
		D3DSURFACE_DESC kDesc;
		spSourceTexture->GetLevelDesc(0, &kDesc);

		ComPtr<IDirect3DTexture9> spWorkingTexture;
		HRESULT result;

		// If the texture is in default pool, create a system memory texture to copy the data to
		if (kDesc.Pool == D3DPOOL_DEFAULT) {
			result = pDevice->CreateTexture(kDesc.Width, kDesc.Height, 1, 0, kDesc.Format, D3DPOOL_SYSTEMMEM, spWorkingTexture.GetAddressOf(), nullptr);
			if (FAILED(result)) {
				_MESSAGE("Failed to create system memory texture - %x", result);
				return result;
			}

			ComPtr<IDirect3DSurface9> pOriginalSurface;
			ComPtr<IDirect3DSurface9> pSysMemSurface;

			result = spSourceTexture->GetSurfaceLevel(0, &pOriginalSurface);
			if (SUCCEEDED(result)) {
				result = spWorkingTexture->GetSurfaceLevel(0, &pSysMemSurface);
				if (SUCCEEDED(result)) {
					result = pDevice->GetRenderTargetData(pOriginalSurface.Get(), pSysMemSurface.Get());
					if (FAILED(result)) {
						_MESSAGE("Failed to update surface - %x", result);
						return result;
					}
				}
			}
		}
		else {
			spWorkingTexture = spSourceTexture;
		}

		// Lock the texture, so we can read its pixel data
		D3DLOCKED_RECT kLockedRect;
		result = spWorkingTexture->LockRect(0, &kLockedRect, nullptr, D3DLOCK_READONLY);
		if (FAILED(result)) {
			if (result == D3DERR_WASSTILLDRAWING)
				_MESSAGE("Texture is still drawing");
			else if (result == D3DERR_INVALIDCALL)
				_MESSAGE("Invalid call to lock texture");
			else
				_MESSAGE("Failed to lock texture - %x", result);
			return result;
		}

		if (kLockedRect.pBits == nullptr) {
			_MESSAGE("Locked bits are null");
			spWorkingTexture->UnlockRect(0);
			return result;
		}

		// Create an image to store the texture data
		DirectX::Image kImg = {};

		// Copy the pixel data to a new buffer
		size_t uiSize = kLockedRect.Pitch * kDesc.Height;
		kImg.pixels = new uint8_t[uiSize];
		memcpy(kImg.pixels, kLockedRect.pBits, uiSize);

		spWorkingTexture->UnlockRect(0);

		// Fill other image properties
		kImg.height = kDesc.Height;
		kImg.width = kDesc.Width;
		kImg.format = D3DToDXGI(kDesc.Format);
		DirectX::ComputePitch(kImg.format, kImg.width, kImg.height, kImg.rowPitch, kImg.slicePitch, DirectX::CP_FLAGS_LEGACY_DWORD);

		DirectX::ScratchImage kScratchImg;
		DirectX::ScratchImage kScratchMipMapImg;

		// Move the image to a scratch image
		if (DirectX::IsCompressed(kImg.format)) {
			DirectX::Decompress(kImg, DXGI_FORMAT_UNKNOWN, kScratchImg);
		}
		else {
			kScratchImg.InitializeFromImage(kImg);
		}

		// Generate mipmaps
		DirectX::GenerateMipMaps(kScratchImg.GetImages(), kScratchImg.GetImageCount(), kScratchImg.GetMetadata(), DirectX::TEX_FILTER_TRIANGLE, 0, kScratchMipMapImg);

		// Set the alpha mode to opaque (default is unknown)
		DirectX::TexMetadata kMetaData = kScratchMipMapImg.GetMetadata();
		kMetaData.SetAlphaMode(DirectX::TEX_ALPHA_MODE_OPAQUE);

		// Convert char to wchar_t
		size_t size = strlen(apPath) + 1;
		wchar_t* wPath = new wchar_t[size];
		mbstowcs_s(nullptr, wPath, size, apPath, size);

		result = DirectX::SaveToDDSFile(kScratchMipMapImg.GetImages(), kScratchMipMapImg.GetImageCount(), kMetaData, DirectX::DDS_FLAGS_FORCE_DX9_LEGACY | DirectX::DDS_FLAGS_FORCE_RGB, wPath);

		if (FAILED(result)) {
			_MESSAGE("Failed to save texture to \"%s\" - %x", apPath, result);
			goto FINISH;
		}

	FINISH:
		delete[] kImg.pixels;
		delete[] wPath;
		return 0;
	}

	// Returns a null pointer, so the body texture would be regenerated
	// Vanilla skips generation if the texture already exists
	void __fastcall CreateTextureImage(void* apThis, void*, const char* apPath, NiSourceTexture* aspTexture, bool abNoFileOK, BOOL abArchiveOnly) {
		aspTexture = nullptr;
	}

	LRESULT __stdcall ActionStart(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam2) {
		LPARAM result = hk_SendMessageA(hWnd, Msg, wParam, lParam2);
		if (SUCCEEDED(result))
			g_bPauseLogging = true;
		return result;
	}

	__HOOK ObjectWindowF4HotkeyHook()
	{
		_asm
		{
			push edx
			call ActionStart
			mov esi, eax

			mov eax, 0x44AB5C
			jmp eax
		}
	}

	LRESULT __cdecl ActionStart2(WPARAM wParam, LPARAM lParam)  {
		LRESULT result = CdeclCall<LRESULT>(0x4657A0, wParam, lParam);
		g_bPauseLogging = true;
		return result;
	}

	LRESULT __cdecl ActionEnd(WPARAM wParam, LPARAM lParam) {
		g_bPauseLogging = false;
		return CdeclCall<LRESULT>(0x4657A0, wParam, lParam);
	}

	static void InitHooks() {
		for (UInt32 addr : { 0x570C36, 0x570D54, 0x574801, 0x5749F9 })
			WriteRelCall(addr, (UInt32)FaceGenExporter::SaveTextureImage);

		WriteRelCall(0x587D7F, (UInt32)FaceGenExporter::CreateTextureImage);

		// Shortcut
		WriteRelJump(0x44AB57, UInt32(ObjectWindowF4HotkeyHook));
		WriteRelCall(0x44AC23, (UInt32)ActionEnd);

		// GUI
		WriteRelCall(0x442064, (UInt32)ActionStart2);
		WriteRelCall(0x44208E, (UInt32)ActionEnd);
	}
};