#include "Application.h"
#include "MainMenuScene.h"
#include "SceneManager.h"

LRESULT WINAPI fakeWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    Application* app = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (app != nullptr)
    {
        return app->WindowProc(hWnd, Msg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
}

Application::Application()
    :_renderer(), _handle()
{
}

Application::~Application()
{
}


int Application::run(HINSTANCE hInstance, const int& nCmdShow)
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpszClassName = "WND";
    wc.hInstance = hInstance;
    wc.lpfnWndProc = fakeWindowProc;
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);

    int ret = RegisterClassEx(&wc);

    RECT wndRect;
    wndRect.right = 500;
    wndRect.bottom = 500;
    BOOL b = AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);
    assert(b);

    _handle = CreateWindowEx(0, "WND", "15 PUZZLE", WS_OVERLAPPEDWINDOW, 0, 0, wndRect.right, wndRect.bottom, nullptr, nullptr, hInstance, nullptr);

    _renderer.init(_handle, 500, 500);

    SceneManager::getSingleton().setActiveScene(new MainMenuScene(&_renderer));

    ShowWindow(_handle, nCmdShow);

    MSG msg = {0};
    size_t frameCounter = 0;

    SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    while(true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            SceneManager& sceneMgr = SceneManager::getSingleton();
            sceneMgr.update();

            if (frameCounter == 10)
            {
                sceneMgr.getActiveScene()->update();
                frameCounter = 0;
            }
            sceneMgr.getActiveScene()->render();
            ++frameCounter;
        }
    }

    ret = UnregisterClass("WND", hInstance);

    return 0;
}

LRESULT WINAPI Application::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
    case WM_SIZE:
        {
            uint32_t width = LOWORD(lParam);
            uint32_t height = HIWORD(lParam);
            _renderer.resize(width, height);
        }
        return 0L;

    case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        return 0L;

    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
}

void Application::Shutdown()
{
    PostQuitMessage(0);
}
