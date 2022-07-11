#pragma once

#include "main.h"
#include "model.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "Player.h"
#include "gameObject.h"
#include "CComponent.h"
#include "primitive.h"

//#include "testhghdfvs.h"
//#include "gameObject.h"

// �f�t�H���g�̃R���W�����̃��f���̃T�C�Y
static const float def_Size = 50.0f;

class Collision : public CComponent
{
private:
	int			m_Collision_type = 0;		// �J�v�Z���q���W�����Ȃ̂��{�b�N�X�Ȃ̂��Ƃ���ݒ肷��
	bool		m_Movable = false;


	// �J�v�Z���R���W����
	D3DXVECTOR2 m_Capusule_Size = D3DXVECTOR2(50.0f, 40.0f);		// �J�v�Z���̏㉺�̃X�t�B�A�̔��a�ƁA�^�񒆂̃{�b�N�X�̍���

	// �X�t�B�A�R���W����
	float		m_Sphere_Radius = 50.0f;			// �X�t�B�A�̔��a

	// �{�b�N�X�R���W����
	D3DXVECTOR2 m_Box_Size = D3DXVECTOR2(20.0f, 20.0f);				// �{�b�N�X�R���W�����̉����Əc��

	Capsule		m_Capsule;

	Model* m_ModelA;
	Model* m_ModelB;
	Model* m_ModelC;

	D3DXVECTOR3 m_ColScale = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 m_ColRotation = { 0.0f, 0.0f, 0.0f };

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;




public:

	// �e�N���X�̃R���X�g���N�^������Ă����B
	using CComponent::CComponent;


	void Init() override
	{
		m_ModelA = new Model();
		m_ModelA->Load("asset\\model\\ColCylinder.obj");
		m_ModelB = new Model();
		m_ModelB->Load("asset\\model\\ColSphere.obj");
		m_ModelC = new Model();
		m_ModelC->Load("asset\\model\\ColSphere.obj");


		Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

		Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
	}

	void Uninit() override
	{
		m_ModelA->Unload();
		delete m_ModelA;
		m_ModelB->Unload();
		delete m_ModelB;
		m_ModelC->Unload();
		delete m_ModelC;

		m_VertexLayout->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
	}

