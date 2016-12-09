#pragma once
#include "DxRenderer.h"
#include "TGATexture.h"

class GameSceneRenderer
{
private:
    DxRenderer* _renderer;
    GameField* _gameField;
    dx_ptr<ID3D11Buffer> _vertexBuffer;
    dx_ptr<ID3D11Buffer> _instanceBuffer;
    TGATexture _texture;

private:
    void createVertexBuffer();
    void createInstanceBuffer();

public:
    GameSceneRenderer(DxRenderer* renderer, GameField* gameField);

    void init();
    void render();
};
