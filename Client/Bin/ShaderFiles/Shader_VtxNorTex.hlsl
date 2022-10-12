
#include "Shader_Defines.hpp"

float	g_fPercent;
cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
	float4x4		g_SocketMatrix;
}

cbuffer LightDesc
{
	vector		g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
	vector		g_vLightPos;
	float		g_fRange;

	vector		g_vLightDiffuse = (vector)1.f;
	vector		g_vLightAmbient = (vector)1.f;
	vector		g_vLightSpecular = (vector)1.f;
};
texture2D			g_NormalTexture;

cbuffer MtrlDesc
{
	vector		g_vMtrlDiffuse;
	vector		g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
	vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
	float		g_fPower = 30.f;
};

cbuffer BrushDesc
{
	vector		g_vBrushPosition = vector(15.f, 0.f, 15.f, 1.f);
	float		g_fBrushRange = 5.f;
};

cbuffer CamPosition
{
	vector		g_vCamPosition;
};

texture2D			g_SourDiffuseTexture;
texture2D			g_DestDiffuseTexture;
texture2D			g_MaskTexture;
texture2D			g_BrushTexture;
texture2D			g_DiffuseTexture;

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

};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
	float4		vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN_TERRAIN(VS_IN In)
{
	VS_OUT		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 0.f), g_WorldMatrix);

	return Out;
}

VS_OUT VS_MAIN_MODEL(VS_IN In)
{
	VS_OUT		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 0.f), g_WorldMatrix);

	return Out;
}

VS_OUT VS_MAIN_WEAPON(VS_IN In)
{
	VS_OUT		Out;

	/*float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);*/

	matrix			matWS, matWSV, matWSVP;

	matWS = mul(g_WorldMatrix, g_SocketMatrix);
	matWSV = mul(matWS, g_ViewMatrix);
	matWSVP = mul(matWSV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWSVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(vector(In.vPosition, 0.f), g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	vector			vPosition : SV_POSITION;
	float4			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_TERRAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out;	

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);

	vector		vSourDiffuse = g_SourDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 10.f);
	vector		vDestDiffuse = g_DestDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 10.f);
	vector		vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);
	vector		vBrush = vector(0.f, 0.f, 0.f, 0.f);

	if (g_vBrushPosition.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPosition.x + g_fBrushRange && 
		g_vBrushPosition.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPosition.z + g_fBrushRange)
	{
		float2		vTexUV;
		vTexUV.x = (In.vWorldPos.x - (g_vBrushPosition.x - g_fBrushRange)) / (2.f * g_fBrushRange);
		vTexUV.y = ((g_vBrushPosition.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = g_BrushTexture.Sample(DefaultSampler, vTexUV);
	}

	vector		vMtrlDiffuse = vDestDiffuse * vMask + vSourDiffuse * (1.f - vMask) + vBrush;

	vector		vReflect = reflect(normalize(g_vLightDir), In.vNormal);
	vector		vLook = normalize(In.vWorldPos - g_vCamPosition);

	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), g_fPower);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	return Out;
}

PS_OUT PS_MAIN_TERRAIN_POINT(PS_IN In)
{
	PS_OUT			Out;

	vector		vLightDir = In.vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	float		fShade = max(dot(normalize(vLightDir) * -1.f, In.vNormal), 0.f) * fAtt;

	vector		vMtrlDiffuse = g_SourDiffuseTexture.Sample(DefaultSampler, In.vTexUV * 10.f);

	vector		vReflect = reflect(normalize(vLightDir), In.vNormal);
	vector		vLook = normalize(In.vWorldPos - g_vCamPosition);

	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), g_fPower) * fAtt;

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
		(g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

	return Out;
}


PS_OUT PS_MAIN_MODEL_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector		vReflect = reflect(normalize(g_vLightDir), In.vNormal);
	vector		vLook = normalize(In.vWorldPos - g_vCamPosition);

	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, vLook), 0.f), g_fPower);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse);

	Out.vColor.a = vMtrlDiffuse.a;

	if (Out.vColor.a < 0.3f)
		discard;


	return Out;
}

PS_OUT PS_MAIN_SKYBOX_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse)/* * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
												 (g_vLightSpecular * g_vMtrlSpecular) * fSpecular*/;

	Out.vColor.a = vMtrlDiffuse.a;

	if (Out.vColor.a < 0.3f)
		discard;


	return Out;
} 

PS_OUT PS_MAIN_SKYBOX_RED(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse)/* * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
												 (g_vLightSpecular * g_vMtrlSpecular) * fSpecular*/;

	Out.vColor.a = vMtrlDiffuse.a;

	if (Out.vColor.a < 0.3f)
		discard;

	float Col1, Col2;
	if (Out.vColor.g >= Out.vColor.b) {
		Col1 = Out.vColor.g;
		Col2 = Out.vColor.b;
	}
	else {
		Col1 = Out.vColor.b;
		Col2 = Out.vColor.g;
	}

	if (Out.vColor.r < Col1 + (70.f / 255.f) || Col1 - Col2 >(10.f / 255.f) || Out.vColor.r < (50.f / 255.f) || Out.vColor.r + Col1 + Col2 > 1.2f)
		Out.vColor.rb = Out.vColor.g;
	return Out;
}

PS_OUT PS_MAIN_SKYBOX_DISTORTION(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse)/* * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
												 (g_vLightSpecular * g_vMtrlSpecular) * fSpecular*/;

	Out.vColor.a = vMtrlDiffuse.a;

	Out.vColor = saturate(Out.vColor + vector(0.5f, 0.f, 0.5f, 0.f));
	return Out;
}

PS_OUT PS_SHELLPART(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, In.vNormal), 0.f);
	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse)/* * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) +
												 (g_vLightSpecular * g_vMtrlSpecular) * fSpecular*/;

	//Out.vColor.a = vMtrlDiffuse.a;
	if (Out.vColor.a < 0.1f)
			discard;
	else if (Out.vColor.a >= 0.1f)
		Out.vColor.a = Out.vColor.a * g_fPercent;



	return Out;
}

technique11 DefaultTechnique
{	
	pass Terrain_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_DIRECTIONAL();
	}	

	pass Terrain_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TERRAIN();
		PixelShader = compile ps_5_0 PS_MAIN_TERRAIN_POINT();
	}

	pass Model
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		PixelShader = compile ps_5_0 PS_MAIN_MODEL_DIRECTIONAL();
	}

	pass SkyBox
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_SkyBox, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		PixelShader = compile ps_5_0 PS_MAIN_SKYBOX_DIRECTIONAL();
	}

	pass Weapon
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_WEAPON();
		PixelShader = compile ps_5_0 PS_MAIN_MODEL_DIRECTIONAL();
	}

	pass Shell
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		PixelShader = compile ps_5_0 PS_SHELLPART();
	}

	pass Alpha
	{
		SetRasterizerState(RS_Not_Cull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		PixelShader = compile ps_5_0 PS_MAIN_MODEL_DIRECTIONAL();
	}

	pass SkyBoxRed
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_SkyBox, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		PixelShader = compile ps_5_0 PS_MAIN_SKYBOX_RED();
	}
	pass SkyBoxDistortion
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_SkyBox, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_MODEL();
		PixelShader = compile ps_5_0 PS_MAIN_SKYBOX_DISTORTION();
	}
}


