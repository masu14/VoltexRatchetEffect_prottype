#include "PiningSite.h"

PiningSite::PiningSite()
{
	position = { -1.0f, -1.0f };	//�s���Ȓl�ŏ�����
	pinR = -1.0f;				//�s���Ȓl�ŏ�����
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

Vector2f PiningSite::GetPinPos() {
	return position;
}

void PiningSite::SetPinPos(float x, float y) {
	position = { x,y };
}