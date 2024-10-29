#pragma once
#include"DxLib.h"

class Timer
{
public:
    Timer(int durationMillis);
    void start();
    bool isActive();
    bool hasElapsed();
    void reset();
    // タイマー開始からの経過時間を秒単位で取得する
    double getElapsedTime() const;
    // タイマーの持続時間を設定する
    void setDuration(int newDuration);

private:
    bool isRunning; //タイマーが作動中か
    int duration;  //タイマーの持続時間
    int startTime;  //タイマーの開始時間
};