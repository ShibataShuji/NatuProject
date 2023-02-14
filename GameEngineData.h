#pragma once

#define M_PI_6		0.5235987755982988  // pi/6 = 30度
#define M_PI_180    0.017453292519943295  // pi/180 = 1度
#define M_PI_x2     6.283185307179586  // pi*2 = 360度

static const int	FieldSize = 256;
static const float  PanelSize = 0.2f;
static const float  interval = (FieldSize - 1) * PanelSize;

// 座標がエリアブロックでの座標の場合を返す
Int2 GetAreaBlockInt2(const D3DXVECTOR3& pos);

// 座標がエリアブロックでの座標の場合を返す
Float2 GetAreaBlockFloat2(const D3DXVECTOR3& pos);