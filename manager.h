#pragma once


#include "Savedata.h"


// string�^->char[64]�֕ϊ�
void StringToChar64(std::string str, char* ch);
void CharToString(char* ch, std::string* str);

class Manager
{
private:
	// static �̃����o�ϐ���cpp���ł�����x�錾���Ă����Ȃ��Ƃ����Ȃ�
	// ��class �����Ă�̂͑O���錾�B����Ȃ�w�b�_�[��include���Ȃ��Ă悭�Ȃ�I�I�I
	// static����ƃO���[�o���ϐ��̂悤�Ɏg����
	// class �̓|�C���^�ϐ��ɂ����g���Ȃ��B
	// include���Əz�Ăяo�����ꂿ����ăG���[�o�邩��A�|�C���^�ϐ��Ȃ� class�ŉ������Ă����āA
	// ����ł����߂Ȃ�C��t���Ȃ���C���N���[�h���g��

	static class Scene* m_Scene;
	static class Savedata* m_Savedata;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static class Scene* GetScene() { return m_Scene; }

	static class Savedata* GetSavedata() { return m_Savedata; }

	template <typename T> 
	static void SetScene() 
	{
		//m_Scene->SetSecenChange(true);

		if (m_Scene != nullptr)
		{
			if (m_Scene)
			{

				m_Scene->Uninit();
				delete m_Scene;

			}
		}

		m_Scene = new T();
		m_Scene->Init();
		m_Scene->SetSecenChange(true);
	}

	//static void SetGameSceneWithStageName()
	//{
	//	if (m_Scene)
	//	{
	//		m_Scene->Uninit();
	//		delete m_Scene;
	//	}

	//	// ��x�_�E���L���X�g����Init�����Ă���m_Scene�ɓ����
		//Game* game = dynamic_cast<Game*>(new Game);
		//game->Init();

	//	m_Scene = game;
	//}

	static void InitSavedata()
	{
		if (m_Savedata)
		{
			delete m_Savedata;
		}

		m_Savedata = new Savedata();
		m_Savedata->Init();
		//m_Scene->Init();
	}

	// override���p����ł���ƁA�X�y���~�X��h�����Ƃ��ł���B���s���ʂ͑S���ς��Ȃ��̂ł��Ȃ��Ă��������ǂ��ĂˁB

};