#include "stdafx.h"

// ���W���G���A�u���b�N�ł̍��W�̏ꍇ��Ԃ�
Int2 GetAreaBlockInt2(const D3DXVECTOR3& pos)
{
	Int2 re;

	// �u���b�N�ԍ��Z�o�B�����͂��̃��b�V���t�B�[���h�̎��_�̈ʒu���I�t�Z�b�g�ǉ����Ȃ��Ƃ����Ȃ�
	re.x = pos.x / interval;
	re.y = pos.z / interval;

	// int�ɂ���Ƃ��ɐ؂�̂Ă����̂ŁA���̏ꍇ-1����K�v������B
	if (pos.x < 0)
		re.x -= 1;
	if (pos.z < 0)
		re.y -= 1;

	return re;
}


// ���W���G���A�u���b�N�ł̍��W�̏ꍇ��Ԃ�,FindoutBeforethehalf()�֐��Ƒg�ݍ��킹��Ƃ�������
Float2 GetAreaBlockFloat2(const D3DXVECTOR3& pos)
{
	Float2 re;

	// �u���b�N�ԍ��Z�o�B�����͂��̃��b�V���t�B�[���h�̎��_�̈ʒu���I�t�Z�b�g�ǉ����Ȃ��Ƃ����Ȃ�
	re.x = pos.x / interval;
	re.y = pos.z / interval;

	return re;
}