	void Update() override
	{
		// �V�[�����Q�b�g���Ă���
		Scene* scene = Manager::GetScene();

		// �v�Z�p�̕ϐ����Q�b�g
		D3DXVECTOR3 t_Position = m_ParentGameObject->GetTempPosition();
		D3DXVECTOR3 t_Velocity = m_ParentGameObject->GetTempVelocity();

		
		// Scale��1.0f��50cm�̃T�C�Y�Ȃ̂ŁAcm��50.0f�Ŋ����Ċ������o���Ă���
		m_Capsule.r = m_Capusule_Size.x / def_Size;

		// �J�v�Z���̏����Z�b�g����B
		m_Capsule.SetCapsule(Point(t_Position.x, t_Position.y + 1.0f, t_Position.z), Point(t_Position.x, t_Position.y - 1.0f, t_Position.z), 1.0f, 1.0f);

		//
		m_ColScale.x = m_Capsule.r;
		m_ColScale.y = m_Capsule.r;
		m_ColScale.z = m_Capsule.r;


		// �S�ẴQ�[���I�u�W�F�N�g���擾
		std::vector<GameObject*> allObj = scene->GetAllGameObjects(1);
		for (GameObject* obj : allObj)
		{
			// ������������R���e�j���[
			if (obj == m_ParentGameObject)
				continue;

			// �S�ẴI�u�W�F�N�g�̒�����Collision�R���|�[�l���g�������Ă�����̂������擾
			if (obj->HasComponent<Collision>())
			{

				Point p1(obj->GetPosition().x, obj->GetPosition().y + 1.0f, obj->GetPosition().z);
				Point p2(obj->GetPosition().x, obj->GetPosition().y - 1.0f, obj->GetPosition().z);

				obj->GetComponent<Collision>()->m_Capsule.SetCapsule(p1, p2, 1.0f, 1.0f);

				// �Փ˂����Ƃ��̃J�v�Z���̒��S�A�R�A�ɂȂ��Ă�������̒��ł̍ŒZ�����ɂȂ�Ƃ��̍��W���Ԃ����
				Point c_p1;
				Point c_p2;
				// c_p1����c_p2�ւ̒P�ʕ����x�N�g��
				Vec3 c_UnitNormal;
				// �ᔽ���Ă��鋗��
				float c_vd;		// violatedistance
				// �P�ʕ����x�N�g���ƈᔽ���������������ړ�������Ώd�Ȃ��h�����Ƃ��ł���B
				Vec3 c_Normal;
				// �Փ˂��Ă���Ƃ��̍ŒZ���W�������ɕۑ�����

				if (colCapsuleCapsule(m_Capsule, obj->GetComponent<Collision>()->m_Capsule, &c_p1, &c_p2, &c_vd))
				{
					// �Փ˂��Ă���Ȃ�

					// �d�Ȃ��������W��߂�t_Position
					c_UnitNormal = (c_p1 - c_p2).getNorm();
					c_Normal = c_UnitNormal * c_vd;

					t_Position.x += c_Normal.x;
					t_Position.y += c_Normal.y;
					t_Position.z += c_Normal.z;

					//t_Velocity.x = 0.0f;
					//t_Velocity.y = 0.0f;
					//t_Velocity.z = 0.0f;
					
				}
				else
				{
					// �Փ˂��Ă��Ȃ��Ȃ�
					int aaasd = 4;
				}
			}
		}














		// �Ƃ肠�����n�ʂ�0.0f�Ƃ��Ă���
		float groundHeight = 0.0f;

		// �n�ʂƂ̓����蔻��B�ڒn
		if (t_Position.y < groundHeight && t_Velocity.y < 0.0f)
		{
			t_Position.y = groundHeight;
			t_Velocity.y = 0.0f;
		}




		// m_TempVelocity�̍X�V
		m_ParentGameObject->SetTempVelocity(t_Velocity);

		// m_Position�̍X�V
		m_ParentGameObject->SetTempPosition(t_Position);

		// �{����Temporary���X�V
		m_ParentGameObject->TemporarySetUpdate();
	}

	void Draw() override
	{
		// ���̓��C�A�E�g�ݒ�
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		// �V�F�[�_�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


		// ���[���h�}�g���N�X�ݒ�
		D3DXMATRIX world, scale, rot, trans;
		//D3DXMatrixScaling(&scale, m_ColScale.x, m_ColScale.y, m_ColScale.z);
		//D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
		//D3DXMatrixTranslation(&trans, m_ParentGameObject->GetPosition().x, m_ParentGameObject->GetPosition().y, m_ParentGameObject->GetPosition().z);
		//world = scale * rot * trans;
		//Renderer::SetWorldMatrix(&world);

		//m_ModelA->Draw();

		// ��̋��̕`��
		D3DXMatrixScaling(&scale, m_ColScale.x, m_ColScale.y, m_ColScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
		D3DXMatrixTranslation(&trans, m_Capsule.s.GetPoint1().x, m_Capsule.s.GetPoint1().y, m_Capsule.s.GetPoint1().z);
		//D3DXMatrixTranslation(&trans, 0.0f, 0.0f, 0.0f);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);
		m_ModelB->Draw();

		// ���̋��̕`��
		D3DXMatrixScaling(&scale, m_ColScale.x, m_ColScale.y, m_ColScale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_ColRotation.y, m_ColRotation.x, m_ColRotation.z);
		D3DXMatrixTranslation(&trans, m_Capsule.s.GetPoint2().x, m_Capsule.s.GetPoint2().y, m_Capsule.s.GetPoint2().z);
		//D3DXMatrixTranslation(&trans, 3.0f, 0.0f, 0.0f);
		world = scale * rot * trans;
		Renderer::SetWorldMatrix(&world);
		m_ModelC->Draw();
	}

	void SetMovable(bool movable)
	{
		m_Movable = movable;
	}


};

