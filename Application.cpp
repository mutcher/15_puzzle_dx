#include "Application.h"
#include "GameScene.h"
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

    _handle = CreateWindowEx(0, "WND", "15 PUZZLE", WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

    RECT clientRect = {0};
    GetClientRect(_handle, &clientRect);

    _renderer.init(_handle, clientRect.right, clientRect.bottom);

    SceneManager::getSingleton().setActiveScene(new GameScene(&_renderer));

    ShowWindow(_handle, nCmdShow);

    MSG msg = {0};
    size_t frameCounter = 0;

    SetWindowLongPtr(_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    while(true)
    {
        if (!IsWindow(_handle))
        {
            break;
        }

        if (PeekMessage(&msg, _handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (frameCounter == 10)
            {
                SceneManager::getSingleton().getActiveScene()->update();
                frameCounter = 0;
            }
            SceneManager::getSingleton().getActiveScene()->render();
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
        return 0;

    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
}