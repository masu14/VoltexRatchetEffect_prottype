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
		std::cout << "voltex["<<i<<"]�̍��W " << voltexs[i].GetPos().transpose() << std::endl;
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
		std::cout << "piningSites["<<i<<"]�̍��W " << piningSites[i].GetPinPos().transpose() << std::endl;
	}

	InitForce(param);
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<< i<< "]�ւ̊O�� " << voltexs[i].GetForce().transpose() << std::endl;
	}

	//�{���e�b�N�X�ւ̊O�͂�����������
	//
	CalcVVI(param);
	std::cout << "VVI���v�Z" << std::endl;
	//�e�X�g�p�A�{���e�b�N�X�ւ̊O�͂��
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<<i<<"]�ւ̊O�� " << voltexs[i].GetForce().transpose() << std::endl;
	}
	//CalcPiningForce();
	//CalcLorentzForce();
	//CalcThermalForce();
	//CalcEOM(param);
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
unique_ptr<Voltex[]> MD::InitVolPos(const Paramater& param) {
	//���̔z�u�����A�{���e�b�N�X�̔z�u���@�͐����i�q���O�p�i�q�Ȃ̂ł��ꂼ��p�ӂ���\��
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos(i, i);
	}
	return voltex;
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�T�C�g�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
unique_ptr<PiningSite[]> MD::InitPinPos(const Paramater& param) {
	std::unique_ptr<PiningSite[]> piningSite = std::make_unique<PiningSite[]>(param.piningSiteNum);
	for (int i = 0; i < param.piningSiteNum; i++) {
		piningSite[i].SetPinPos(i, i);
	}
	return piningSite;
}

//-------------------------------------------------------------------------------------------------
//		�O�͂�0�ɏ���������
//-------------------------------------------------------------------------------------------------
void MD::InitForce(const Paramater& param) {
	for (int i = 0; i < param.voltexNum; i++) {
		voltexs[i].SetForce(0.0f, 0.0f);
	}
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI(const Paramater& param) {
	for (int i = 0; i < param.voltexNum -1 ; i++) {
		for (int j = i+1; j < param.voltexNum; j++) {
			float f0 = 1.0f;	//VVI�̌W��f0
			
			Vector2f difPos = voltexs[i].GetPos() - voltexs[j].GetPos();	//�x�N�g���̍�

			//�����I�ɌJ��Ԃ��{���e�b�N�X�̂����A�߂��{���e�b�N�X���v�Z����
			//�����I���E�����ɑ΂��ăJ�b�g�I�t�������Z����΂��̌v�Z�ł����܂��������v����
			if (difPos.x() < -param.weight / 2) difPos.x() += param.weight;
			if (difPos.x() >  param.weight / 2) difPos.x() -= param.weight;
			if (difPos.y() < -param.height / 2) difPos.y() += param.height;
			if (difPos.y() <  param.height / 2) difPos.y() -= param.height;

			float difPos2 = difPos.dot(difPos);						//difPos2�̓{���e�b�N�X���m�̋�����2��
			if (difPos2 > param.CUTOFF * param.CUTOFF) continue;	//�{���e�b�N�X���m�̋������J�b�g�I�t������蒷����Όv�Z���Ȃ�
			
			float xForce = f0 * expf(- difPos.x()/ lambda) / difPos.norm() * difPos.x();	//VVI��x����
			float yForce = f0 * expf(- difPos.y()/ lambda) / difPos.norm() * difPos.y();	//VVI��y����
			voltexs[i].AddForce(xForce, yForce);	//��p
			voltexs[j].AddForce(-xForce, -yForce);	//����p
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcPiningForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		���[�����c�͂��v�Z����	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce() {
	float force = 5;
	
}

//-------------------------------------------------------------------------------------------------
//		�T�[�}���͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		�^���������������Ĉʒu�A���x���X�V����
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(const Paramater& param) {
	

}

