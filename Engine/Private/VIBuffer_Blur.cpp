#include "..\Public\VIBuffer_Blur.h"

CVIBuffer_Blur::CVIBuffer_Blur(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)
{
}

CVIBuffer_Blur::CVIBuffer_Blur(const CVIBuffer_Blur & rhs)
	: CVIBuffer(rhs)
	, m_pVBSize(rhs.m_pVBSize)
	, m_VBSizeDesc(rhs.m_VBSizeDesc)
{
	Safe_AddRef(m_pVBSize);
}

HRESULT CVIBuffer_Blur::NativeConstruct_Prototype()
{
#pragma region VERTEX_BUFFER
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.StructureByteStride = m_iStride;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;

	VTXTEX*		pVertices = new VTXTEX[m_iNumVertices];
	
	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.0f, 0.0f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.0f, 0.0f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.0f, 1.0f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.0f, 1.0f);

	ZeroMemory(&m_VBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

//#pragma region SIZE_BUFFER
//
//	ZeroMemory(&m_VBSizeDesc, sizeof(D3D11_BUFFER_DESC));
//	m_VBSizeDesc.ByteWidth = sizeof(SizeBuffer);
//	m_VBSizeDesc.StructureByteStride = 0;
//	m_VBSizeDesc.Usage = D3D11_USAGE_DYNAMIC;
//	m_VBSizeDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	m_VBSizeDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	m_VBSizeDesc.MiscFlags = 0;
//
//	if (FAILED(m_pDevice->CreateBuffer(&m_VBSizeDesc, NULL, &m_pVBSize)))
//		return E_FAIL;
//
//#pragma endregion

#pragma region INDEX_BUFFER

	m_iPrimitiveIndicesSize = sizeof(FACELISTINDICES16);
	m_iNumPrimitive = 2;
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.StructureByteStride = 0;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	
	FACELISTINDICES16*		pIndices = new FACELISTINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACELISTINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	ZeroMemory(&m_IBSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_IBSubResourceData.pSysMem = pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;
#pragma endregion

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Blur::NativeConstruct(void * pArg)
{
	//SizeBuffer* pPtr;
	//SizeBuffer* pSizebuffer = (SizeBuffer*)pArg;

	//if (nullptr == m_pDeviceContext)
	//	return E_FAIL;

	//D3D11_MAPPED_SUBRESOURCE			SubResource;

	//m_pDeviceContext->Map(m_pVBSize, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	//pPtr = (SizeBuffer*)SubResource.pData;
	//pPtr->Height = pSizebuffer->Height;
	//pPtr->Width = pSizebuffer->Width;
	//pPtr->Padding = XMVectorSet(0.f,0.f,0.f,0.f);

	//m_pDeviceContext->Unmap(m_pVBSize, 0);

	//m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pVBSize);
	return S_OK;
}

CVIBuffer_Blur * CVIBuffer_Blur::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CVIBuffer_Blur*	pInstance = new CVIBuffer_Blur(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Blur"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Blur::Clone(void * pArg)
{
	CVIBuffer_Blur*	pInstance = new CVIBuffer_Blur(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CVIBuffer_Blur"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Blur::Free()
{
	__super::Free();

	Safe_Release(m_pVBSize);

}
