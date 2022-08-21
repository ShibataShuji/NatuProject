#pragma once

#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include <string>

// �f�t�H���g�T�C�Y
static const float def_Size = 100.0f;

// �ۑ��ǂݍ��ݎ��Ɉ����₷���悤�ɐ����Ŏ��ʂ���
const static int NullId = 0;
const static int CollisionId = 1;
const static int RigidbodyId = 2;

class GameObject;

// CComponent��Update�Ƃ���gameObject��Update�Ƃ��ŌĂ�ł���

class CComponent
{
protected:
	GameObject* m_ParentGameObject;		// �e�ɂȂ��Ă���Q�[���I�u�W�F�N�g
	std::string m_CompName = "null";	// �R���|�[�l���g��	
	int			m_CompId = 0;			// �R���|�[�l���g�̎���ID

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
	virtual void Uninit() {}		// ������virtual�����āAoverride�����֐��̕��Őe�̊֐����ĂԂ悤�ɂ���ΐe���������Ă΂��OK
	virtual void Update() {}
	virtual void Draw() {}

	virtual void Save(std::fstream* Objfile) {}

	std::string GetComponentName()
	{
		return m_CompName;
	}

	int GetComponentID()
	{
		return m_CompId;
	}

};

