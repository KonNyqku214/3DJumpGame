#include"timer.h"

Timer::Timer(int durationMillis)
{
    startTime = 0;
    duration = durationMillis;
    isRunning = false;
}

//タイマーを開始
void Timer::start()
{
    startTime = GetNowCount();
    isRunning = true;
}

//タイマーが動作中か確認
bool Timer::isActive()
{
    return isRunning;
}

//タイマーが指定時間経過したか確認
bool Timer::hasElapsed()
{
    int currentTime;
    currentTime = GetNowCount();
    if (isRunning && (currentTime - startTime >= duration)) {
        isRunning = false;  // タイマーを停止
        return true;
    }
    return false;
}

// タイマー開始からの経過時間を秒単位で取得
double Timer::getElapsedTime() const {
    if (!isRunning) {
        return 0.0;  // タイマーが動作していない場合
    }
    int currentTime = GetNowCount();
    return (currentTime - startTime) / 1000.0;  // 経過時間を秒に変換
}

//タイマーのリセット
void Timer::reset()
{
    isRunning = false;
}

//持続時間を変更
void Timer::setDuration(int newDuration) {
    duration = newDuration;
}