#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>		// ��������ϒ��̔z��ŏ�������
#include <map>			// ����(key, value)���g��
#include <algorithm>	// std::remove_if ���̓f�[�^�̕����񏈗�
#include <cctype>		// std::isspace   ���̓f�[�^�̕����񏈗�

//�^�G�C���A�X
using string = std::string;

/// <summary>
/// Paramater�\����
/// �����������܂Ƃ߂��\���́A�{�V�~�����[�V�����̃p�����[�^�͂��ׂĂ����ŊǗ�����B
/// <summary>
struct Paramater {
	int voltexNum;				//�{���e�b�N�X�̐�
	int piningSiteNum;			//�s�j���O�T�C�g�̐�
	double dt = 0.001;			//���ԕω���
	double maxTime = 10.0;		//�v�Z����
	double a = 0.25;			//�����z�u���̃{���e�b�N�X�ԋ���(�O�p�i�q)
	int cutoff = 4;				//�{���e�b�N�X�֑��ݍ�p���y�ڂ��Ώۂ̗L���͈�
	double eta = 1.0;			//�S����R��
	double lorentzForce;		//���[�����c�͂̑傫��
};


/// <summary>
/// InputReader�N���X
/// </summary>
class InputReader
{
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	

	//=========================================================================================
	// public methods
	//=========================================================================================
	InputReader();
	~InputReader();

	Paramater GetParam();
	void ReadParam(const string& filename);
	
private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	std::map<string, std::map<string, string>> sections;


	//=========================================================================================
	// private methods
	//=========================================================================================
	template <typename T>
	T					StringToNumber(const string& str);		// ������𐔒l�ɕϊ�����֐�
	template <typename T>
	std::vector<T>		ReadRange(const string& str);			// �����񂩂�ϐ��̒�`��ƍ��ݕ����擾����֐�
	bool				stringToBool(const string& str);		// �������^�U�l�ɕϊ�����֐�
	std::map <string, std::map<string, string>> ReadInputFile(const string& filename);	//���̓t�@�C����ǂݍ��ފ֐�

};