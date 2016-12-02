#include "Application.h"
#include <sstream>

Application::Application()
    :_gameField(), _renderer(500, 500), _handle(),
    _stepCount(0)
{
}

Application::~Application()
{
}

void Application::update()
{
    _gameField.update();
    if (!_gameField.isGameCompleted())
    {
        if (isKeyDown(VK_UP))
        {
            _gameField.move(GameField::MoveDirection::up);
        }
        else if (isKeyDown(VK_DOWN))
        {
            _gameField.move(GameField::MoveDirection::down);
        }
        else if (isKeyDown(VK_RIGHT))
        {
            _gameField.move(GameField::MoveDirection::right);
        }
        else if (isKeyDown(VK_LEFT))
        {
            _gameField.move(GameField::MoveDirection::left);
        }
        if (_gameField.isFieldChanged())
        {
            ++_stepCount;
        }
    }
#ifdef _DEBUG
    if (isKeyDown(VK_ESCAPE))
    {
        exit(0);
    }
#endif

    printWindowCaption();
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
                update();
                frameCounter = 0;
            }
            _renderer.render(_gameField);
            ++frameCounter;
        }
    }

    ret = UnregisterClass("WND", hInstance);

    return 0;
}

bool Application::isKeyDown(const int32_t& key)
{
    return GetAsyncKeyState(key) != 0;
}

void Application::printWindowCaption()
{
    std::stringstream ss;
    ss << "15 PUZZLE Step:";
    ss << _stepCount;
    if (_gameField.isGameCompleted())
    {
        ss << " YOU WIN!";
    }

    SetWindowText(_handle, ss.str().c_str());
}