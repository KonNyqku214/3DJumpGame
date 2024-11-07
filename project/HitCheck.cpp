#include"HitCheck.h"

bool HitChecker::hitCheck(VECTOR playerPos, VECTOR enemyPos, float playerRadius, float hitRadius, float safeRadius, int& Score,bool&addScore,bool isHitting,VECTOR& effectPos)
{
	// 2つのプレイヤー間の距離を計算
	VECTOR diff = VSub(playerPos, enemyPos);
	float distance = VSize(diff);

	// 半径の合計
	float totalHitRadius = playerRadius + hitRadius - 0.1f;//外側
	float totalSafeRadius = playerRadius + safeRadius - 0.1f;//内側

	bool hitDamageRadius=false;		//衝撃波に当たったか
	bool hitSafeRadius=false;			//衝撃波の内側に入ったか


	//衝撃波にあたったか
	if (distance < totalHitRadius)
	{
		if (playerPos.y < shockWaveHeight)
		{
			hitDamageRadius = true;
		}
		else
		{
			hitDamageRadius = false;
		}
	}
	else
	{
		hitDamageRadius = false;
		addScore = false;
	}

	//安全地帯に入ったか
	if (distance < totalSafeRadius)
	{
		hitSafeRadius = true;
	}
	else
	{
		hitSafeRadius = false;
	}

	//プレイヤーのYが3以下で、衝撃波にあたり、安全地帯にはいっていない場合
	if (hitDamageRadius && !hitSafeRadius)
	{

		return true;		//ダメージを受ける
	}
	else if (hitDamageRadius && hitSafeRadius)
	{
		if (!addScore&&!isHitting)
		{
			Score++;
			effectPos = playerPos;

			effectPos.y += 5.0f;
			addScore = true;
		}
		return false;		//ダメージを受けない
	}
	//衝撃波にあたらず、安全地帯にはいっていない場合
	else if ( !hitDamageRadius && !hitSafeRadius)
	{
		return false;
	}
	else if (!hitDamageRadius && hitSafeRadius)
	{
		return false;
	}
}


bool HitChecker::hitCheckItem(VECTOR playerPos, VECTOR itemPos, float playerRadius, float itemRadius)
{
	// 2つのプレイヤー間の距離を計算
	VECTOR diff = VSub(playerPos, itemPos);
	float distance = VSize(diff);

	// 半径の合計
	float totalRadius = playerRadius + itemRadius - 0.1f;//外側

	if (distance < totalRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}