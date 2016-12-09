#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <vector>

#include "dx_ptr.h"
#include "GameField.h"

class DxRenderer final
{
private:
    HWND _handle;
    uint32_t _width;
    uint32_t _height;
    dx_ptr<IDXGISwapChain> _swapChain;
    dx_ptr<ID3D11Device> _device;
    dx_ptr<ID3D11DeviceContext> _context;
    dx_ptr<ID3D11RenderTargetView> _renderTargetView;
    dx_ptr<ID3D11VertexShader> _vertexShader;
    dx_ptr<ID3D11PixelShader> _pixelShader;
    dx_ptr<ID3D11InputLayout> _inputLayout;
    dx_ptr<ID3D11SamplerState> _samplerState;
    dx_ptr<ID3D11BlendState> _blendState;

private:
    void initShaders();
    void initBlendState();
    void initSamplerState();
    void initBackBufferAndViewport();

public:
    DxRenderer();
    ~DxRenderer() {}

    bool init(HWND handle, const uint32_t& width, const uint32_t& height);
    void resize(const uint32_t& width, const uint32_t& height);

    void beginRender();
    void endRender();

    inline ID3D11Device* getDevice()
    {
        return _device;
    }

    inline ID3D11DeviceContext* getContext()
    {
        return _context;
    }

    static std::vector<uint8_t> readFile(const std::wstring& fileName);
};
