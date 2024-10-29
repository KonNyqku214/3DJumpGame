#include"BackGround.h"

BackGround::BackGround()
{
	skyHandle = MV1LoadModel("data/model/BackGround/SkyDome.mv1");
	groundHandle = MV1LoadModel("data/model/BackGround/Ground.mv1");
	for (int i = 0; i < 4; i++)
	{
		S_groundHandle[i] = MV1LoadModel("data/model/BackGround/Ground_Small.mv1");
	}

	for (int i = 0; i < 2; i++)
	{
		Title_ground[i] = MV1LoadModel("data/model/BackGround/Ground_Small.mv1");
	}
	
	skyPos = VGet(0, 0, 0);
	groundPos = VGet(0, 0, 0);
	groundRotate = VGet(0, 45 * DX_PI_F / 180.0f, 0);
	groundScale = VGet(0.07, 0.07, 0.07);
	S_groundPos[0] = VGet(60, 0, 40);
	S_groundPos[1] = VGet(-60, 0, 40);
	S_groundPos[2] = VGet(40, 0, 60);
	S_groundPos[3] = VGet(-40, 0, 60);

	S_groundRotate = VGet(0, 45 * DX_PI_F / 180.0f, 0);
	S_groundScale = VGet(0.07, 0.07, 0.07);
	Title_groundScale = VGet(0.07, 0.07, 0.07);
	Title_groundRotate = VGet(0, 45 * DX_PI_F / 180.0f, 0);

	MV1SetScale(skyHandle, VGet(0.5,0.5,0.5));

	MV1SetPosition(groundHandle, groundPos);
	MV1SetRotationXYZ(groundHandle, groundRotate);
	MV1SetScale(groundHandle, groundScale);
	for (int i = 0; i < 4; i++)
	{
		MV1SetPosition(S_groundHandle[i], S_groundPos[i]);
		MV1SetRotationXYZ(S_groundHandle[i], S_groundRotate);
		MV1SetScale(S_groundHandle[i], S_groundScale);
	}
}

BackGround::~BackGround()
{
	MV1DeleteModel(skyHandle);
	MV1DeleteModel(groundHandle);
	for (int i = 0; i < 4; i++)
	{
		MV1DeleteModel(S_groundHandle[i]);
	}
}

void BackGround::Init()
{
	skyPos = VGet(0, 0, 0);
	groundPos = VGet(0, 0, 0);
	groundRotate = VGet(0, 45 * DX_PI_F / 180.0f, 0);
	groundScale = VGet(0.07, 0.07, 0.07);
	S_groundPos[0] = VGet(60, 0, 40);
	S_groundPos[1] = VGet(-60, 0, 40);
	S_groundPos[2] = VGet(40, 0, 60);
	S_groundPos[3] = VGet(-40, 0, 60);
	S_groundRotate = VGet(0, 45 * DX_PI_F / 180.0f, 0);
	S_groundScale = VGet(0.07, 0.07, 0.07);

	MV1SetScale(skyHandle, VGet(0.5, 0.5, 0.5));

	MV1SetPosition(groundHandle, groundPos);
	MV1SetRotationXYZ(groundHandle, groundRotate);
	MV1SetScale(groundHandle, groundScale);
	for (int i = 0; i < 4; i++)
	{
		MV1SetPosition(S_groundHandle[i], S_groundPos[i]);
		MV1SetRotationXYZ(S_groundHandle[i], S_groundRotate);
		MV1SetScale(S_groundHandle[i], S_groundScale);
	}

}

void BackGround::Update()
{
	MV1SetPosition(skyHandle, skyPos);
	MV1SetPosition(groundHandle, groundPos);
	MV1SetRotationXYZ(groundHandle, groundRotate);
	MV1SetScale(groundHandle, groundScale);
	for (int i = 0; i < 4; i++)
	{
		MV1SetPosition(S_groundHandle[i], S_groundPos[i]);
		MV1SetRotationXYZ(S_groundHandle[i], S_groundRotate);
		MV1SetScale(S_groundHandle[i], S_groundScale);
	}
}

void BackGround::Draw()
{
	MV1DrawModel(skyHandle);
	MV1DrawModel(groundHandle);
	for (int i = 0; i < 4; i++)
	{
		MV1DrawModel(S_groundHandle[i]);
	}
}

void BackGround::Title_Update()
{
	Title_groundPos[1] = VGet(20, 0, 20);

	MV1SetPosition(skyHandle, skyPos);
	for (int i = 0; i < 2; i++)
	{
		MV1SetPosition(Title_ground[i], Title_groundPos[i]);
		MV1SetRotationXYZ(Title_ground[i], Title_groundRotate);
		MV1SetScale(Title_ground[i], Title_groundScale);
	}
}

void BackGround::Title_Draw()
{
	MV1DrawModel(skyHandle);
	for (int i = 0; i < 2; i++)
	{
		MV1DrawModel(Title_ground[i]);
	}
}