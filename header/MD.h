#pragma once
#include <memory>
#include <math.h>			//��{�I�Ȋ֐����������߂̕W�����C�u����
#include <random>
#include "Vortex.h"
#include "PiningSite.h"
#include "FileHandler.h"
#include "InputReader.h"	//Paramater�\���̂��Ƃ�

template<typename T> using unique_ptr = std::unique_ptr<T>;		//�����ď璷�Ȃ̂Łustd::�v���ȗ����Ă邾��


//�N���X�̑O���錾
class Vortex;		
class PiningSite;
class PiningSiteCircle;

enum class PiningType
{
	tripleCircle,
	doubleCircle,
};

/// <summary>
/// ���q���͊w�@�̃}�l�[�W���[�N���X�A�e�{���e�b�N�X�A�s�j���O�T�C�g�̊Ǘ�������
/// �{���e�b�N�X�A�s�j���O�T�C�g�̏������������߁A�R���X�g���N�^�ŃC���X�^���X�𐶐����邵�A
/// �f�X�g���N�^�ŃC���X�^���X���폜����
/// </summary>
class MD{
public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	/* NOTHING */

	//=======================================================================================
	// public methods.
	//=======================================================================================
	MD();			//�{���e�b�N�X�A�s�j���O�T�C�g�̏�����
	~MD();			//�C���X�^���X���폜����
	void Run(Paramater<double> param);		//���q���͊w�@�̎��s

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string EOM;			//�^���������̎�ށAordinary or overdamp
	std::string condition;		//��������
	int vortexNum;				//�{���e�b�N�X�̐�
	int piningSiteNum;			//�s�j���O�T�C�g�̐�
	double dt = 0.001;			//���ԕω���
	double maxTime = 10.0;		//�v�Z����
	double a = 0.25;			//�����z�u���̃{���e�b�N�X�ԋ���(�O�p�i�q)
	double height = 5;			//�V�~�����[�V�����{�b�N�X�̍���
	double weight = 5;			//�V�~�����[�V�����{�b�N�X�̕�
	int cutoff = 4;				//�{���e�b�N�X�֑��ݍ�p���y�ڂ��Ώۂ̗L���͈�
	double eta = 1.0;			//�S����R��
	double lorentzForce;		//���[�����c�͂̑傫��
	double siteDistance;		//�Ώۂ̃s�j���O�T�C�g�̂��炷�l
	double annealTime;			//���߂Ƀ��[�����c�͂��������ɂ��鎞��
	double lorentzFrequency;	//���[�����c�͂̌�����؂�ւ��鎞��
	double f0;					//VVI�̌W���̑傫��
	double kp;					//�s�j���O�͂̌W���̑傫��
	double lp;					//
	std::string var1name;
	std::string var2name;

	PiningType piningType;

	unique_ptr<Vortex[]> vortexs;					//�{���e�b�N�X�̃C���X�^���X�A�@vortexNum�̔z��Ƃ��Ĉ���
	unique_ptr<PiningSiteCircle[]> piningSites;		//�s�j���O�T�C�g�̃C���X�^���X�ApiningSiteNum�̔z��Ƃ��Ĉ���
	bool noPiningSite = false;						//�s�j���O�T�C�g�����̏ꍇ�̃t���O
	double lambda = 1.0;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	bool InitApp();
	void MainLoop();

	bool InitVorPos();			//�{���e�b�N�X�̏����z�u���s��
	bool InitPinPos();			//�s�j���O�T�C�g�̏����z�u���s��

	
	void InitForce();						//�O�͂�0�ɏ���������
	void CalcVVI();							//�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
	void CalcPiningForce();					//�s�j���O�͂��v�Z����
	void CalcLorentzForce(double time);				//���[�����c�͂��v�Z����	
	void CalcResistForce();					//�S����R�ɂ��͂��v�Z����
	void CalcThermalForce();				//�T�[�}���͂��v�Z����
	void CalcEOM(double time);				//�^���������������Ĉʒu�A���x�A�����x���X�V����
	void CalcEOMOverDamp(double time);

	void PlaceVorTriangle();		//�{���e�b�N�X�̏����z�u���O�p�i�q�ɂ���
	void PlaceVorSquare();
	void PlaceVorRandom();			//�{���e�b�N�X�̏����z�u�������_���ɂ���
	void PlaceVorManual();			//�{���e�b�N�X�̏����z�u������w�肷��

	PiningType SetPinType();
	void PlacePin();
	void PlaceCirclePinTriple();
	void PlaceCirclePinDouble();
	void ShiftCirclePinTriple();
	void ShiftCirclePinDouble();

	std::string SetVariableName(std::string varname);

};

