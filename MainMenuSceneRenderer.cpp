#include "MainMenuSceneRenderer.h"

struct vertex_t
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};

MainMenuSceneRenderer::MainMenuSceneRenderer(DxRenderer* renderer)
    :_renderer(renderer), _texture(), _vertexBuffer(),
    _vertexShader(), _pixelShader(), _inputLayout()
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
    const auto& textureInfo = _texture.getTextureInfo();
    const uint8_t vertexCount = 24;
    vertex_t vertexes[vertexCount];
    struct
    {
        float x1;
        float y1;
        float x2;
        float y2;
    } textureBoxSize;

    // logo box
    textureBoxSize.x1 = 1.f / static_cast<float>(textureInfo.width);
    textureBoxSize.y1 = 1.f / static_cast<float>(textureInfo.height);
    textureBoxSize.x2 = (textureInfo.width - 1.f) / static_cast<float>(textureInfo.width);
    textureBoxSize.y2 = 96.f / static_cast<float>(textureInfo.height);

    vertexes[0].pos = DirectX::XMFLOAT3(-.95f, 1.f, .5f);
    vertexes[0].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y1);
    
    vertexes[1].pos = DirectX::XMFLOAT3(.95f, 1.f, .5f);
    vertexes[1].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y1);
    
    vertexes[2].pos = DirectX::XMFLOAT3(-.95f, 0.29f, .5f);
    vertexes[2].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y2);
    
    vertexes[3] = vertexes[1];

    vertexes[4].pos = DirectX::XMFLOAT3(.95f, 0.29f, .5f);
    vertexes[4].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y2);

    vertexes[5] = vertexes[2];

    // start button
    textureBoxSize.x1 = 1.f / static_cast<float>(textureInfo.width);
    textureBoxSize.y1 = 97.f / static_cast<float>(textureInfo.height);
    textureBoxSize.x2 = (textureInfo.width - 1.f) / static_cast<float>(textureInfo.width);
    textureBoxSize.y2 = 133.f / static_cast<float>(textureInfo.height);
    
    vertexes[6].pos = DirectX::XMFLOAT3(-.70f, .15f, .5f);
    vertexes[6].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y1);

    vertexes[7].pos = DirectX::XMFLOAT3(.70f, .15f, .5f);
    vertexes[7].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y1);

    vertexes[8].pos = DirectX::XMFLOAT3(-.70f, -.1f, .5f);
    vertexes[8].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y2);

    vertexes[9] = vertexes[7];

    vertexes[10].pos = DirectX::XMFLOAT3(.70f, -.1f, .5f);
    vertexes[10].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y2);

    vertexes[11] = vertexes[8];
    // exit button
    textureBoxSize.x1 = 1.f / static_cast<float>(textureInfo.width);
    textureBoxSize.y1 = 134.f / static_cast<float>(textureInfo.height);
    textureBoxSize.x2 = (textureInfo.width - 1.f) / static_cast<float>(textureInfo.width);
    textureBoxSize.y2 = 171.f / static_cast<float>(textureInfo.height);

    vertexes[12].pos = DirectX::XMFLOAT3(-.70f, -.15f, .5f);
    vertexes[12].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y1);

    vertexes[13].pos = DirectX::XMFLOAT3(.70f, -.15f, .5f);
    vertexes[13].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y1);

    vertexes[14].pos = DirectX::XMFLOAT3(-.70f, -.4f, .5f);
    vertexes[14].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y2);

    vertexes[15] = vertexes[13];

    vertexes[16].pos = DirectX::XMFLOAT3(.70f, -.4f, .5f);
    vertexes[16].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y2);

    vertexes[17] = vertexes[14];

    // footer
    textureBoxSize.x1 = 1.f / static_cast<float>(textureInfo.width);
    textureBoxSize.y1 = 234.f / static_cast<float>(textureInfo.height);
    textureBoxSize.x2 = (textureInfo.width - 1.f) / static_cast<float>(textureInfo.width);
    textureBoxSize.y2 = 255.f / static_cast<float>(textureInfo.height);

    vertexes[18].pos = DirectX::XMFLOAT3(-.95f, -.82f, 0.5f);
    vertexes[18].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y1);

    vertexes[19].pos = DirectX::XMFLOAT3(.95f, -.82f, 0.5f);
    vertexes[19].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y1);

    vertexes[20].pos = DirectX::XMFLOAT3(-.95f, -1.f, 0.5f);
    vertexes[20].tex = DirectX::XMFLOAT2(textureBoxSize.x1, textureBoxSize.y2);

    vertexes[21] = vertexes[19];

    vertexes[22].pos = DirectX::XMFLOAT3(.95f, -1.f, 0.5f);
    vertexes[22].tex = DirectX::XMFLOAT2(textureBoxSize.x2, textureBoxSize.y2);

    vertexes[23] = vertexes[20];

    D3D11_BUFFER_DESC vbDesc;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.ByteWidth = sizeof(vertex_t) * vertexCount;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;
    vbDesc.StructureByteStride = 0;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA vbResouces;
    vbResouces.pSysMem = vertexes;
    vbResouces.SysMemPitch = 0;
    vbResouces.SysMemSlicePitch = 0;

    hr = device->CreateBuffer(&vbDesc, &vbResouces, _vertexBuffer.getpp());
}

void MainMenuSceneRenderer::startup()
{
    auto* context = _renderer->getContext();
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(_inputLayout);
    context->PSSetShader(_pixelShader, nullptr, 0);
    context->VSSetShader(_vertexShader, nullptr, 0);

    // resource view
    const uint8_t viewCount = 1;
    ID3D11ShaderResourceView* resourceViews[viewCount];
    resourceViews[0] = _texture.getTextureView();
    context->PSSetShaderResources(0, viewCount, resourceViews);
}

void MainMenuSceneRenderer::render()
{
    _renderer->beginRender();
    UINT stride = sizeof(vertex_t), offset = 0;
    _renderer->getContext()->IASetVertexBuffers(0, 1, _vertexBuffer.getpp(), &stride, &offset);
    _renderer->getContext()->Draw(24, 0);
    _renderer->endRender();
}