#pragma once
#include"DxLib.h"

enum phaseState
{
	STEP1,
	STEP2,
	STEP3,
	CLEAR
};

class Fishing
{
public:
	Fishing(VECTOR pos);
	~Fishing();
	void Init(VECTOR pos);
	void Update();
	void Draw();
	void DebugMode();

	VECTOR getFloatPos() const { return floatPos; }
	VECTOR getFloatSize()const { return floatSize; }


private:
	//モデルハンドル
	int floatHandle;
	int sharkHandle;

	//座標、回転、サイズ
	VECTOR floatPos;
	VECTOR floatRotate;
	VECTOR floatSize;
	VECTOR fishPos;
	VECTOR fishRotate;
	VECTOR fishSize;

	void floatMove();
};