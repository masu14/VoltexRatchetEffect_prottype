#include "MD.h"


//�R���X�g���N�^
MD::MD()
{
	/* DO NOTHING */
}

//�f�X�g���N�^
MD::~MD() {
	/* DO NOTHING */
}

//main.cpp��paramater��ݒ肵�ARun���\�b�h��md�v�Z�����s����
void MD::Run(Paramater param) {

	//�p�����[�^�[�����Ƃɕϐ���ݒ肷��
	voltexNum		= param.voltexNum;
	piningSiteNum	= param.piningSiteNum;
	dt				= param.dt;
	maxTime			= param.maxTime;
	a				= param.a;
	weight			= param.a * 3.0;
	height			= param.a * 2.0 * sqrt(3.0) * voltexNum / 12.0;
	cutoff			= param.cutoff;
	eta				= param.eta;
	

	//�����������������Ƃ�MD�@�����s����
	if (InitApp())
	{
		MainLoop();
	}

}

bool MD::InitApp() {

	// �{���e�b�N�X�̏�����
	if(!InitVolPos())
	{
		std::cout << "!initvoltexpos" << std::endl;
		return false;
	}

	// �s�j���O�T�C�g�̏�����
	if(!InitPinPos())
	{
		std::cout << "!initpinpos" << std::endl;
		return false;
	}

	//�{���e�b�N�X�ւ̊O�͂�������
	InitForce();
	//std::cout << "mainroop�ɓ���܂�" << std::endl;
	return true;
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
bool MD::InitVolPos() {
	if (voltexNum <= 0) {
		std::cout << "vortexNum�ɕs���Ȓl�����͂���܂���" << std::endl;
		return false;
	}
	voltexs = std::make_unique<Voltex[]>(voltexNum);
	
	PlaceVorSquare();		//�{���e�b�N�X���O�p�i�q�ƂȂ�悤�ɔz�u
	
	return true;
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�T�C�g�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
bool MD::InitPinPos() {
	if (piningSiteNum < 0) {
		std::cout << "piningSiteNum�ɕs���Ȓl�����͂���܂���" << std::endl;
		return false;
	}
	else if(piningSiteNum == 0){
		std::cout << "�s�j���O�T�C�g��z�u���Ă��܂���" << std::endl;
		noPiningSite = true;
		return true;
	}
	
	piningSites = std::make_unique<PiningSiteCircle[]>(piningSiteNum);

	
	PlacePinManual();
	return true;

}

//-------------------------------------------------------------------------------------------------
//     ���Ԕ��W�����郁�C�����[�v
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {

	//�����̓��t�̃f�B���N�g�����쐬
	std::string dirName = "output/" + FileHandler::GetCurrentTimeStr();
	FileHandler::CreateDir(dirName);

	//�o�̓t�@�C���̍쐬
	FileHandler::SetIndex(dirName);
	FileHandler filePos;
	FileHandler fileVelocity;
	FileHandler fileForce;
	
	filePos.     CreateFile(dirName, OutputType::position);
	fileVelocity.CreateFile(dirName, OutputType::velocity);
	fileForce.   CreateFile(dirName, OutputType::force);

	//�{���e�b�N�X�̏������z(�ʒu�A���x�A�O��)�̏�������
	//filePos.WritePos(voltexs, voltexNum);
	//fileVelocity.WriteVelocity(voltexs, voltexNum);
	//fileForce.WriteForce(voltexs, voltexNum);
	
	//���x���̏������݁A�ȈՓI�Ȃ���
	filePos.WritePinPos(piningSites, piningSiteNum);
	filePos.     WriteLabel(voltexNum);
	fileVelocity.WriteLabel(voltexNum);
	fileForce.   WriteLabel(voltexNum);
	
	//���C�����[�v
	double time = 0;
	
	while (time <= maxTime) {
		//�^��������������
		CalcEOM(time);

		//�v�Z���ʂ��t�@�C���ɏ�������
		filePos.     WritePos(time, voltexs, voltexNum);
		fileVelocity.WriteVelocity(time, voltexs, voltexNum);
		fileForce.   WriteForce(time, voltexs, voltexNum);

		time += dt;
	}

	//�ŏI�I�ȃ{���e�b�N�X�̕��z(�ʒu�A���x�A�O��)�̏�������
	//filePos.WritePos(voltexs, voltexNum);
	//fileVelocity.WriteVelocity(voltexs, voltexNum);
	//fileForce.WriteForce(voltexs, voltexNum);
}

//-------------------------------------------------------------------------------------------------
//		�O�͂�0�ɏ���������
//-------------------------------------------------------------------------------------------------
void MD::InitForce() {
	for (int i = 0; i < voltexNum; i++) {
		voltexs[i].SetForce(0.0, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI() {
	for (int i = 0; i < voltexNum -1 ; i++) {
		for (int j = i+1; j < voltexNum; j++) {
			double f0 = 3.0;	//VVI�̌W��f0
			
			Vector2d difPos = voltexs[i].GetPos() - voltexs[j].GetPos();		//�x�N�g���̍�
			std::cout << i << difPos.transpose() << std::endl;
			
			//�����I�ɌJ��Ԃ��{���e�b�N�X�̂����A�߂��{���e�b�N�X���v�Z����
			//�����I���E�����ɑ΂��ăJ�b�g�I�t�������Z����΂��̌v�Z�ł����܂��������v����
			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() >  weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() >  height / 2) difPos(1) -= height;
			
			//�ȉ��A�{���e�b�N�X���m�̋������J�b�g�I�t������蒷����Όv�Z���Ȃ�
			//if (difPos.norm() > cutoff) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos/difPos.norm();	//VVI�̎���p����
			
			double xForce = force.x();				//VVI��x����
			double yForce = force.y();				//VVI��y����
			
			voltexs[i].AddForce(xForce, yForce);	//��p
			voltexs[j].AddForce(-xForce, -yForce);	//����p
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcPiningForce() {
	double kp = 1.0;	//kp�̓s�j���O�͂̑傫�������߂�W��
	double lp = 0.3*sqrt(2.0);	//lp�̓s�j���O�T�C�g�ɂ������`�����璴�`���ւ̉񕜒�
	
	for (int i = 0; i < voltexNum; i++) {
		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = voltexs[i].GetPos() - piningSites[j].GetPos();

			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() > weight / 2) difPos(0) -= weight;
			//if (difPos.y() < -height / 2) difPos(1) += height;
			//if (difPos.y() > height / 2) difPos(1) -= height;

			if (difPos.norm() < piningSites[j].GetR()) continue;
			if (difPos.norm() > cutoff) continue;

			Vector2d force = -kp / pow(cosh((difPos.norm() - piningSites[j].GetR()) / lp), 2.0) * difPos/difPos.norm();
			double xForce = force.x();
			double yForce = force.y();
			voltexs[i].AddForce(xForce, yForce);
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		���[�����c�͂��v�Z����	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce() {
	double force = 0.9;
	for (int i = 0; i < voltexNum; i++) {
		voltexs[i].AddForce(force, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�S����R�ɂ��͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcResistForce() {
	for (int i = 0; i < voltexNum; i++) {
		Vector2d velocity = voltexs[i].GetVelocity();	//�{���e�b�N�X�̑��x���擾����
		Vector2d force = -eta * velocity;				//�S����R�ɂ��͂��v�Z����
		voltexs[i].AddForce(force.x(), force.y());		//�{���e�b�N�X�ւ̊O�͂ɉ�����
	}
}

//-------------------------------------------------------------------------------------------------
//		�T�[�}���͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcThermalForce() {
	
}

//-------------------------------------------------------------------------------------------------
//		�^���������������ă{���e�b�N�X�̈ʒu�A���x�A�O�͂��X�V����
//-------------------------------------------------------------------------------------------------
void MD::CalcEOM(double time) 
{
	//���x�x�����@��p�������Ԕ��W�Ń{���e�b�N�X�̈ʒu�A���x���X�V����
	//r(t+dt) = r(t) + v(t)*dt + (1/2)*(F(t)/m)*dt^2
	//v(t+dt) = v(t) + (1/2)*((F(t)+F(t+dt))/m)*dt
	{
		if (time == 0) {
			return;
		}

		unique_ptr<Vector2d[]> v1 = std::make_unique<Vector2d[]>(voltexNum);	//���xv(t)�̓��I�z��Av(t+dt)�̌v�Z�Ɏg��
		unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(voltexNum);	//�O��F(t)�Av(t+dt)�̌v�Z�Ɏg��
		for (int i = 0; i < voltexNum; i++) {

			Vector2d r1 = voltexs[i].GetPos();		//�ʒur(t)
			v1[i] = voltexs[i].GetVelocity();		//���xv(t)
			f1[i] = voltexs[i].GetForce();			//�O��F(t)

			//�ʒur(t+dt)���v�Z���A�X�V����
			Vector2d r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//�ʒur(t+dt)�̌v�Z
			
			//�����I���E����
			if (r2.x() < 0)      r2(0) += weight;
			if (r2.x() > weight) r2(0) -= weight;
			if (r2.y() < 0)      r2(1) += height;
			if (r2.y() > height) r2(1) -= height;
			
			voltexs[i].SetPos(r2.x(), r2.y());								//�ʒur(t+dt)�̍X�V
		}

		//�O�͂̍Čv�Z���s���AF(t+dt)���v�Z����
		InitForce();	//�{���e�b�N�X�ւ̊O�͂�������

		//F(t+dt)�̌v�Z
		CalcVVI();
		CalcPiningForce();
		CalcLorentzForce();
		CalcResistForce();

		//v(t),F(t),F(t+dt)��p���đ��xv(t+dt)���v�Z���A�X�V����
		for (int i = 0; i < voltexNum; i++) {
			
			Vector2d f2 = voltexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//���xv(t+dt)�̌v�Z
			
			voltexs[i].SetForce(f2.x(), f2.y());					//�O��F(t+dt)�̍X�V�A���̎��Ԕ��W�̈ʒur(t)�v�Z�Ŏg��
			voltexs[i].SetVelocity(v2.x(), v2.y());					//���xv(t+dt)�̍X�V
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		�I�[���x�ɒB�����ۂ̉ߌ����^���������������ă{���e�b�N�X�̈ʒu�A���x�A�O�͂��X�V����
//-------------------------------------------------------------------------------------------------
void MD::CalcEOMOverDamp(double time) 
{
	//�I�[���x�ɒB�����ۂ̑��x����{���e�b�N�X�̈ʒu�A���x���X�V����
	//v(t+dt) = F(t+dt) / m
	//r(t+dt) = r(t) + v(t+dt)*dt + (1/2)*((F(t+dt)-F(t))/m)
	if (time == 0) {
		return;
	}
	double eta = 1.0;	//�S����R��

	unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(voltexNum);
	for (int i = 0; i < voltexNum; i++) {

		f1[i] = voltexs[i].GetForce();	//f(t)�̎擾
	}

	InitForce();	//�{���e�b�N�X�ւ̊O�͂�������

	//F(t+dt)�̌v�Z
	CalcVVI();
	CalcPiningForce();
	CalcLorentzForce();
	
	//�I�[���x�����߁A��������ʒu�����߂�
	for (int i = 0; i < voltexNum; i++) {
		Vector2d r1 = voltexs[i].GetPos();							//r(t)�̎擾
		Vector2d f2 = voltexs[i].GetForce();						//f(t+dt)�̎擾
		Vector2d v2 = f2 / eta;										//v(t+dt)�̌v�Z
		Vector2d r2 = r1 + v2 * dt + (f2 - f1[i]) / (2 * eta) * dt;	//r(t+dt)�̌v�Z

		//�����I���E����
		if (r2.x() < 0)      r2(0) += weight;
		if (r2.x() > weight) r2(0) -= weight;
		if (r2.y() < 0)      r2(1) += height;
		if (r2.y() > height) r2(1) -= height;

		voltexs[i].SetForce(f2.x(), f2.y());		//�O�͂̍X�V
		voltexs[i].SetVelocity(v2.x(), v2.y());		//���x�̍X�V
		voltexs[i].SetPos(r2.x(), r2.y());			//�ʒu�̍X�V
	}
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u���O�p�i�q�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceVorTriangle() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < voltexNum / 3.0; i++) { 
		double x = a / 4.0;
		if (i % 2 == 1) x += a / 2.0;
		
		for (int j = 0; j < 3; j++) {
			voltexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u�𒷕��`�z�u�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceVorSquare() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < voltexNum / 3.0; i++) {
		double x = a / 4.0;
		for (int j = 0; j < 3; j++) {
			voltexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u�������_���ɂ���
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorRandom() {

	std::random_device rd;
	std::mt19937_64 gen(rd());
	double xmin = 0.0;
	double xmax = weight;
	double ymin = 0.0;
	double ymax = height;
	std::uniform_real_distribution<> xdis(xmin, xmax);
	std::uniform_real_distribution<> ydis(ymin, ymax);
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			double x = xdis(gen);
			double y = ydis(gen);
			voltexs[3 * i + j].SetPos(x, y);
		}
		
	}
}

//-----------------------------------------------------------------------------------------------
//    �f�o�b�N�p�A�z��̒����ɒ���
//�@�@����̔z�u�ŊO�͍���ς��Ď����������Ƃ��Ɏg��
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorManual()
{
	voltexs[0].SetPos(0.01, 0.108253);
	/*voltexs[1].SetPos(0.4, 0.4);
	voltexs[2].SetPos(0.744743, 0.180336);
	voltexs[3].SetPos(0.172043,0.705768);
	voltexs[4].SetPos(0.135809,0.483818);
	voltexs[5].SetPos(0.550069,0.052258);
	voltexs[6].SetPos(0.551065,0.176116);
	voltexs[7].SetPos(0.292968,0.059621);
	voltexs[8].SetPos(0.081272,0.490255);
	voltexs[9].SetPos(0.351897,0.686276);
	voltexs[10].SetPos(0.113886,0.285068);
	voltexs[11].SetPos(0.610364,0.741576);*/
	
}

//-----------------------------------------------------------------------------------------------
//    �s�j���O�T�C�g�̏����z�u
//-----------------------------------------------------------------------------------------------
void MD::PlacePin()
{
	for (int i = 0; i < piningSiteNum; i++) {
		double x = voltexs[i].GetPos().x();
		double y = voltexs[i].GetPos().y();
		piningSites[i].SetPos(x, y);
		piningSites[i].SetR(a * (double)(i + 1.0) / 16.0);
		std::cout << "piningSite[" << i << "] pos" << piningSites[i].GetPos().transpose() << std::endl;
		std::cout << "piningSite[" << i << "] r  " << piningSites[i].GetR() << std::endl;

	}
}

//-----------------------------------------------------------------------------------------------
//    �f�o�b�N�p�A�z��̒����ɒ���
//�@�@����̔z�u�ŊO�͍���ς��Ď����������Ƃ��Ɏg��
//-----------------------------------------------------------------------------------------------
void MD::PlacePinManual()
{
	piningSites[0].SetPos(1.5, 2.6);
	piningSites[1].SetPos(7.5, 2.6);
	piningSites[2].SetPos(13.5, 2.6);
	piningSites[3].SetPos(1.5, 7.8);
	piningSites[4].SetPos(7.5, 7.8);
	piningSites[5].SetPos(13.5, 7.8);

	piningSites[0].SetR(1.5);
	piningSites[1].SetR(1.0);
	piningSites[2].SetR(0.5);
	piningSites[3].SetR(1.5);
	piningSites[4].SetR(1.0);
	piningSites[5].SetR(0.5);
}