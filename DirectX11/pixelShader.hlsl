cbuffer alphaBuffer : register(b0)
{
    float alpha;
}


struct PS_INTPUT
{
    float4 inPosition : SV_Position;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INTPUT input) : SV_Target
{
    float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    return float4(pixelColor, alpha);
}