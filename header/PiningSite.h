// 1. マクロを定義．
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
/// PiningSite
/// ピニングサイトの座標や形状の情報をまとめたクラス、とりあえず円形のピニングサイトを考えるが
/// 他の形状を作る可能性があるため継承を使って書き直す予定
/// </summary>
class PiningSite {
public:
	//=========================================================================================
	// public variables
	//=========================================================================================
	/* NOTHING */

	//=========================================================================================
	// public methods
	//=========================================================================================
	PiningSite();				//不正な値で初期化(初期化もれした際にエラーを出すため)
	~PiningSite();				

	Vector2d GetPos();				//ピニングサイトの代表的な座標(円の場合は中心)を得る
	void SetPos(double x, double y);	//ピニングサイトの座標を書き込む、初期化以降は変更しない


private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	Vector2d position;		//ピニングサイトの座標

	//=========================================================================================
	// private methods
	//=========================================================================================
};

class PiningSiteCircle : public PiningSite {
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

private:
	//=========================================================================================
	// private variables
	//=========================================================================================
	double r=-1.0;

	//=========================================================================================
	// private methods
	//=========================================================================================
};
