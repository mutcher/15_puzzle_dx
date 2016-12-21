#include "TGATexture.h"
#include <fstream>
#include <vector>
#include <algorithm>

// all possible image types
// described in TGA format specification
enum class tga_image_type : uint8_t
{
    none         = 0,  // no image data
    colormap     = 1,  // uncompressed, colormap
    bgr          = 2,  // uncompressed, bgr
    mono         = 3,  // uncompressed, black-white image
    colormap_rle = 9,  // compressed, colormap
    bgr_rle      = 10, // compressed, bgr
    mono_rle     = 11  // compressed, black-white image
};

namespace ifstream_helpers
{
    template<typename T>
    inline void read(std::ifstream& stream, T* data, const size_t& count = 1)
    {
        stream.read(reinterpret_cast<char*>(data), sizeof(T) * count);
    }

    template<typename T>
    inline void read(std::ifstream& stream, T& data, const size_t& count = 1)
    {
        stream.read(reinterpret_cast<char*>(&data), sizeof(T) * count);
    }
}

// TGATexture class
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
        uint8_t data1[2];
        tga_image_type tga_type;
        uint8_t data2[5];
        // 0,0 point for origin x,y
        // is bottom left
        uint16_t x_origin;
        uint16_t y_origin;
        uint16_t width;
        uint16_t height;
        uint8_t bpp;
        uint8_t data3;
    } img_header;
    ifstream_helpers::read(stream, img_header);

    if (img_header.bpp != sizeof(uint32_t) * 8) // image should be 32 bpp
    {
        stream.setstate(std::ios_base::failbit);
    }

    _textureInfo.width = img_header.width;
    _textureInfo.height = img_header.height;
    _textureInfo.bpp = img_header.bpp;

    size_t imgSize = img_header.height * img_header.width;
    std::vector<uint32_t> tmpData(imgSize);
    tmpData.shrink_to_fit();
    if (img_header.tga_type == tga_image_type::bgr)
    {
        ifstream_helpers::read(stream, tmpData.data(), imgSize * sizeof(uint32_t));
    }
    else if (img_header.tga_type == tga_image_type::bgr_rle)
    {
        size_t loadedCount = 0;
        uint8_t tmpByte;

        while(loadedCount < tmpData.size())
        {
            ifstream_helpers::read(stream, tmpByte);
            size_t pointCount = (tmpByte & ~0x80) + 1;
            if (tmpByte & 0x80) // RLE packet
            {
                uint32_t tmpPacket;
                ifstream_helpers::read(stream, tmpPacket);
                std::fill_n(tmpData.begin() + loadedCount, pointCount, tmpPacket);
            }
            else // RAW packet
            {
                for(size_t i = 0; i < pointCount; ++i)
                {
                    ifstream_helpers::read(stream, tmpData[loadedCount + i]);
                }
            }
            loadedCount += pointCount;
        }
    }
    else // unsupported format
    {
        stream.setstate(std::ios_base::failbit);
    }
    stream.close();

    // origin point is left-bottom
    if (img_header.y_origin == 0)
    {
        for(auto fIter = tmpData.begin(), lIter = tmpData.end() - img_header.width;
            fIter < lIter;
            fIter += img_header.width, lIter -= img_header.width)
        {
            std::swap_ranges(fIter, fIter + img_header.width, lIter);
        }
    }

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