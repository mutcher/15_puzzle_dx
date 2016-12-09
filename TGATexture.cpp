#include "TGATexture.h"
#include <fstream>
#include <vector>

TGATexture::TGATexture()
    :_texture(), _textureView()
{
    memset(&_textureInfo, 0, sizeof(_textureInfo));
}

void TGATexture::load(const std::string& filename, ID3D11Device* device)
{
    // in case if texture exists
    // we need to release all resources
    _texture.reset();
    _textureView.reset();

    std::ifstream stream(filename, std::ios_base::beg | std::ios_base::binary);
    stream.exceptions(std::ios_base::failbit | std::ios_base::badbit | std::ios_base::eofbit);

    // reading tga image header
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

    HRESULT hr = device->CreateTexture2D(&textureDesc, &subData, _texture.getpp());

    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
    memset(&textureViewDesc, 0, sizeof(textureViewDesc));
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    textureViewDesc.Texture2D.MipLevels = 1;
    textureViewDesc.Texture2D.MostDetailedMip = 0;

    hr = device->CreateShaderResourceView(_texture.getp(), &textureViewDesc, _textureView.getpp());
}