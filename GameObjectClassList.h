#pragma once

//#include <string>
//#include <list>
//
//// ゲームオブジェクトを継承しているもの全てのリスト
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
//		//for (auto c : m_ComponentNameList)	// これはクラスじゃないのでdeleteする必要ない
//		//	delete c;
//		m_ComponentNameList.clear();
//
//		for (auto c : m_Child)
//			delete c;
//		m_Child.clear();
//	}
//
//};