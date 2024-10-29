#pragma once
#include"DxLib.h"
#include"Player.h"
#include"Enemy.h"
class HitChecker
{
public:
	bool hitCheck(VECTOR playerPos, VECTOR enemyPos, float playerRadius, float hitRadius, float safeRadius,int& Score);
	bool addScore = false;
};