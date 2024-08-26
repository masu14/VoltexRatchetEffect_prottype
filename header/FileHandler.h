#pragma once
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>		//std::setw, std::setfill
#include <filesystem>	//
#include <regex>		//���K�\��
#include "Voltex.h"

template<typename T> using unique_ptr = std::unique_ptr<T>;

class Voltex;

class FileHandler {

public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	static int index;

	//=======================================================================================
	// public methods.
	//=======================================================================================
	FileHandler();
	~FileHandler();

	void SetIndex(std::string dirName);
	void SetName(std::string fileName);

	int  GetIndex() const;
	std::string GetName() const;

	std::string CreateFilePos(std::string dir);			//csv�t�@�C���쐬�p�A�ʒu���������ރt�@�C��
	void WriteLabel(std::ofstream& file, int voltexNum);//csv�t�@�C���������ݗp�A���x�����L�ڂ���
	void WriteAll(std::ofstream& file, double time);	//csv�t�@�C���������ݗp�A�{���e�b�N�X�̈ʒu�A���x�A�O�͂���������
	void WritePos(std::ofstream& file, double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string fileName;

	//=======================================================================================
	// private methods.
	//=======================================================================================
						//csv�t�@�C���������ݗp�A�{���e�b�N�X�̈ʒu����������
};