// 1. �}�N�����`�D
#define EIGEN_NO_DEBUG // �R�[�h����assert�𖳌����D
#define EIGEN_DONT_VECTORIZE // SIMD�𖳌����D
#define EIGEN_DONT_PARALLELIZE // ����𖳌����D
#define EIGEN_MPL2_ONLY // LGPL���C�Z���X�̃R�[�h���g��Ȃ��D

#pragma once
#include <iostream>
#include <math.h>			//��{�I�Ȋ֐����������߂̕W�����C�u����
#include <Eigen/Core>		//�x�N�g�����Z���s�����߂̃��C�u����

using namespace Eigen;


/// <summary>
/// PiningSite
/// �s�j���O�T�C�g�̍��W��`��̏����܂Ƃ߂��N���X�A�Ƃ肠�����~�`�̃s�j���O�T�C�g���l���邪
/// ���̌`������\�������邽�ߌp�����g���ď��������\��
/// </summary>
class PiningSite {
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	static double potentialE;		//�s�j���O�T�C�g���`������|�e���V�����G�l���M�[
	static double kp;
	static double lp;

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSite();				//�s���Ȓl�ŏ�����(���������ꂵ���ۂɃG���[���o������)
	~PiningSite();

	static void Setkp(double kp);
	static void Setlp(double lp);

	Vector2d GetPos();					//�s�j���O�T�C�g�̒��S���W�𓾂�
	void SetPos(double x, double y);	//�s�j���O�T�C�g�̍��W���������ށA�������ȍ~�͕ύX���Ȃ�
	void AddPos(double x, double y);

	double GetPotentialE();
	void SetPotentialE(double energy);
	void AddPotentialE(double energy);
	virtual Vector2d CalcPiningForce(Vector2d difPos) const = 0;	//�h����̌`��ɓK�����v�Z���s��
	virtual double CalcPotentialE(Vector2d pos) const = 0;		//�h����̌`��ɓK�����v�Z���s��

private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	Vector2d position;		//�s�j���O�T�C�g�̍��W
	

	//=========================================================================================
	// private methods
	//=========================================================================================
};

/// <summary>
/// �~�`�s�j���O�T�C�g�̃N���X
/// PiningSite�N���X���p�����Ă���
/// </summary>
class PiningSiteCircle : public PiningSite 
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	/* NOTHING */

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSiteCircle();
	~PiningSiteCircle();

	double GetR();
	void SetR(double r);

	Vector2d CalcPiningForce(Vector2d difPos) const override;
	double CalcPotentialE(Vector2d pos) const override;
private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	double r=-1.0;

	//=========================================================================================
	// private methods
	//=========================================================================================
};

/// <summary>
/// �~�`�s�j���O�T�C�g�̃N���X
/// PiningSite�N���X���p�����Ă���
/// </summary>
class PiningSiteLine : public PiningSite
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	/* NOTHING */

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSiteLine();
	~PiningSiteLine();

	double GetLength();
	void SetLength(double length);

	Vector2d CalcPiningForce(Vector2d difPos) const override;
	double CalcPotentialE(Vector2d pos) const override;



private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	double length = -1.0;

	//=========================================================================================
	// private methods
	//=========================================================================================

};