#pragma once
#include "DxRenderer.h"
#include "TGATexture.h"

class GameSceneRenderer
{
private:
    DxRenderer* _renderer;
    GameField* _gameField;
    TGATexture _texture;
    dx_ptr<ID3D11Buffer> _vertexBuffer;
    dx_ptr<ID3D11Buffer> _instanceBuffer;
    dx_ptr<ID3D11VertexShader> _vertexShader;
    dx_ptr<ID3D11PixelShader> _pixelShader;
    dx_ptr<ID3D11InputLayout> _inputLayout;

private:
    void createShaders();
    void createVertexBuffer();
    void createInstanceBuffer();

public:
    GameSceneRenderer(DxRenderer* renderer, GameField* gameField);

    void startup();
    void init();
    void render();
};
