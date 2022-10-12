#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Player.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CModel;
END

BEGIN(Client)

class CDollRight final : public CGameObject
{
private:
	explicit CDollRight(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CDollRight(const CDollRight& rhs);
	virtual ~CDollRight() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:	

	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;	
	CModel*				m_pModelCom = nullptr;
private:
	_float m_fTimeDeleta = 0.f;
	_float m_fPercent =1.f;
	_float m_fOriginPosY = 0.f;
	_vector m_fGO = XMVectorSet(0.f,0.f, 0.f, 0.f);
	_float m_fG = 0.f;

private:
	ID3D11ShaderResourceView*		m_pMaskSRV = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();
	HRESULT Create_MaskTexture();
	

public:
	static CDollRight* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END