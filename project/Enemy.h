#pragma once
#include"DxLib.h"
#include"math.h"
#include"Timer.h"

//�萔
const float waveSpeed = 0.0015f;
const float hitRadiusScale = 500.0f;
const float safeRadiusScale = 50.0f;
const float maxWaveScale = 0.2f;

const int Enemy_IDLE = 42;
const int Enemy_Attack = 12;
const int Enemy_Inective = 70;
const int Enemy_Awaken = 69;

enum EnemyState
{
	IDLE,
	ATTACK,
	INACTIVE,
	AWAKEN
};

class Enemy
{
public:
	Enemy(VECTOR groundPos);
	~Enemy();

	void Init(VECTOR groundPos);
	void Update(VECTOR playerPos);
	void Draw();
	void DrawDebug();

	VECTOR& getEnemyPos() { return enemyPos; }
	VECTOR& getEnemyRotate() { return enemyRotate; }
	VECTOR& getEnemyScale() { return enemyScale; }
	float getHitRadius() { return (waveScale.x * hitRadiusScale); }
	float getSafeRadius() { return ((waveScale.x * hitRadiusScale) - hitRadiusWidth - 3.0); }
	float getHitRadiusWeight() { return hitRadiusWidth; }
	bool& setIsRunning() { return isRunning; }

private:
	//���f���n���h��
	int enemyHandle;
	int waveHandle;
	int waitTime;				//�U���̑ҋ@����
	int runningEnemyNum;		//�N�����̓G�̐�
	float hitRadius;			//�Ռ��g�̔��a
	float safeRadius;			//���S�͈͂̔��a
	float hitRadiusWidth;		//�Ռ��g�̕�
	bool isAttack;				//�U������
	bool waveRunning;			//�Ռ��g�������Ă��邩
	bool isRunning;				//�G���N������

	//�A�j���[�V����
	int animNum;
	int motionNum;
	int animIndex;
	int attachAnimIndex;
	double animTimer;
	double animTime;
	double totalAnimTime;


	VECTOR enemyPos;
	VECTOR enemyRotate;
	VECTOR enemyScale;
	VECTOR wavePos;
	VECTOR waveRotate;
	VECTOR waveScale;
	EnemyState enemyState;
	EnemyState lastState;
	Timer* waitTimer;

	void calcRotateY(VECTOR playerPos);
	void ChangeAnim(int animIndex);
	void Animation();
	void AttackManager();


};