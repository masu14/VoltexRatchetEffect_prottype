#include "MD.h"



//�R���X�g���N�^�A�{���e�b�N�X�A�s�j���O�T�C�g�̐���������
MD::MD()
{
	
}

//�f�X�g���N�^�ŃC���X�^���X���폜����
MD::~MD() {
	
}

void MD::Run() {
	ExperimentalParam paramater = {
		5,	//�{���e�b�N�X�̐�
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
std::unique_ptr<Voltex[]> MD::InitVolPos(const ExperimentalParam& param) {
	//���̔z�u�����A�{���e�b�N�X�̔z�u���@�͐����i�q���O�p�i�q�Ȃ̂ł��ꂼ��p�ӂ���\��
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
	}
	return voltex;
}

//�s�j���O�T�C�g�������ʒu�ɔz�u����
std::unique_ptr<PiningSite[]> MD::InitPinPos(const ExperimentalParam& param) {
	std::unique_ptr<PiningSite[]> piningSite = std::make_unique<PiningSite[]>(param.piningSiteNum);
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}