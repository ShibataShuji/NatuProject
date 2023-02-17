#include "stdafx.h"


void Scene::SetCollisionVisibility_GameObjectList(bool visibility)
{
	m_Collision_Visility = visibility;

	//std::list<GameObject*> a = GetGameObjectList(1);

	for (auto gameobject : m_GameObjectList[1])
	{
		std::list<CComponent*> complist = gameobject->GetComponentList();
		for (auto comp : complist)
		{
			Collision* collision = dynamic_cast<Collision*>(comp);
			if (collision != nullptr)
			{
				collision->SetVisibility(visibility);
			}
		}
	}
}



void Scene::Init()
{
	//m_SecenChange = true;

	// �v���C���[�ƃJ�����̃|�C���^�̏�����
	m_Player = nullptr;
	m_Camera = nullptr;

	// �n�ʂ�-100.0f�ŏ���������B��ɂ��������s����Ă���A������p�����Ă�V�[����Init���Ă�ł�̂ŁA�ݒ肵������΂������ł��
	m_GroundMinHeight = -100.0f;
}

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* object : m_GameObjectList[i])
		{
			assert(object);
			object->Uninit();
			delete object;
		}
		m_GameObjectList[i].clear();	// ���X�g�̃N���A
		assert(m_GameObjectList[i].empty());
	}

	for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
	{
		meshfieldobject->Uninit();
		delete meshfieldobject;
	}
	m_MeshFieldObjectList.clear();
	assert(m_MeshFieldObjectList.empty());

	m_Player = nullptr;
	m_Camera = nullptr;
}

void Scene::Update()
{
	//// �z�u�����u���b�N�̃Z�[�u�A��������
	//if (Input::GetKeyTrigger(DIK_P))
	//{
	//	m_Savedata->Save();
	//}
	//if (Input::GetKeyTrigger(DIK_O))
	//{
	//	m_Savedata->Load();
	//}
	//

	// �V�[���`�F���W���Ă��̃��Z�b�g
	m_SecenChange = false;

	if (!m_GameObjectList->empty())
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == 1)
			{
				// �A�b�v�f�[�g����I�u�W�F�N�g�����A�b�v�f�[�g����
				std::list<GameObject*> updateobjectlist = GetUpdateGameObjectList();

				for (auto object : updateobjectlist)
				{
					object->Update();
					if (m_SecenChange || m_GameObjectList->empty())
						return;
				}
			}
			else
			{
				for (GameObject* object : m_GameObjectList[i])	// �͈�for���[�v
				{
					object->Update();

					if (m_SecenChange || m_GameObjectList->empty())
						return;

				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			// Destroy�֐��ŏ����\�񂳂�Ă���������B���������񂶂�Ȃ��Ă����ŏ������Ƃŏ����ɖ�肪�������Ȃ�
			m_GameObjectList[i].remove_if([](GameObject* object) {return object->Destroy(); });		// �����_��
		}
	}

	if (!m_MeshFieldObjectList.empty())
	{
		for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
		{
			meshfieldobject->Update();
		}

		m_MeshFieldObjectList.remove_if([](MeshFieldObject* meshfieldobject) {return meshfieldobject->Destroy(); });		// �����_��
	}
}


void Scene::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		if (i == 1)
		{
			// �\������I�u�W�F�N�g�����`�ʂ���
			std::list<GameObject*> drawobjectlist = GetDrawGameObjectList();

			for (auto object : drawobjectlist)
			{
				object->Draw();
			}
		}
		else
		{
			for (GameObject* object : m_GameObjectList[i])
			{
				object->Draw();
			}
		}
	}

	// �`�ʂ���t�B�[���h�����`�ʂ���
	if (m_Player != nullptr)
	{
		//assert(m_Player);

		std::list<MeshFieldObject*> drawfieldlist = GetDrawMeshFieldObjectList();

		for (MeshFieldObject* meshfieldobject : drawfieldlist)
		{
			meshfieldobject->Draw();
		}
	}
	else if (m_Camera != nullptr)
	{
		std::list<MeshFieldObject*> drawfieldlist = GetDrawMeshFieldObjectList();

		for (MeshFieldObject* meshfieldobject : drawfieldlist)
		{
			meshfieldobject->Draw();
		}
	}
	else
	{
		for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
		{
			meshfieldobject->Draw();
		}
	}

}


