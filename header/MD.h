#pragma once
#include <memory>
#include "Voltex.h"
#include "PiningSite.h"

//クラスの前方宣言
class Voltex;		
class PiningSite;

/// <summary>
/// ExperimentalParam
/// 実験条件をまとめた構造体、本シミュレーションのパラメータはすべてここで管理する。
/// 物理定数や、システム上の変数はここでは扱わない
/// </summary>
struct ExperimentalParam {
	int voltexNum;
	int piningSiteNum;
};

struct SystemParam {
	const float dt = 0.01f;		//時間変化量
	const int height = 10;		//シミュレーションボックスの高さ
	const int weight = 10;		//シミュレーションボックスの幅
};

/// <summary>
/// 分子動力学法のマネージャークラス、各ボルテックス、ピニングサイトの管理をする
/// ボルテックス、ピニングサイトの初期条件を決め、コンストラクタでインスタンスを生成するし、
/// デストラクタでインスタンスを削除する
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
	MD();			//ボルテックス、ピニングサイトの初期化
	~MD();			//インスタンスを削除する
	void Run();		//分子動力学法の実行

private:
	//=======================================================================================
	// private variables.
	//=======================================================================================
	std::unique_ptr<Voltex[]> voltexs;			//ボルテックスのインスタンス、voltexNum個の配列として扱う
	std::unique_ptr<PiningSite[]> piningSites;	//ピニングサイトのインスタンス、piningSiteNum個の配列として扱う
	bool noPiningSite = false;	//ピニングサイト無しの場合のフラグ

	//=======================================================================================
	// private methods.
	//=======================================================================================
	std::unique_ptr<Voltex[]> InitVolPos(const ExperimentalParam& param);			//ボルテックスの初期配置を行う
	std::unique_ptr<PiningSite[]> InitPinPos(const ExperimentalParam& param);		//ピニングサイトの初期配置を行う
};

