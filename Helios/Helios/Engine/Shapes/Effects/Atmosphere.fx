//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );
Texture2D txAtmosphere : register( t1 );
Texture2D txNight : register( t2 );

SamplerState samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
  float4 vCameraPosition;
	float4 vLightPos[2];
	float4 vLightColor[2];
  float4 vAnimSources;
	float4 vOutputColor;
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Norm : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 PosW : TEXCOORD0;
    float3 NormW : TEXCOORD1;
    float2 Tex : TEXCOORD2;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.PosW = mul( input.Pos, World );
    output.NormW = mul( input.Norm, World );
    output.Tex = input.Tex;   
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    half3 normal =   normalize(input.NormW).xyz;
    half3 viewDir =  normalize((vCameraPosition.xyz - input.PosW.xyz).xyz);
    half3 lightDirOp = normalize((input.PosW  - vLightPos[0]).xyz);

    half cos_nv = 1 - saturate( dot(normal, viewDir));
    half cos_nl = 1 - saturate( dot(normal, lightDirOp));
    return half4( 0.5,0.7, 0.9, 0.4) * cos_nv *  cos_nl;
}


//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return vOutputColor;
}
 