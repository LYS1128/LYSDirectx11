




RasterizerState	RS_Default
{
	FillMode = Solid;
	FrontCounterClockwise = false;
	CullMode = back;
};

RasterizerState	RS_WireFrame
{
	FillMode = WireFrame;
};

RasterizerState	RS_Cull_CW
{
	FillMode = Solid;
	FrontCounterClockwise = false;
	CullMode = front;
};

RasterizerState	RS_Not_Cull
{
	FillMode = Solid;
CullMode = None;
};

RasterizerState	RS_LightDepth
{
	DepthBias = 100000;
DepthBiasClamp = 0.0f;
SlopeScaledDepthBias = 1.0f;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_UI
{
	DepthEnable = true;
DepthWriteMask = all;
DepthFunc = less_equal;
};

DepthStencilState DSS_SkyBox
{
	DepthEnable = false;
DepthWriteMask = zero;
};

BlendState BS_NonBlend
{
	BlendEnable[0] = false;
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
