﻿#pragma once
#include "MD.h"
#include <cmath>

int FileHandler::index = 0;

int main() {
	{
		//シミュレーションの設定パラメータをinput.iniファイルから取得する
		InputReader inputReader;
		inputReader.ReadParam("../input/input.ini");

		//パラメーターの設定
		Paramater<double> param = inputReader.GetParam();
		
		std::cout << "\n[Paramater]" << std::endl;
		std::cout << "condition: " << param.condition << std::endl;
		std::cout << "vortexNum: " << param.vortexNum << std::endl;
		std::cout << "piningSiteNum: " << param.piningSiteNum << std::endl;
		std::cout << "dt: " << param.dt << std::endl;
		std::cout << "maxTime: " << param.maxTime << std::endl;
		std::cout << "a: " << param.a << std::endl;
		std::cout << "cutoff: " << param.cutoff << std::endl;
		std::cout << "eta: " << param.eta << std::endl;

		std::cout << "\n[Variable]" << std::endl;
		std::cout << param.var1name <<": " << param.var1[0] << "," << param.var1[1] << "," << param.var1[2] << std::endl;
		std::cout << param.var2name <<": " << param.var2[0] << "," << param.var2[1] << "," << param.var2[2] << std::endl;

		if (param.var1name == "lorentzForce") param.lorentzForce = param.var1[0];
		if (param.var2name == "siteDistance") param.siteDistance = param.var2[0];

		//md法を行う回数を見積もる
		int mdNum = static_cast<int>(std::floor((param.var1[1] - param.var1[0]) / param.var1[2]))+1;	
		mdNum *= (static_cast<int>(std::floor((param.var2[1] - param.var2[0]) / param.var2[2]))+1);
		
		int count = 1;
		std::string dirName = "../output/" + param.condition;
		FileHandler::CreateDir(dirName);
		FileHandler::SetIndex(dirName);
		while (param.lorentzForce <= param.var1[1]) {
			param.siteDistance = param.var2[0];
			while (param.siteDistance <= param.var2[1]) {
				std::cout << "全" << mdNum << "回のうち、第" << count << "回目が実行されています。" << std::endl;
				//分子動力学法の実行
				unique_ptr<MD> md = std::make_unique<MD>();
				md->Run(param);
				param.siteDistance += param.var2[2];
				count++;
			}
			
			param.lorentzForce += param.var1[2];
		}
	}
	
	return 0;
}

