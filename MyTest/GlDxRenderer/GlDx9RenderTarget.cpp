#include "GlDx9RenderTarget.h"

// --- This File Includes ---
#include "GlDxInterop.h"

// --- Using Namespace Declarations ---



// Stereo Blitdefines
#define NVSTEREO_IMAGE_SIGNATURE 0x4433564e //NV3D
typedef struct _Nv_Stereo_Image_Header
{
unsigned int dwSignature;
unsigned int dwWidth;
unsigned int dwHeight;
unsigned int dwBPP;
unsigned int dwFlags;
} NVSTEREOIMAGEHEADER, *LPNVSTEREOIMAGEHEADER;
// ORedflags in the dwFlagsfielsof the _Nv_Stereo_Image_Headerstructure above
#define SIH_SWAP_EYES 0x00000001
#define SIH_SCALE_TO_FIT 0x00000002
#define SIH_SCALE_TO_FIT2 0x00000004


/// <summary>
/// </summary>
GlDx9RenderTarget::GlDx9RenderTarget(void)
: IGlDxRenderTarget()
{
	m_dxLeftColorBuffer = NULL;
	m_dxRightColorBuffer = NULL;
	m_dxLeftColorTexture = NULL;
	m_dxRightColorTexture = NULL;
	m_dxRenderedBuffer = NULL;
	m_dxDepthBuffer = NULL;
	m_device = NULL;
	m_direct3D = NULL;
	m_isStereo = false;
}


/// <summary>
/// </summary>
GlDx9RenderTarget::~GlDx9RenderTarget(void)
{
	Cleanup();
}
/// <summary>
/// </summary>
void GlDx9RenderTarget::Cleanup()
{
    if (m_dxLeftColorTexture) 
        m_dxLeftColorTexture->Release();
    if (m_dxRightColorTexture) 
        m_dxRightColorTexture->Release();
	if (m_dxLeftColorBuffer) 
        m_dxLeftColorBuffer->Release();
    if (m_dxRightColorBuffer) 
        m_dxRightColorBuffer->Release();
	if (m_dxRenderedBuffer) 
        m_dxRenderedBuffer->Release();
    if (m_dxDepthBuffer) 
        m_dxDepthBuffer->Release();
	if (m_device)
		m_device->Release();
	if (m_direct3D)
		m_direct3D->Release();

	m_dxLeftColorTexture = NULL;
	m_dxRightColorTexture = NULL;
	m_dxLeftColorBuffer = NULL;
	m_dxRightColorBuffer = NULL;
	m_dxRenderedBuffer = NULL;
	m_dxDepthBuffer = NULL;
	m_device = NULL;
	m_direct3D = NULL;

	IGlDxRenderTarget::Cleanup();
}

/// <summary>
/// Initialize the DirectX to OpenGL interop with the appropriate
/// left and right textures, along with the depth buffer.
/// </summary>
/// <returns>False if initialization was unsuccessful.  Check
/// m_lastError for more details.</returns>
bool GlDx9RenderTarget::InitializeInterop()
{
	strcpy(m_lastError, "");

	// Create the interop
	m_interop = new GlDxInterop();

	// Initialize the interop
	if(m_interop->Initialize(m_device) == false)
	{
		strcpy(m_lastError, m_interop->GetLastError());
		return false;
	}

	// Register the buffers
	if(m_interop->RegisterDXLeftColorBuffer(m_dxLeftColorTexture, GL_TEXTURE_2D) == false)
	{
		strcpy(m_lastError, m_interop->GetLastError());
		return false;
	}
	if(m_interop->RegisterDXRightColorBuffer(m_dxRightColorTexture, GL_TEXTURE_2D) == false)
	{
		strcpy(m_lastError, m_interop->GetLastError());
		return false;
	}
	if(m_interop->RegisterDXDepthBuffer(m_dxDepthBuffer, GL_TEXTURE_RECTANGLE_NV) == false)
	{
		strcpy(m_lastError, m_interop->GetLastError());
		return false;
	}

	return true;
}

