#define NOMINMAX
#include "DxRenderer.h"
#include <fstream>
#include <vector>
#pragma comment(lib, "d3d11.lib")

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

DxRenderer::DxRenderer()
    :_handle(nullptr), _width(0), _height(0),
    _swapChain(), _device(), _context(), _renderTargetView(),
    _vertexShader(), _pixelShader(), _inputLayout(), _blendState()
{
}

bool DxRenderer::init(HWND handle, const uint32_t& width, const uint32_t& height)
{
    _handle = handle;
    _width = width;
    _height = height;

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

    initBackBufferAndViewport();
    initShaders();
    initSamplerState();
    initBlendState();
    return true;
}

void DxRenderer::initShaders()
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

    HRESULT hr = _device->CreateInputLayout(inputLayoutDesc, 4, fileData.data(), fileData.size(), _inputLayout.getpp());
    hr = _device->CreateVertexShader(fileData.data(), fileData.size(), nullptr, _vertexShader.getpp());

    fileData = readFile(L"ps.cso");
    hr = _device->CreatePixelShader(fileData.data(), fileData.size(), nullptr, _pixelShader.getpp());

    _context->IASetInputLayout(_inputLayout);
    _context->VSSetShader(_vertexShader, nullptr, 0);
    _context->PSSetShader(_pixelShader, nullptr, 0);
}

void DxRenderer::beginRender()
{
    FLOAT backColor[] = {0.259f, 0.518f, 0.49f, 1.0f};
    _context->ClearRenderTargetView(_renderTargetView, backColor);
}

void DxRenderer::endRender()
{
    _swapChain->Present(1, 0);
}

void DxRenderer::initBlendState()
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

    HRESULT hr = _device->CreateBlendState(&blendDesc, _blendState.getpp());
    _context->OMSetBlendState(_blendState, nullptr, std::numeric_limits<UINT>::max());
}

void DxRenderer::initSamplerState()
{
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

    HRESULT hr = _device->CreateSamplerState(&samplerDesc, _samplerState.getpp());
    _context->PSSetSamplers(0, 1, _samplerState.getpp());
}

void DxRenderer::resize(const uint32_t& width, const uint32_t& height)
{
    _height = height;
    _width = width;

    if (_swapChain.getp() != nullptr)
    {
        // cleanup render target
        _context->OMSetRenderTargets(0, nullptr, nullptr);
        // release render target
        _renderTargetView.reset();
        // resizing swap chain back buffer
        HRESULT hr = _swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        // recreating back buffer and viewport
        initBackBufferAndViewport();
    }
}

void DxRenderer::initBackBufferAndViewport()
{
    dx_ptr<ID3D11Texture2D> backBuffer;
    // getting back buffer
    HRESULT hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.getpp()));
    // creating render target for back buffer
    hr = _device->CreateRenderTargetView(backBuffer, nullptr, _renderTargetView.getpp());
    // setting render target
    _context->OMSetRenderTargets(1, _renderTargetView.getpp(), nullptr);

    // viewport init
    D3D11_VIEWPORT vp;
    vp.Height = static_cast<float>(_height);
    vp.MaxDepth = 1.f;
    vp.MinDepth = 0.f;
    vp.TopLeftX = 0.f;
    vp.TopLeftY = 0.f;
    vp.Width = static_cast<float>(_width);
    // viewport setting
    _context->RSSetViewports(1, &vp);
}
