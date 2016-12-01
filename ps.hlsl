#include "include.hlsli"

Texture2D tex;
SamplerState samp;

float4 main(psInput input) : SV_TARGET
{
	return tex.Sample(samp, input.tex);
}