/// <summary>
/// Verify that the graphics device can indeed support the
/// formats as passed in.
/// </summary>
/// <param name="in_desiredColorFormat">The desired color pixel format.</param>
/// <param name="in_desiredDepthFormat">The desired depth stencil format.</param>
/// <returns>False if device does not support desired formats.  Check
/// m_lastError for more details.</returns>
bool GlDx9RenderTarget::VerifyDeviceCapabilities(D3DFORMAT in_desiredColorFormat, D3DFORMAT in_desiredDepthFormat)
{
	HRESULT result;
	D3DCAPS9 d3dCaps;
	strcpy(m_lastError, "");

	// Make sure the device can support the requested formats
	result =  m_direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, in_desiredColorFormat, 
											D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, in_desiredDepthFormat);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Graphics device does not support required display formats." );
		return false;
	}

	// Get the device capabilities
	result = m_direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not retrieve DirectX graphics device capabilities." );
		return false;
	}

	// Check to make sure we have hardware vertex processing
	if( d3dCaps.VertexProcessingCaps == 0 )
	{
		strcpy(m_lastError, "Graphics device does not support hardware vertex processing." );
		return false;
	}

	return true;
}

/// <summary>
/// </summary>
bool GlDx9RenderTarget::Initialize(HWND in_hwnd)
{
	HRESULT result;
	int width = 0;
	int height = 0;
	D3DDISPLAYMODE d3ddm;
	D3DMULTISAMPLE_TYPE sampleType = D3DMULTISAMPLE_NONE;

	// Clear the error
	strcpy(m_lastError, "");

	// Make sure we are as clean as a baby's bottom here
	Cleanup();

	// Create the directx sdk
	result = Direct3DCreate9Ex(D3D_SDK_VERSION, &m_direct3D);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not initialize the DirectX SDK.");
		return false;
	}

	// Assign the window handle
	m_hWnd = in_hwnd;

	// Get the adapter display mode - we'll use this for the display format
    result = m_direct3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not get the adapter display mode.");
		return false;
	}

	// Verify that the device will actually support our desired formats
	VerifyDeviceCapabilities(d3ddm.Format, D3DFMT_D24S8);

	// Get the size of the window
	RECT windowRect;
	GetWindowRect(in_hwnd, &windowRect);
	width = windowRect.right - windowRect.left;
	height = windowRect.bottom - windowRect.top;

	// Setup the device parameters
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.BackBufferWidth = width;
	m_d3dpp.BackBufferHeight = height;
	m_d3dpp.hDeviceWindow = in_hwnd;
	m_d3dpp.MultiSampleQuality = 0;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Windowed = TRUE;												// TODO: Add ability for fullscreen
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// Create the DirectX device
	result = m_direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, in_hwnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_MULTITHREADED,
							&m_d3dpp, (IDirect3DDevice9 **)&m_device);	
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not create the DirectX device.");
		return false;
	}

	// Create the LEFT rendering texture
	result = m_device->CreateTexture(width, height, 0, 0, d3ddm.Format, D3DPOOL_DEFAULT,
						&m_dxLeftColorTexture, NULL);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not create the left DirectX rendering texture.");
		return false;
	}

	// Create the RIGHT rendering texture
	result = m_device->CreateTexture(width, height, 0, 0, d3ddm.Format, D3DPOOL_DEFAULT,
						&m_dxRightColorTexture, NULL);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not create the right DirectX rendering buffer.");
		return false;
	}

	// Create a rendering target to put the left and right images on
	result = m_device->CreateRenderTarget(width * 2, height + 1, d3ddm.Format, sampleType, 0, TRUE,
							&m_dxRenderedBuffer, NULL);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not create the DirectX rendering buffer.");
		return false;
	}

	// Create the depth stencil
	result = m_device->CreateDepthStencilSurface(width, height, D3DFMT_D24S8, sampleType, 0, FALSE,
							&m_dxDepthBuffer, NULL);
	if(result != S_OK)
	{
		strcpy(m_lastError, "Could not create the DirectX depth stencil surface.");
		return false;
	}

	// Initialize the interop
	if(InitializeInterop() == false)
		return false;

	// Associate the texture to the surface
	m_dxLeftColorTexture->GetSurfaceLevel(0, &m_dxLeftColorBuffer);
	m_dxRightColorTexture->GetSurfaceLevel(0, &m_dxRightColorBuffer);

	return true;
}

