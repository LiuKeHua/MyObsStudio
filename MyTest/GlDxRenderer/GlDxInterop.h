#pragma once

// --- Standard Includes ---
#include <d3d11.h>
#include <d3d10.h>
#include <d3d9.h>
#include <d3dx10math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "glext.h"
#include "wglext.h"

// --- Forward Class Definitions ---

// --- Using Namespace Declarations ---


class GlDxInterop
{
	// ---------- CREATORS ----------
	public:
		GlDxInterop(void);
		virtual ~GlDxInterop(void);

	// ---------- FUNCTIONS ----------
	public:
		bool Initialize(ID3D11Device * in_device);
		bool Initialize(IDirect3DDevice9Ex * in_device);
		bool RegisterDXLeftColorBuffer(void * in_buffer, GLenum in_textureType);
		bool RegisterDXRightColorBuffer(void * in_buffer, GLenum in_textureType);
		bool RegisterDXDepthBuffer(void * in_buffer, GLenum in_textureType);
		void BeginGLDraw();
		void EndGLDraw();		

		// --- Getters ---
		char * GetLastError() { return m_lastError; }

		// --- Setters ---

	protected:
		bool InitializeInterop(void * in_device);
		bool LoadWglFunctions();

	// ---------- VARIABLES ----------
	protected:
		char m_lastError[512];
		HANDLE m_glD3DHandle;
		HANDLE m_glD3DSharedLeftColorHandle;
		HANDLE m_glD3DSharedRightColorHandle;
		HANDLE m_glD3DSharedDepthHandle;
		GLuint m_glFbo;
		GLuint m_glLeftColorBuffer;
		GLuint m_glRightColorBuffer;
		GLuint m_glDepthBuffer;

		// OpenGL extension functions
		PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
		PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
		PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
		PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
		PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
		PFNWGLDXOPENDEVICENVPROC wglDXOpenDeviceNV;
		PFNWGLDXCLOSEDEVICENVPROC wglDXCloseDeviceNV;
		PFNWGLDXREGISTEROBJECTNVPROC wglDXRegisterObjectNV;
		PFNWGLDXUNREGISTEROBJECTNVPROC wglDXUnregisterObjectNV;
		PFNWGLDXLOCKOBJECTSNVPROC wglDXLockObjectsNV;
		PFNWGLDXUNLOCKOBJECTSNVPROC wglDXUnlockObjectsNV;
};

