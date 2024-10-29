#include"HitCheck.h"

bool HitChecker::hitCheck(VECTOR playerPos, VECTOR enemyPos,float playerRadius,float hitRadius,float safeRadius,int& Score)
{
	// 2つのプレイヤー間の距離を計算
	VECTOR diff = VSub(playerPos, enemyPos);
	float distance = VSize(diff);

	// 半径の合計
	float totalHitRadius = playerRadius + hitRadius - 0.1f;//外側
	float totalSafeRadius = playerRadius + safeRadius - 0.1f;//内側

	bool hitDamageRadius;		//衝撃波に当たったか
	bool hitSafeRadius;			//衝撃波の内側に入ったか


	//衝撃波にあたったか
	if (distance < totalHitRadius)
	{
		if (playerPos.y < 5.0f)
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
	}

	//安全地帯に入ったか
	if (distance < totalSafeRadius)
	{
		hitSafeRadius = true;
		if (!addScore)
		{
			Score++;
			addScore = true;
		}
	}
	else
	{
		hitSafeRadius = false;
		addScore = false;
	}

	//プレイヤーのYが3以下で、衝撃波にあたり、安全地帯にはいっていない場合
	if (hitDamageRadius && !hitSafeRadius)
	{
		return true;		//ダメージを受ける
	}
	//プレイヤーのYが3以下で、衝撃波にあたり、安全地帯にはいっている場合
	else if (hitDamageRadius && hitSafeRadius)
	{
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