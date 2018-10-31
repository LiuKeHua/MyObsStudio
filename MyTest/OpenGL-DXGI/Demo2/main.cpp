#define COBJMACROS
#define INITGUID

#include <dxgi1_3.h>

#include <comdef.h>


#include <intrin.h>
#include <windows.h>
#include <d3d11.h>
#include <gl/GL.h>

#include "glext.h" // https://www.opengl.org/registry/api/GL/glext.h
#include "wglext.h" // https://www.opengl.org/registry/api/GL/wglext.h



#define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)
#define AssertHR(hr) Assert(SUCCEEDED(hr))

static GLuint colorRbuf;
static GLuint dsRbuf;
static GLuint fbuf;

static HANDLE dxDevice;
static HANDLE dxColor;
static HANDLE dxDepthStencil;

static ID3D11Device* device;
static ID3D11DeviceContext* context;
static IDXGISwapChain* swapChain;
static ID3D11RenderTargetView* colorView;
static ID3D11DepthStencilView* dsView;

static PFNWGLDXOPENDEVICENVPROC wglDXOpenDeviceNV_;
static PFNWGLDXCLOSEDEVICENVPROC wglDXCloseDeviceNV_;
static PFNWGLDXREGISTEROBJECTNVPROC wglDXRegisterObjectNV_;
static PFNWGLDXUNREGISTEROBJECTNVPROC wglDXUnregisterObjectNV_;
static PFNWGLDXLOCKOBJECTSNVPROC wglDXLockObjectsNV_;
static PFNWGLDXUNLOCKOBJECTSNVPROC wglDXUnlockObjectsNV_;

static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers_;
static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers_;
static PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers_;
static PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers_;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer_;
static PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer_;

static HWND temp;
static HDC tempdc;
static HGLRC temprc;

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
}

static void Create(HWND window)
{
    // GL context on temporary window, no drawing will happen to this window
    {
        temp = CreateWindowA("STATIC", "temp", WS_OVERLAPPED,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, NULL, NULL);
        Assert(temp);

        tempdc = GetDC(temp);
        Assert(tempdc);

		PIXELFORMATDESCRIPTOR pfd;
        
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_SUPPORT_OPENGL;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;
       

        int format = ChoosePixelFormat(tempdc, &pfd);
        Assert(format);

        DescribePixelFormat(tempdc, format, sizeof(pfd), &pfd);
        BOOL set = SetPixelFormat(tempdc, format, &pfd);
        Assert(set);

        temprc = wglCreateContext(tempdc);
        Assert(temprc);

        BOOL make = wglMakeCurrent(tempdc, temprc);
        Assert(make);

        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        int attrib[] =
        {
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
            0,
        };

        HGLRC newrc = wglCreateContextAttribsARB(tempdc, NULL, attrib);
        Assert(newrc);

        make = wglMakeCurrent(tempdc, newrc);
        Assert(make);

        wglDeleteContext(temprc);
        temprc = newrc;

        PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");
        glDebugMessageCallback(DebugCallback, 0);

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }


    wglDXOpenDeviceNV_ = (PFNWGLDXOPENDEVICENVPROC)wglGetProcAddress("wglDXOpenDeviceNV");
    wglDXCloseDeviceNV_ = (PFNWGLDXCLOSEDEVICENVPROC)wglGetProcAddress("wglDXCloseDeviceNV");

    wglDXRegisterObjectNV_ = (PFNWGLDXREGISTEROBJECTNVPROC)wglGetProcAddress("wglDXRegisterObjectNV");
    wglDXUnregisterObjectNV_ = (PFNWGLDXUNREGISTEROBJECTNVPROC)wglGetProcAddress("wglDXUnregisterObjectNV");

    wglDXLockObjectsNV_ = (PFNWGLDXLOCKOBJECTSNVPROC)wglGetProcAddress("wglDXLockObjectsNV");
    wglDXUnlockObjectsNV_ = (PFNWGLDXUNLOCKOBJECTSNVPROC)wglGetProcAddress("wglDXUnlockObjectsNV");

    glGenFramebuffers_ = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
    glDeleteFramebuffers_ = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");

    glGenRenderbuffers_ = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
    glDeleteRenderbuffers_ = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");

    glBindFramebuffer_ = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
    glFramebufferRenderbuffer_ = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");

    RECT rect;
    GetClientRect(window, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;


	DXGI_SWAP_CHAIN_DESC desc = { 0 };
	
	
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = window;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;
	
	

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0,
        D3D11_SDK_VERSION, &desc, &swapChain, &device, NULL, &context);
    AssertHR(hr);

    dxDevice = wglDXOpenDeviceNV_(device);
    Assert(dxDevice);

    glGenRenderbuffers_(1, &colorRbuf);
    glGenRenderbuffers_(1, &dsRbuf);
    glGenFramebuffers_(1, &fbuf);
    glBindFramebuffer_(GL_FRAMEBUFFER, fbuf);
}

