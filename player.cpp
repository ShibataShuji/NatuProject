#include "main.h"
#include "renderer.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "scene.h"
#include "manager.h"
#include "gameObject.h"
#include "Bullet.h"
#include "Shadow.h"
#include "audio.h"
#include "Rigidbody.h"
#include "Collision.h"
#include "Platform.h"
//
//struct CAPSULEB
//{
//	float m_Radius;				// ���a
//	D3DXVECTOR3 m_PointUpper;	// �J�v�Z���̏�̋��̕����̋��̒��S���W
//	D3DXVECTOR3 m_PointLower;	// �J�v�Z���̉��̋��̕����̋��̒��S���W
//
//	// �R���X�g���N�^
//	CAPSULEB() : m_Radius(0.5f), m_PointUpper(0, -0.5f, 0), m_PointLower(0, 0.5f, 0) {}
//
//	// �J�v�Z���̒��S���W���擾
//	D3DXVECTOR3 GetCenter() const
//	{
//		return m_PointUpper + ((m_PointLower - m_PointUpper) * 0.5f);
//	}
//
//	// �J�v�Z���̒��S���W����㉺2�̋��̍��W���Z�b�g
//	void SetCenter(const D3DXVECTOR3& Center) {
//		D3DXVECTOR3 CenterToPointA = ((m_PointUpper - m_PointLower) * 0.5f);
//		D3DXVECTOR3 CenterToPointB = ((m_PointLower - m_PointUpper) * 0.5f);
//		m_PointUpper = Center + CenterToPointA;
//		m_PointLower = Center + CenterToPointB;
//	}
//	// �c�̔��a���擾����
//	float GetHeightRadius()const {
//		D3DXVECTOR3 temp_calclength = m_PointLower - m_PointUpper;
//		float PointLen = D3DXVec3Length(&temp_calclength) * 0.5f;
//		PointLen += m_Radius;
//		return PointLen;
//	}
//};




class Gameobject;

void Player::Init()
{
	// Component�̒ǉ�
	AddComponent<Rigidbody>();
	AddComponent<Collision>();

	Player::SetBulletNumMax();
	m_Model = new Model();
	m_Model->Load("asset\\model\\torus.obj");

	m_Position = D3DXVECTOR3(0.0f, 10.0f, 5.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale	   = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_TagNum = 2;



	// Component�̐ݒ�
	// <Collision>
	//�@�J�v�Z���R���W�����̐ݒ�
	//Point offset = { 0.0f,0.0f + m_Scale.y,0.0f };
	//GetComponent<Collision>()->SetCollisionOffset(offset);
	//GetComponent<Collision>()->SetCapsule(Point(m_Position.x + offset.x, m_Position.y + offset.y, m_Position.z + offset.z),50.0f, 100.0f);
	GetComponent<Collision>()->SetMovable(true);
	GetComponent<Collision>()->SetCollisionType(BOX_COLLISION);
	GetComponent<Collision>()->LoadCollisionModel();
	GetComponent<Collision>()->SetBoxScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	//GetComponent<Collision>()->SetCollisionType(CAPSULE_COLLISION);
	//GetComponent<Collision>()->LoadCollisionModel();
	//GetComponent<Collision>()->SetCapsuleScale(1.0f, 2.0f);		// �J�v�Z���̓X�t�B�A�݂�����(1.0f,1.0f)�ɂ�����0�ƂȂ��Ă��܂������������Ȃ��B

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,"vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");


	Scene* scene = Manager::GetScene();
	m_ShotSE = scene->AddGameObject<Audio>(2);
	m_ShotSE->Load("asset\\audio\\wan.wav");

	m_Shadow = scene->AddGameObject<Shadow>(1);
	m_Shadow->SetPosition(m_Position);
	//m_Shadow->SetScale(D3DXVECTOR3(2.0f, 1.0f, 2.0f));

}


void Player::Uninit()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();



}


