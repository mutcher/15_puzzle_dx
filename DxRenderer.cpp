#include "DxRenderer.h"
#include <fstream>
#include <vector>

#pragma comment(lib, "d3d11.lib")

// internal types
struct vertex_t
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};

struct instance_t
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
};

// internal constants
const float g_windowMargin = .2f;
const float g_cellMargin   = .03f;
const float g_cellWidth    = (2.f - (2.f * g_windowMargin + 3.f * g_cellMargin)) / 4.f;

std::vector<uint8_t> readFile(const std::wstring& fileName)
{
    std::ifstream stream(fileName, std::ios_base::ate | std::ios_base::binary);
    stream.exceptions(std::ios_base::badbit | std::ios_base::eofbit | std::ios_base::failbit);
    fpos_t pos = stream.tellg();
    assert(pos != fpos_t(-1));
    std::vector<uint8_t> fileData(pos);
    fileData.shrink_to_fit();
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(fileData.data()), fileData.size());
    stream.close();
    return fileData;
}

DxRenderer::DxRenderer(int width, int height)
    :_handle(nullptr), _width(width), _height(height),
    _swapChain(), _device(), _context(), _renderTargetView(),
    _vertexShader(), _pixelShader(), _inputLayout(), _vertexBuffer(),
    _texture(), _textureView(), _textureInfo(), _blendState()
{
}

bool DxRenderer::init(HWND handle)
{
    _handle = handle;

    HRESULT hr = 0;

    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.BufferDesc.Height = 500;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 75;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Width = 500;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
#ifdef _DEBUG
        swapChainDesc.Flags = D3D11_CREATE_DEVICE_DEBUG;
#else
        swapChainDesc.Flags = 0;
#endif //_DEBUG
        swapChainDesc.OutputWindow = handle;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Windowed = true;


        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
        hr = D3D11CreateDeviceAndSwapChain(nullptr,
            D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &swapChainDesc,
            _swapChain.getpp(),
            _device.getpp(),
            nullptr,
            _context.getpp());
    }

    {
        dx_ptr<ID3D11Texture2D> backBuffer;
        hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.getpp()));
        hr = _device->CreateRenderTargetView(backBuffer, nullptr, _renderTargetView.getpp());
    }
    _context->OMSetRenderTargets(1, _renderTargetView.getpp(), nullptr);


    {
        D3D11_VIEWPORT vp;
        vp.Height = static_cast<float>(_height);
        vp.MaxDepth = 1.f;
        vp.MinDepth = 0.f;
        vp.TopLeftX = 0.f;
        vp.TopLeftY = 0.f;
        vp.Width = static_cast<float>(_width);

        _context->RSSetViewports(1, &vp);
    }


    {
        std::vector<uint8_t> fileData = readFile(L"vs.cso");

        D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[4];
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

        hr = _device->CreateInputLayout(inputLayoutDesc, 4, fileData.data(), fileData.size(), _inputLayout.getpp());
        hr = _device->CreateVertexShader(fileData.data(), fileData.size(), nullptr, _vertexShader.getpp());

        fileData = readFile(L"ps.cso");
        hr = _device->CreatePixelShader(fileData.data(), fileData.size(), nullptr, _pixelShader.getpp());
    }
    _context->IASetInputLayout(_inputLayout);


    loadTGATexture();
    createVertexBuffer();

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0.f;
    samplerDesc.BorderColor[1] = 0.f;
    samplerDesc.BorderColor[2] = 0.f;
    samplerDesc.BorderColor[3] = 0.f;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = _device->CreateSamplerState(&samplerDesc, _samplerState.getpp());
    _context->PSSetSamplers(0, 1, _samplerState.getpp());


    _context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _context->VSSetShader(_vertexShader, nullptr, 0);
    _context->PSSetShader(_pixelShader, nullptr, 0);

    //blend state
    {
        D3D11_BLEND_DESC blendDesc;
        memset(&blendDesc, 0, sizeof(blendDesc));
        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        hr = _device->CreateBlendState(&blendDesc, _blendState.getpp());
#undef max
        _context->OMSetBlendState(_blendState, nullptr, std::numeric_limits<UINT>::max());
    }

    return true;
}

void DxRenderer::render(const GameField& gameField)
{
    FLOAT backColor[] = {0.259f, 0.518f, 0.49f, 1.0f};
    _context->ClearRenderTargetView(_renderTargetView, backColor);
    createInstanceBuffer(gameField);

    _context->DrawInstanced(6, 16, 0, 0);
    _swapChain->Present(1, 0);
}

void DxRenderer::createInstanceBuffer(const GameField& gameField)
{
    if (_instanceBuffer.getp() == nullptr || gameField.isFieldChanged())
    {
        _instanceBuffer.reset();

        instance_t instances[16];
        DirectX::XMFLOAT2 startPos(-1.f + g_windowMargin, 1.f - g_windowMargin);

        const auto& basicArray = gameField.getBasicArray();
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

        _device->CreateBuffer(&ibDesc, &subData, _instanceBuffer.getpp());

        UINT stride = sizeof(instance_t), offset = 0;
        _context->IASetVertexBuffers(1, 1, _instanceBuffer.getpp(), &stride, &offset);
    }
}

void DxRenderer::createVertexBuffer()
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

    HRESULT hr = _device->CreateBuffer(&vbDesc, &subData, _vertexBuffer.getpp());
    UINT stride = sizeof(vertex_t), offset = 0;
    _context->IASetVertexBuffers(0, 1, _vertexBuffer.getpp(), &stride, &offset);
}

void DxRenderer::loadTGATexture()
{
    const std::string textureFilename = "data/texture.tga";
    std::ifstream stream(textureFilename, std::ios_base::beg | std::ios_base::binary);
    stream.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);
    if (stream)
    {
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

        HRESULT hr = _device->CreateTexture2D(&textureDesc, &subData, _texture.getpp());

        D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
        memset(&textureViewDesc, 0, sizeof(textureViewDesc));
        textureViewDesc.Format = textureDesc.Format;
        textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        textureViewDesc.Texture2D.MipLevels = 1;
        textureViewDesc.Texture2D.MostDetailedMip = 0;

        hr = _device->CreateShaderResourceView(_texture.getp(), &textureViewDesc, _textureView.getpp());
        _context->PSSetShaderResources(0, 1, _textureView.getpp());
    }
}