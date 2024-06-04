#include "MD.h"

MD::MD()
{
	/* DO NOTHING */
}

MD::~MD() {
	/* DO NOTHING */
}

void MD::Run(Paramater paramater) {
	
	

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
unique_ptr<Voltex[]> MD::InitVolPos(const Paramater& param) {
	//���̔z�u�����A�{���e�b�N�X�̔z�u���@�͐����i�q���O�p�i�q�Ȃ̂ł��ꂼ��p�ӂ���\��
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
	}
	return voltex;
}

//�s�j���O�T�C�g�������ʒu�ɔz�u����
unique_ptr<PiningSite[]> MD::InitPinPos(const Paramater& param) {
	std::unique_ptr<PiningSite[]> piningSite = std::make_unique<PiningSite[]>(param.piningSiteNum);
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}

//�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
float MD::CalcVVI() {
	return 0;
}

//�s�j���O�͂��v�Z����
float MD::CalcPiningForce() {
	return 0;
}

//���[�����c�͂��v�Z����	
float MD::CalcLorentzForce() {
	float force = 5;
	return force;
}

//�T�[�}���͂��v�Z����
float MD::CalcThermalForce() {
	return 0;
}

//�^���������������Ĉʒu�A���x���X�V����
float MD::CalcEOM() {
	float force = CalcVVI() + CalcPiningForce() + CalcLorentzForce() + CalcThermalForce();
	return 0;

}