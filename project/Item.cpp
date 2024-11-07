#include"Item.h"

Item::Item()
{
    HeartHandle = MV1LoadModel("data/model/Item/Heart.mv1");
    HeartPos = GenerateRandomPosition();
    HeartRotate = VGet(0, 0, 0);
    HeartScale = VGet(3.0, 3.0, 3.0);
    MV1SetScale(HeartHandle, HeartScale);
    IsActiveHeart = false;
    IsTakeHeart = false;
    HeartRadius = 1.5f;

    HeartSpawnTimer = new Timer(30000);
    HeartDisappearTimer = new Timer(10000);
}

Item::~Item()
{
    MV1DeleteModel(HeartHandle);
}

void Item::Init()
{
    HeartPos = GenerateRandomPosition();
    HeartRotate = VGet(0, 0, 0);
    HeartScale = VGet(3.0, 3.0, 3.0);
    MV1SetScale(HeartHandle, HeartScale);
    IsActiveHeart = false;
    IsTakeHeart = false;
    HeartRadius = 1.5f;
}

void Item::Update(int playerHP)
{
    //プレイヤーの体力が3以下の場合のみ動作
    if (playerHP < 3)
    {
        HeartRotate.y += RotateSpeed;   //ハートを回転させる

        //ハートが出現中でない場合
        if (!IsActiveHeart)
        {
            if (!HeartSpawnTimer->isActive())
            {
                HeartSpawnTimer->start();
            }
        }
        //出現中の場合
        else
        {
            if (!HeartDisappearTimer->isActive())
            {
                HeartDisappearTimer->start();
            }
        }

        if (HeartSpawnTimer->hasElapsed())  //スポーンまでの時間が経過
        {
            IsActiveHeart = true;
            HeartSpawnTimer->reset();
        }

        if (HeartDisappearTimer->hasElapsed())  //消えるまでの時間が経過
        {
            IsActiveHeart = false;
            HeartPos = GenerateRandomPosition();    //ハートが消えたら座標を再設定
            HeartDisappearTimer->reset();
        }


        MV1SetPosition(HeartHandle, HeartPos);
        MV1SetRotationXYZ(HeartHandle, HeartRotate);
    }
}

void Item::Draw()
{
    if (IsActiveHeart)
    {
        MV1DrawModel(HeartHandle);
    }
}


VECTOR Item::GenerateRandomPosition()
{
    float x, z;

    // x, z の絶対値の合計が 35 以下となるようにランダムな値を生成
    do {
        x = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f の範囲でランダムなx座標
        z = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f の範囲でランダムなz座標
    } while (fabs(x) + fabs(z) > 35.0f);  // xとzの絶対値の合計が35を超えないように再生成

    // y座標は常に2.0f
    float y = 2.0f;

    // VECTORに座標を格納して返す
    return VGet(x, y, z);
}