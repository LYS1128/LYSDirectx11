
#include "Shader_Defines.hpp"

float	g_fPercent;
float	g_Rand;

cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

}

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD;
	float4		vLocalPos : TEXCOORD1;
};

texture2D			g_Texture;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	Out.vLocalPos = vector(In.vPosition, 1.f);
	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vPosition : SV_POSITION;
	float2			vTexUV : TEXCOORD0;
	float4			vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a < 0.3f)
		discard;

	return Out;
}

PS_OUT PS_OVERLOAD(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (-0.5f + g_fPercent <= In.vLocalPos.y)
		discard;

	if (Out.vColor.a < 0.3f)
		discard;

	return Out;
}

PS_OUT PS_BFLY(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a >= 0.1f)
		Out.vColor.a = g_fPercent;
	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_CIRCLE(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	
	Out.vColor.g = Out.vColor.g + (100.f*g_fPercent)/255.f;

	if (Out.vColor.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_DUST(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vColor.a >= 0.01f)
		Out.vColor.a *= g_fPercent;

	if (Out.vColor.a < 0.3f)
		discard;

	return Out;
}



//BOOL BlendEnable;
//D3D11_BLEND SrcBlend;
//D3D11_BLEND DestBlend;
//D3D11_BLEND_OP BlendOp;
//D3D11_BLEND SrcBlendAlpha;
//D3D11_BLEND DestBlendAlpha;
//D3D11_BLEND_OP BlendOpAlpha;
//UINT8 RenderTargetWriteMask;


technique11 DefaultTechnique
{
	/* Pass를 나누는 기준 == 그릴려고하는 리소스에게 어떤 셰이더 세트 효과를 줄것인지?! */

	/* 명암 + 그림자 + 노멀 */
	pass DefaultRendering
	{		
		SetRasterizerState(RS_Default);		
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
	/*	SetDepthStencilState();
			*/			

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Overload
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_OVERLOAD();
	}

	pass RushBfly
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_BFLY();
	}
	pass Circle
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_CIRCLE();
	}

	pass Trail
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Dust
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_DUST();
	}
}


