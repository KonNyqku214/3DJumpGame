#pragma once
#include"DxLib.h"
#include"Timer.h"
#include"math.h"
#include <Windows.h> // AddFontResourceEx ���g�����߂ɕK�v

//�萔
const float JumpPower = 1.0f;		//�W�����v��
const float Gravity = 0.05f;		//�d��

const float KnockBackPower = 0.5f;	//�m�b�N�o�b�N�̑傫��

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
	//�R���X�g���N�^�A�f�X�g���N�^
	Player();
	~Player();

	void Init();	// ������
	void Update();	// �X�V
	void Draw();	// �`��
	void DrawDebug();// �f�o�b�O�̕`��

	void Title(float waveScale);	//�^�C�g�����

	int getPlayerHP() { return HP;}						// HP�擾
	int& getScore() { return Score; }					// �X�R�A�擾
	VECTOR& getPlayerPos(){ return playerPos; }			// ���W�擾
	VECTOR& getPlayerRotate(){ return playerRotate; }	// ��]�擾
	VECTOR& getPlayerScale(){ return playerScale; }		// �X�P�[���擾
	double getAnimTime() { return animTime; }			// ���݂̃A�j�����Ԃ��擾
	float getHitRadius() { return playerWidth; }			// �����蔻��̔��a�擾
	Timer* invincibleTimer;
	Timer* Title_jumpTimer;		//�^�C�g����ʂł̃W�����v�Ԋu

	void takeDamage();
	bool& setIsHit() { return isHit; }
	bool& setIsTakeDamage() { return isTakeDamage; }
	bool getIsInvincible() { return isInvincible; }
	bool getIsHitting() { return isHitting; }
	bool getIsGameOver() { return isGameOver; }

	int fontHandle_Score;	// �X�R�A�̃t�H���g�n���h��

private:
	int playerHandle;		// �v���C���[���f���n���h��

	const char* fontFilePath;
	int fontCount;

	float alpha;			// �v���C���[�����x
	int alphaTime;			// �����x�p�^�C�}�[
	int HP;					// �̗�
	int Score;				// �l���X�R�A
	float moveSpeed;		// �v���C���[�̈ړ����x
	float fallSpeed;		// �v���C���[�̗������x
	float maxSpeed;			// �ő�ړ����x
	float playerWidth;		//�v���C���[�̓����蔻��̔��a
	float shadowWidth;
	bool isJumping;			// �W�����v�����ǂ���
	bool isHit;
	bool isTakeDamage;		// �_���[�W���󂯂����ǂ���
	bool isHitting;			// �_���[�W���󂯂Ă���Œ���
	bool isInvincible;		// ���G��Ԃ��ǂ���
	bool isGameOver;		//�Q�[���I�[�o�[��

	//�R���g���[���[
	int padInputX;			// ���X�e�B�b�N��X��
	int padInputY;			// ���X�e�B�b�N��Y��
	int padState;			// PAD�̏��
	int isInputB;			// B��������Ă��邩

	//�A�j���[�V����
	int animNum;
	int motionNum;
	int animIndex;
	int attachAnimIndex;
	double animTimer;
	double animTime;
	double totalAnimTime;

	VECTOR playerPos;		// �v���C���[���W
	VECTOR shadowPos;		// �e�̍��W
	VECTOR playerVelocity;	//�v���C���[�̈ړ���
	VECTOR playerRotate;	// �v���C���[��]
	VECTOR playerScale;		// �v���C���[�X�P�[��
	PlayerState playerState; // �v���C���[�̓���
	PlayerState lastState;	// 1�t���[���O�̓���

	void playerMove();
	void playerJump();
	void Animation();
	void ChangeAnim(int animIndex);
	void playerAlpha();
};