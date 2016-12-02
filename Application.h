#pragma once
#include "GameField.h"
#include "DxRenderer.h"

class Application final
{
private:
    GameField _gameField;
    DxRenderer _renderer;
    HWND _handle;
    uint16_t _stepCount;

private:
    void update();
    bool isKeyDown(const int32_t& key);
    void printWindowCaption();

public:
    Application();
    ~Application();

    int run(HINSTANCE hInstance, const int& nCmdShow);
};
