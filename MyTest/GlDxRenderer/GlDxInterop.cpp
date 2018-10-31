#include "GlDxInterop.h"

// --- This File Includes ---

// --- Using Namespace Declarations ---

/// <summary>
/// </summary>
GlDxInterop::GlDxInterop(void)
{
	m_lastError[0] = '\0';
	m_glD3DHandle = NULL;
	m_glD3DSharedLeftColorHandle = NULL;
	m_glD3DSharedRightColorHandle = NULL;
	m_glD3DSharedDepthHandle = NULL;

	glGenFramebuffers = NULL;
	glBindFramebuffer = NULL;
	glGenRenderbuffers = NULL;
	glFramebufferRenderbuffer = NULL;
	glFramebufferTexture2D = NULL;
	wglDXOpenDeviceNV = NULL;
	wglDXCloseDeviceNV = NULL;
	wglDXRegisterObjectNV = NULL;
	wglDXUnregisterObjectNV = NULL;
	wglDXLockObjectsNV = NULL;
	wglDXUnlockObjectsNV = NULL;
}


/// <summary>
/// </summary>
GlDxInterop::~GlDxInterop(void)
{
	if(m_glD3DHandle)
	{
		// Unshare all the resources
		// TODO: This throws an error for some reason!!!
		if (m_glD3DSharedLeftColorHandle)
		{
			BOOL result = wglDXUnregisterObjectNV(m_glD3DHandle, m_glD3DSharedLeftColorHandle);			
			m_glD3DSharedLeftColorHandle = NULL;
		}
		if (m_glD3DSharedRightColorHandle)
		{
			BOOL result = wglDXUnregisterObjectNV(m_glD3DHandle, m_glD3DSharedRightColorHandle);			
			m_glD3DSharedRightColorHandle = NULL;
		}
		if (m_glD3DSharedDepthHandle)
		{
			wglDXUnregisterObjectNV(m_glD3DHandle, m_glD3DSharedDepthHandle);
			m_glD3DSharedDepthHandle = NULL;
		}

		wglDXCloseDeviceNV(m_glD3DHandle);
		m_glD3DHandle = NULL;
	}

	glGenFramebuffers = NULL;
	glBindFramebuffer = NULL;
	glGenRenderbuffers = NULL;
	glFramebufferRenderbuffer = NULL;
	glFramebufferTexture2D = NULL;
	wglDXOpenDeviceNV = NULL;
	wglDXCloseDeviceNV = NULL;
	wglDXRegisterObjectNV = NULL;
	wglDXUnregisterObjectNV = NULL;
	wglDXLockObjectsNV = NULL;
	wglDXUnlockObjectsNV = NULL;
}

/// <summary>
/// </summary>
bool GlDxInterop::Initialize(ID3D11Device * in_device)
{
	// Perform some checks on the device

	return InitializeInterop(in_device);
}

/// <summary>
/// </summary>
bool GlDxInterop::Initialize(IDirect3DDevice9Ex * in_device)
{
	// Perform some checks on the device

	return InitializeInterop(in_device);
}

/// <summary>
/// Initialize the interop functionality
/// </summary>
bool GlDxInterop::InitializeInterop(void * in_device)
{
	GLenum error;

	if(LoadWglFunctions() == false)
		return false;

	m_glD3DHandle = wglDXOpenDeviceNV(in_device);
	if(m_glD3DHandle == NULL)
	{
		strcpy(m_lastError, "Could not create the GL <-> DirectX Interop.");
		return false;
	}

	// Initialize the frame buffer object
	glGenFramebuffers(1, &m_glFbo);
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		strcpy(m_lastError, "Could not create frame buffer object.");
		return false;
	}

	// Initialize the color and depth buffers
	glGenTextures(1, &m_glLeftColorBuffer);
	glGenTextures(1, &m_glRightColorBuffer);
	glGenTextures(1, &m_glDepthBuffer);
	error = glGetError();
	if(error != GL_NO_ERROR)
	{
		strcpy(m_lastError, "Could not create color or depth buffer objects.");
		return false;
	}

	strcpy(m_lastError, "");
	return true;
}

