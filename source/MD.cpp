#include "MD.h"
#include "Voltex.h"

//�R���X�g���N�^�Ń{���e�b�N�X�̃C���X�^���X�𐶐��A�\���̂̏������͂����ł��ׂ��łȂ�����
MD::MD() {
	

}

//�f�X�g���N�^�ŃC���X�^���X���폜����
MD::~MD() {
	delete voltexs;
}

void MD::Run() {
	InitVolPos();
}

//�{���e�b�N�X�������ʒu�ɔz�u����
void MD::InitVolPos() {
	ExperimentalParam params = {
		5,
		5
	};
	voltexs = new Voltex[params.voltexNum];
	for (int i = 0; i < params.voltexNum; i++) {
		voltexs[i].SetPos(i,i);
		std::cout << "voltex[" << i << "]: " << voltexs[i].GetPos() << std::endl;
	}
	
}