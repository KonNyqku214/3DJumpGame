#pragma once
#include"DxLib.h"
#include"math.h"
#include"Timer.h"

//’è”
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
	void Title_Update();


	VECTOR& getEnemyPos() { return enemyPos; }
	VECTOR& getEnemyRotate() { return enemyRotate; }
	VECTOR& getEnemyScale() { return enemyScale; }
	float getHitRadius() { return (waveScale.x * hitRadiusScale); }
	float getSafeRadius() { return ((waveScale.x * hitRadiusScale) - hitRadiusWidth - 3.0); }
	float getHitRadiusWeight() { return hitRadiusWidth; }
	float getWaveScale() { return waveScale.x; }
	bool& setIsRunning() { return isRunning; }

private:
	//ƒ‚ƒfƒ‹ƒnƒ“ƒhƒ‹
	int enemyHandle;
	int waveHandle;
	int waitTime;				//UŒ‚‚Ì‘Ò‹@ŠÔ
	int runningEnemyNum;		//‹N“®’†‚Ì“G‚Ì”
	float hitRadius;			//ÕŒ‚”g‚Ì”¼Œa
	float safeRadius;			//ˆÀ‘S”ÍˆÍ‚Ì”¼Œa
	float hitRadiusWidth;		//ÕŒ‚”g‚Ì•
	float shadowWidth;			//‰e‚Ì•
	bool isAttack;				//UŒ‚’†‚©
	bool waveRunning;			//ÕŒ‚”g‚ª“®‚¢‚Ä‚¢‚é‚©
	bool isRunning;				//“G‚ª‹N“®’†‚©
	bool titleInit;


	//ƒAƒjƒ[ƒVƒ‡ƒ“
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
	VECTOR shadowPos;
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