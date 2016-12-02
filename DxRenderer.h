#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>

#include "dx_ptr.h"
#include "GameField.h"

class DxRenderer
{
private:
    HWND _handle;
    int _width;
    int _height;
    dx_ptr<IDXGISwapChain> _swapChain;
    dx_ptr<ID3D11Device> _device;
    dx_ptr<ID3D11DeviceContext> _context;
    dx_ptr<ID3D11RenderTargetView> _renderTargetView;
    dx_ptr<ID3D11VertexShader> _vertexShader;
    dx_ptr<ID3D11PixelShader> _pixelShader;
    dx_ptr<ID3D11InputLayout> _inputLayout;
    dx_ptr<ID3D11Buffer> _vertexBuffer;
    dx_ptr<ID3D11Buffer> _instanceBuffer;
    dx_ptr<ID3D11Texture2D> _texture;
    dx_ptr<ID3D11ShaderResourceView> _textureView;
    dx_ptr<ID3D11SamplerState> _samplerState;
    dx_ptr<ID3D11BlendState> _blendState;
    struct
    {
        uint16_t width;
        uint16_t height;
        uint8_t  bpp;
    } _textureInfo;

private:
    void createVertexBuffer();
    void createInstanceBuffer(const GameField& gameField);
    void loadTGATexture();

public:
    DxRenderer(int width, int height);
    ~DxRenderer() {}

    bool init(HWND handle);
    void render(const GameField& gameField);
};
