#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_ExeTrail final : public CVIBuffer
{
protected:
	CVIBuffer_ExeTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_ExeTrail(const CVIBuffer_ExeTrail& rhs);
	virtual ~CVIBuffer_ExeTrail() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	HRESULT Update(_float4x4 * Mat);

private:
	_float3 m_vLocalLow = _float3(0.f,1.4f, 0.f);
	_float3 m_vLocalHigh = _float3(0.f,2.4f, 0.f);
	_matrix m_TargetMatrix;
	_uint m_iCount = 0;
	_uint m_iTickCout = 9;
	_uint m_iCatmullRomIndex[4]{};
	_uint m_iCatmullRomCount = 4;
	_uint m_iEndIndex = 10;
	_bool m_iStart = false;
public:
	static CVIBuffer_ExeTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

