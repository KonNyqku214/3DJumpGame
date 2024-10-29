#include"HitCheck.h"

bool HitChecker::hitCheck(VECTOR playerPos, VECTOR enemyPos,float playerRadius,float hitRadius,float safeRadius,int& Score)
{
	// 2�̃v���C���[�Ԃ̋������v�Z
	VECTOR diff = VSub(playerPos, enemyPos);
	float distance = VSize(diff);

	// ���a�̍��v
	float totalHitRadius = playerRadius + hitRadius - 0.1f;//�O��
	float totalSafeRadius = playerRadius + safeRadius - 0.1f;//����

	bool hitDamageRadius;		//�Ռ��g�ɓ���������
	bool hitSafeRadius;			//�Ռ��g�̓����ɓ�������


	//�Ռ��g�ɂ���������
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

	//���S�n�тɓ�������
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

	//�v���C���[��Y��3�ȉ��ŁA�Ռ��g�ɂ�����A���S�n�тɂ͂����Ă��Ȃ��ꍇ
	if (hitDamageRadius && !hitSafeRadius)
	{
		return true;		//�_���[�W���󂯂�
	}
	//�v���C���[��Y��3�ȉ��ŁA�Ռ��g�ɂ�����A���S�n�тɂ͂����Ă���ꍇ
	else if (hitDamageRadius && hitSafeRadius)
	{
		return false;		//�_���[�W���󂯂Ȃ�
	}
	//�Ռ��g�ɂ����炸�A���S�n�тɂ͂����Ă��Ȃ��ꍇ
	else if ( !hitDamageRadius && !hitSafeRadius)
	{
		return false;
	}
	else if (!hitDamageRadius && hitSafeRadius)
	{
		return false;
	}
}