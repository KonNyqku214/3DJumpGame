#pragma once
#include"DxLib.h"

const int maxGroundNum = 6;

class BackGround
{
public:
	BackGround();
	~BackGround();
	void Init();
	void Update();
	void Draw();
	VECTOR getSkyPos() const { return skyPos; }

	VECTOR& getGroundPos() { return groundPos; }
	VECTOR& getGroundRotate() { return groundRotate; }
	VECTOR& getGroundScale() { return groundScale; }
	VECTOR getS_groundPos(int i) { return S_groundPos[i]; }


private:

	//モデルハンドル
	int skyHandle;
	int groundHandle;
	int S_groundHandle[maxGroundNum];
	//座標
	VECTOR skyPos;
	VECTOR groundPos;
	VECTOR groundRotate;
	VECTOR groundScale;
	VECTOR S_groundPos[maxGroundNum];
	VECTOR S_groundRotate;
	VECTOR S_groundScale;
};