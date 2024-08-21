#include "PiningSite.h"

//----------------------------------------------------------------------------------------
//  PiningSite�N���X(���N���X)
// �s�j���O�T�C�g�͌`���ς��Ĕz�u����\�������邽�߁A���ʂ̐��������̃N���X�ɒ�`���A
// �`��ɂ�鏈���̈Ⴂ�͔h���N���X�Œ�`����B
//----------------------------------------------------------------------------------------
PiningSite::PiningSite()
{
	position = { -1.0, -1.0 };	//�s���Ȓl�ŏ�����
	
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

Vector2d PiningSite::GetPos() {
	return position;
}

void PiningSite::SetPos(double x, double y) {
	position = { x,y };
}

//----------------------------------------------------------------------------------------
// PiningSiteCircle�N���X(�h���N���X)
// �~�`�̃s�j���O�T�C�g�̃N���X
//----------------------------------------------------------------------------------------
PiningSiteCircle::PiningSiteCircle() {
	SetPos(-1.0, -1.0);
	r = -1.0;
}

PiningSiteCircle::~PiningSiteCircle() {
	/* DO NOTHING */
}

double PiningSiteCircle::GetR() {
	return r;
}

void PiningSiteCircle::SetR(double radian) {
	r = radian;
}