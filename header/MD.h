#pragma once
#include <memory>
#include "Voltex.h"
#include "PiningSite.h"

template<typename T> using unique_ptr = std::unique_ptr<T>;	//�����ď璷�Ȃ̂ŏȗ����Ă邾��

//�N���X�̑O���錾
class Voltex;		
class PiningSite;

/// <summary>
/// ExperimentalParam
/// �����������܂Ƃ߂��\���́A�{�V�~�����[�V�����̃p�����[�^�͂��ׂĂ����ŊǗ�����B
/// </summary>
struct Paramater {
	int voltexNum;				//�{���e�b�N�X�̐�
	int piningSiteNum;			//�s�j���O�T�C�g�̐�
	float dt = 0.01f;			//���ԕω���
	int height = 1;				//�V�~�����[�V�����{�b�N�X�̍���
	int weight = 1;				//�V�~�����[�V�����{�b�N�X�̕�
	const int CUTOFF = 4;		//�{���e�b�N�X�֑��ݍ�p���y�ڂ��Ώۂ̗L���͈�
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
	void Run(Paramater param);		//���q���͊w�@�̎��s

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	unique_ptr<Voltex[]> voltexs;			//�{���e�b�N�X�̃C���X�^���X�AvoltexNum�̔z��Ƃ��Ĉ���
	unique_ptr<PiningSite[]> piningSites;	//�s�j���O�T�C�g�̃C���X�^���X�ApiningSiteNum�̔z��Ƃ��Ĉ���
	bool noPiningSite = false;					//�s�j���O�T�C�g�����̏ꍇ�̃t���O

	//=======================================================================================
	// private methods.
	//=======================================================================================
	unique_ptr<Voltex[]> InitVolPos(const Paramater& param);			//�{���e�b�N�X�̏����z�u���s��
	unique_ptr<PiningSite[]> InitPinPos(const Paramater& param);		//�s�j���O�T�C�g�̏����z�u���s��

	float CalcVVI(Paramater param);				//�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
	float CalcPiningForce();		//�s�j���O�͂��v�Z����
	float CalcLorentzForce();		//���[�����c�͂��v�Z����	
	float CalcThermalForce();		//�T�[�}���͂��v�Z����
	float CalcEOM();				//�^���������������Ĉʒu�A���x���X�V����

	Vector2f AjustPeriod(Vector2f dPos, Paramater param);
};

