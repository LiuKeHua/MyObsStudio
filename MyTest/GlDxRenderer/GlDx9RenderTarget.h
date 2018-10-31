#pragma once

// --- Standard Includes ---
#include "IGlDxRenderTarget.h"
#include <d3d9.h>

// --- Forward Class Definitions ---

// --- Using Namespace Declarations ---


class GlDx9RenderTarget : public IGlDxRenderTarget
{
	// ---------- CREATORS ----------
	public:
		GlDx9RenderTarget(void);
		virtual ~GlDx9RenderTarget(void);

	// ---------- FUNCTIONS ----------
	public:
		virtual bool Initialize(HWND in_hwnd);
		virtual void Cleanup();
		virtual void Flush();

		// --- Getters ---
		IDirect3DDevice9Ex * GetDevice() { return m_device; }

		// --- Setters ---		

	protected:
		bool InitializeInterop();
		bool VerifyDeviceCapabilities(D3DFORMAT in_desiredColorFormat, D3DFORMAT in_desiredDepthFormat);


	// ---------- VARIABLES ----------
	protected:
		HWND m_hWnd;
		IDirect3D9Ex * m_direct3D;
		IDirect3DDevice9Ex  *m_device;
		D3DPRESENT_PARAMETERS m_d3dpp;
		IDirect3DSurface9 * m_dxLeftColorBuffer;
		IDirect3DTexture9 * m_dxLeftColorTexture;
		IDirect3DSurface9 * m_dxRightColorBuffer;
		IDirect3DTexture9 * m_dxRightColorTexture;
		IDirect3DSurface9 * m_dxRenderedBuffer;
		IDirect3DSurface9 * m_dxDepthBuffer;
};

