#include "PiningSite.h"

PiningSite::PiningSite()
{
	pinPos = { -1.0f, -1.0f };	//�s���Ȓl�ŏ�����
	pinR = -1.0f;				//�s���Ȓl�ŏ�����
}

PiningSite::~PiningSite(){
	/* DO NOTHING */
}

Vector2f PiningSite::GetPinPos() {
	return pinPos;
}

void PiningSite::SetPinPos(float x, float y) {
	pinPos = { x,y };
}