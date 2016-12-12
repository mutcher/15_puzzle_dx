#pragma once
#include "dx_ptr.h"
#include <d3d11.h>
#include <string>
#include <cstdint>

class TGATexture
{
private:
    dx_ptr<ID3D11Texture2D> _texture;
    dx_ptr<ID3D11ShaderResourceView> _textureView;
    struct TextureInfo
    {
        uint16_t width;
        uint16_t height;
        uint8_t  bpp;
    } _textureInfo;

public:
    TGATexture();
    
    inline ID3D11ShaderResourceView* getTextureView()
    {
        return _textureView.getp();
    }

    inline const TextureInfo& getTextureInfo() const
    {
        return _textureInfo;
    }

    void load(const std::string& filename, ID3D11Device* device);
};
