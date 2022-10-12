
cbuffer Matrices
{
	float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
	float4x4		g_ViewMatrixInv, g_ProjMatrixInv;
	float4x4		g_matLightView, g_matLightProj;
}

cbuffer LightDesc
{
	float4		g_vLightDir;
	float4		g_vLightPos;
	float		g_fRange;
	float4		g_vLightDiffuse;
	float4		g_vLightAmbient;
	float4		g_vLightSpecular;
}

cbuffer MaterialDesc
{
	float4		g_vMtrlAmbient = (float4)1.f;
	float4		g_vMtrlSpecular = (float4)1.f;
	float		g_fPower = 30.0f;
}

cbuffer CameraDesc
{
	float4		g_vCamPosition;
}

texture2D		g_Texture;
texture2D		g_NormalTexture;
texture2D		g_DiffuseTexture;
texture2D		g_ShadeTexture;
texture2D		g_DepthTexture;
texture2D		g_SpecularTexture;
texture2D		g_AlphaBlendTexture;
texture2D		g_LightDepthTexture;
texture2D		g_DistortionTexture;

sampler DefaultSampler = sampler_state {
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler ShadowDepthSampler = sampler_state
{
	filter = min_mag_linear_mip_point;
	AddressU = clamp;
	AddressV = clamp;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out;

	float4		vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector			vPosition : SV_POSITION;
	float2			vTexUV : TEXCOORD0;	
};

struct PS_OUT
{
	vector			vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);	

	return Out;
}
PS_OUT PS_MAIN_LIGHTDEBUG(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor.gb = Out.vColor.r;
	Out.vColor.a = 1.f;

	return Out;
}

struct PS_OUT_LIGHT
{
	vector			vShade : SV_TARGET0;
	vector			vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y * 500.f;

	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient);
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(g_vLightDir), vNormal);
		
	vector			vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y * 500.f;

	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector			vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector			vLightDir = vWorldPos - g_vLightPos;
	float			fDistance = length(vLightDir);

	float			fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	Out.vShade.a = 1.f;

	vector			vReflect = reflect(normalize(vLightDir), vNormal);	

	vector			vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = pow(saturate(dot(normalize(vReflect) * -1.f,  normalize(vLook))), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular) * fAtt;

	return Out;
}

