
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuseCore : register( t0 );
Texture2D txDiffuse2 : register( t1 );
Texture2D txDiffuse3 : register( t2 );

SamplerState samLinear : register( s0 );


cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
  float4 vCameraPosition;
  float4 vtypeColor;
  float4 vScale;
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
    float3 Norm : TEXCOORD0;
    float2 Tex : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    float4 screenPos = mul( float4(0,0,0,1), World );
    screenPos = mul( screenPos, View );
    screenPos = mul( screenPos, Projection );
    screenPos.x /= screenPos.w;
    screenPos.y /= screenPos.w;
    screenPos.z /= screenPos.w;
    screenPos.w = 0;


    output.Pos = input.Pos * vScale + screenPos;
    output.Pos.w = 1;
    output.Norm = input.Norm;
    output.Tex = input.Tex;   

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
  float4 textureColor = txDiffuseCore.Sample( samLinear, input.Tex );
  return float4(textureColor.r, textureColor.g, textureColor.b, textureColor.r) * vtypeColor;
}

//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return float4( 1.0f, 1.0f, 0.0f, 1.0f );   
}









