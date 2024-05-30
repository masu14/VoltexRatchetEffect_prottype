#pragma once
#include "Voltex.h"
#include "PiningSite.h"

//�N���X�̑O���錾
class Voltex;		
class PiningSite;

/// <summary>
/// ExperimentalParam
/// �����������܂Ƃ߂��\���́A�{�V�~�����[�V�����̃p�����[�^�͂��ׂĂ����ŊǗ�����B
/// �����萔��A�V�X�e����̕ϐ��͂����ł͈���Ȃ�
/// </summary>
struct ExperimentalParam {
	int voltexNum;
	int piningSiteNum;
};

struct SystemParam {
	const float dt = 0.01f;		//���ԕω���
	const int height = 10;		//�V�~�����[�V�����{�b�N�X�̍���
	const int weight = 10;		//�V�~�����[�V�����{�b�N�X�̕�
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
	void Run();		//���q���͊w�@�̎��s

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	Voltex* voltexs;			//�{���e�b�N�X�̃C���X�^���X�AvoltexNum�̔z��Ƃ��Ĉ���
	PiningSite* piningSites;	//�s�j���O�T�C�g�̃C���X�^���X�ApiningSiteNum�̔z��Ƃ��Ĉ���

	//=======================================================================================
	// private methods.
	//=======================================================================================
	Voltex* InitVolPos(const ExperimentalParam& param);			//�{���e�b�N�X�̏����z�u���s��
	PiningSite* InitPinPos(const ExperimentalParam& param);		//�s�j���O�T�C�g�̏����z�u���s��
};

