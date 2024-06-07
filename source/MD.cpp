#include "MD.h"

MD::MD()
{
	/* DO NOTHING */
}

MD::~MD() {
	/* DO NOTHING */
}

void MD::Run(Paramater param) {
	
	

	//�{���e�b�N�X�������z�u�ɕ��ׂ�
	if (param.voltexNum > 0) {
		voltexs = InitVolPos(param);
	}
	else {
		std::cout << "�{���e�b�N�X�̐��ɕs���Ȓl�����͂���܂���" << std::endl;
		return;
	}
	//�e�X�g�p�A�{���e�b�N�X�̍��W���
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().transpose() << std::endl;
	}
	
	//�s�j���O�T�C�g�������z�u�ɕ��ׂ�
	if (param.piningSiteNum > 0) {
		piningSites = InitPinPos(param);
	}
	else if(param.piningSiteNum==0){
		noPiningSite = true;
	}
	else {
		std::cout << "�s�j���O�T�C�g�̐��ɕs���Ȓl�����͂���܂���" << std::endl;
		return;
	}
	//�e�X�g�p�A�s�j���O�T�C�g�̍��W���
	for (int i = 0; i < param.piningSiteNum; i++) {
		std::cout << piningSites[i].GetPinPos().transpose() << std::endl;
	}

	forceToVoltexs = std::make_unique<float[]>(param.voltexNum);
	forceToVoltexs = {};
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
float MD::CalcVVI(const Paramater& param) {
	for (int i = 0; i < param.voltexNum ; i++) {
		float force = 0;
		for (int j = 0; j < param.voltexNum; j++) {
			if (j == i) continue;
			Vector2f difPos = voltexs[i].GetPos() - voltexs[j].GetPos();	//�x�N�g���̍�
			if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
			if (difPos.x() >  param.weight / 2) difPos.x() -= param.weight;
			if (difPos.y() < -param.height / 2) difPos.y() += param.height;
			if (difPos.y() <  param.height / 2) difPos.y() -= param.height;
			float r2 = difPos.dot(difPos);
			if (r2 > param.CUTOFF * param.CUTOFF) continue;
			
			force += r2;
		}
	}
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
float MD::CalcEOM(Paramater param) {
	
	return 0;

}


void MD::AjustPeriod(Vector2f difPos, Paramater param) {
	if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
	if (difPos.x() > param.weight / 2)  difPos.x() -= param.weight;
	if (difPos.y() < -param.height / 2)difPos.y()  += param.height;
	if (difPos.y() > param.height / 2)difPos.y()   -= param.height;

}
