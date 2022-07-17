//#include "Collision.h"
//
//void Collision::CapsuleCapsule(GameObject* obj, D3DXVECTOR3* t_Position)
//{
//	// 衝突したときのカプセルの中心、コアになっている線分の中での最短距離になるとこの座標が返される
//	Point c_p1;
//	Point c_p2;
//	// c_p1からc_p2への単位方向ベクトル
//	Vec3 c_UnitNormal;
//	// 違反している距離
//	float c_vd;		// violatedistance
//	// 方向ベクトル。単位方向ベクトルと違反距離をかけた分移動させれば重なりを防ぐことができる。
//	Vec3 c_Normal;
//	// 衝突しているときの最短座標をここに保存する
//
//	if (colCapsuleCapsule(m_Capsule, obj->GetComponent<Collision>()->m_Capsule, &c_p1, &c_p2, &c_vd))
//	{
//		// 衝突しているなら
//
//
//		// 重なった分座標を戻すt_Position
//		c_UnitNormal = (c_p1 - c_p2).getNorm();
//		c_Normal = c_UnitNormal * c_vd;				// 方向ベクトル		
//
//		// 当たっているものが動かないものなら
//		if (!obj->GetComponent<Collision>()->m_Movable)
//		{
//			// 自分が動くものなら
//			if (m_Movable)
//			{
//				// 方向ベクトル分自分だけを戻してあげる
//				(*t_Position).x += c_Normal.x;
//				(*t_Position).y += c_Normal.y;
//				(*t_Position).z += c_Normal.z;
//			}
//		}
//		else
//		{
//
//			// 当たっているものも移動させたい場合どちらも影響0.5倍で動かす
//			// 自分が動くものなら
//			if (m_Movable)
//			{
//				(*t_Position).x += c_Normal.x * 0.5f;
//				(*t_Position).y += c_Normal.y * 0.5f;
//				(*t_Position).z += c_Normal.z * 0.5f;
//			}
//
//			// 当たっているもののポジションを変える。
//			// 本当はコリジョンにもう1個メンバ変数追加して、D3DXVECTOR3で他のコリジョンからの影響どれくらい受けたか
//			// を保存して、コリジョンのUpdateの最初にそれを仮の座標に追加してあげる計算のがいいかも。
//			D3DXVECTOR3 DX_c_Normal = { c_Normal.x, c_Normal.y, c_Normal.z };
//			DX_c_Normal *= -1;		// ベクトルを反転させる
//
//			D3DXVECTOR3 HitObjPos = obj->GetPosition();
//			D3DXVECTOR3 HitObjSetpos = HitObjPos + (DX_c_Normal * 0.5);
//			obj->SetPosition(HitObjSetpos);
//		}
//
//
//		//t_Velocity.x = 0.0f;
//		//t_Velocity.y = 0.0f;
//		//t_Velocity.z = 0.0f;
//
//	}
//	else
//	{
//		// 衝突していないなら
//		int aaasd = 4;
//	}
//}
