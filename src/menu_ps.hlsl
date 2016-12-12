#include "menu_inc.hlsli"

Texture2D tex;
SamplerState samp;

cbuffer focusBuffer
{
    uint isFocused;
};

float4 main(psInput input) : SV_TARGET
{
    float4 color = tex.Sample(samp, input.tex);
    if (isFocused && color.a != 0.f)
    {
        color = lerp(color, float4(1.f, 1.f, 1.f, 1.f), .23f);
    }
	return color;
}