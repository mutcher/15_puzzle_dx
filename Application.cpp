#include "Application.h"
#include <sstream>
#include <memory>
#include "GameScene.h"
#include "SceneManager.h"

Application::Application()
    :_renderer(500, 500), _handle()
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
    wc.lpfnWndProc = DefWindowProc;
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);

    int ret = RegisterClassEx(&wc);

    _handle = CreateWindowEx(0, "WND", "15 PUZZLE", WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);
    _renderer.init(_handle);

    std::unique_ptr<GameScene> gameScene(new GameScene(&_renderer));
    SceneManager::getSingleton().setActiveScene(gameScene.get());


    ShowWindow(_handle, nCmdShow);

    MSG msg = {0};
    size_t frameCounter = 0;

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

void Application::printWindowCaption()
{
    /*std::stringstream ss;
    ss << "15 PUZZLE Step:";
    ss << _stepCount;
    if (_gameField.isGameCompleted())
    {
        ss << " YOU WIN!";
    }

    SetWindowText(_handle, ss.str().c_str());*/
}