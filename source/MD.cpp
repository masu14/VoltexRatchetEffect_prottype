#include "MD.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>

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
	a				= param.a;
	weight			= param.a * 3.0;
	height			= param.a * 2.0 * sqrt(3.0);
	cutoff			= param.cutoff;

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
	
	PlaceTriangle();
	
	
	for (int i = 0; i < voltexNum; i++) {
		std::cout << i << "," << voltexs[i].GetPos().transpose() << std::endl;
	}
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
	}
	
	piningSites = std::make_unique<PiningSite[]>(piningSiteNum);
	for (int i = 0; i < piningSiteNum; i++) {
		piningSites[i].SetPos(i, i);
	}

	return true;
}

//-------------------------------------------------------------------------------------------------
//     ���Ԕ��W�����郁�C�����[�v
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {
	std::string currentTime = GetCurrentTimeStr();
	std::string output_file = "output/test_" + currentTime + ".csv";
	std::ofstream file(output_file);

	WriteLabel(file);
	
	double time = 0;
	double maxtime = 1.0;
	while (time <= maxtime) {
		CalcEOM(time);
		WriteAll(file, time);
		time += dt;
	}
	
	WritePos(file);
	file.close();
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
			double f0 = 1.0;	//VVI�̌W��f0
			
			Vector2d difPos = voltexs[i].GetPos() - voltexs[j].GetPos();		//�x�N�g���̍�
			std::cout << i << difPos.transpose() << std::endl;
			
			//�����I�ɌJ��Ԃ��{���e�b�N�X�̂����A�߂��{���e�b�N�X���v�Z����
			//�����I���E�����ɑ΂��ăJ�b�g�I�t�������Z����΂��̌v�Z�ł����܂��������v����
			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() >  weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() >  height / 2) difPos(1) -= height;
			
			//�ȉ��A�{���e�b�N�X���m�̋������J�b�g�I�t������蒷����Όv�Z���Ȃ�
			if (difPos.norm() > cutoff) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos;	//VVI
			std::cout << force.transpose() << std::endl;
			double xForce = force.x();				//VVI��x����
			double yForce = force.y();				//VVI��y����
			std::cout << xForce << ", " << yForce << std::endl;
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
	double force = 1.0;
	for (int i = 0; i < voltexNum; i++) {
		voltexs[i].AddForce(force, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�S����R�ɂ��͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcResistForce() {
	double eta = 10.0;
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
//		�^���������������ă{���e�b�N�X�̈ʒu�A���x���X�V����
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
		double eta = 10.0;							//�S����R��

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
//    csv�t�@�C���������ݗp�A���x�����L�ڂ���
//-------------------------------------------------------------------------------------------------
void MD::WriteLabel(std::ofstream& file) {
	file << "\n,time,";
	for (int i = 0; i < voltexNum; i++) {
		file << "x,y,v_x,v_y,f_x,f_y,";
	}
	file << "\n";
}

//-------------------------------------------------------------------------------------------------
//     csv�t�@�C���������ݗp�A�e�����̈ʒu�A���x�A�O�͂��t�@�C���ɏ�������
//-------------------------------------------------------------------------------------------------
void MD::WriteAll(std::ofstream& file, double time) {
	file << "," << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y()
			<< "," << voltexs[i].GetVelocity().x() << "," << voltexs[i].GetVelocity().y()
			<< "," << voltexs[i].GetForce().x() << "," << voltexs[i].GetForce().y();
	}
	file << "\n";
}

//------------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̈ʒu���t�@�C���ɏ�������
//------------------------------------------------------------------------------------------------
void MD::WritePos(std::ofstream& file) {
	file << "\n,x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y() << "\n";
	}
}

//------------------------------------------------------------------------------------------------
//    csv�t�@�C�������p�A���ݎ������擾����
//------------------------------------------------------------------------------------------------
std::string MD::GetCurrentTimeStr() {
	auto now = std::chrono::system_clock::now();
	auto inTimeT = std::chrono::system_clock::to_time_t(now);

	std::tm buf;
	localtime_s(&buf, &inTimeT);
	std::stringstream ss;
	ss << std::put_time(&buf, "%Y%m%d%H%M%S");	//�N���������b�܂Ŏ擾����

	return ss.str();	//�擾�������Ԃ𕶎���ɕϊ����ĕԂ�
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u���O�p�i�q�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceTriangle() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < 4; i++) {
		double x = a / 4.0;
		if (i % 2 == 1) x += a / 2.0;
		//if (i == 1) x += 0.01;	//������Ƃ��炵�Ă݂�
		for (int j = 0; j < 3; j++) {
			voltexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u�������_���ɂ���
//-----------------------------------------------------------------------------------------------
void MD::PlaceRandom() {

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
