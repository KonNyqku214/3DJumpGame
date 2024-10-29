#include"Fishing.h"

Fishing::Fishing(VECTOR pos)
{
	floatPos = pos;
	floatRotate = VGet(0, 0, 0);
	floatSize = VGet(0.05, 0.05, 0.05);
	fishPos = VGet(0, 0, 0);
	fishPos = VGet(0, 0, 0);
	fishSize = VGet(0, 0, 0);

	floatHandle = MV1LoadModel("data/model/float.mv1");
	MV1SetScale(floatHandle, floatSize);
}

Fishing::~Fishing()
{
	MV1DeleteModel(floatHandle);
	MV1DeleteModel(sharkHandle);
}

void Fishing::Init(VECTOR pos)
{
	floatPos = pos;
	floatRotate = VGet(0, 0, 0);
	fishPos = VGet(0, 0, 0);
	fishPos = VGet(0, 0, 0);

}

void Fishing::Update()
{
	MV1SetPosition(floatHandle, floatPos);
	MV1SetScale(floatHandle, floatSize);
}

void Fishing::Draw()
{
	MV1DrawModel(floatHandle);
}

void floatMove()
{

}

void Fishing::DebugMode()
{
	//上矢印キーで座標を前に移動
	if (CheckHitKey(KEY_INPUT_UP))
	{
		floatPos.z++;
	}
	//下矢印キーで座標を後ろに移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		floatPos.z--;
	}
	//右矢印キーで座標を右に移動
	else if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		floatPos.x++;
	}
	//左矢印キーで座標を左に移動
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		floatPos.x--;
	}

	//左シフトキー押しながら上下矢印でY座標変更
	if (CheckHitKey(KEY_INPUT_LSHIFT) && CheckHitKey(KEY_INPUT_UP))
	{
		floatPos.y++;
	}
	else if (CheckHitKey(KEY_INPUT_LSHIFT) && CheckHitKey(KEY_INPUT_DOWN))
	{
		floatPos.y--;
	}

	//Sキー押しながら上下矢印で大きさ変更
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_UP))
	{
		floatSize.x += 0.01;
		floatSize.y += 0.01;
		floatSize.z += 0.01;
	}
	else if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_DOWN))
	{
		floatSize.x -= 0.01;
		floatSize.y -= 0.01;
		floatSize.z -= 0.01;
	}
}