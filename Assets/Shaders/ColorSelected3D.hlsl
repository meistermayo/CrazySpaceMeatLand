#pragma pack_matrix( row_major )

cbuffer CamData : register(b0)
{
	float4x4 View;
	float4x4 Projection;
}

cbuffer InstanceData : register(b1)
{
	float4x4 World;
    float4x4 WorldInv;
	float4 SelectedColor;
};
cbuffer FogData : register(b2)
{
    float fogMin;
    float fogMax;
    float4 fogCol;
    float4 EyePosWorld;
};
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
    float4 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION )
{
    VS_OUTPUT output;
    output.PosMS = Pos;
    output.Pos = mul( Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Color = SelectedColor;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    float4 msEyePos = mul(EyePosWorld, WorldInv);

    float distToEye = length(msEyePos - input.PosMS);
    float FogPercent = saturate((distToEye - fogMin) / fogMax); // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509645(v=vs.85).aspx

    return lerp(input.Color, fogCol, FogPercent); // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509618(v=vs.85).aspx
}
