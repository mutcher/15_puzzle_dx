#pragma once
#include "DxRenderer.h"

class MainMenuSceneRenderer
{
private:
    DxRenderer* _renderer;

private:
    void loadTexture()
    {
    }

public:
    MainMenuSceneRenderer(DxRenderer* renderer)
        :_renderer(renderer)
    {
    }
};
