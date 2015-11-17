//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D txDiffuse : register( t0 );
Texture2D txAtmosphere : register( t1 );
Texture2D txNight : register( t2 );
Texture2D txSpecular : register( t3 );

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
    output.Pos   = mul( input.Pos,  World );
    output.Pos   = mul( output.Pos, View );
    output.Pos   = mul( output.Pos, Projection );
    output.PosW  = mul( input.Pos, World );
    output.NormW = mul( input.Norm, World );
    output.Tex = input.Tex;   
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    half roughness2 =  0.2; 
    half3 oren_nayar_fraction = roughness2 / (roughness2 + half3(0.33, 0.13, 0.09));
    half3 oren_nayar = half3(1, 0, 0) + half3(-0.5, 0.17, 0.45) * oren_nayar_fraction;

    half3 normal =   normalize(input.NormW).xyz;
    half3 lightDir = normalize(((vLightPos[0] - input.PosW ).xyz));
    half3 viewDir =  normalize(((vCameraPosition - input.PosW ).xyz));

    //reflection
    float3 reflectDir = reflect(-lightDir, normal);
    float specAngle = max(dot(reflectDir, viewDir), 0.0);
    // note that the exponent is different here
    float specular = pow(specAngle, 10.0) * txSpecular.Sample( samLinear, input.Tex ).r;

    //components
    half cos_nl = saturate(dot(normal, lightDir));
    half cos_nv = saturate(dot(normal, viewDir));
    half oren_nayar_s = saturate(dot(lightDir, viewDir)) - cos_nl * cos_nv;
    oren_nayar_s /= lerp(max(cos_nl, cos_nv), 1, step(oren_nayar_s, 0));

    //composition
    half3 result = 0.035 + 1.0 * cos_nl * (oren_nayar.x + 1.0* oren_nayar.y + oren_nayar.z * oren_nayar_s);
    
  
    saturate(result);
    half3 resultNeg = half3(1, 1, 1) - result;

     half4 atmosphere =  txAtmosphere.Sample( samLinear, input.Tex );
     atmosphere.a = atmosphere.r;
     atmosphere.r = atmosphere.r * result.x;
     atmosphere.g = atmosphere.g * result.y;
     atmosphere.b = atmosphere.b * result.z;
     
     half4 diffuse =     ((half4(result, 1))    * txDiffuse.Sample( samLinear, input.Tex ));
     half4 diffuseNeg =  ((half4(resultNeg, 1)) * txNight.Sample( samLinear, input.Tex ));
     half4 diffuseFinal = diffuse + diffuseNeg + specular * (half4(result, 1));

     return lerp(diffuseFinal, atmosphere, atmosphere.a);
}


//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid( PS_INPUT input) : SV_Target
{
    return vOutputColor;
}
