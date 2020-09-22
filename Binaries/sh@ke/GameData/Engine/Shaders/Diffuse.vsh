/*	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**	
**	Diffuse Shader
**		Shader used to blit render targets to backbuffer when there is no lights
*/

//------------------------------------------------------------------------------------------------------------------------

//	World-View-Projection matrix
float4x4 WorldViewProj;

//------------------------------------------------------------------------------------------------------------------------

struct VS_INPUT
{
	float3 Position  : POSITION0;
	float4 Normal    : NORMAL;
	float2 TexCoord  : TEXCOORD0;
	float4 Color     : COLOR0;
};

//------------------------------------------------------------------------------------------------------------------------

struct VS_OUTPUT
{
	float4 Position  : POSITION0;
	float2 TexCoord  : TEXCOORD0;
	float4 Color     : COLOR0;	
};

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------

VS_OUTPUT VS_Main (VS_INPUT IN)
{
	VS_OUTPUT OUT;

	float4 v = float4 (IN.Position.x, IN.Position.y, IN.Position.z, 1.0f);

    OUT.Position  = mul (v, WorldViewProj);
    OUT.Color0    = IN.Color;
    OUT.TexCoord  = IN.TexCoord;

    return OUT;
}

