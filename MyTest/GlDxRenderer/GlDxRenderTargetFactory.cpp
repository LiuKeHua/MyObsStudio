#include "GlDxRenderTargetFactory.h"

// --- This File Includes ---
#include "GlDx9RenderTarget.h"
#include "GlDx11RenderTarget.h"

// --- Using Namespace Declarations ---



/// <summary>
/// </summary>
GlDxRenderTargetFactory::GlDxRenderTargetFactory(void)
{
}

/// <summary>
/// </summary>
GlDxRenderTargetFactory::~GlDxRenderTargetFactory(void)
{
}

/// <summary>
/// Create a render target with the specified type
/// </summary>
IGlDxRenderTarget * GlDxRenderTargetFactory::CreateRenderTarget(GLDX_DIRECTX_SDK_TYPE in_type)
{
	if(in_type == DX_SDK_9)
		return new GlDx9RenderTarget();
	else
		return new GlDx11RenderTarget();
}
