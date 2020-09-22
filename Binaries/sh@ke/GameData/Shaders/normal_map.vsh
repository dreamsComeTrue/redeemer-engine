// Normal mapping vertex shader
// Copyright (C) Wojciech Toman 2007

float4x4 WorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 CameraPos;

// Input for the VertexShader
struct VertexInput
{
	float4 position   : POSITION0;
	float3 normal     : NORMAL;
	float2 texCoord   : TEXCOORD0;
};

// VertexShader results
struct VertexOutput
{
	float4 position   : POSITION0;
	float2 texCoord   : TEXCOORD0;
	float3 normal	  : TEXCOORD1;
	float3 V		  : TEXCOORD2;
	float3 positionV  : TEXCOORD3;
};


// VERTEX SHADER
VertexOutput vs_main(VertexInput IN) 
{
	VertexOutput OUT;
	
	float3 worldPos = mul(IN.position, matWorld).xyz;
	
	OUT.V		 	= CameraPos - worldPos;
	OUT.texCoord    = IN.texCoord;
    
    OUT.position    = mul(IN.position, WorldViewProj);
    OUT.normal		= mul(float4(IN.normal, 0.0f), matWorld).xyz;
    OUT.positionV	= mul(IN.position, matWorldView).xyz;
    
    return OUT;
}