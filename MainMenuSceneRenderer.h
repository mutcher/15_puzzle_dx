#pragma once
#include "DxRenderer.h"
#include "TGATexture.h"

class MainMenuSceneRenderer
{
public:
    enum class MainMenuButtons : uint8_t
    {
        Start = 0,
        Exit = 1
    };

private:
    DxRenderer* _renderer;
    TGATexture _texture;

    dx_ptr<ID3D11InputLayout> _inputLayout;
    dx_ptr<ID3D11VertexShader> _vertexShader;
    dx_ptr<ID3D11PixelShader> _pixelShader;
    dx_ptr<ID3D11Buffer> _vertexBuffer;
    dx_ptr<ID3D11SamplerState> _samplerState;
    dx_ptr<ID3D11Buffer> _focusBuffer;

private:
    void renderLogoAndFooter();
    void renderMenuButtons(const MainMenuButtons& activeButton);

public:
    MainMenuSceneRenderer(DxRenderer* renderer);

    void init();
    void startup();
    void render(const MainMenuButtons& activeButton);
};
