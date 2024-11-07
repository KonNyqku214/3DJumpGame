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
    //�v���C���[�̗̑͂�3�ȉ��̏ꍇ�̂ݓ���
    if (playerHP < 3)
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

        if (HeartSpawnTimer->hasElapsed())  //�X�|�[���܂ł̎��Ԃ��o��
        {
            IsActiveHeart = true;
            HeartSpawnTimer->reset();
        }

        if (HeartDisappearTimer->hasElapsed())  //������܂ł̎��Ԃ��o��
        {
            IsActiveHeart = false;
            HeartPos = GenerateRandomPosition();    //�n�[�g������������W���Đݒ�
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

    // x, z �̐�Βl�̍��v�� 35 �ȉ��ƂȂ�悤�Ƀ����_���Ȓl�𐶐�
    do {
        x = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f �͈̔͂Ń����_����x���W
        z = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f �͈̔͂Ń����_����z���W
    } while (fabs(x) + fabs(z) > 35.0f);  // x��z�̐�Βl�̍��v��35�𒴂��Ȃ��悤�ɍĐ���

    // y���W�͏��2.0f
    float y = 2.0f;

    // VECTOR�ɍ��W���i�[���ĕԂ�
    return VGet(x, y, z);
}