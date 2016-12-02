#pragma once
#include "DxRenderer.h"

class GameSceneRenderer
{
private:
    DxRenderer* _renderer;
    GameField* _gameField;
    dx_ptr<ID3D11Buffer> _vertexBuffer;
    dx_ptr<ID3D11Buffer> _instanceBuffer;
    dx_ptr<ID3D11Texture2D> _texture;
    dx_ptr<ID3D11ShaderResourceView> _textureView;
    struct
    {
        uint16_t width;
        uint16_t height;
        uint8_t  bpp;
    } _textureInfo;

private:
    void createVertexBuffer();
    void createInstanceBuffer();
    void loadTexture();

public:
    GameSceneRenderer(DxRenderer* renderer, GameField* gameField);

    void init();
    void render();
};
