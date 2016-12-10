#include "GameSceneRenderer.h"


// internal types
struct instance_t
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};

struct vertex_t
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};


// internal constants
const float g_windowMargin = .2f;
const float g_cellMargin   = .03f;
const float g_cellWidth    = (2.f - (2.f * g_windowMargin + 3.f * g_cellMargin)) / 4.f;


GameSceneRenderer::GameSceneRenderer(DxRenderer* renderer, GameField* gameField)
    :_renderer(renderer), _gameField(gameField), _vertexBuffer(), 
    _instanceBuffer(), _texture(), _vertexShader(), _pixelShader(), _inputLayout()
{
}

void GameSceneRenderer::startup()
{
    auto* context = _renderer->getContext();
    context->IASetInputLayout(_inputLayout);
    context->VSSetShader(_vertexShader, nullptr, 0);
    context->PSSetShader(_pixelShader, nullptr, 0);
}

void GameSceneRenderer::init()
{
    _texture.load("data/texture.tga", _renderer->getDevice());
    this->createVertexBuffer();
    this->createInstanceBuffer();
    this->createShaders();
}

void GameSceneRenderer::createShaders()
{
    std::vector<uint8_t> fileData = _renderer->readFile(L"game_vs.cso");

    const uint8_t inputLayoutCount = 4;
    D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[inputLayoutCount];
    inputLayoutDesc[0].AlignedByteOffset = 0;
    inputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputLayoutDesc[0].InputSlot = 0;
    inputLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[0].InstanceDataStepRate = 0;
    inputLayoutDesc[0].SemanticIndex = 0;
    inputLayoutDesc[0].SemanticName = "POSITION";

    inputLayoutDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputLayoutDesc[1].InputSlot = 0;
    inputLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayoutDesc[1].InstanceDataStepRate = 0;
    inputLayoutDesc[1].SemanticIndex = 0;
    inputLayoutDesc[1].SemanticName = "TEXCOORD";

    inputLayoutDesc[2].AlignedByteOffset = 0;
    inputLayoutDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputLayoutDesc[2].InputSlot = 1;
    inputLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    inputLayoutDesc[2].InstanceDataStepRate = 1;
    inputLayoutDesc[2].SemanticIndex = 1;
    inputLayoutDesc[2].SemanticName = "POSITION";

    inputLayoutDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputLayoutDesc[3].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputLayoutDesc[3].InputSlot = 1;
    inputLayoutDesc[3].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
    inputLayoutDesc[3].InstanceDataStepRate = 1;
    inputLayoutDesc[3].SemanticIndex = 1;
    inputLayoutDesc[3].SemanticName = "TEXCOORD";

    HRESULT hr = _renderer->getDevice()->CreateInputLayout(inputLayoutDesc, inputLayoutCount, fileData.data(), fileData.size(), _inputLayout.getpp());
    hr = _renderer->getDevice()->CreateVertexShader(fileData.data(), fileData.size(), nullptr, _vertexShader.getpp());

    fileData = _renderer->readFile(L"game_ps.cso");
    hr = _renderer->getDevice()->CreatePixelShader(fileData.data(), fileData.size(), nullptr, _pixelShader.getpp());
}

void GameSceneRenderer::createVertexBuffer()
{
    vertex_t vertices[6];
    const float textureCellWidth = 21.f / _texture.getTextureInfo().width;

    // left top
    vertices[0].pos = DirectX::XMFLOAT3(0.f, 0.f, .5f);
    vertices[0].tex = DirectX::XMFLOAT2(0.f, 0.f);

    // right top
    vertices[1].pos = DirectX::XMFLOAT3(g_cellWidth, 0.f, .5f);
    vertices[1].tex = DirectX::XMFLOAT2(textureCellWidth, 0.f);

    // left bottom
    vertices[2].pos = DirectX::XMFLOAT3(0.f, -g_cellWidth, 0.5f);
    vertices[2].tex = DirectX::XMFLOAT2(0.f, textureCellWidth);

    vertices[3] = vertices[2];
    vertices[4] = vertices[1];

    // right bottom
    vertices[5].pos = DirectX::XMFLOAT3(g_cellWidth, -g_cellWidth, 0.5f);
    vertices[5].tex = DirectX::XMFLOAT2(textureCellWidth, textureCellWidth);

    D3D11_BUFFER_DESC vbDesc;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.ByteWidth = sizeof(vertex_t) * 6;
    vbDesc.CPUAccessFlags = 0;
    vbDesc.MiscFlags = 0;
    vbDesc.StructureByteStride = 0;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;

    D3D11_SUBRESOURCE_DATA subData;
    subData.pSysMem = vertices;
    subData.SysMemPitch = 0;
    subData.SysMemSlicePitch = 0;

    HRESULT hr = _renderer->getDevice()->CreateBuffer(&vbDesc, &subData, _vertexBuffer.getpp());
}

void GameSceneRenderer::createInstanceBuffer()
{
    if (_instanceBuffer.getp() == nullptr || _gameField->isFieldChanged())
    {
        _instanceBuffer.reset();

        instance_t instances[16];
        DirectX::XMFLOAT2 startPos(-1.f + g_windowMargin, 1.f - g_windowMargin);

        const auto& basicArray = _gameField->getBasicArray();
        const auto& textureInfo = _texture.getTextureInfo();
        for(uint8_t i = 0; i < basicArray.size(); ++i)
        {
            // position calculation
            if (i && (i % 4 == 0))
            {
                startPos.y -= g_cellWidth + g_cellMargin;
                startPos.x = -1.f + g_windowMargin;
            }
            instances[i].pos = DirectX::XMFLOAT3(startPos.x, startPos.y, 0.5f);
            startPos.x += g_cellWidth + g_cellMargin;

            // texture uv calculation
            uint8_t row = basicArray[i] / 4;
            uint8_t col = basicArray[i] - row * 4;

            instances[i].tex.x = (col * 22.f + 1.f) / textureInfo.width;
            instances[i].tex.y = (row * 22.f + 1.f) / textureInfo.height;
        }

        D3D11_BUFFER_DESC ibDesc;
        ibDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        ibDesc.ByteWidth = sizeof(instance_t) * 16;
        ibDesc.CPUAccessFlags = 0;
        ibDesc.MiscFlags = 0;
        ibDesc.StructureByteStride = 0;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA subData;
        subData.pSysMem = instances;
        subData.SysMemPitch = 0;
        subData.SysMemSlicePitch = 0;

        _renderer->getDevice()->CreateBuffer(&ibDesc, &subData, _instanceBuffer.getpp());
    }
}

void GameSceneRenderer::render()
{
    _renderer->beginRender();
    createInstanceBuffer();

    auto* context = _renderer->getContext();
    UINT stride = sizeof(instance_t), offset = 0;
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetVertexBuffers(1, 1, _instanceBuffer.getpp(), &stride, &offset);
    context->IASetVertexBuffers(0, 1, _vertexBuffer.getpp(), &stride, &offset);

    // setting texture views
    const uint16_t textureViewCount = 1;
    ID3D11ShaderResourceView* textureViews[textureViewCount];
    textureViews[0] = _texture.getTextureView();
    context->PSSetShaderResources(0, textureViewCount, textureViews);

    context->DrawInstanced(6, 16, 0, 0);
    _renderer->endRender();
}