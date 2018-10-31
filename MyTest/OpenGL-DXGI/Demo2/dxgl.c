#define COBJMACROS
#define INITGUID
#include <intrin.h>
#include <windows.h>
#include <d3d11.h>
#include <gl/GL.h>

#include "glext.h" // https://www.opengl.org/registry/api/GL/glext.h
#include "wglext.h" // https://www.opengl.org/registry/api/GL/wglext.h

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "opengl32.lib")

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

        PIXELFORMATDESCRIPTOR pfd =
        {
            .nSize = sizeof(pfd),
            .nVersion = 1,
            .dwFlags = PFD_SUPPORT_OPENGL,
            .iPixelType = PFD_TYPE_RGBA,
            .iLayerType = PFD_MAIN_PLANE,
        };

        int format = ChoosePixelFormat(tempdc, &pfd);
        Assert(format);

        DescribePixelFormat(tempdc, format, sizeof(pfd), &pfd);
        BOOL set = SetPixelFormat(tempdc, format, &pfd);
        Assert(set);

        temprc = wglCreateContext(tempdc);
        Assert(temprc);

        BOOL make = wglMakeCurrent(tempdc, temprc);
        Assert(make);

        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (void*)wglGetProcAddress("wglCreateContextAttribsARB");

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

        PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = (void*)wglGetProcAddress("glDebugMessageCallback");
        glDebugMessageCallback(DebugCallback, 0);

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    wglDXOpenDeviceNV_ = (void*)wglGetProcAddress("wglDXOpenDeviceNV");
    wglDXCloseDeviceNV_ = (void*)wglGetProcAddress("wglDXCloseDeviceNV");

    wglDXRegisterObjectNV_ = (void*)wglGetProcAddress("wglDXRegisterObjectNV");
    wglDXUnregisterObjectNV_ = (void*)wglGetProcAddress("wglDXUnregisterObjectNV");

    wglDXLockObjectsNV_ = (void*)wglGetProcAddress("wglDXLockObjectsNV");
    wglDXUnlockObjectsNV_ = (void*)wglGetProcAddress("wglDXUnlockObjectsNV");

    glGenFramebuffers_ = (void*)wglGetProcAddress("glGenFramebuffers");
    glDeleteFramebuffers_ = (void*)wglGetProcAddress("glDeleteFramebuffers");

    glGenRenderbuffers_ = (void*)wglGetProcAddress("glGenRenderbuffers");
    glDeleteRenderbuffers_ = (void*)wglGetProcAddress("glDeleteRenderbuffers");

    glBindFramebuffer_ = (void*)wglGetProcAddress("glBindFramebuffer");
    glFramebufferRenderbuffer_ = (void*)wglGetProcAddress("glFramebufferRenderbuffer");

    RECT rect;
    GetClientRect(window, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    DXGI_SWAP_CHAIN_DESC desc =
    {
        .BufferDesc =
        {
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .RefreshRate =
            {
                .Numerator = 60,
                .Denominator = 1,
            },
        },
        .SampleDesc =
        {
            .Count = 1,
            .Quality = 0,
        },
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 1,
        .OutputWindow = window,
        .Windowed = TRUE,
        .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
        .Flags = 0,
    };

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

static void Destroy()
{
    ID3D11DeviceContext_ClearState(context);

    wglDXUnregisterObjectNV_(dxDevice, dxColor);
    wglDXUnregisterObjectNV_(dxDevice, dxDepthStencil);

    glDeleteFramebuffers_(1, &fbuf);
    glDeleteRenderbuffers_(1, &colorRbuf);
    glDeleteRenderbuffers_(1, &dsRbuf);

    wglDXCloseDeviceNV_(dxDevice);

    wglMakeCurrent(tempdc, NULL);
    wglDeleteContext(temprc);
    ReleaseDC(temp, tempdc);

    ID3D11RenderTargetView_Release(colorView);
    ID3D11DepthStencilView_Release(dsView);
    ID3D11DeviceContext_Release(context);
    ID3D11Device_Release(device);
    IDXGISwapChain_Release(swapChain);
}

static void Resize(int width, int height)
{
    HRESULT hr;

    if (colorView)
    {
        wglDXUnregisterObjectNV_(dxDevice, dxColor);
        wglDXUnregisterObjectNV_(dxDevice, dxDepthStencil);

        ID3D11DeviceContext_OMSetRenderTargets(context, 0, NULL, NULL);
        ID3D11RenderTargetView_Release(colorView);
        ID3D11DepthStencilView_Release(dsView);

        hr = IDXGISwapChain_ResizeBuffers(swapChain, 1, width, height, DXGI_FORMAT_UNKNOWN, 0);
        AssertHR(hr);
    }

    ID3D11Texture2D* colorBuffer;
    hr = IDXGISwapChain_GetBuffer(swapChain, 0, &IID_ID3D11Texture2D, &colorBuffer);
    AssertHR(hr);

    hr = ID3D11Device_CreateRenderTargetView(device, (void*)colorBuffer, NULL, &colorView);
    AssertHR(hr);

    D3D11_TEXTURE2D_DESC desc =
    {
        .Width = width,
        .Height = height,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc =
        {
            .Count = 1,
            .Quality = 0,
        },
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_DEPTH_STENCIL,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
    };

    ID3D11Texture2D* dsBuffer;
    hr = ID3D11Device_CreateTexture2D(device, &desc, NULL, &dsBuffer);
    AssertHR(hr);

    hr = ID3D11Device_CreateDepthStencilView(device, (void*)dsBuffer, NULL, &dsView);
    AssertHR(hr);

    dxColor = wglDXRegisterObjectNV_(dxDevice, colorBuffer, colorRbuf, GL_RENDERBUFFER, WGL_ACCESS_READ_WRITE_NV);
    Assert(dxColor);

    dxDepthStencil = wglDXRegisterObjectNV_(dxDevice, dsBuffer, dsRbuf, GL_RENDERBUFFER, WGL_ACCESS_READ_WRITE_NV);
    Assert(dxDepthStencil);

    ID3D11Texture2D_Release(colorBuffer);
    ID3D11Texture2D_Release(dsBuffer);

    D3D11_VIEWPORT view =
    {
        .TopLeftX = 0.f,
        .TopLeftY = 0.f,
        .Width = (float)width,
        .Height = (float)height,
        .MinDepth = 0.f,
        .MaxDepth = 1.f,
    };
    ID3D11DeviceContext_RSSetViewports(context, 1, &view);

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
    WNDCLASSA wc =
    {
        .lpfnWndProc = WindowProc,
        .lpszClassName = "DXGL",
    };

    ATOM atom = RegisterClassA(&wc);
    Assert(atom);

    HWND window = CreateWindowA(wc.lpszClassName, "DXGL",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
    Assert(window);

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
            ID3D11DeviceContext_OMSetRenderTargets(context, 1, &colorView, dsView);
            ID3D11DeviceContext_ClearRenderTargetView(context, colorView, cornflowerBlue);
            ID3D11DeviceContext_ClearDepthStencilView(context, dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0, 0);
        }

        HANDLE dxObjects[] = { dxColor, dxDepthStencil };
        wglDXLockObjectsNV_(dxDevice, _countof(dxObjects), dxObjects);

        // render with GL
        {
            glBindFramebuffer_(GL_FRAMEBUFFER, fbuf);

            glBegin(GL_TRIANGLES);
            glColor3f(1, 0, 0);
            glVertex2f(0.f, -0.5f);
            glColor3f(0, 1, 0);
            glVertex2f(0.5f, 0.5f);
            glColor3f(0, 0, 1);
            glVertex2f(-0.5f, 0.5f);
            glEnd();

            glBindFramebuffer_(GL_FRAMEBUFFER, fbuf);
        }

        wglDXUnlockObjectsNV_(dxDevice, _countof(dxObjects), dxObjects);

        HRESULT hr = IDXGISwapChain_Present(swapChain, 1, 0);
        Assert(SUCCEEDED(hr));
    }
}
