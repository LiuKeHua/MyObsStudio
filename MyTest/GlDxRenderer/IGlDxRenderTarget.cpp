#include "IGlDxRenderTarget.h"

// --- This File Includes ---
#include "GlDxInterop.h"
#include "GlDx9RenderTarget.h"
#include "GlDx11RenderTarget.h"

// --- Using Namespace Declarations ---



/// <summary>
/// </summary>
IGlDxRenderTarget::IGlDxRenderTarget(void)
{
	m_interop = NULL;
}

/// <summary>
/// </summary>
IGlDxRenderTarget::~IGlDxRenderTarget(void)
{
	Cleanup();
}

void IGlDxRenderTarget::Cleanup()
{
	if(m_interop)
	{
		delete m_interop;
		m_interop = NULL;
	}
}

void IGlDxRenderTarget::BeginGlDraw()
{
	m_interop->BeginGLDraw();
}

void IGlDxRenderTarget::EndGlDraw()
{
	m_interop->EndGLDraw();
}