#pragma once
#include "DxRenderer.h"
#include "TGATexture.h"

class MainMenuSceneRenderer
{
private:
    DxRenderer* _renderer;
    TGATexture _texture;

    dx_ptr<ID3D11InputLayout> _inputLayout;
    dx_ptr<ID3D11VertexShader> _vertexShader;
    dx_ptr<ID3D11PixelShader> _pixelShader;

public:
    MainMenuSceneRenderer(DxRenderer* renderer);

    void init();
    void startup();
    void render();
};