void Destroy()
{
	context->ClearState();

    wglDXUnregisterObjectNV_(dxDevice, dxColor);
    wglDXUnregisterObjectNV_(dxDevice, dxDepthStencil);

    glDeleteFramebuffers_(1, &fbuf);
    glDeleteRenderbuffers_(1, &colorRbuf);
    glDeleteRenderbuffers_(1, &dsRbuf);

    wglDXCloseDeviceNV_(dxDevice);

    wglMakeCurrent(tempdc, NULL);
    wglDeleteContext(temprc);
    ReleaseDC(temp, tempdc);

	colorView->Release();
	dsView->Release();
	context->Release();
	device->Release();
	swapChain->Release();
}

 void Resize(int width, int height)
{
    HRESULT hr;

    if (colorView)
    {
        wglDXUnregisterObjectNV_(dxDevice, dxColor);
        wglDXUnregisterObjectNV_(dxDevice, dxDepthStencil);

		context->OMSetRenderTargets(0, NULL, NULL);
		colorView->Release();
		dsView->Release();

		hr = swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, 0);
        AssertHR(hr);
    }

    ID3D11Texture2D* colorBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&colorBuffer);
    AssertHR(hr);

	hr = device->CreateRenderTargetView(colorBuffer, NULL, &colorView);

    AssertHR(hr);

	D3D11_TEXTURE2D_DESC desc;

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	

    ID3D11Texture2D* dsBuffer;
	hr = device->CreateTexture2D(&desc, NULL, &dsBuffer);

    AssertHR(hr);

    hr = device->CreateDepthStencilView(dsBuffer, NULL, &dsView);
    AssertHR(hr);

    dxColor = wglDXRegisterObjectNV_(dxDevice, colorBuffer, colorRbuf, GL_RENDERBUFFER, WGL_ACCESS_READ_WRITE_NV);
    Assert(dxColor);

    dxDepthStencil = wglDXRegisterObjectNV_(dxDevice, dsBuffer, dsRbuf, GL_RENDERBUFFER, WGL_ACCESS_READ_WRITE_NV);
    Assert(dxDepthStencil);

	colorBuffer->Release();
	dsBuffer->Release();

	D3D11_VIEWPORT view;

	view.TopLeftX = 0.f;
	view.TopLeftY = 0.f;
	view.Width = (float)width;
	view.Height = (float)height;
	view.MinDepth = 0.f;
	view.MaxDepth = 1.f;
    
	context->RSSetViewports(1, &view);


    glViewport(0, 0, width, height);
    glFramebufferRenderbuffer_(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbuf);
    glFramebufferRenderbuffer_(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dsRbuf);
    glFramebufferRenderbuffer_(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dsRbuf);
}

static LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
        Create(window);
        return 0;

    case WM_DESTROY:
        Destroy();
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
        Resize(LOWORD(lparam), HIWORD(lparam));
        return 0;
    }

    return DefWindowProcA(window, msg, wparam, lparam);
}

int main()
{
	WNDCLASSA wc;
    
	memset(&wc, 0, sizeof(wc));
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = "DXGL";
	wc.hInstance = GetModuleHandle(NULL);
	wc.style = CS_OWNDC;

    ATOM atom = RegisterClassA(&wc);
    Assert(atom);

    HWND window = CreateWindowA(wc.lpszClassName, "DXGL",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
    Assert(window);

	float delta = -0.5f;
	bool shouldAdd = true;

    int running = 1;
    for (;;)
    {
        MSG msg;
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = 0;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if (!running)
        {
            break;
        }

        // render with D3D
        {
            FLOAT cornflowerBlue[] = { 100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 1.f };
			context->OMSetRenderTargets(1, &colorView, dsView);

			context->ClearRenderTargetView(colorView, cornflowerBlue);
			context->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0, 0);

        }

        HANDLE dxObjects[] = { dxColor, dxDepthStencil };
        wglDXLockObjectsNV_(dxDevice, _countof(dxObjects), dxObjects);

        // render with GL
        {
            glBindFramebuffer_(GL_FRAMEBUFFER, fbuf);

			if(shouldAdd)				
			{
				delta += 0.01f;
			}
			else
			{
				delta -= 0.01f;
			}

			if (delta > 0.5)
			{
				shouldAdd = false;
				delta = 0.5f;
			}
			else if (delta < -0.5)
			{
				shouldAdd = true;
				delta = -0.5f;
			}

            glBegin(GL_TRIANGLES);
				glColor3f(1, 0, 0);
				glVertex2f(0.f + delta, -0.5f);
				glColor3f(0, 1, 0);
				glVertex2f(0.5f + delta, 0.5f);
				glColor3f(0, 0, 1);
				glVertex2f(-0.5f + delta, 0.5f);
            glEnd();
        }

        wglDXUnlockObjectsNV_(dxDevice, _countof(dxObjects), dxObjects);

		HRESULT hr = swapChain->Present(1, 0);
       
        Assert(SUCCEEDED(hr));
    }
}