void Scene::ShadowDraw()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* object : m_GameObjectList[i])
		{
			if (object->GetUseShadow())
				object->Draw();
		}
	}

	for (MeshFieldObject* meshfieldobject : m_MeshFieldObjectList)
	{
		if (meshfieldobject->GetUseShadow())
			meshfieldobject->Draw();
	}
}


//	�������̃|�W�V��������A�`�ʂ��郁�b�V���t�B�[���h�𔲐��������X�g���擾
std::list<MeshFieldObject*> Scene::GetDrawMeshFieldObjectList()
{
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	if (m_Camera != nullptr)
	{
		float drawCenterLength = 20.0f;
		D3DXVECTOR3 cameraDirection = m_Camera->GetCameraForwardIgnoreY();
		D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
		pos = cameraPos + cameraDirection * drawCenterLength;
	}


	std::list<MeshFieldObject*> re;

	// ���݂���G���A�u���b�N�����߂�
	Int2   areablockI = GetAreaBlockInt2(pos);
	Float2 areablockF = GetAreaBlockFloat2(pos);

	// ���݂̃G���A�u���b�N��ǉ�����
	MeshFieldObject* additem = GetMeshFieldObject(areablockI);
	if(additem != nullptr)
		re.push_back(additem);

	{
		Int2 viewareablock = areablockI;
		// x��
		{
			if (FindoutBeforethehalf(areablockF.x, 1.0f))
			{
				// �O����������ׂ̃G���A�u���b�N��ǉ�����
				viewareablock.x -= 1;
			}
			else
			{
				// �㔼��������ׂ̃G���A�u���b�N��ǉ�����
				viewareablock.x += 1;
			}
		}
		// y��
		{
			if (FindoutBeforethehalf(areablockF.y, 1.0f))
			{
				// �O����������ׂ̃G���A�u���b�N��ǉ�����
				viewareablock.y -= 1;
			}
			else
			{
				// �㔼��������ׂ̃G���A�u���b�N��ǉ�����
				viewareablock.y += 1;

			}
		}
		// �`�ʂ���G���A�u���b�N��ǉ�����
		MeshFieldObject* additem;
		// x
		additem = GetMeshFieldObject(Int2(viewareablock.x, areablockI.y));
		if (additem != nullptr)
			re.push_back(additem);
		// z
		additem = GetMeshFieldObject(Int2(areablockI.x, viewareablock.y));
		if (additem != nullptr)
			re.push_back(additem);
		// xz
		additem = GetMeshFieldObject(Int2(viewareablock.x, viewareablock.y));
		if (additem != nullptr)
			re.push_back(additem);



	}
	return re;
}


// �������̍��W�ł̃��b�V���t�B�[���h�I�u�W�F�N�g�̂��̃|���S���ł̍��������߂�B�����Ƀt�B�[���h���Ȃ��ꍇ-100��Ԃ��B
float Scene::GetMeshFieldObjectHeight(Int2* pOut_block, const D3DXVECTOR3& pos)
{
	// ���݂���G���A�u���b�N�����߂�
	Int2   areablockI = GetAreaBlockInt2(pos);
	*pOut_block = areablockI;

	// ���݂̃G���A�u���b�N�̃��b�V���t�B�[���h�I�u�W�F�N�g�����߂�
	MeshFieldObject* field = GetMeshFieldObject(areablockI);
	if (field == nullptr)
		return -100.0f;

	return field->GetHeight(pos);
}

// ���̃u���b�N�̃��b�V���̍����Ɩ@����Ԃ��B���������u���b�N�����b�V���͈̔͊O��������false��Ԃ��B
bool Scene::GetMeshFieldObjectHeightandNormal(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, const D3DXVECTOR3& pos)
{
	// ���݂���G���A�u���b�N�����߂�
	Int2   areablockI = GetAreaBlockInt2(pos);
	*pOut_block = areablockI;

	// ���݂̃G���A�u���b�N�̃��b�V���t�B�[���h�I�u�W�F�N�g�����߂�
	MeshFieldObject* field = GetMeshFieldObject(areablockI);
	if (field == nullptr)
		return false;

	return field->GetHeightandNormal(pOut_height, pOut_normal, pos);
}

