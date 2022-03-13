cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;
}


struct PS_INTPUT
{
    float4 inPosition : SV_Position;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INTPUT input) : SV_Target
{
    //float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 sampleColor = input.inNormal;
    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 finalColor = sampleColor * ambientLight;
    
    return float4(finalColor, 1.0f);
}