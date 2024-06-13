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
	
	//�{���e�b�N�X�ւ̊O�͂�����������
	InitForce(param);
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<< i<< "]�ւ̊O�� " << voltexs[i].GetForce().transpose() << std::endl;
	}

	//VVI���v�Z
	CalcVVI(param);
	std::cout << "VVI���v�Z" << std::endl;
	//�e�X�g�p�A�{���e�b�N�X�ւ̊O�͂��
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << "voltex["<<i<<"]�ւ̊O�� " << voltexs[i].GetForce().transpose() << std::endl;
	}
	//CalcPiningForce();
	//CalcLorentzForce();
	//CalcThermalForce();
	float time = 0.0f;
	while (time < 1000.0f) {
		CalcEOM(param);
		time += 0.01f;
	}
	for (int i = 0; i < param.voltexNum; i++) {
		std::cout << voltexs[i].GetPos().x() <<", ";
		std::cout << voltexs[i].GetPos().y() << std::endl;
	}
	

}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
unique_ptr<Voltex[]> MD::InitVolPos(const Paramater& param) {
	//���̔z�u�����A�{���e�b�N�X�̔z�u���@�͐����i�q���O�p�i�q�Ȃ̂ł��ꂼ��p�ӂ���\��
	std::unique_ptr<Voltex[]> voltex = std::make_unique<Voltex[]>(param.voltexNum);
	for (int i = 0; i < param.voltexNum; i++) {
		voltex[i].SetPos((float)i/5.0f, (float)i/5.0f);
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

			//�ȉ��A�{���e�b�N�X���m�̋������J�b�g�I�t������蒷����Όv�Z���Ȃ�
			if (difPos.norm() > param.CUTOFF) continue;						
			
			Vector2f force = f0 * expf(- difPos.norm() / lambda) * difPos;	//VVI
			float xForce = force.x();				//VVI��x����
			float yForce = force.y();				//VVI��y����
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
//		�^���������������ă{���e�b�N�X�̈ʒu�A���x���X�V����
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(const Paramater& param) 
{
	//���x�x�����@��p�������Ԕ��W�Ń{���e�b�N�X�̈ʒu�A���x���X�V����
	//r(t+dt) = r(t) + v(t)*dt + (1/2)*(F(t)/m)*dt^2
	//v(t+dt) = v(t) + (1/2)*((F(t)+F(t+dt))/m)*dt
	{
		const float dt = param.dt;					//���ԕω���
		float eta = 10.0f;							//�S����R��

		unique_ptr<Vector2f[]> v1 = std::make_unique<Vector2f[]>(param.voltexNum);	//���xv(t)�̓��I�z��Av(t+dt)�̌v�Z�Ɏg��
		unique_ptr<Vector2f[]> f1 = std::make_unique<Vector2f[]>(param.voltexNum);	//�O��F(t)�Av(t+dt)�̌v�Z�Ɏg��
		for (int i = 0; i < param.voltexNum; i++) {

			Vector2f r1 = voltexs[i].GetPos();		//�ʒur(t)
			v1[i] = voltexs[i].GetVelocity();		//���xv(t)
			f1[i] = voltexs[i].GetForce();			//�O��F(t)

			//�ʒur(t+dt)���v�Z���A�X�V����
			Vector2f r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//�ʒur(t+dt)�̌v�Z

			if (r2.x() < 0) r2.x() += param.weight;
			if (r2.x() > param.weight)r2.x() -= param.weight;
			if (r2.y() < 0)r2.y() += param.height;
			if (r2.y() > param.height)r2.y() -= param.height;

			voltexs[i].SetPos(r2.x(), r2.y());								//�ʒur(t+dt)�̍X�V
		}

		//�O�͂̍Čv�Z���s���AF(t+dt)���v�Z����
		InitForce(param);	//�{���e�b�N�X�ւ̊O�͂�������
		CalcVVI(param);		//F(t+dt)�̌v�Z

		//v(t),F(t),F(t+dt)��p���đ��xv(t+dt)���v�Z���A�X�V����
		for (int i = 0; i < param.voltexNum; i++) {
			
			Vector2f f2 = voltexs[i].GetForce();
			Vector2f v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//���xv(t+dt)�̌v�Z
			
			voltexs[i].SetForce(f2.x(), f2.y());					//�O��F(t+dt)�̍X�V�A���̎��Ԕ��W�̈ʒur(t)�v�Z�Ŏg��
			voltexs[i].SetVelocity(v2.x(), v2.y());					//���xv(t+dt)�̍X�V
		}
	}
}

