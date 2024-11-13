#pragma once
#include"DxLib.h"

const int maxGroundNum = 4;

class BackGround
{
public:
	BackGround();
	~BackGround();
	void Init();
	void Update();
	void Draw();
	void Title_Update();
	void Title_Draw();
	VECTOR getSkyPos() const { return skyPos; }

	VECTOR& getGroundPos() { return groundPos; }
	VECTOR& getGroundRotate() { return groundRotate; }
	VECTOR& getGroundScale() { return groundScale; }
	VECTOR getS_groundPos(int i) { return S_groundPos[i]; }
	VECTOR getTitle_groundPos() { return VGet(20, 0, 20); }


private:

	//モデルハンドル
	int skyHandle;
	int groundHandle;
	int S_groundHandle[maxGroundNum];
	int Title_ground[2];

	//座標
	VECTOR skyPos;
	VECTOR groundPos;
	VECTOR groundRotate;
	VECTOR groundScale;
	VECTOR S_groundPos[maxGroundNum];
	VECTOR S_groundRotate;
	VECTOR S_groundScale;
	VECTOR Title_groundPos[2];
	VECTOR Title_groundRotate;
	VECTOR Title_groundScale;
};