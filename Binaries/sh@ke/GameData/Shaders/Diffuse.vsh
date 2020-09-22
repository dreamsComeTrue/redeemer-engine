float4x4 WorldViewProj;

struct VS_INPUT
{
	float3 position  : POSITION0;
	float4 normal    : NORMAL;
	float2 texcoord0 : TEXCOORD0;
	float4 color0    : COLOR0;
};

struct VS_OUTPUT
{
	float4 hposition : POSITION0;
	float2 texcoord0 : TEXCOORD0;
	float4 color0    : COLOR0;	
};

VS_OUTPUT vs_main( VS_INPUT IN )
{
	VS_OUTPUT OUT;

	float4 v = float4( IN.position.x,
		               IN.position.y,
					   IN.position.z,
					   1.0f );

    OUT.hposition = mul( v, WorldViewProj );
    OUT.color0    = IN.color0;
    OUT.texcoord0 = IN.texcoord0;

    return OUT;
}

