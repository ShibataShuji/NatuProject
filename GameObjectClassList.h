#pragma once

//#include <string>
//#include <list>
//
//// �Q�[���I�u�W�F�N�g���p�����Ă�����̑S�Ẵ��X�g
//class GameObjectClassList
//{
//private:
//	std::list<int> m_p_GameObjectClassList;
//
//public:
//
//	GameObject() {}
//
//	virtual ~GameObject()
//	{
//		for (auto c : m_ComponentList)
//			delete c;
//		m_ComponentList.clear();
//
//		//for (auto c : m_ComponentNameList)	// ����̓N���X����Ȃ��̂�delete����K�v�Ȃ�
//		//	delete c;
//		m_ComponentNameList.clear();
//
//		for (auto c : m_Child)
//			delete c;
//		m_Child.clear();
//	}
//
//};