#pragma once
#include"DxLib.h"
#include"Timer.h"
#include"math.h"

const float RotateSpeed = 0.1f;  //回転速度

class Item
{
public:
    Item();
    ~Item();
    void Init();
    void Update(int playerHP);
    void Draw();

    VECTOR getHeartPos() { return HeartPos; }
    float getHeartRadius() { return HeartRadius; }
    bool& getIsActiveHeart() { return IsActiveHeart; }
    bool& getIsTakeHeart() { return IsTakeHeart; }
private:
    int HeartHandle;    //ハートのモデルハンドル
    VECTOR HeartPos;    //ハートの座標
    VECTOR HeartRotate; //ハートの回転
    VECTOR HeartScale;  //ハートのスケール

    VECTOR GenerateRandomPosition();    //ランダム座標を取得

    Timer* HeartSpawnTimer; //ハートが出現するまでの間隔
    Timer* HeartDisappearTimer;  //ハートが消えるまでの間隔

    bool IsActiveHeart;      //ハートが出現中か
    bool IsTakeHeart;        //ハートを取得したか
    float HeartRadius;      //アイテムの当たり判定の半径

};