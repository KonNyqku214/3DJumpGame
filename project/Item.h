#pragma once
#include"DxLib.h"
#include"Timer.h"

const float RotateSpeed = 0.1f;  //回転速度

class Item
{
public:
    Item();
    ~Item();
    void Init();
    void Update();
    void Draw();
private:
    int HeartHandle;    //ハートのモデルハンドル
    VECTOR HeartPos;    //ハートの座標
    VECTOR HeartRotate; //ハートの回転
    VECTOR HeartScale;  //ハートのスケール

    Timer* HeartSpawnTimer; //ハートが出現するまでの間隔
    Timer* HeartDisappearTimer;  //ハートが消えるまでの間隔

    bool IsActiveHeart;      //ハートが出現中か
    float Radius;      //アイテムの当たり判定の半径

};