#pragma once

// --- Standard Includes ---
#include <Windows.h>

// --- Forward Class Definitions ---
class GlDxInterop;

// --- Using Namespace Declarations ---


class IGlDxRenderTarget
{
	// ---------- CREATORS ----------
	protected:
		IGlDxRenderTarget(void);
	public:
		virtual ~IGlDxRenderTarget(void);

	// ---------- FUNCTIONS ----------
	public:
		virtual bool Initialize(HWND in_hwnd) = 0;
		virtual void Cleanup();
		virtual void Flush() = 0;
		virtual void BeginGlDraw();
		virtual void EndGlDraw();

		// --- Getters ---		
		char * GetLastError() { return m_lastError; }

		// --- Setters ---
		void EnableStereo(bool in_isStereo = true) { m_isStereo = in_isStereo; }

	protected:


	// ---------- VARIABLES ----------
	protected:
		char m_lastError[512];
		bool m_isStereo;
		GlDxInterop * m_interop;
};

