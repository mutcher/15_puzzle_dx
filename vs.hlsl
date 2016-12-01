#include "include.hlsli"

psInput main(vsInput input)
{
    float4 npos = input.pos;
    npos.w = 1.0f;
    npos.xy += input.posInst.xy;

    psInput output;
    output.pos = npos;
    output.tex = input.texInst + input.tex;

    return output;
}