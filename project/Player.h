#pragma once
#include"DxLib.h"
#include"Timer.h"
#include"math.h"
#include <Windows.h> // AddFontResourceEx を使うために必要

//定数
const float JumpPower = 1.0f;		//ジャンプ力
const float Gravity = 0.05f;		//重力

const float KnockBackPower = 0.5f;	//ノックバックの大きさ

const int Anim_IDLE = 1;
const int Anim_WALK = 3;
const int Anim_RUNNING = 7;
const int Anim_JUMPING = 11;
const int Anim_JUMP_START = 12;
const int Anim_JUMP_IDLE = 13;
const int Anim_JUMP_END = 14;
const int Anim_DAMAGE = 25;
const int Anim_DIE = 28;


enum PlayerState
{
	STOP,
	WALK,
	RUNNING,
	JUMPING,
	JUMP_START,
	JUMP_IDLE,
	JUMP_END,
	DAMAGE,
	DIE
};

class Player
{
public:
	//コンストラクタ、デストラクタ
	Player();
	~Player();

	void Init();	// 初期化
	void Update();	// 更新
	void Draw();	// 描画
	void DrawDebug();// デバッグの描画

	void Title(float waveScale);	//タイトル画面

	int getPlayerHP() { return HP;}						// HP取得
	int& getScore() { return Score; }					// スコア取得
	VECTOR& getPlayerPos(){ return playerPos; }			// 座標取得
	VECTOR& getPlayerRotate(){ return playerRotate; }	// 回転取得
	VECTOR& getPlayerScale(){ return playerScale; }		// スケール取得
	double getAnimTime() { return animTime; }			// 現在のアニメ時間を取得
	float getHitRadius() { return playerWidth; }			// 当たり判定の半径取得
	Timer* invincibleTimer;
	Timer* Title_jumpTimer;		//タイトル画面でのジャンプ間隔

	void takeDamage();
	bool& setIsHit() { return isHit; }
	bool& setIsTakeDamage() { return isTakeDamage; }
	bool getIsInvincible() { return isInvincible; }
	bool getIsHitting() { return isHitting; }
	bool getIsGameOver() { return isGameOver; }

	int fontHandle_Score;	// スコアのフォントハンドル

private:
	int playerHandle;		// プレイヤーモデルハンドル

	const char* fontFilePath;
	int fontCount;

	float alpha;			// プレイヤー透明度
	int alphaTime;			// 透明度用タイマー
	int HP;					// 体力
	int Score;				// 獲得スコア
	float moveSpeed;		// プレイヤーの移動速度
	float fallSpeed;		// プレイヤーの落下速度
	float maxSpeed;			// 最大移動速度
	float playerWidth;		//プレイヤーの当たり判定の半径
	float shadowWidth;
	bool isJumping;			// ジャンプ中かどうか
	bool isHit;
	bool isTakeDamage;		// ダメージを受けたかどうか
	bool isHitting;			// ダメージを受けている最中か
	bool isInvincible;		// 無敵常態かどうか
	bool isGameOver;		//ゲームオーバーか

	//コントローラー
	int padInputX;			// 左スティックのX軸
	int padInputY;			// 左スティックのY軸
	int padState;			// PADの状態
	int isInputB;			// Bが押されているか

	//アニメーション
	int animNum;
	int motionNum;
	int animIndex;
	int attachAnimIndex;
	double animTimer;
	double animTime;
	double totalAnimTime;

	VECTOR playerPos;		// プレイヤー座標
	VECTOR shadowPos;		// 影の座標
	VECTOR playerVelocity;	//プレイヤーの移動量
	VECTOR playerRotate;	// プレイヤー回転
	VECTOR playerScale;		// プレイヤースケール
	PlayerState playerState; // プレイヤーの動作
	PlayerState lastState;	// 1フレーム前の動作

	void playerMove();
	void playerJump();
	void Animation();
	void ChangeAnim(int animIndex);
	void playerAlpha();
};
