#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pScene(rhs.m_pScene)
	, m_MeshContainers(rhs.m_MeshContainers)
	, m_iNumMeshContainers(rhs.m_iNumMeshContainers)
	, m_Materials(rhs.m_Materials)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Animations(rhs.m_Animations)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)	
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eType(rhs.m_eType)
{
	for (auto& pMeshContainer : m_MeshContainers)
		Safe_AddRef(pMeshContainer);	

	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_AddRef(pMaterial.pMaterials[i]);
		}
	}

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);
}

_float4x4 * CModel::Get_CombinedTransformationMatrix(const char * pNodeName)
{
	CHierarchyNode*		pNode = nullptr;

	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		if (!strcmp(pNodeName, pHierarchyNode->Get_Name()))
		{
			pNode = pHierarchyNode;
			break;
		}
	}

	if (nullptr == pNode)
		return nullptr;

	return pNode->Get_CombinedFloat4x4Ptr();
}

HRESULT CModel::NativeConstruct_Prototype(TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	//1@�� ���θ� �غ���. �δ����� � �Ž��� Add_Prototype�� �Ҹ��� ����� ���� �ȴ�.
	//eType�� NONANIM�̰ų� ANIM�̴�
	//pModelFilePath�� ���� �ִ� ���� ����̴�. ex) "../Bin/Resources/Meshes/Fiona/"
	//pModelFileName�� ���� �̸��̴�. ex) "SK_AliceW_End.fbx"
	//PivotMatrix�� ������ 0.01f�� 180�� ȸ���� ����ִ� ����̴�

	//�켱 ��ζ� ���ϸ��� ��ģ��
	char			szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);	

	//Ÿ�Ե� �޾ƿ´�
	m_eType = eType;

	_uint		iFlag;

	//�ִϸ��̼ǸŽ��� �ƴҰ�� aiProcess_PreTransformVertices �̰� ���ĸ� ���������� �ִ������� �����ϰ� ���������� �����ϴ� ���� �뷮�̳������پ�峪��
	//aiProcess_ConvertToLeftHanded �̰� �޼� ��ǥ��� �ε�
	//aiProcess_CalcTangentSpace ź��Ʈ�� ��ź��Ʈ ���� ����??�̰Թ���
	//aiProcess_Triangulate �ﰢ �Ž� ���·� �����ϰڴٴ� ��

	if (TYPE_NONANIM == m_eType)	
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;	

	//���� ��θ� ���󰡼� ���� ���� �÷��� ��Ģ��� �о Scene���ٰ� ����
	m_pScene = m_Importer.ReadFile(szFullPath, iFlag);
	if (nullptr == m_pScene)
		return E_FAIL;

	//������ �޾ƿ� �ǹ������ �־���
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	//H��¼����带 �غ��ϴ� �Լ��� ����!!!
	if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode)))
		return E_FAIL;

	//5@������ ���� H������ ���� �������� �����Ѵ�
	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	//6@�Ž������̳ʸ� �غ��ϴ� �Լ��� ����!!!
	if (FAILED(Ready_MeshContainers()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	if (FAILED(Ready_HierarchyNodes(m_pScene->mRootNode)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pSour, CHierarchyNode* pDest)->_bool
	{
		return pSour->Get_Depth() < pDest->Get_Depth();
	});

	if (FAILED(Clone_MeshContainers()))
		return E_FAIL;

	if (FAILED(Clone_Animations()))
		return E_FAIL;

	return S_OK;
}

_bool CModel::Update(_double TimeDelta)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations)
		return false;

	if (m_iCurrentAnimIndex != m_iPreAnimIndex) {	
		if (m_bGetPreKey == false) {
  			m_PreKey.clear();
			m_PreKey = m_Animations[m_iPreAnimIndex]->Pre_Change_Animation(TimeDelta);
			m_bGetPreKey = true;
			m_Animations[m_iCurrentAnimIndex]->Set_TimeAcc_Zero();
			m_bChangeEnd = false;
		}
		if (m_Animations[m_iCurrentAnimIndex]->Change_Animation(m_PreKey, TimeDelta)) {
			m_Animations[m_iPreAnimIndex]->Set_KeyFrameZero();
			m_Animations[m_iPreAnimIndex]->Set_TimeAcc_Zero();
			m_Animations[m_iPreAnimIndex]->Set_KeyFrameEnd(false);
			m_iPreAnimIndex = m_iCurrentAnimIndex;
			m_bChangeEnd = true;
			m_bGetPreKey = false;
		}
	}
	else {
		m_Animations[m_iCurrentAnimIndex]->Update(TimeDelta, m_bCut, m_fSpeed);
	}
	/* ���� �ִԿ��� ���Ǵ� ������ �������(m_TransformationMatrix)�� �����ߴ�. */
	

	/* ������ ��ȸ�ϸ鼭(�θ𿡼����� �ڽ�����) ���ŵ� m_TransformationMatrix�� �θ������ ���Ͽ� �����Ѵ�. */
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombinedTransformationMatrix();
	}

	return m_bChangeEnd;
}