// ���̃u���b�N�̃��b�V���̍����Ɩ@����Ԃ��B�Փ˂��Ă�����1~3��Ԃ�
int8_t Scene::CheckCollisionMeshFieldObjects(Int2* pOut_block, float* pOut_height, D3DXVECTOR3* pOut_normal, D3DXVECTOR3* pOut_beforpos, const D3DXVECTOR3& pos)
{
	// ���݂���G���A�u���b�N�����߂�
	Int2   areablockI = GetAreaBlockInt2(pos);
	*pOut_block = areablockI;

	// ���݂̃G���A�u���b�N�̃��b�V���t�B�[���h�I�u�W�F�N�g�����߂�
	MeshFieldObject* field = GetMeshFieldObject(areablockI);
	if (field == nullptr)
		return 0;

	return field->CheckCollisionMeshFieldObject(pOut_height, pOut_normal, pOut_beforpos, pos);
}


//// �Q�[���I�u�W�F�N�g�S�Ă̊ȈՃo�E���f�B���O�{�b�N�X2D��\���I�t�ɂ���
//void Scene::SetALLSimpleBoundingBox2DUseDraw(const bool& usedraw)
//{
//	for (GameObject* object : m_GameObjectList[1])
//	{
//		object->SetSimpleBoundingBox2DUseDraw(false);
//	}
//	return;
//}

// �Q�[���I�u�W�F�N�g�S�Ă̊ȈՃo�E���f�B���O�{�b�N�X3D��\���I�t�ɂ���
void Scene::SetALLSimpleBoundingBox3DUseDraw(const bool& usedraw)
{
	for (GameObject* object : m_GameObjectList[1])
	{
		object->SetSimpleBoundingBox3DUseDraw(false);
	}
	return;
}

// �Q�[���I�u�W�F�N�g�S�Ă̊ȈՃo�E���f�B���O�{�b�N�X3D�ƁA�}�E�X�J�[�\���ŏՓ˔�����Ƃ�B1�ԋ߂�����GameObject��Ԃ�
GameObject* Scene::GetCollisionRaySimpleBoundingBox3D(D3DXVECTOR3* q1, D3DXVECTOR3* q2)
{
	// �}�E�X�����[���h���W�ɕϊ�
	D3DXVECTOR3 MouseWorldOrigin;
	D3DXVECTOR3 MouseWorldDirection;
	POINT Sp = GetScreenMousePosition();
	m_Camera->ScreenToWorld(&MouseWorldOrigin, &MouseWorldDirection, Sp.x, Sp.y);

	float lengthminsq = 999.0f;	// �ŒZ����sq
	GameObject* minobject = nullptr;

	for (GameObject* object : m_GameObjectList[1])
	{
		// OriginalBlock����Ȃ������玟�ցB�Ⴆ�΋�Ƃ��������炻��̓��C�Ɋ܂߂����Ȃ��̂�
		OriginalBlock* OBobject = dynamic_cast<OriginalBlock*>(object);
		if (OBobject == nullptr)
			continue;

		float radius = object->GetSimpleBoundingBox3DRadius() * 1.20f;	// ���������傫�߂ɂƂ�
		D3DXVECTOR3 pos = object->GetPosition();

		if (calcRaySphere(MouseWorldOrigin, MouseWorldDirection, pos, radius, q1, q2))
		{
			D3DXVECTOR3 distance = *q1 - MouseWorldOrigin;
			float lengthsq = D3DXVec3LengthSq(&distance);
			// �ŒZ�̍X�V
			if (lengthsq < pow(lengthminsq, 2))
			{
				lengthminsq = lengthsq;
				minobject = object;
			}
		}
	}

	// �Փ˂��Ȃ������ꍇnullptr��Ԃ�
	return minobject;
}