/// <summary>
/// </summary>
void GlDx9RenderTarget::Flush()
{
	IDirect3DSurface9 * backBuffer = NULL;
	RECT srcRect;
	RECT destRect;
	int width = m_d3dpp.BackBufferWidth;
	int height = m_d3dpp.BackBufferHeight;


	// Get the back buffer
	m_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);	

	if(m_isStereo)
	{
		// Stretch to the left
		srcRect.left = 0;		srcRect.right = width;
		srcRect.top = 0;		srcRect.bottom = height;
		destRect.left = 0;		destRect.right = width;
		destRect.top = 0;		destRect.bottom = height;
		if(FAILED(m_device->StretchRect(m_dxLeftColorBuffer, &srcRect, m_dxRenderedBuffer, &destRect, D3DTEXF_LINEAR)))
		{
			return;
		}  

		// Stretch to the right
		srcRect.left = 0;			srcRect.right = width;
		srcRect.top = 0;			srcRect.bottom = height;
		destRect.left = width;	destRect.right = 2 * width;
		destRect.top = 0;			destRect.bottom = height;
		if(FAILED(m_device->StretchRect(m_dxRightColorBuffer, &srcRect,m_dxRenderedBuffer, &destRect, D3DTEXF_LINEAR)))
		{
			return;
		}  

		// Lock the image
		D3DLOCKED_RECT lr;
		m_dxRenderedBuffer->LockRect(&lr, NULL, 0);

		// Put in the special stereo image header
		LPNVSTEREOIMAGEHEADER pSIH = (LPNVSTEREOIMAGEHEADER)(((unsigned char *) lr.pBits) + (lr.Pitch * height));
		pSIH->dwSignature = NVSTEREO_IMAGE_SIGNATURE;
		pSIH->dwBPP = 32;
		pSIH->dwFlags = SIH_SCALE_TO_FIT | SIH_SCALE_TO_FIT2; // Srcimage has left on left and right on right
		pSIH->dwWidth = width * 2;
		pSIH->dwHeight = height;

		// Unlock surface
		m_dxRenderedBuffer->UnlockRect();

		// Stretch to the back buffer
		srcRect.left = 0;		srcRect.right = width * 2;
		srcRect.top = 0;		srcRect.bottom = height;
		destRect.left = 0;		destRect.right = width;
		destRect.top = 0;		destRect.bottom = height;
		if(FAILED(m_device->StretchRect(m_dxRenderedBuffer, &srcRect, backBuffer, &destRect, D3DTEXF_LINEAR)))
		{
			return;
		} 
	}
	else
	{
		// Just stretch rect the left image to the back buffer
		srcRect.left = 0;		srcRect.right = width;
		srcRect.top = 0;		srcRect.bottom = height;
		destRect.left = 0;		destRect.right = width;
		destRect.top = 0;		destRect.bottom = height;
		if(FAILED(m_device->StretchRect(m_dxLeftColorBuffer, &srcRect, backBuffer, &destRect, D3DTEXF_LINEAR)))
		{
			return;
		} 
	}

	// Show the back buffer
    m_device->Present( NULL, NULL, NULL, NULL );

	// Cleanup
    if (backBuffer) 
        backBuffer->Release();
}