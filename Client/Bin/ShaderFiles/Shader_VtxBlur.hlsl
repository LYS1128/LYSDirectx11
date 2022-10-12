
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

}

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		vWPos1 : TEXCOORD1;
	float2		vWPos2 : TEXCOORD2;
	float2		vWPos3 : TEXCOORD3;
	float2		vWPos4 : TEXCOORD4;
	float2		vWPos5 : TEXCOORD5;
	float2		vWPos6 : TEXCOORD6;
	float2		vWPos7 : TEXCOORD7;
	float2		vWPos8 : TEXCOORD8;
	float2		vWPos9 : TEXCOORD9;
	float2		vHPos1 : TEXCOORD10;
	float2		vHPos2 : TEXCOORD11;
	float2		vHPos3 : TEXCOORD12;
	float2		vHPos4 : TEXCOORD13;
	float2		vHPos5 : TEXCOORD14;
	float2		vHPos6 : TEXCOORD15;
	float2		vHPos7 : TEXCOORD16;
	float2		vHPos8 : TEXCOORD17;
	float2		vHPos9 : TEXCOORD18;
};

texture2D			g_Texture;
texture2D			g_ColorTexture;
texture2D			g_DepthTexture;
float				g_Percent;
float				Width;
float				Height;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;
	float fWTexelSize;
	float fHTexelSize;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	fWTexelSize = 1.0f / Width;
	fHTexelSize = 1.0f / Height;

	Out.vWPos1 = In.vTexUV + float2(fWTexelSize * -4.0f, 0.0f);
	Out.vWPos2 = In.vTexUV + float2(fWTexelSize * -3.0f, 0.0f);
	Out.vWPos3 = In.vTexUV + float2(fWTexelSize * -2.0f, 0.0f);
	Out.vWPos4 = In.vTexUV + float2(fWTexelSize * -1.0f, 0.0f);
	Out.vWPos5 = In.vTexUV + float2(fWTexelSize * 0.0f, 0.0f);
	Out.vWPos6 = In.vTexUV + float2(fWTexelSize * 1.0f, 0.0f);
	Out.vWPos7 = In.vTexUV + float2(fWTexelSize * 2.0f, 0.0f);
	Out.vWPos8 = In.vTexUV + float2(fWTexelSize * 3.0f, 0.0f);
	Out.vWPos9 = In.vTexUV + float2(fWTexelSize * 4.0f, 0.0f);
	Out.vHPos1 = In.vTexUV + float2( 0.0f,fHTexelSize * -4.0f);
	Out.vHPos2 = In.vTexUV + float2( 0.0f,fHTexelSize * -3.0f);
	Out.vHPos3 = In.vTexUV + float2( 0.0f,fHTexelSize * -2.0f);
	Out.vHPos4 = In.vTexUV + float2( 0.0f,fHTexelSize * -1.0f);
	Out.vHPos5 = In.vTexUV + float2(0.f,fHTexelSize * 0.0f);
	Out.vHPos6 = In.vTexUV + float2(0.f,fHTexelSize * 1.0f);
	Out.vHPos7 = In.vTexUV + float2(0.f,fHTexelSize * 2.0f);
	Out.vHPos8 = In.vTexUV + float2(0.f,fHTexelSize * 3.0f);
	Out.vHPos9 = In.vTexUV + float2(0.f,fHTexelSize * 4.0f);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float2		vWPos1 : TEXCOORD1;
	float2		vWPos2 : TEXCOORD2;
	float2		vWPos3 : TEXCOORD3;
	float2		vWPos4 : TEXCOORD4;
	float2		vWPos5 : TEXCOORD5;
	float2		vWPos6 : TEXCOORD6;
	float2		vWPos7 : TEXCOORD7;
	float2		vWPos8 : TEXCOORD8;
	float2		vWPos9 : TEXCOORD9;
	float2		vHPos1 : TEXCOORD10;
	float2		vHPos2 : TEXCOORD11;
	float2		vHPos3 : TEXCOORD12;
	float2		vHPos4 : TEXCOORD13;
	float2		vHPos5 : TEXCOORD14;
	float2		vHPos6 : TEXCOORD15;
	float2		vHPos7 : TEXCOORD16;
	float2		vHPos8 : TEXCOORD17;
	float2		vHPos9 : TEXCOORD18;
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	vector GlowColor;
	PS_OUT			Out = (PS_OUT)0;

	// 각 픽셀의 기여도에 해당하는 가중치를 정합니다.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.6f;
	weight3 = 0.3f;
	weight4 = 0.1f;

	// 가중치들을 살짝 평균내어 정규화 값을 만듭니다.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// 가중치들을 정규화합니다.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	Out.vColor = vector(0.0f, 0.0f, 0.0f, 0.0f);

	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos1) * weight4;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos2) * weight3;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos3) * weight2;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos4) * weight1;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos5) * weight0;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos6) * weight1;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos7) * weight2;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos8) * weight3;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos9) * weight4;

	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos1) * weight4;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos2) * weight3;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos3) * weight2;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos4) * weight1;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos5) * weight0;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos6) * weight1;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos7) * weight2;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos8) * weight3;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos9) * weight4;


	GlowColor = g_ColorTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor = saturate(saturate(Out.vColor)*vector(1.f, 1.f, 1.f, 0.3f) - GlowColor*vector(0.f, 0.f, 0.3f, -0.7f)) * (1.f,1.f,1.f,g_Percent);

	float		fViewZ = In.vPosition.w;

	float2		vUV;

	vUV.x = (In.vPosition.x / In.vPosition.w) * 0.5f + 0.5f;
	vUV.y = (In.vPosition.y / In.vPosition.w) * -0.5f + 0.5f;

	float4		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, vUV);
	float		fTargetViewZ = vDepthDesc.y * 500.f;

	Out.vColor.a = max(min(fTargetViewZ - fViewZ, Out.vColor.a), 0.f);


	return Out;
}

PS_OUT PS_MAIN_TWO(PS_IN In)
{
	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	vector GlowColor;
	PS_OUT			Out = (PS_OUT)0;

	// 각 픽셀의 기여도에 해당하는 가중치를 정합니다.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.8f;
	weight3 = 0.7f;
	weight4 = 0.6f;

	// 가중치들을 살짝 평균내어 정규화 값을 만듭니다.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// 가중치들을 정규화합니다.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	Out.vColor = vector(0.0f, 0.0f, 0.0f, 0.0f);

	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos1) * weight4;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos2) * weight3;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos3) * weight2;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos4) * weight1;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos5) * weight0;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos6) * weight1;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos7) * weight2;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos8) * weight3;
	//Out.vColor += g_Texture.Sample(DefaultSampler, In.vWPos9) * weight4;

	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos1) * weight4;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos2) * weight3;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos3) * weight2;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos4) * weight1;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos5) * weight0;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos6) * weight1;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos7) * weight2;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos8) * weight3;
	Out.vColor += g_Texture.Sample(DefaultSampler, In.vHPos9) * weight4;


	//GlowColor = g_ColorTexture.Sample(DefaultSampler, In.vTexUV);
	//Out.vColor = saturate(Out.vColor + (GlowColor*3.0f));

	return Out;
}

technique11 DefaultTechnique
{
	pass WBlur
	{		
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
	
		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass HBlur
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_UI, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_TWO();
	}
}


