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
void MD::Run(Paramater<double> param) {

	//�p�����[�^�[�����Ƃɕϐ���ݒ肷��
	EOM					= param.EOM;
	condition			= param.condition;
	vortexNum			= param.vortexNum;
	piningSiteNum		= param.piningSiteNum;
	dt					= param.dt;
	maxTime				= param.maxTime;
	a					= param.a;
	weight				= param.weight;
	height				= param.height;
	cutoff				= param.cutoff;
	eta					= param.eta;
	lorentzForce		= param.lorentzForce;
	siteDistance		= param.siteDistance;
	annealTime			= param.annealTime;
	lorentzFrequency	= param.lorentzFrequency;
	f0					= param.f0;
	kp					= param.kp;
	lp					= param.lp;
	var1name			= param.var1name;
	var2name			= param.var2name;

	
	


	//�����������������Ƃ�MD�@�����s����
	if (InitApp())
	{
		MainLoop();
	}

}

bool MD::InitApp() {

	// �s�j���O�T�C�g�̏�����
	if(!InitPinPos())
	{
		std::cout << "!initpinpos" << std::endl;
		return false;
	}

	// �{���e�b�N�X�̏�����
	if (!InitVorPos())
	{
		std::cout << "!initvortexpos" << std::endl;
		return false;
	}

	//�{���e�b�N�X�ւ̊O�͂�������
	InitForce();
	//std::cout << "mainroop�ɓ���܂�" << std::endl;
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
	else if (piningSiteNum == 0) {
		std::cout << "�s�j���O�T�C�g��z�u���Ă��܂���" << std::endl;
		noPiningSite = true;
		return true;
	}

	//�s�j���O�T�C�g�̎�ނ�ݒ肷��
	piningType = SetPinType();

	// TODO: input.ini�̃s�j���O�T�C�g�̐ݒ�ɉ����Č^���ς��悤�ɕύX����
	piningSites = std::make_unique<PiningSiteCircle[]>(piningSiteNum);

	if(piningType == PiningType::tripleCircle) PlaceCirclePinTriple();
	if (piningType == PiningType::doubleCircle) PlaceCirclePinDouble();

	//siteDistance�����~�̒��S�����炷�A
	// TODO: ���������œ������s�j���O�T�C�g��ύX����K�v�L
	if (piningType == PiningType::tripleCircle) ShiftCirclePinTriple();
	if (piningType == PiningType::doubleCircle) ShiftCirclePinDouble();

	return true;
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�������ʒu�ɔz�u����
//-------------------------------------------------------------------------------------------------
bool MD::InitVorPos() {
	if (vortexNum <= 0) {
		std::cout << "vortexNum�ɕs���Ȓl�����͂���܂���" << std::endl;
		return false;
	}
	vortexs = std::make_unique<Vortex[]>(vortexNum);
	
	PlaceVorManual();		//�{���e�b�N�X��z�u
	
	return true;
}

std::string MD::SetVariableName(std::string varname)
{
	try {
		if (varname == "lorentzForce") return FileHandler::FixedValueStr(2, lorentzForce);
		if (varname == "siteDistance") return FileHandler::FixedValueStr(2, siteDistance);
		else throw "�ϐ����ɊY�����镶���񂪐���������܂���";
	}
	catch (const char* e) {
		std::cout << "Error: " << e << std::endl;
	}
	
}

//-------------------------------------------------------------------------------------------------
//     ���Ԕ��W�����郁�C�����[�v
//-------------------------------------------------------------------------------------------------
void MD::MainLoop() {

	//dirMD�ɋL�ڂ���ϐ��p�����[�^�����擾����
	//var1name�͕ϐ����Avar1str�͕ϐ��̒l�̕�����
	std::string var1str, var2str;
	var1str = SetVariableName(var1name);
	var2str = SetVariableName(var2name);

	//�o�̓t�@�C��������f�B���N�g��dirMD���쐬����
	std::string dirName = "../output/" + condition;
	std::string dirMD;

	dirMD = dirName + "/MD" + FileHandler::GetIndex();
	dirMD += "/MD_" + var1name + "=" + var1str + "_" + var2name + "=" + var2str;
	FileHandler::CreateDir(dirMD);

	//�o�̓t�@�C���̍쐬
	FileHandler filePos;
	FileHandler fileVelocity;
	//FileHandler fileForce;
	
	filePos.     CreateFile(dirMD, "position.csv");
	fileVelocity.CreateFile(dirMD, "velocity.csv");
	//fileForce.   CreateFile(dirMD, "force.csv");
	
	//���x���̏�������
	filePos.WritePinPos(piningSites, piningSiteNum);
	filePos.     WriteLabel(vortexNum);
	fileVelocity.WriteLabel(vortexNum);
	//fileForce.   WriteLabel(vortexNum);
	
	//���C�����[�v
	double time = 0;
	
	while (time <= maxTime) {
		//�^��������������
		if (EOM == "ordinary") CalcEOM(time);
		if (EOM == "overdamp") CalcEOMOverDamp(time);
		

		//�v�Z���ʂ��t�@�C���ɏ�������
		filePos.     WritePos(time, vortexs, vortexNum);
		fileVelocity.WriteVelocity(time, vortexs, vortexNum);
		//fileForce.   WriteForce(time, vortexs, vortexNum);

		time += dt;
	}
}

//-------------------------------------------------------------------------------------------------
//		�O�͂�0�ɏ���������
//-------------------------------------------------------------------------------------------------
void MD::InitForce() {
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].SetForce(0.0, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcVVI() {
	const double eps = 1e-10;
	for (int i = 0; i < vortexNum -1 ; i++) {
		for (int j = i+1; j < vortexNum; j++) {
			
			Vector2d difPos = vortexs[i].GetPos() - vortexs[j].GetPos();		//�x�N�g���̍�
			
			//�����I�ɌJ��Ԃ��{���e�b�N�X�̂����A�߂��{���e�b�N�X���v�Z����
			//�����I���E�����ɑ΂��ăJ�b�g�I�t�������Z����΂��̌v�Z�ł����܂��������v����
			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() >  weight / 2) difPos(0) -= weight;
			if (difPos.y() < -height / 2) difPos(1) += height;
			if (difPos.y() >  height / 2) difPos(1) -= height;
			
			//�ȉ��A�{���e�b�N�X���m�̋������J�b�g�I�t������蒷����Όv�Z���Ȃ�
			//if (difPos.norm() > cutoff) continue;						
			
			Vector2d force = f0 * exp(- difPos.norm() / lambda) * difPos/(difPos.norm() + eps);	//VVI�̎���p����
			
			double xForce = force.x();				//VVI��x����
			double yForce = force.y();				//VVI��y����
			
			vortexs[i].AddForce(xForce, yForce);	//��p
			vortexs[j].AddForce(-xForce, -yForce);	//����p
		}
	}
	
}

//-------------------------------------------------------------------------------------------------
//		�s�j���O�͂��v�Z����(�~�`)
//-------------------------------------------------------------------------------------------------
void MD::CalcCirclePiningForce() {
	const double eps = 1e-10;
	
	for (int i = 0; i < vortexNum; i++) {
		bool inCircle = false;
		//�{���e�b�N�X���s�j���O�T�C�g�̓����ɂ���ꍇ�͂ǂ̃s�j���O�T�C�g������O�͂��󂯂Ȃ�
		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = vortexs[i].GetPos() - piningSites[j].GetPos();
			if (difPos.norm() <= piningSites[j].GetR()) {
				inCircle = true;
				break;
			}
		}

		if (inCircle == true) continue;

		for (int j = 0; j < piningSiteNum; j++) {
			Vector2d difPos = vortexs[i].GetPos() - piningSites[j].GetPos();

			if (difPos.x() < -weight / 2) difPos(0) += weight;
			if (difPos.x() > weight / 2) difPos(0) -= weight;
			//if (difPos.y() < -height / 2) difPos(1) += height;
			//if (difPos.y() > height / 2) difPos(1) -= height;

			
			if (difPos.norm() > cutoff) continue;

			Vector2d force = piningSites[j].CalcPiningForce(difPos, kp, lp);

			double xForce = force.x();
			double yForce = force.y();
			vortexs[i].AddForce(xForce, yForce);
		}
	}
}