PS_OUT PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vAlphaBlend = g_AlphaBlendTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y* 500.f;

	
	vector fogColor = vector(0.15f, 0.15f, 0.13f, 1.f);
	float fogfactor = 1.0 / pow(2.71828, vDepthDesc.x * fViewZ * 0.008);
	if(vDiffuse.a != 0)
		vDiffuse = vDiffuse*fogfactor + (1.0 - fogfactor) * fogColor;

	//Out.vColor = (vDiffuse * vShade + vSpecular);	

	vector			vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos = mul(vWorldPos, g_matLightView);
	vWorldPos = mul(vWorldPos, g_matLightProj);
	float2		vNewUV;

	vNewUV.x = (vWorldPos.x / vWorldPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vWorldPos.y / vWorldPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo = g_LightDepthTexture.Sample(ShadowDepthSampler, vNewUV);
	if ((vWorldPos.z / vWorldPos.w) - 0.001f > vShadowDepthInfo.r)
		vDiffuse *= vector(0.5f, 0.5f, 0.5f, 1.f);

	Out.vColor = (vDiffuse * vShade + vSpecular)*(1 - vAlphaBlend.a) + (vAlphaBlend);

	if (0.0f == Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_RED(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vAlphaBlend = g_AlphaBlendTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = (vDiffuse * vShade + vSpecular)*(1 - vAlphaBlend.a) + (vAlphaBlend);
	//Out.vColor = (vDiffuse * vShade + vSpecular);
	if (0.0f == Out.vColor.a)
		discard;

	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y* 500.f;

	vector			vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos = mul(vWorldPos, g_matLightView);
	vWorldPos = mul(vWorldPos, g_matLightProj);
	float2		vNewUV;

	vNewUV.x = (vWorldPos.x / vWorldPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vWorldPos.y / vWorldPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo = g_LightDepthTexture.Sample(ShadowDepthSampler, vNewUV);
	if ((vWorldPos.z / vWorldPos.w) - 0.001f > vShadowDepthInfo.r)
		Out.vColor *= vector(0.5f, 0.5f, 0.5f, 1.f);

	//Out.vColor = Out.vColor*(1 - vAlphaBlend.a) + (vAlphaBlend);

	float Col1, Col2;
	if (Out.vColor.g >= Out.vColor.b) {
		Col1 = Out.vColor.g;
		Col2 = Out.vColor.b;
	}
	else {
		Col1 = Out.vColor.b;
		Col2 = Out.vColor.g;
	}

	if (Out.vColor.r < Col1+(70.f/255.f) || Col1 - Col2 >(10.f / 255.f) || Out.vColor.r < (50.f / 255.f) || Out.vColor.r + Col1 + Col2 > 1.2f)
		Out.vColor.rb = Out.vColor.g;
	return Out;
}

PS_OUT PS_MAIN_DISTORTION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vAlphaBlend = g_AlphaBlendTexture.Sample(DefaultSampler, In.vTexUV);


	vector			vDistortion = g_DistortionTexture.Sample(DefaultSampler, In.vTexUV);
	float fWeight = vDistortion.r * 0.02f;

	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV +fWeight);

	Out.vColor = (vDiffuse * vShade + vSpecular)*(1 - vAlphaBlend.a) + (vAlphaBlend);
	//Out.vColor = (vDiffuse * vShade + vSpecular);
	if (0.0f == Out.vColor.a)
		discard;

	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	float			fViewZ = vDepthDesc.y* 500.f;

	vector			vWorldPos;
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	vWorldPos = vWorldPos * fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos = mul(vWorldPos, g_matLightView);
	vWorldPos = mul(vWorldPos, g_matLightProj);
	float2		vNewUV;

	vNewUV.x = (vWorldPos.x / vWorldPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vWorldPos.y / vWorldPos.w) * -0.5f + 0.5f;

	vector		vShadowDepthInfo = g_LightDepthTexture.Sample(ShadowDepthSampler, vNewUV);
	if ((vWorldPos.z / vWorldPos.w) - 0.001f > vShadowDepthInfo.r)
		Out.vColor *= vector(0.5f, 0.5f, 0.5f, 1.f);

	//Out.vColor = Out.vColor*(1 - vAlphaBlend.a) + (vAlphaBlend);

	Out.vColor = saturate(Out.vColor+vector(0.5f,0.f,0.5f,0.f));

	


	return Out;
}



RasterizerState	RS_Default
{
	FillMode = Solid;
	FrontCounterClockwise = false;
	CullMode = back;
};


BlendState BS_NonBlend
{
	BlendEnable[0] = false;
	BlendEnable[1] = false;
};

BlendState BS_LightBlend
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;
	SrcBlend = one;
	DestBlend = one;
	Blendop = add;

	/*BlendEnable[0] = true;
	SrcBlend[0] = one;
	DestBlend[0] = one;
	Blendop[0] = add;*/

	//BlendEnable[1] = true;
	//SrcBlend[1] = one;
	//DestBlend[1] = one;
	//Blendop[1] = add;
	
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	SrcBlend[0] = Src_Alpha;
	DestBlend[0] = Inv_Src_Alpha;
	BlendOp[0] = Add;
	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = Zero;
	BlendOpAlpha[0] = Add;

};


DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_NonZTestAndWrite
{
	DepthEnable = false;
	DepthWriteMask = zero;
};


technique11 DefaultTechnique
{
	
	pass Debug_RenderTarget
	{		
		SetRasterizerState(RS_Default);		
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_LightBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass Blend
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}
	pass Blend_Red
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RED();
	}
	pass LightDebug_RenderTarget
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_NonBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHTDEBUG();
	}
	pass Blend_Distortion
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_NonZTestAndWrite, 0);
		SetBlendState(BS_AlphaBlend, vector(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
	}

}


