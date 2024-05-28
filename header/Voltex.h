﻿// 1. マクロを定義．
#define EIGEN_NO_DEBUG // コード内のassertを無効化．
#define EIGEN_DONT_VECTORIZE // SIMDを無効化．
#define EIGEN_DONT_PARALLELIZE // 並列を無効化．
#define EIGEN_MPL2_ONLY // LGPLライセンスのコードを使わない．

#pragma once
#include <iostream>
#include <math.h>			//基本的な関数を扱うための標準ライブラリ
#include <Eigen/Core>		//ベクトル、行列演算を行うためのライブラリ

using namespace Eigen;

/// <summary>
/// Voltex class
/// ボルテックスの位置、粘性(運動方程式における質量に相当するもの)、外部から加わる力から
/// 運動方程式を解き、ボルテックスの速度変化の時間発展を計算する
/// 
/// </summary>

class Voltex {

public:
	//=======================================================================================
	// public variables.
	//=======================================================================================
	/* NOTHING */

	//=======================================================================================
	// public methods.
	//=======================================================================================
	Voltex();		//コンストラクタでボルテックスの速度を初期化する
	~Voltex();
	void Run();		//一連の計算を呼び出す
	Vector2f GetPos();
	void SetPos(float x, float y);

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	Vector2f volPos;							//ボルテックスの位置
	Vector2f volVelocity = { 0.0f, 0.0f };	//ボルテックスの速度
	

	//=======================================================================================
	// private methods.
	//=======================================================================================
	float CalcVVI();				//ボルテックス・ボルテックス相互作用(VVI)を計算する
	float CalcPiningForce();		//ピニング力を計算する
	float CalcLorentzForce();		//ローレンツ力を計算する	
	float CalcThermalForce();		//サーマル力を計算する
};

	
