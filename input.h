//=============================================================================
//
// ���͏��� [input.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD				// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE					// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD						// �錾����ƃp�b�h�ő���\�ɂȂ�


/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.IX>0)

//#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
//#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
//#define BUTTON_C		0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
//#define BUTTON_X		0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
//#define BUTTON_Y		0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
//#define BUTTON_Z		0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
//#define BUTTON_L		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
//#define BUTTON_R		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
//#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
//#define BUTTON_M		0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)

#define BUTTON_X		0x00000010l	// �w�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_A		0x00000020l	// �`�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_B		0x00000040l	// �a�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_Y		0x00000080l	// �x�{�^��(.rgbButtons[3]&0x80)
#define BUTTON_L		0x00000100l	// �k�{�^��(.rgbButtons[4]&0x80)
#define BUTTON_R		0x00000200l	// �q�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L2		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R2		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_SELECT	0x00001000l	// �y�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_START	0x00002000l	// �r�s�`�q�s�{�^��(.rgbButtons[9]&0x80)

#define GAMEPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

enum {	// �{�^���ɑ΂���rgbButtons[?]�z��̔z��ԍ�(DirectInput��ł�XYAB�{�^���ʒu�ɍ��킹��)
	rgbButtons_X,
	rgbButtons_A,
	rgbButtons_B,
	rgbButtons_Y,
	rgbButtons_L,
	rgbButtons_R,
	rgbButtons_L2,
	rgbButtons_R2,
	rgbButtons_SELECT,
	rgbButtons_START,
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

public:
	//static void Init();
	//static void Uninit();
	//static void Update();

	//static bool GetKeyPress(BYTE KeyCode);
	//static bool GetKeyTrigger(BYTE KeyCode);

	//static long GetMouseX(void);               // �}�E�X��X�����ɓ��������Βl
	//static long GetMouseY(void);               // �}�E�X��Y�����ɓ��������Βl
	//static long GetMouseZ(void);               // �}�E�X�z�C�[�������������Βl


	HRESULT static Init(HINSTANCE hInst, HWND hWnd);
	static void Uninit(void);
	static void Update(void);

	//---------------------------- keyboard
	static bool GetKeyPress(int nKey);
	static bool GetKeyTrigger(int nKey);
	static bool GetKeyRepeat(int nKey);
	static bool GetKeyRelease(int nKey);

	//---------------------------- mouse
	static BOOL IsMouseLeftPressed(void);      // ���N���b�N�������
	static BOOL IsMouseLeftTriggered(void);    // ���N���b�N�����u��
	static BOOL IsMouseRightPressed(void);     // �E�N���b�N�������
	static BOOL IsMouseRightTriggered(void);   // �E�N���b�N�����u��
	static BOOL IsMouseCenterPressed(void);    // ���N���b�N�������
	static BOOL IsMouseCenterTriggered(void);  // ���N���b�N�����u��
	static long GetMouseX(void);               // �}�E�X��X�����ɓ��������Βl 1�t���[���ł̈ړ���
	static long GetMouseY(void);               // �}�E�X��Y�����ɓ��������Βl
	static long GetMouseZ(void);               // �}�E�X�z�C�[�������������Βl

	//---------------------------- game pad
	static BOOL IsButtonPressed(int padNo, DWORD button);
	static BOOL IsButtonTriggered(int padNo, DWORD button);


};
