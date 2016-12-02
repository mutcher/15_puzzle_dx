#include "GameSceneRenderer.h"
#include <fstream>
#include <vector>


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
    _instanceBuffer(), _texture(), _textureView(), _textureInfo()
{
}

void GameSceneRenderer::init()
{
    this->loadTexture();
    this->createVertexBuffer();
    this->createInstanceBuffer();
}

void GameSceneRenderer::createVertexBuffer()
{
    vertex_t vertices[6];
    const float textureCellWidth = 21.f / _textureInfo.width;

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

            instances[i].tex.x = (col * 22.f + 1.f) / _textureInfo.width;
            instances[i].tex.y = (row * 22.f + 1.f) / _textureInfo.height;
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

void GameSceneRenderer::loadTexture()
{
    const std::string textureFilename = "data/texture.tga";
    std::ifstream stream(textureFilename, std::ios_base::beg | std::ios_base::binary);
    stream.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);
    struct 
    {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    } img_header;

    stream.read(reinterpret_cast<char*>(&img_header), sizeof(img_header));

    // image should be 32 bpp
    if (img_header.bpp != sizeof(uint32_t) * 8)
    {
        stream.setstate(std::ios_base::failbit);
    }

    _textureInfo.width = img_header.width;
    _textureInfo.height = img_header.height;
    _textureInfo.bpp = img_header.bpp;

    size_t imgSize = img_header.height * img_header.width;
    std::vector<uint32_t> tmpData(imgSize);
    tmpData.shrink_to_fit();
    stream.read(reinterpret_cast<char*>(tmpData.data()), imgSize * sizeof(uint32_t));
    stream.close();

    for(auto iter = tmpData.begin(); iter != tmpData.end(); ++iter)
    {
        uint8_t* bgra = reinterpret_cast<uint8_t*>(&(*iter));
        // swap b and r components to get rgba from bgra
        std::swap(bgra[0], bgra[2]);
    }

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.ArraySize = 1;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Height = img_header.height;
    textureDesc.MipLevels = 1;
    textureDesc.MiscFlags = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.Width = img_header.width;

    D3D11_SUBRESOURCE_DATA subData;
    subData.pSysMem = tmpData.data();
    subData.SysMemPitch = img_header.width * sizeof(uint32_t);
    subData.SysMemSlicePitch = 0;

    HRESULT hr = _renderer->getDevice()->CreateTexture2D(&textureDesc, &subData, _texture.getpp());

    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
    memset(&textureViewDesc, 0, sizeof(textureViewDesc));
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    textureViewDesc.Texture2D.MipLevels = 1;
    textureViewDesc.Texture2D.MostDetailedMip = 0;

    hr = _renderer->getDevice()->CreateShaderResourceView(_texture.getp(), &textureViewDesc, _textureView.getpp());
}

void GameSceneRenderer::render()
{
    _renderer->beginRender();
    createInstanceBuffer();

    UINT stride = sizeof(instance_t), offset = 0;
    _renderer->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _renderer->getContext()->IASetVertexBuffers(1, 1, _instanceBuffer.getpp(), &stride, &offset);
    _renderer->getContext()->IASetVertexBuffers(0, 1, _vertexBuffer.getpp(), &stride, &offset);
    _renderer->getContext()->PSSetShaderResources(0, 1, _textureView.getpp());

    _renderer->getContext()->DrawInstanced(6, 16, 0, 0);
    _renderer->endRender();
}