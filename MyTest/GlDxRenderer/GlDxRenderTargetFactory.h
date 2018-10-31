#pragma once

// --- Standard Includes ---
#include <Windows.h>

// --- Forward Class Definitions ---
class IGlDxRenderTarget;

// --- Using Namespace Declarations ---


enum GLDX_DIRECTX_SDK_TYPE
{
	DX_SDK_9,
	DX_SDK_11
};

class GlDxRenderTargetFactory
{
	// ---------- CREATORS ----------
	protected:
		GlDxRenderTargetFactory(void);
	public:
		virtual ~GlDxRenderTargetFactory(void);

	// ---------- FUNCTIONS ----------
	public:

		static IGlDxRenderTarget * CreateRenderTarget(GLDX_DIRECTX_SDK_TYPE in_type);

		// --- Getters ---		

		// --- Setters ---

	protected:


	// ---------- VARIABLES ----------
	protected:
};

