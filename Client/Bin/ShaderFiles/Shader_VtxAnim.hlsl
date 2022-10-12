
#include "Shader_Defines.hpp"

struct BoneMatrix
{
	float4x4		BoneMatrices[128];
};

cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
	BoneMatrix		g_BoneMatrices;
	float4x4		g_SocketMatrix;
}

texture2D			g_DiffuseTexture;
texture2D			g_NormalTexture;
texture2D			g_DissolveTexture;
unsigned int		g_Count;
vector				g_CamDirection;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state {
	filter = min_mag_mip_point;
	AddressU = wrap;
	AddressV = wrap;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorldPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	float4x4	BoneMatrix =
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * fWeightW;


	float4x4	matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4		vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
	Out.vPosition = mul(vPosition, matWVP);

	float4		vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));

	float3		vTangent = mul(float4(In.vTangent, 0.f), BoneMatrix);
	Out.vTangent = normalize(mul(vector(vTangent, 0.f), g_WorldMatrix)).xyz;

	Out.vBinormal = cross(Out.vNormal.xyz, Out.vTangent);

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);

	return Out;
}

VS_OUT VS_MAIN_WEAPON(VS_IN In)
{
	VS_OUT		Out;

	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	float4x4	BoneMatrix =
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * fWeightW;

	matrix			matWS, matWSV, matWSVP;

	matWS = mul(g_WorldMatrix, g_SocketMatrix);
	matWSV = mul(matWS, g_ViewMatrix);
	matWSVP = mul(matWSV, g_ProjMatrix);

	vector		vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
	Out.vPosition = mul(vPosition, matWSVP);
	vector		vNormal = mul(float4(In.vNormal, 0.f), BoneMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	float3		vTangent = mul(float4(In.vTangent, 0.f), BoneMatrix);
	Out.vTangent = normalize(mul(vector(vTangent, 0.f), g_WorldMatrix)).xyz;

	Out.vBinormal = cross(Out.vNormal.xyz, Out.vTangent);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float4			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vProjPos : TEXCOORD1;
	
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4			vWorldPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vDiffuse : SV_TARGET0;
	vector			vNormal : SV_TARGET1;
	vector			vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(vNormal.xyz* 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.0f, 0.f);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	return Out;
}

PS_OUT PS_DISSOLVE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(vNormal.xyz* 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.0f, 0.f);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	vector		vDissolve = g_DissolveTexture.Sample(DefaultSampler, In.vTexUV);

	if (g_Count / 255.f > vDissolve.r)
		discard;

	return Out;
}

PS_OUT PS_RIMLIGHTRED (PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(vNormal.xyz* 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.0f, 0.f);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	vector ToCam = normalize(g_CamDirection - In.vWorldPos);
	float Rim =  smoothstep(0.2f,1.0f,1.f - max(0.f,dot(vNormal, ToCam)));

	Out.vDiffuse += Rim*vector(1.f,0.f,0.f,0.f);

	return Out;
}

PS_OUT PS_RIMLIGHTWHITE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(vNormal.xyz* 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.0f, 0.f);

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	vector ToCam = normalize(g_CamDirection - In.vWorldPos);
	float Rim = smoothstep(0.2f, 1.0f, 1.f - max(0.f, dot(vNormal, ToCam)));

	Out.vDiffuse += Rim;

	return Out;
}

PS_OUT PS_MAIN_HEART(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vDiffuse = vMtrlDiffuse;

	Out.vNormal = vector(vNormal.xyz* 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 300.f, 0.0f, 0.f);

	Out.vDiffuse.r = saturate(Out.vDiffuse.r * (((float)g_Count / 10.f) + 1.f));

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	/* Pass를 나누는 기준 == 그릴려고하는 리소스에게 어떤 셰이더 세트 효과를 줄것인지?! */

	/* 명암 + 그림자 + 노멀 */
	pass DefaultRendering
	{		
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
	/*	SetDepthStencilState();
			*/			

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass Weapon
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN_WEAPON();
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Dead
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE();
	}

	pass RIMRED
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RIMLIGHTRED();
	}

	pass RIMWHITE
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RIMLIGHTWHITE();
	}

	pass Heart
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		/*	SetDepthStencilState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HEART();
	}

}


