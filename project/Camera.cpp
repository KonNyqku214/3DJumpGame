#include"Camera.h"

Camera::Camera(VECTOR playerPos)
{
	cameraPosDiff = VGet(0.0f, 20.0f, -20.0f);
	cameraPos = VGet(playerPos.x, 15.0f, playerPos.z - 25.0f);
	targetPos = VGet(playerPos.x, 5.0f, playerPos.z);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 1000.0f);
	//(0,10,-20)�̎��_����(0,10,0)�̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

}

void Camera::Init(VECTOR playerPos)
{
	cameraPosDiff = VGet(0.0f, 20.0f, -20.0f);
	cameraPos = VGet(playerPos.x, 15.0f, playerPos.z-25.0f);
	targetPos = VGet(playerPos.x, 5.0f, playerPos.z);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 10000.0f);

	//(0,10,-20)�̎��_����(0,10,0)�̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);
}
void Camera::Update(VECTOR playerPos)
{
	cameraPos = VGet(playerPos.x, 15.0f, playerPos.z-25.0f);
	targetPos = VGet(playerPos.x, 5.0f, playerPos.z);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 1000.0f);

	//(0,10,-20)�̎��_����(0,10,0)�̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(cameraPos, targetPos);

}

void Camera::DebugMode()
{
	//����L�[�ŃJ������O�Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP))
	{
		cameraPosDiff.z++;
	}
	//�����L�[�ŃJ���������Ɉړ�
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		cameraPosDiff.z--;
	}
	//�E���L�[�ŃJ�������E�Ɉړ�
	else if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		cameraPosDiff.x++;
	}
	//�����L�[�ŃJ���������Ɉړ�
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		cameraPosDiff.x--;
	}

	//���V�t�g�L�[�����Ȃ���㉺����Y���W�ύX
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
	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 1000.0f);
	//(0,10,-20)�̎��_����(0,10,0)�̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(VGet(28, 25, -38), VGet(29, 0, 15));

}