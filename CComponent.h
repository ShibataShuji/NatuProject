#pragma once


// �f�t�H���g�T�C�Y
static const float def_Size = 100.0f;

// �ۑ��ǂݍ��ݎ��Ɉ����₷���悤�ɐ����Ŏ��ʂ���
const static int NullId = 0;
const static int CollisionId = 1;
const static int RigidbodyId = 2;
const static int ModelComponentId = 3;

const static int UpdatePriority_Max = 3;	// �R���|�[�l���g�̃A�b�v�f�[�g�̗D��x

class GameObject;

// CComponent��Update�Ƃ���gameObject��Update�Ƃ��ŌĂ�ł���

class CComponent
{
protected:
	GameObject* m_ParentGameObject;		// �e�ɂȂ��Ă���Q�[���I�u�W�F�N�g
	std::string m_CompName = "null";	// �R���|�[�l���g��	
	int			m_CompId = 0;			// �R���|�[�l���g�̎���ID

	int			m_UpdatePriority = 0;	// Update�̗D��x�B0����ŏ��Ɏ��s����Ă����ł��x����3(���̂Ƃ�)

public:

	// �R���X�g���N�^�B����̈������Ȃ��R���X�g���N�^�́A��΂ɐe�̃I�u�W�F�N�g��ݒ肵�Ȃ��ƃ_���ɂ��邽�߂ɏ����Ă���B
	CComponent() = delete;
	CComponent(GameObject* gameObject)
	{
		m_ParentGameObject = gameObject;
	}
	// �f�X�g���N�^
	virtual ~CComponent(){}

	virtual void Init() {}			// ���z�֐�virtual������Ɠ���CComponent.Init()���Ă�ł�override�������̊֐����Ă΂��
	virtual void Uninit() { delete this; }		// ������virtual�����āAoverride�����֐��̕��Őe�̊֐����ĂԂ悤�ɂ���ΐe���������Ă΂��OK
	virtual void Update() {}
	virtual void Draw() {}

	virtual void CopyThisComponent(CComponent* fromComponent) {}	// �R�s�[�R���X�g���N�^�̂悤�Ȃ���

	virtual void Save(std::ofstream* Objfile, std::ofstream* ObjfileB) {}			// �I�[�o�[���C�h����
	virtual void Load(std::ifstream* Objfile) {}			// �I�[�o�[���C�h����

	std::string GetComponentName()
	{
		return m_CompName;
	}

	int GetComponentID()
	{
		return m_CompId;
	}

	int GetComponentUpdatePriority()
	{
		return m_UpdatePriority;
	}

};

