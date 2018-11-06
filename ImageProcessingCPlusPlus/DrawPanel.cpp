#include "pch.h"
#include "DrawPanel.h"

using namespace Gdiplus;

namespace {
    const wchar_t WINDOW_CLASS[] = L"DrawPanelClass";
    const DWORD WINDOW_STYLE = WS_POPUPWINDOW | WS_CAPTION;

    // Computes the required size of a window to fit the size of a certain client area.
    std::pair<int, int> AdjustWindowSize(int width, int height) {
        RECT rect{ 0 };
        rect.right = width;
        rect.bottom = height;

        AdjustWindowRect(&rect, WINDOW_STYLE, false);
        return std::make_pair(rect.right - rect.left, rect.bottom - rect.top);
    }
}

LRESULT CALLBACK DrawPanel::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    DrawPanel *drawPanel = reinterpret_cast<DrawPanel*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
        // Remember "this" pointer for subsequent messages.
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
        return 0;

    case WM_PAINT: {
        HDC          hdc;
        PAINTSTRUCT  ps;

        hdc = BeginPaint(hwnd, &ps);
        drawPanel->OnPaint(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CHAR: // Press ESC to close the window.
        if (wparam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
    return 0;
}

DrawPanel::DrawPanel() : DrawPanel(nullptr)
{
}

DrawPanel::DrawPanel(Gdiplus::Image * image) : image_(image)
{
    RegisterWindowClassOnce();

    std::pair<int, int> size{ CW_USEDEFAULT, CW_USEDEFAULT };
    if (image != nullptr) {
        size = AdjustWindowSize(image->GetWidth(), image->GetHeight());
    }
    hwnd_ = CreateWindow(WINDOW_CLASS,
        NULL,
        WINDOW_STYLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        size.first,
        size.second,
        NULL,
        NULL,
        NULL,
        this);
    ShowWindow(hwnd_, SW_RESTORE);
}

DrawPanel::~DrawPanel()
{
    DeleteObject(hwnd_);
}

void DrawPanel::SetImage(Image * image)
{
    image_ = image;
    if (image != nullptr) {
        std::pair<int, int> size = AdjustWindowSize(image->GetWidth(), image->GetHeight());
        // Resize the window to the new image's dimensions.
        SetWindowPos(hwnd_, NULL, 0, 0, size.first, size.second, SWP_NOMOVE | SWP_NOZORDER);
    }
}

WPARAM DrawPanel::MessageLoop()
{
    MSG messages;
    while (GetMessage(&messages, NULL, 0, 0) > 0)
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}

void DrawPanel::OnPaint(HDC hdc)
{
    if (image_ != nullptr) {
        Graphics graphics(hdc);
        graphics.DrawImage(image_, 0, 0);
    }
}

bool DrawPanel::RegisterWindowClassOnce()
{
    // Using static variable initialization to ensure this piece of code is only executed once.
    // See:
    //  https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables
    //  https://en.cppreference.com/w/cpp/language/lambda
    static bool initialized = []() {
        WNDCLASS windowClass = { 0 };
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        windowClass.hInstance = NULL;
        windowClass.lpfnWndProc = WndProc;
        windowClass.lpszClassName = WINDOW_CLASS;
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        if (!RegisterClass(&windowClass)) {
            MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
            return false;
        }
        return true;
    }();
    return initialized;
}
