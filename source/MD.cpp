#include "MD.h"
#include "Voltex.h"

//�R���X�g���N�^�A�{���e�b�N�X�A�s�j���O�T�C�g�̐���������
MD::MD()
{
	
}

//�f�X�g���N�^�ŃC���X�^���X���폜����
MD::~MD() {
	//delete voltexs;
}

void MD::Run() {
	ExperimentalParam paramater = {
		5,	
		5,
	};
	InitVolPos(paramater);
	
}

//�{���e�b�N�X�������ʒu�ɔz�u����
Voltex* MD::InitVolPos(ExperimentalParam param) {
	//���̔z�u�����A�{���e�b�N�X�̔z�u���@�͐����i�q���O�p�i�q�Ȃ̂ł��ꂼ��p�ӂ���\��
	Voltex *voltex = new Voltex[param.voltexNum];
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
		std::cout << voltex[i].GetPos() << std::endl;
	}
	return voltex;
	
}
