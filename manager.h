#pragma once

#include "scene.h"

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

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static class Scene* GetScene() { return m_Scene; }

	template <typename T> 
	static void SetScene() 
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = new T();
		m_Scene->Init();
	}

	// overrideを継承先でつけると、スペルミスを防ぐことができる。実行結果は全く変わらないのでつけなくてもいいけどつけてね。

};