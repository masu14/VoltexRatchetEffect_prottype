#pragma once
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>		//std::setw, std::setfill
#include <filesystem>	//
#include <regex>		//���K�\��
#include "Voltex.h"

enum class OutputType
{
	position,
	velocity,
	force
};

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
	static void SetIndex(const std::string& dirName);
	static std::string GetCurrentTimeStr();
	static void CreateDir(std::string dirName);

	void CreateFile(std::string dirName, OutputType type);

	int  GetIndex() const;
	std::string GetName() const;

	
	void WriteLabel(int voltexNum);//csv�t�@�C���������ݗp�A���x�����L�ڂ���
	
	void WritePos(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);
	void WriteVelocity(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);
	void WriteForce(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum);

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::string fileName;
	std::ofstream file;

	//=======================================================================================
	// private methods.
	//=======================================================================================
	std::string CreatePosFile(std::string dir);			//csv�t�@�C���쐬�p�A�ʒu���������ރt�@�C��
	std::string CreateVelFile(std::string dir);			//csv�t�@�C���쐬�p�A���x���������ރt�@�C��
	std::string CreateForceFile(std::string dir);		//csv�t�@�C���쐬�p�A�O�͂��������ރt�@�C��
};