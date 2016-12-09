#include "MainMenuSceneRenderer.h"

struct vertex_t
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};

MainMenuSceneRenderer::MainMenuSceneRenderer(DxRenderer* renderer)
    :_renderer(renderer), _texture()
{
}

void MainMenuSceneRenderer::init()
{
    _texture.load("data/menu.tga", _renderer->getDevice());

    //shader initialization
    auto data = _renderer->readFile(L"menu_vs.cso");
    
    const uint8_t inputLayoutCount = 2;
    D3D11_INPUT_ELEMENT_DESC inputLayout[inputLayoutCount];
    
    inputLayout[0].AlignedByteOffset = 0;
    inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputLayout[0].InputSlot = 0;
    inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayout[0].InstanceDataStepRate = 0;
    inputLayout[0].SemanticIndex = 0;
    inputLayout[0].SemanticName = "POSITION";

    inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputLayout[1].InputSlot = 0;
    inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayout[1].InstanceDataStepRate = 0;
    inputLayout[1].SemanticIndex = 0;
    inputLayout[1].SemanticName = "TEXCOORD";

    HRESULT hr = _renderer->getDevice()->CreateInputLayout(inputLayout, inputLayoutCount, data.data(), data.size(), _inputLayout.getpp());
    hr = _renderer->getDevice()->CreateVertexShader(data.data(), data.size(), nullptr, _vertexShader.getpp());
    
    data = _renderer->readFile(L"menu_ps.cso");
    hr = _renderer->getDevice()->CreatePixelShader(data.data(), data.size(), nullptr, _pixelShader.getpp());
}

void MainMenuSceneRenderer::startup()
{
}

void MainMenuSceneRenderer::render()
{
    _renderer->beginRender();

    _renderer->endRender();
}