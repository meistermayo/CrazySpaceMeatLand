#pragma pack_matrix( row_major )

Texture2D mainTexture : register(t0);

SamplerState aSampler : register(s0);

cbuffer CamData : register(b0)
{
	float4x4 View;			
	float4x4 Projection;
}

cbuffer InstanceData : register(b1)
{
	matrix World;
    matrix WorldInv;
	// Texture might change too, but that's done through mainTexture
};
cbuffer FogData : register(b2)
{
    float fogMin;
    float fogMax;
    float4 fogCol;
    float4 EyePosWorld;
};
//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;	// u v maps
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
	float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT output;
    output.PosMS = input.Pos;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 texCol = mainTexture.Sample(aSampler, input.Tex);
    
    float4 msEyePos = mul(EyePosWorld, WorldInv);

    float distToEye = length(msEyePos - input.PosMS);
    float FogPercent = saturate((distToEye - fogMin) / fogMax); // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509645(v=vs.85).aspx

    return lerp(texCol, fogCol, FogPercent); // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509618(v=vs.85).aspx
    
    return texCol;
}