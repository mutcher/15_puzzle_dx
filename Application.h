#pragma once
#include "GameField.h"
#include "DxRenderer.h"

class Application final
{
private:
    DxRenderer _renderer;
    HWND _handle;

private:
    void printWindowCaption();

public:
    Application();
    ~Application();

    static inline bool isKeyDown(const int32_t& key)
    {
        return GetAsyncKeyState(key) != 0;
    }

    int run(HINSTANCE hInstance, const int& nCmdShow);
    LRESULT WINAPI WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
