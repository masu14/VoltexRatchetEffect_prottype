#pragma once
#include <memory>
#include <math.h>			//��{�I�Ȋ֐����������߂̕W�����C�u����
#include "Voltex.h"
#include "PiningSite.h"

template<typename T> using unique_ptr = std::unique_ptr<T>;		//�����ď璷�Ȃ̂Łustd::�v���ȗ����Ă邾��


//�N���X�̑O���錾
class Voltex;		
class PiningSite;

/// <summary>
/// Paramater�\����
/// �����������܂Ƃ߂��\���́A�{�V�~�����[�V�����̃p�����[�^�͂��ׂĂ����ŊǗ�����B
/// </summary>
struct Paramater {
	int voltexNum;				//�{���e�b�N�X�̐�
	int piningSiteNum;			//�s�j���O�T�C�g�̐�
	float dt = 0.01f;			//���ԕω���
	int height = 5;				//�V�~�����[�V�����{�b�N�X�̍���
	int weight = 5;				//�V�~�����[�V�����{�b�N�X�̕�
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
	float lambda = 1.0f;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	unique_ptr<Voltex[]> InitVolPos(const Paramater& param);			//�{���e�b�N�X�̏����z�u���s��
	unique_ptr<PiningSite[]> InitPinPos(const Paramater& param);		//�s�j���O�T�C�g�̏����z�u���s��

	void InitForce(const Paramater& param);							//�O�͂�0�ɏ���������
	void CalcVVI(const Paramater& param);		//�{���e�b�N�X�E�{���e�b�N�X���ݍ�p(VVI)���v�Z����
	void CalcPiningForce();						//�s�j���O�͂��v�Z����
	void CalcLorentzForce();					//���[�����c�͂��v�Z����	
	void CalcThermalForce();					//�T�[�}���͂��v�Z����
	void CalcEOM(const Paramater& param);				//�^���������������Ĉʒu�A���x���X�V����
	

};