//	�`�ʂ���Q�[���I�u�W�F�N�g�𔲐��������X�g���擾
std::list<GameObject*> Scene::GetDrawGameObjectList()
{
	if (m_Camera == nullptr)
		return m_GameObjectList[1];
	// �J�����̕����ɋ��𐔌z�u����悤�ɂ��āA���̋��̓����ɂ���Ȃ�Ε\���Ƃ������ɂ���

	std::list<GameObject*> re;

	//std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;

	const int snum = 3;	//spherenum
	SPHERE sphere[snum];
	D3DXVECTOR3 cameraDirection = m_Camera->GetCameraForward();
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	sphere[0].m_Center = cameraPos;
	sphere[0].m_Radius = 4.0f;
	sphere[1].m_Center = cameraPos + cameraDirection * 13.0f;
	sphere[1].m_Radius = 16.0f;
	sphere[2].m_Center = cameraPos + cameraDirection * 37.0f;
	sphere[2].m_Radius = 35.0f;
	//sphere[3].m_Center = cameraPos + cameraDirection * 65.0f;
	//sphere[3].m_Radius = 40.0f;


	for (auto obj : GetGameObjectList(1))
	{
		D3DXVECTOR3 objPos = obj->GetPosition();

		D3DXVECTOR3 dist = cameraPos - objPos;
		float CameraLenthsq = D3DXVec3LengthSq(&dist);
		float Lenthsq;

		dist = sphere[0].m_Center - objPos;
		Lenthsq = D3DXVec3LengthSq(&dist);
		if (Lenthsq < pow(sphere[0].m_Radius, 2))
			re.push_back(obj);
		else
		{
			dist = sphere[1].m_Center - objPos;
			Lenthsq = D3DXVec3LengthSq(&dist);
			if (Lenthsq < pow(sphere[1].m_Radius, 2))
				re.push_back(obj);
			else
			{
				dist = sphere[2].m_Center - objPos;
				Lenthsq = D3DXVec3LengthSq(&dist);
				if (Lenthsq < pow(sphere[2].m_Radius, 2))
					re.push_back(obj);
				//else
				//{
				//	dist = sphere[3].m_Center - objPos;
				//	Lenthsq = D3DXVec3LengthSq(&dist);
				//	if (Lenthsq < pow(sphere[3].m_Radius, 2))
				//		re.push_back(obj);
				//}
			}
		}
	}
	
	return re;
}


//	�A�b�v�f�[�g����Q�[���I�u�W�F�N�g�𔲐��������X�g���擾�BDraw����sphere[0]�̂Ƃ��ς�������
std::list<GameObject*> Scene::GetUpdateGameObjectList()
{
	if (m_Camera == nullptr || m_Player == nullptr)
		return m_GameObjectList[1];

	// �J�����̕����ɋ��𐔌z�u����悤�ɂ��āA���̋��̓����ɂ���Ȃ�Ε\���Ƃ������ɂ���

	std::list<GameObject*> re;

	//std::array<std::array<float, FieldSize + 1>, FieldSize + 1> g_fieldHeight;

	const int snum = 3;	//spherenum
	SPHERE sphere[snum];
	D3DXVECTOR3 cameraDirection = m_Camera->GetCameraForward();
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	sphere[0].m_Center = m_Player->GetPosition();
	sphere[0].m_Radius = 25.0f;
	sphere[1].m_Center = cameraPos + cameraDirection * 13.0f;
	sphere[1].m_Radius = 16.0f;
	sphere[2].m_Center = cameraPos + cameraDirection * 37.0f;
	sphere[2].m_Radius = 35.0f;
	//sphere[3].m_Center = cameraPos + cameraDirection * 65.0f;
	//sphere[3].m_Radius = 40.0f;


	for (auto obj : GetGameObjectList(1))
	{
		{
			D3DXVECTOR3 objPos = obj->GetPosition();

			D3DXVECTOR3 dist = cameraPos - objPos;
			float CameraLenthsq = D3DXVec3LengthSq(&dist);
			float Lenthsq;

			dist = sphere[0].m_Center - objPos;
			Lenthsq = D3DXVec3LengthSq(&dist);
			if (Lenthsq < pow(sphere[0].m_Radius, 2))
				re.push_back(obj);
			else
			{
				dist = sphere[1].m_Center - objPos;
				Lenthsq = D3DXVec3LengthSq(&dist);
				if (Lenthsq < pow(sphere[1].m_Radius, 2))
					re.push_back(obj);
				else
				{
					dist = sphere[2].m_Center - objPos;
					Lenthsq = D3DXVec3LengthSq(&dist);
					if (Lenthsq < pow(sphere[2].m_Radius, 2))
						re.push_back(obj);
					//else
					//{
					//	dist = sphere[3].m_Center - objPos;
					//	Lenthsq = D3DXVec3LengthSq(&dist);
					//	if (Lenthsq < pow(sphere[3].m_Radius, 2))
					//		re.push_back(obj);
					//}
				}
			}
		}
	}

	return re;
}