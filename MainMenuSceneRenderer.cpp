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

    auto* device = _renderer->getDevice();

    HRESULT hr = device->CreateInputLayout(inputLayout, inputLayoutCount, data.data(), data.size(), _inputLayout.getpp());
    hr = device->CreateVertexShader(data.data(), data.size(), nullptr, _vertexShader.getpp());
    
    data = _renderer->readFile(L"menu_ps.cso");
    hr = device->CreatePixelShader(data.data(), data.size(), nullptr, _pixelShader.getpp());

    //vertex buffer initialization
}

void MainMenuSceneRenderer::startup()
{
    _renderer->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _renderer->getContext()->IASetInputLayout(_inputLayout);
    _renderer->getContext()->PSSetShader(_pixelShader, nullptr, 0);
    _renderer->getContext()->VSSetShader(_vertexShader, nullptr, 0);

    // resource view
    const uint8_t viewCount = 1;
    ID3D11ShaderResourceView* resourceViews[viewCount];
    resourceViews[0] = _texture.getTextureView();
    _renderer->getContext()->PSSetShaderResources(0, viewCount, resourceViews);
}

void MainMenuSceneRenderer::render()
{
    _renderer->beginRender();

    _renderer->endRender();
}