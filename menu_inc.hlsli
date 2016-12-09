struct vsInput
{
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
};

struct psInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};
