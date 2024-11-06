#pragma once
#include"DxLib.h"
#include"Timer.h"

const float RotateSpeed = 0.1f;  //��]���x

class Item
{
public:
    Item();
    ~Item();
    void Init();
    void Update();
    void Draw();
private:
    int HeartHandle;    //�n�[�g�̃��f���n���h��
    VECTOR HeartPos;    //�n�[�g�̍��W
    VECTOR HeartRotate; //�n�[�g�̉�]
    VECTOR HeartScale;  //�n�[�g�̃X�P�[��

    Timer* HeartSpawnTimer; //�n�[�g���o������܂ł̊Ԋu
    Timer* HeartDisappearTimer;  //�n�[�g��������܂ł̊Ԋu

    bool IsActiveHeart;      //�n�[�g���o������
    float Radius;      //�A�C�e���̓����蔻��̔��a

};