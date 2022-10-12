#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Blur final : public CVIBuffer
{



protected:
	CVIBuffer_Blur(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CVIBuffer_Blur(const CVIBuffer_Blur& rhs);
	virtual ~CVIBuffer_Blur() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

private:
	ID3D11Buffer*			m_pVBSize = nullptr;
	D3D11_BUFFER_DESC		m_VBSizeDesc;

public:
	static CVIBuffer_Blur* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

