#pragma once


class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	static long GetMouseX(void);               // マウスがX方向に動いた相対値
	static long GetMouseY(void);               // マウスがY方向に動いた相対値
	static long GetMouseZ(void);               // マウスホイールが動いた相対値

};
