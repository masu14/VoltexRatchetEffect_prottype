#include "PiningSite.h"

PiningSite::PiningSite()
{
	position = { -1.0, -1.0 };	//�s���Ȓl�ŏ�����
	pinR = -1.0;				//�s���Ȓl�ŏ�����
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

Vector2d PiningSite::GetPinPos() {
	return position;
}

void PiningSite::SetPos(double x, double y) {
	position = { x,y };
}