void Player::Update()
{
	// �e�N���X��Update���Ă�ł���B�����̒��ŃR���|�[�l���g��Update���Ă΂�Ă���B
	GameObject::Update();


	// ���f�����ړ������肷��Ƃ��͂����ɏ������肷��

	// �C���v�b�g�̎��A���t�@�x�b�g�Ȃ�V���O���N�H�[�e�[�V�����ł��̃A���t�@�x�b�g�ɂȂ�B
	//�v���X�͉����Ă����
	float PlayerMoveSpeed = 0.1f;

	D3DXVECTOR3 forward = GetForward();
	D3DXVECTOR3 right = GetRight();

	if (Input::GetKeyPress('A'))
	{
		m_Velocity -= right * 0.01;
		//m_temp_Velocity -= right * 0.01;
		//m_Position -= GetComponent<Rigidbody>()->GetVelocity() * 0.01f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Velocity += right * 0.01;
		//m_temp_Velocity += right * 0.01;
	}

	if (Input::GetKeyPress('W'))
	{
		m_Velocity += forward * 0.01;
		//m_temp_Velocity += forward * 0.01;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Velocity -= forward * 0.01;
		//m_temp_Velocity -= forward * 0.01;
	}

	if (Input::GetKeyPress('Q'))
	{
		m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Rotation.y += 0.1f;
	}
	if (Input::GetKeyPress('R'))
	{
		m_Rotation.x += 0.1f;
	}

	// F�L�[�Ń����[�h�݂�����
	if (Input::GetKeyPress('F'))
	{
		Player::SetBulletNumMax();
	}

	// �W�����v�B�ڒn���Ă���ꍇ�̂݉\
	if (m_OnTheGround)
	{
		if (Input::GetKeyTrigger('J'))
		{
			m_Velocity.y = 0.25f;
			//m_temp_Velocity.y = 0.25f;
		}
	}



	D3DXVECTOR3 shadowPos = m_Position;
	shadowPos.y = 0.0f;

	m_Shadow->SetPosition(shadowPos);


	// �A���t�@�x�b�g�ȊO��VK_�Ȃ񂽂�ł�������������B�E�N���b�N�Ń}�N���ɔ�ԂƑ��̂�������
	// �g���K�[�͉������u�Ԃ���
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		// �e��1���ȏ゠��Ȃ�
		if (m_BulletNum >= 1)
		{
			m_BulletNum--;
			Scene* scene = Manager::GetScene();
			Bullet* bullet = scene->AddGameObject<Bullet>(1);
			bullet->SetPosition(m_Position);
			bullet->SetVelocity(forward * 0.3);

			m_ShotSE->Play();
		}
	}


	// �v���C���[�̈ړ��̓��͏������I�������ɃR���|�[�l���g(������q���W����)��Update���s��
	GameObject::ComponentUpdate();
	// �{����Temporary���X�V
	/*GameObject::TemporarySetUpdate();*/
}


void Player::Draw()
{
	GameObject::Draw();

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


	// ���[���h�}�g���N�X(4x4�̍s��)�͖���0����v�Z���Ȃ���
	// �X�P�[���A��]�A���W�A�������������킹�邱�Ƃł����Ƃ������[���h�}�g���N�X�ƂȂ�B
	// 0�����蒼���Ȃ��Ƃ����Ȃ����R�́A30�x��]�������̂��܂���]�����悤�Ƃ����Ƃ��ɁA��]�̎����ς���Ă��邩�炾�B
	// ����Ȋ����ŏ�����Ԃ��Ⴄ�����ōŏI���ʂ��ς���Ă���̂ŁA����0������B
	// �Ȃ̂ŁA�ϐ��ŕۑ����Ȃ������ǂ�(���̏�Ŏg�������̃��[�J���Ȃ炢���Ǝv��)
	// ���[���h�}�g���N�X�ݒ�
	/*D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;*/
	D3DXMATRIX world = GetWorldMatrix();	// �֐��ɂ܂Ƃ߂�
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

	
}

// �v���C���[�̍��W��Ԃ�
D3DXVECTOR3 Player::GetPlayerPosition()
{
	return m_Position;
}



void Player::SetBulletNumMax()
{
	m_BulletNum = MaxBulletNum;

}