HRESULT CModel::Render(CShader* pShader, const char* pBoneMatricesName, _uint iMeshContainerIndex, _uint iPassIndex)
{
	if (iMeshContainerIndex >= m_iNumMeshContainers)
		return E_FAIL;

	if (TYPE_ANIM == m_eType)
	{	
		_float4x4		BoneMatrices[128];

		ZeroMemory(BoneMatrices, sizeof(_float4x4) * 128);

		m_MeshContainers[iMeshContainerIndex]->Get_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

		pShader->Set_RawValue(pBoneMatricesName, BoneMatrices, sizeof(_float4x4) * 128);
	}

	pShader->Begin(iPassIndex);
	
	if (nullptr != m_MeshContainers[iMeshContainerIndex])
		m_MeshContainers[iMeshContainerIndex]->Render();	

	return S_OK;
}

HRESULT CModel::Bind_Material_OnShader(CShader * pShader, aiTextureType eType, const char * pConstantName, _uint iMeshContainerIndex)
{
	_uint		iMaterialIndex = m_MeshContainers[iMeshContainerIndex]->Get_NumMaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (nullptr == m_Materials[iMaterialIndex].pMaterials[eType])
		return S_OK;


	return m_Materials[iMaterialIndex].pMaterials[eType]->SetUp_ShaderResourceView(pShader, pConstantName, 0);	
}

HRESULT CModel::Ready_MeshContainers()
{
	//7@�Ž������̳ʸ� �غ��غ���...
	if (nullptr == m_pScene)
		return E_FAIL;

	//�켱 Scene���� �Ž������� �޾ƿ´�
	m_iNumMeshContainers = m_pScene->mNumMeshes;

	//�޾ƿ� �Ž�������ŭ �Ž������̳ʸ� �����
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		/* ���������� �о ������ �޽������̳� ��ü�� �ش� ������ �����Բ� �����!!!*/
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, m_eType, m_pScene->mMeshes[i], m_PivotMatrix, m_HierarchyNodes);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Clone_MeshContainers()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumMeshContainers = m_pScene->mNumMeshes;

	vector<CMeshContainer*>		MeshContainer;

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		CMeshContainer*		pMeshContainer = (CMeshContainer*)m_MeshContainers[i]->Clone(&m_HierarchyNodes);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		Safe_Release(m_MeshContainers[i]);

		MeshContainer.push_back(pMeshContainer);
	}
	m_MeshContainers.clear();

	m_MeshContainers = MeshContainer;

	return S_OK;	
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumMaterials = m_pScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*			pAIMaterial = m_pScene->mMaterials[i];
		if (nullptr == pAIMaterial)
			return E_FAIL;

		MODELMATERIAL		Materials;
		ZeroMemory(&Materials, sizeof(MODELMATERIAL));		

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char		szTextureFilePath[MAX_PATH] = "";

			aiString	strPath;		
			
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char		szFileName[MAX_PATH] = "";
			char		szEXT[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			strcpy_s(szTextureFilePath, pModelFilePath);
			strcat_s(szTextureFilePath, szFileName);
			strcat_s(szTextureFilePath, szEXT);

			_tchar		szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szTextureFilePath, strlen(szTextureFilePath), szFullPath, MAX_PATH);

			Materials.pMaterials[j] = CTexture::Create(m_pDevice, m_pDeviceContext, szFullPath);
			if (nullptr == Materials.pMaterials[j])
				return E_FAIL;				
		}
		m_Materials.push_back(Materials);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	if (nullptr == m_pScene)
		return E_FAIL;

	m_iNumAnimations = m_pScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];		

		CAnimation*		pAnimation = CAnimation::Create(pAIAnimation, m_HierarchyNodes);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Clone_Animations()
{
	if (nullptr == m_pScene)
		return E_FAIL;	

	vector<CAnimation*>		Animations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pScene->mAnimations[i];

		CAnimation*		pAnimation = m_Animations[i]->Clone(pAIAnimation, m_HierarchyNodes);
		if (nullptr == pAnimation)
			return E_FAIL;

		Safe_Release(m_Animations[i]);

		Animations.push_back(pAnimation);
	}

	m_Animations = Animations;

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	//2@ù��° ���� Scene�������� ������ Node����, �ι����Ͱ� ����°���� �޾ƿ��� �ʾҴٸ� ���� nullptr�� 0��

	if (nullptr == m_pScene)
		return E_FAIL;		

	//��������κ��� �ڽĳ���� ������ �о��
	_uint	iNumChildren = pNode->mNumChildren;

	//H��带 ������ִ� �Լ��� ����!!!
	CHierarchyNode*			pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	//�̰��� �θ� �ִ� �ڱ���� ��� ����� ��...? �Ƹ���? ���� ���� ����Լ���
	for (_uint i = 0; i < iNumChildren; ++i)
	{		
		if (FAILED(Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1)))
			return E_FAIL;	
	}

	//4@���� ��带 ���Ϳ� Ǫ�ù��ؼ� ����
	m_HierarchyNodes.push_back(pHierarchyNode);

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);

	m_HierarchyNodes.clear();



	for (auto& pMaterial : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			Safe_Release(pMaterial.pMaterials[i]);
		}		
	}
	m_Materials.clear();

	m_Importer.FreeScene();
}
