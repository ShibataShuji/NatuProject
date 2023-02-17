#pragma once


#include "Savedata.h"


// string型->char[64]へ変換
void StringToChar64(std::string str, char* ch);
void CharToString(char* ch, std::string* str);

class Manager
{
private:
	// static のメンバ変数はcpp側でもう一度宣言してあげないといけない
	// ↓class をつけてるのは前方宣言。これならヘッダーをincludeしなくてよくなる！！！
	// staticつけるとグローバル変数のように使える
	// class はポインタ変数にしか使えない。
	// includeだと循環呼び出しされちゃってエラー出るから、ポインタ変数なら classで解決してあげて、
	// それでもだめなら気を付けながらインクルードを使う

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

	//	// 一度ダウンキャストしてInitをしてからm_Sceneに入れる
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

	// overrideを継承先でつけると、スペルミスを防ぐことができる。実行結果は全く変わらないのでつけなくてもいいけどつけてね。

};