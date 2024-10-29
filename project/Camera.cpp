#include"Camera.h"

Camera::Camera(VECTOR playerPos)
{
	cameraPosDiff = VGet(0.0f, 20.0f, -20.0f);
	cameraPos = VGet(playerPos.x, 15.0f, playerPos.z - 25.0f);
	targetPos = VGet(playerPos.x, 5.0f, playerPos.z);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);
	//(0,10,-20)の視点から(0,10,0)のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

}

void Camera::Init(VECTOR playerPos)
{
	cameraPosDiff = VGet(0.0f, 20.0f, -20.0f);
	cameraPos = VGet(playerPos.x, 15.0f, playerPos.z-25.0f);
	targetPos = VGet(playerPos.x, 5.0f, playerPos.z);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 10000.0f);

	//(0,10,-20)の視点から(0,10,0)のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);
}
void Camera::Update(VECTOR playerPos)
{
	cameraPos = VGet(playerPos.x, 15.0f, playerPos.z-25.0f);
	targetPos = VGet(playerPos.x, 5.0f, playerPos.z);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);

	//(0,10,-20)の視点から(0,10,0)のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

}

void Camera::DebugMode()
{
	//上矢印キーでカメラを前に移動
	if (CheckHitKey(KEY_INPUT_UP))
	{
		cameraPosDiff.z++;
	}
	//下矢印キーでカメラを後ろに移動
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPosDiff.z--;
	}
	//右矢印キーでカメラを右に移動
	else if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		cameraPosDiff.x++;
	}
	//左矢印キーでカメラを左に移動
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		cameraPosDiff.x--;
	}

	//左シフトキー押しながら上下矢印でY座標変更
	if (CheckHitKey(KEY_INPUT_LSHIFT)&&CheckHitKey(KEY_INPUT_UP))
	{
		cameraPosDiff.y++;
	}
	else if (CheckHitKey(KEY_INPUT_LSHIFT)&&CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPosDiff.y--;
	}
}

void Camera::Title_Update()
{
	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 1000.0f);
	//(0,10,-20)の視点から(0,10,0)のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(VGet(28, 25, -38), VGet(29, 0, 15));

}