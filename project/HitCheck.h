#pragma once
#include"DxLib.h"
#include"Player.h"
#include"Enemy.h"
class HitChecker
{
public:
	bool hitCheck(VECTOR playerPos, VECTOR enemyPos, float playerRadius, float hitRadius, float safeRadius);
private:
	float shockWaveHeight = 1.0f;
};