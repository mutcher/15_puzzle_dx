#pragma once
#include "DxRenderer.h"
#include "TGATexture.h"

class MainMenuSceneRenderer
{
private:
    DxRenderer* _renderer;
    TGATexture _texture;

public:
    MainMenuSceneRenderer(DxRenderer* renderer);

    void init();
    void render();
};
