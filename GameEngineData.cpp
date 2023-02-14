#include "stdafx.h"

// 座標がエリアブロックでの座標の場合を返す
Int2 GetAreaBlockInt2(const D3DXVECTOR3& pos)
{
	Int2 re;

	// ブロック番号算出。ここはこのメッシュフィールドの視点の位置分オフセット追加しないといけない
	re.x = pos.x / interval;
	re.y = pos.z / interval;

	// intにするときに切り捨てされるので、負の場合-1する必要がある。
	if (pos.x < 0)
		re.x -= 1;
	if (pos.z < 0)
		re.y -= 1;

	return re;
}


// 座標がエリアブロックでの座標の場合を返す,FindoutBeforethehalf()関数と組み合わせるといい感じ
Float2 GetAreaBlockFloat2(const D3DXVECTOR3& pos)
{
	Float2 re;

	// ブロック番号算出。ここはこのメッシュフィールドの視点の位置分オフセット追加しないといけない
	re.x = pos.x / interval;
	re.y = pos.z / interval;

	return re;
}