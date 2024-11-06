#include"Item.h"

Item::Item()
{
    HeartHandle = MV1LoadModel("data/model/Item/Heart.mv1");
    HeartPos = VGet(0, 2, 0);
    HeartRotate = VGet(0, 0, 0);
    HeartScale = VGet(3.0, 3.0, 3.0);
    MV1SetScale(HeartHandle, HeartScale);
    IsActiveHeart = false;
    Radius = 1.5f;

    HeartSpawnTimer = new Timer(30000);
    HeartDisappearTimer = new Timer(10000);
}

Item::~Item()
{
    MV1DeleteModel(HeartHandle);
}

void Item::Init()
{
    HeartPos = VGet(0, 2, 0);
    HeartRotate = VGet(0, 0, 0);
    HeartScale = VGet(3.0, 3.0, 3.0);
    MV1SetScale(HeartHandle, HeartScale);
    IsActiveHeart = false;
    Radius = 1.5f;
}

void Item::Update()
{
    HeartRotate.y += RotateSpeed;   //�n�[�g����]������

    //�n�[�g���o�����łȂ��ꍇ
    if (!IsActiveHeart)
    {
        if (!HeartSpawnTimer->isActive())
        {
            HeartSpawnTimer->start();
        }
    }
    //�o�����̏ꍇ
    else
    {
        if (!HeartDisappearTimer->isActive())
        {
            HeartDisappearTimer->start();
        }
    }

    if (HeartSpawnTimer->hasElapsed())
    {
        IsActiveHeart = true;
        HeartSpawnTimer->reset();
    }

    if (HeartDisappearTimer->hasElapsed())
    {
        IsActiveHeart = false;
        HeartDisappearTimer->reset();
    }


    MV1SetPosition(HeartHandle, HeartPos);
    MV1SetRotationXYZ(HeartHandle, HeartRotate);
}

void Item::Draw()
{
    if (IsActiveHeart)
    {
        MV1DrawModel(HeartHandle);
    }
}
