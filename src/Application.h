#pragma once
#include "GameField.h"
#include "DxRenderer.h"

class Application final
{
private:
    DxRenderer _renderer;
    HWND _handle;

private:
    Application();

public:
    ~Application();

    static Application& getSingleton();

    bool isKeyDown(const int32_t& key);
    static void Shutdown();

    int run(HINSTANCE hInstance, const int& nCmdShow);
    LRESULT WINAPI WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
