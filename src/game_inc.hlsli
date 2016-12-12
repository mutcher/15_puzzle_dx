struct vsInput
{
    // per vertex
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
    
    //per instance
    float4 posInst : POSITION1;
    float2 texInst : TEXCOORD1;
};

struct psInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};