/// <summary>
/// Loads the Wgl function pointers from the opengl library
/// </summary>
bool GlDxInterop::LoadWglFunctions()
{
	// Load the extension functions
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	if(glGenFramebuffers == NULL)
	{
		strcpy(m_lastError, "Could not find the glGenFramebuffers function pointer.");
		return false;
	}
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	if(glBindFramebuffer == NULL)
	{
		strcpy(m_lastError, "Could not find the glBindFramebuffer function pointer.");
		return false;
	}
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	if(glGenRenderbuffers == NULL)
	{
		strcpy(m_lastError, "Could not find the glGenRenderbuffers function pointer.");
		return false;
	}
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	if(glFramebufferRenderbuffer == NULL)
	{
		strcpy(m_lastError, "Could not find the glFramebufferRenderbuffer function pointer.");
		return false;
	}
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	if(glFramebufferTexture2D == NULL)
	{
		strcpy(m_lastError, "Could not find the glFramebufferTexture2D function pointer.");
		return false;
	}
	wglDXOpenDeviceNV = (PFNWGLDXOPENDEVICENVPROC)wglGetProcAddress("wglDXOpenDeviceNVX");
	if(wglDXOpenDeviceNV == NULL)
	{
		strcpy(m_lastError, "Could not find the wglDXOpenDeviceNV function pointer.");
		return false;
	}
	wglDXCloseDeviceNV = (PFNWGLDXCLOSEDEVICENVPROC)wglGetProcAddress("wglDXCloseDeviceNV");
	if(wglDXCloseDeviceNV == NULL)
	{
		strcpy(m_lastError, "Could not find the wglDXCloseDeviceNV function pointer.");
		return false;
	}
	wglDXRegisterObjectNV = (PFNWGLDXREGISTEROBJECTNVPROC)wglGetProcAddress("wglDXRegisterObjectNV");
	if(wglDXRegisterObjectNV == NULL)
	{
		strcpy(m_lastError, "Could not find the wglDXRegisterObjectNV function pointer.");
		return false;
	}
	wglDXUnregisterObjectNV = (PFNWGLDXUNREGISTEROBJECTNVPROC)wglGetProcAddress("wglDXUnregisterObjectNV");
	if(wglDXUnregisterObjectNV == NULL)
	{
		strcpy(m_lastError, "Could not find the wglDXUnregisterObjectNV function pointer.");
		return false;
	}
	wglDXLockObjectsNV = (PFNWGLDXLOCKOBJECTSNVPROC)wglGetProcAddress("wglDXLockObjectsNV");
	if(wglDXLockObjectsNV == NULL)
	{
		strcpy(m_lastError, "Could not find the wglDXLockObjectsNV function pointer.");
		return false;
	}
	wglDXUnlockObjectsNV = (PFNWGLDXUNLOCKOBJECTSNVPROC)wglGetProcAddress("wglDXUnlockObjectsNV");
	if(wglDXUnlockObjectsNV == NULL)
	{
		strcpy(m_lastError, "Could not find the wglDXUnlockObjectsNV function pointer.");
		return false;
	}
	return true;
}

/// <summary>
/// Registers the left color buffer with opengl.  You must ensure that the texture type
/// is compatible with the texture type used when creating the directx device.
/// </summary>
bool GlDxInterop::RegisterDXLeftColorBuffer(void * in_buffer, GLenum in_textureType)
{
	// Register the directx color buffer with OpenGL
	m_glD3DSharedLeftColorHandle = wglDXRegisterObjectNV(m_glD3DHandle, in_buffer, m_glLeftColorBuffer, in_textureType, WGL_ACCESS_READ_WRITE_NV);
	if(m_glD3DSharedLeftColorHandle == NULL)
	{
		strcpy(m_lastError, "Could not register left color buffer.");
		return false;
	}

	// Bind the color buffer to the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_glFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, in_textureType, m_glLeftColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
/// <summary>
/// Registers the right color buffer with opengl.  You must ensure that the texture type
/// is compatible with the texture type used when creating the directx device.
/// </summary>
bool GlDxInterop::RegisterDXRightColorBuffer(void * in_buffer, GLenum in_textureType)
{
	// Register the directx color buffer with OpenGL
	m_glD3DSharedRightColorHandle = wglDXRegisterObjectNV(m_glD3DHandle, in_buffer, m_glRightColorBuffer, in_textureType, WGL_ACCESS_READ_WRITE_NV);
	if(m_glD3DSharedRightColorHandle == NULL)
	{
		strcpy(m_lastError, "Could not register right color buffer.");
		return false;
	}

	// Bind the color buffer to the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_glFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, in_textureType, m_glRightColorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

/// <summary>
/// Registers the depth buffer with opengl.  You must ensure that the texture type
/// is compatible with the texture type used when creating the directx device.
/// </summary>
bool GlDxInterop::RegisterDXDepthBuffer(void * in_buffer, GLenum in_textureType)
{
	// Register the directx color buffer with OpenGL
	m_glD3DSharedDepthHandle = wglDXRegisterObjectNV(m_glD3DHandle, in_buffer, m_glDepthBuffer, in_textureType, WGL_ACCESS_READ_WRITE_NV);
	if(m_glD3DSharedDepthHandle == NULL)
	{
		strcpy(m_lastError, "Could not register depth buffer.");
		return false;
	}

	// Bind the color buffer to the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_glFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, in_textureType, m_glDepthBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, in_textureType, m_glDepthBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void GlDxInterop::BeginGLDraw()
{
	// Lock for opengl
	if(m_glD3DSharedLeftColorHandle)
		wglDXLockObjectsNV(m_glD3DHandle, 1, &m_glD3DSharedLeftColorHandle);
	if(m_glD3DSharedRightColorHandle)
		wglDXLockObjectsNV(m_glD3DHandle, 1, &m_glD3DSharedRightColorHandle);
	if(m_glD3DSharedDepthHandle)
		wglDXLockObjectsNV(m_glD3DHandle, 1, &m_glD3DSharedDepthHandle);

	// Bind to the frame buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_glFbo);
}

void GlDxInterop::EndGLDraw()
{
	// Unbind the frame buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Unlock for opengl
	if(m_glD3DSharedLeftColorHandle)
		wglDXUnlockObjectsNV(m_glD3DHandle, 1, &m_glD3DSharedLeftColorHandle);
	if(m_glD3DSharedRightColorHandle)
		wglDXLockObjectsNV(m_glD3DHandle, 1, &m_glD3DSharedRightColorHandle);
	if(m_glD3DSharedDepthHandle)
		wglDXUnlockObjectsNV(m_glD3DHandle, 1, &m_glD3DSharedDepthHandle);
}