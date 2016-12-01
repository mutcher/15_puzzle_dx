#include <Windows.h>
#include <thread>
#include <cstdint>
#include <sstream>
#include "DxRenderer.h"

uint16_t stepCount = 0;

bool isKeyDown(int key)
{
    return GetAsyncKeyState(key) != 0;
}

void printWindowLabel(const GameField& gameField, HWND handle)
{
    std::stringstream ss;
    ss << "15 PUZZLE Step:";
    ss << stepCount;
    if (gameField.isGameCompleted())
    {
        ss << " YOU WIN!";
    }

    SetWindowText(handle, ss.str().c_str());
}

void update(GameField& gameField, HWND handle)
{
    gameField.update();
    if (!gameField.isGameCompleted())
    {
        if (isKeyDown(VK_UP))
        {
            gameField.move(GameField::MoveDirection::up);
        }
        else if (isKeyDown(VK_DOWN))
        {
            gameField.move(GameField::MoveDirection::down);
        }
        else if (isKeyDown(VK_RIGHT))
        {
            gameField.move(GameField::MoveDirection::right);
        }
        else if (isKeyDown(VK_LEFT))
        {
            gameField.move(GameField::MoveDirection::left);
        }
        if (gameField.isFieldChanged())
        {
            ++stepCount;
        }
    }
#ifdef _DEBUG
    if (isKeyDown(VK_ESCAPE))
    {
        exit(0);
    }
#endif

    printWindowLabel(gameField, handle);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.lpszClassName = "WND";
    wc.hInstance = hInstance;
    wc.lpfnWndProc = DefWindowProc;
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);

    int ret = RegisterClassEx(&wc);

    HWND handle = CreateWindowEx(0, "WND", "15 PUZZLE", WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

    GameField gameField;
    DxRenderer renderer(500, 500);
    renderer.init(handle);

    ShowWindow(handle, nCmdShow);

    MSG msg = {0};
    size_t frameCounter = 0;

    while(true)
    {
        if (!IsWindow(handle))
        {
            break;
        }

        if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (frameCounter == 10)
            {
                update(gameField, handle);
                frameCounter = 0;
            }
            renderer.render(gameField);
            ++frameCounter;
        }
    }

    ret = UnregisterClass("WND", hInstance);

    return 0;
}