//-------------------------------------------------------------------------------------------------
//		���[�����c�͂��v�Z����	
//-------------------------------------------------------------------------------------------------
void MD::CalcLorentzForce(double time) {
	const double PI = 3.141592653589;
	double force;
	if (sin( PI / lorentzFrequency * (time-annealTime)) > 0) force = lorentzForce;
	else force = -lorentzForce;
	
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].AddForce(force, 0.0);
	}
}

//-------------------------------------------------------------------------------------------------
//		�S����R�ɂ��͂��v�Z����
//-------------------------------------------------------------------------------------------------
void MD::CalcResistForce() {
	for (int i = 0; i < vortexNum; i++) {
		Vector2d velocity = vortexs[i].GetVelocity();	//�{���e�b�N�X�̑��x���擾����
		Vector2d force = -eta * velocity;				//�S����R�ɂ��͂��v�Z����
		vortexs[i].AddForce(force.x(), force.y());		//�{���e�b�N�X�ւ̊O�͂ɉ�����
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

		unique_ptr<Vector2d[]> v1 = std::make_unique<Vector2d[]>(vortexNum);	//���xv(t)�̓��I�z��Av(t+dt)�̌v�Z�Ɏg��
		unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(vortexNum);	//�O��F(t)�Av(t+dt)�̌v�Z�Ɏg��
		for (int i = 0; i < vortexNum; i++) {

			Vector2d r1 = vortexs[i].GetPos();		//�ʒur(t)
			v1[i] = vortexs[i].GetVelocity();		//���xv(t)
			f1[i] = vortexs[i].GetForce();			//�O��F(t)

			//�ʒur(t+dt)���v�Z���A�X�V����
			Vector2d r2 = r1 + v1[i] * dt + (f1[i] / eta) / 2 * dt * dt;	//�ʒur(t+dt)�̌v�Z
			
			//�����I���E����
			if (r2.x() < 0)      r2(0) += weight;
			if (r2.x() > weight) r2(0) -= weight;
			if (r2.y() < 0)      r2(1) += height;
			if (r2.y() > height) r2(1) -= height;
			
			vortexs[i].SetPos(r2.x(), r2.y());								//�ʒur(t+dt)�̍X�V
		}

		//�O�͂̍Čv�Z���s���AF(t+dt)���v�Z����
		InitForce();	//�{���e�b�N�X�ւ̊O�͂�������

		//F(t+dt)�̌v�Z
		CalcVVI();
		CalcCirclePiningForce();
		if (time > annealTime)CalcLorentzForce(time);
		CalcResistForce();

		//v(t),F(t),F(t+dt)��p���đ��xv(t+dt)���v�Z���A�X�V����
		for (int i = 0; i < vortexNum; i++) {
			
			Vector2d f2 = vortexs[i].GetForce();
			Vector2d v2 = v1[i] + (f1[i] + f2) / (2 * eta) * dt;	//���xv(t+dt)�̌v�Z
			
			vortexs[i].SetForce(f2.x(), f2.y());					//�O��F(t+dt)�̍X�V�A���̎��Ԕ��W�̈ʒur(t)�v�Z�Ŏg��
			vortexs[i].SetVelocity(v2.x(), v2.y());					//���xv(t+dt)�̍X�V
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

	unique_ptr<Vector2d[]> f1 = std::make_unique<Vector2d[]>(vortexNum);
	for (int i = 0; i < vortexNum; i++) {

		f1[i] = vortexs[i].GetForce();	//f(t)�̎擾
	}

	InitForce();	//�{���e�b�N�X�ւ̊O�͂�������

	//F(t+dt)�̌v�Z
	CalcVVI();
	CalcCirclePiningForce();
	if (time > annealTime)CalcLorentzForce(time);
	
	//�I�[���x�����߁A��������ʒu�����߂�
	for (int i = 0; i < vortexNum; i++) {
		Vector2d r1 = vortexs[i].GetPos();							//r(t)�̎擾
		Vector2d f2 = vortexs[i].GetForce();						//f(t+dt)�̎擾
		Vector2d v2 = f2 / eta;										//v(t+dt)�̌v�Z
		Vector2d r2 = r1 + v2 * dt + (f2 - f1[i]) / (2 * eta) * dt;	//r(t+dt)�̌v�Z

		//�����I���E����
		if (r2.x() < 0)      r2(0) += weight;
		if (r2.x() > weight) r2(0) -= weight;
		if (r2.y() < 0)      r2(1) += height;
		if (r2.y() > height) r2(1) -= height;

		vortexs[i].SetForce(f2.x(), f2.y());		//�O�͂̍X�V
		vortexs[i].SetVelocity(v2.x(), v2.y());		//���x�̍X�V
		vortexs[i].SetPos(r2.x(), r2.y());			//�ʒu�̍X�V
	}
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u���O�p�i�q�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceVorTriangle() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < vortexNum / 3.0; i++) { 
		double x = a / 4.0;
		if (i % 2 == 1) x += a / 2.0;
		
		for (int j = 0; j < 3; j++) {
			vortexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
		}
	}
}

//------------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̏����z�u�𒷕��`�z�u�ɂ���
//	  �{���e�b�N�X�̐���6�̔{���̂Ƃ��Ɏg���悤�ɂ���
//------------------------------------------------------------------------------------------------
void MD::PlaceVorSquare() {
	double y = a * sqrt(3.0) / 4.0;
	for (int i = 0; i < vortexNum / 3.0; i++) {
		double x = a / 4.0;
		for (int j = 0; j < 3; j++) {
			vortexs[3 * i + j].SetPos(x + a * (double)j, y + sqrt(3) / 2.0 * a * (double)i);
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
			vortexs[3 * i + j].SetPos(x, y);
		}
		
	}
}

//-----------------------------------------------------------------------------------------------
//    �f�o�b�N�p�A�z��̒����ɒ���
//�@�@����̔z�u�ŊO�͍���ς��Ď����������Ƃ��Ɏg��
//-----------------------------------------------------------------------------------------------
void MD::PlaceVorManual()
{
	for (int i = 0; i < vortexNum; i++) {
		vortexs[i].SetPos(piningSites[i].GetPos().x(), piningSites[i].GetPos().y());
	}
	
}

//-----------------------------------------------------------------------------------------------
//    
//-----------------------------------------------------------------------------------------------
PiningType MD::SetPinType()
{
	if (condition == "Circle-S2M2L2-M_is_Variable" ||
		condition == "Circle-S2M2L2-L_is_Variable" ||
		condition == "Circle-S2M2L2-S_is_Variable") {
		return PiningType::tripleCircle;
	}

	if (condition == "Circle-S2L2-S_is_Variable" ||
		condition == "Circle-S2L2-L_is_Variable") {
		return PiningType::doubleCircle;
	}
}

//-----------------------------------------------------------------------------------------------
//    �~�`�s�j���O�T�C�g�̏����z�u�A�{���e�b�N�X�Ɠ����ʒu�ɔz�u�A
//-----------------------------------------------------------------------------------------------
void MD::PlacePin()
{
	// TODO; �z�u���@�ς���\��A�{���e�b�N�X��Ŕz�u����Ӗ����܂�Ȃ�
	for (int i = 0; i < piningSiteNum; i++) {
		double x = vortexs[i].GetPos().x();
		double y = vortexs[i].GetPos().y();
		piningSites[i].SetPos(x, y);
		piningSites[i].SetR(a * (double)(i + 1.0) / 16.0);
		std::cout << "piningSite[" << i << "] pos" << piningSites[i].GetPos().transpose() << std::endl;
		std::cout << "piningSite[" << i << "] r  " << piningSites[i].GetR() << std::endl;

	}
}

//-----------------------------------------------------------------------------------------------
//    �召2�̉~�`�s�j���O�T�C�g�̍��W�Ɣ��a���Z�b�g����
//-----------------------------------------------------------------------------------------------
void MD::PlaceCirclePinDouble()
{
	piningSites[0].SetPos(1.5, 2.6);
	piningSites[1].SetPos(5.5, 2.6);
	piningSites[2].SetPos(9.5, 2.6);
	piningSites[3].SetPos(13.5, 2.6);
	piningSites[4].SetPos(1.5, 7.8);
	piningSites[5].SetPos(5.5, 7.8);
	piningSites[6].SetPos(9.5, 7.8);
	piningSites[7].SetPos(13.5, 7.8);

	double r1 = 0.0, r2 = 0.0;
	if (condition == "Circle-S2L2-S_is_Variable") {
		r1 = 1.5, r2 = 0.5;
	}
	else if (condition == "Circle-S2L2-L_is_Variable") {
		r1 = 0.5, r2 = 1.5;
	}
	else {
		std::cout << "�Y������condition�����݂��܂���" << std::endl;
	}

	piningSites[0].SetR(r1);
	piningSites[1].SetR(r2);
	piningSites[2].SetR(r1);
	piningSites[3].SetR(r2);
	piningSites[4].SetR(r1);
	piningSites[5].SetR(r2);
	piningSites[6].SetR(r1);
	piningSites[7].SetR(r2);
}

//-----------------------------------------------------------------------------------------------
//    �咆��3�̉~�`�s�j���O�T�C�g�̍��W�Ɣ��a���Z�b�g����
//-----------------------------------------------------------------------------------------------

void MD::PlaceCirclePinTriple()
{
	piningSites[0].SetPos(1.5, 2.6);
	piningSites[1].SetPos(5.5, 2.6);
	piningSites[2].SetPos(9.5, 2.6);
	piningSites[3].SetPos(1.5, 7.8);
	piningSites[4].SetPos(5.5, 7.8);
	piningSites[5].SetPos(9.5, 7.8);

	double r1 = 0.0, r2 = 0.0, r3 = 0.0;
	if (condition == "Circle-S2M2L2-M_is_Variable") {
		r1 = 1.5, r2 = 1.0, r3 = 0.5;
	}
	else if (condition == "Circle-S2M2L2-S_is_Variable") {
		r1 = 1.0, r2 = 0.5, r3 = 1.5;
	}
	else if (condition == "Circle-S2M2L2-L_is_Variable") {
		r1 = 0.5, r2 = 1.5, r3 = 1.0;
	}
	else {
		std::cout << "�Y������condition�����݂��܂���" << std::endl;
	}

	piningSites[0].SetR(r1);
	piningSites[1].SetR(r2);
	piningSites[2].SetR(r3);
	piningSites[3].SetR(r1);
	piningSites[4].SetR(r2);
	piningSites[5].SetR(r3);
}

//-----------------------------------------------------------------------------------------------
//    �召2�̃s�j���O�T�C�g�����炷
//-----------------------------------------------------------------------------------------------

void MD::ShiftCirclePinDouble()
{
	piningSites[1].AddPos(siteDistance, 0);
	piningSites[3].AddPos(siteDistance, 0);
	piningSites[5].AddPos(siteDistance, 0);
	piningSites[7].AddPos(siteDistance, 0);
}

//-----------------------------------------------------------------------------------------------
//    �咆��3�̃s�j���O�T�C�g�����炷
//-----------------------------------------------------------------------------------------------

void MD::ShiftCirclePinTriple()
{
	piningSites[1].AddPos(siteDistance, 0);
	piningSites[4].AddPos(siteDistance, 0);
}