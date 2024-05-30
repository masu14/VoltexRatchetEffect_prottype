#include "MD.h"


//�R���X�g���N�^�A�{���e�b�N�X�A�s�j���O�T�C�g�̐���������
MD::MD()
{
	
}

//�f�X�g���N�^�ŃC���X�^���X���폜����
MD::~MD() {
	delete[] voltexs;
	delete[] piningSites;
}

void MD::Run() {
	ExperimentalParam paramater = {
		1,	//�{���e�b�N�X�̐�
		0,	//�s�j���O�T�C�g�̐�
	};

	//�{���e�b�N�X�������z�u�ɕ��ׂ�
	if (paramater.voltexNum > 0) {
		voltexs = InitVolPos(paramater);
	}
	else {
		std::cout << "�{���e�b�N�X�̐��ɕs���Ȓl�����͂���܂���" << std::endl;
		return;
	}
	//�e�X�g�p�A�{���e�b�N�X�̍��W���
	for (int i = 0; i < paramater.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().transpose() << std::endl;
	}
	
	//�s�j���O�T�C�g�������z�u�ɕ��ׂ�
	if (paramater.piningSiteNum > 0) {
		piningSites = InitPinPos(paramater);
	}
	else if(paramater.piningSiteNum==0){
		piningSites = nullptr;
		noPiningSite = true;
	}
	else {
		std::cout << "�s�j���O�T�C�g�̐��ɕs���Ȓl�����͂���܂���" << std::endl;
		return;
	}
	//�e�X�g�p�A�s�j���O�T�C�g�̍��W���
	for (int i = 0; i < paramater.piningSiteNum; i++) {
		std::cout << piningSites[i].GetPinPos().transpose() << std::endl;
	}
	
	
}

//�{���e�b�N�X�������ʒu�ɔz�u����
Voltex* MD::InitVolPos(const ExperimentalParam& param) {
	//���̔z�u�����A�{���e�b�N�X�̔z�u���@�͐����i�q���O�p�i�q�Ȃ̂ł��ꂼ��p�ӂ���\��
	Voltex *voltex = new Voltex[param.voltexNum];
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
		
	}
	return voltex;
}

PiningSite* MD::InitPinPos(const ExperimentalParam& param) {
	PiningSite* piningSite = new PiningSite[param.piningSiteNum];
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}