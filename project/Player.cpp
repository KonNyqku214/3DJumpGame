#include"Player.h"

Player::Player()
{
	playerHandle = MV1LoadModel("data/model/Character/Rogue.mv1");
	playerState = STOP;
	lastState = STOP;
	playerPos = VGet(0, 0, 0);
	shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
	playerWidth = 1.5f;
	shadowWidth = (-0.05 * playerPos.y) + 1.5f;
	playerVelocity = VGet(0, 0, 0);
	playerRotate = VGet(0, 0, 0);
	playerScale = VGet(3, 3, 3);
	HP = 3;
	Score = 0;
	alpha = 1.0f;
	isJumping = false;
	isTakeDamage = false;
	isHit = false;
	isHitting = false;
	isInvincible = false;
	isGameOver = false;

	moveSpeed = 0.0001f;
	fallSpeed = 0.0f;

	invincibleTimer = new Timer(3000);
	Title_jumpTimer = new Timer(3000);
	alpha = 1.0;
	alphaTime = 0;
	padInputX = 0;
	padInputY = 0;
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	isInputB = false;


	animNum = MV1GetAnimNum(playerHandle);
	MV1DetachAnim(playerHandle, 0);
	animIndex = 1;
	attachAnimIndex = MV1AttachAnim(playerHandle, animIndex, -1, FALSE);
	animTimer = 0;
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);

	MV1SetScale(playerHandle, VGet(0.01, 0.01, 0.01));

	// �t�H���g���ꎞ�I�ɃV�X�e���ɓo�^
	fontFilePath = "data/font/ZenMaruGothic-Black.ttf";
	fontCount = AddFontResourceEx(fontFilePath, FR_PRIVATE, NULL);
	fontHandle_Score = CreateFontToHandle("Zen Maru Gothic Black", 24, 3, DX_FONTTYPE_ANTIALIASING);

}

Player::~Player()
{
	MV1DeleteModel(playerHandle);

}

void Player::Init()
{
	playerState = STOP;
	lastState = STOP;
	playerPos = VGet(0, 0, 0);
	playerWidth = 1.5f;
	shadowWidth = (-0.05 * playerPos.y) + 1.5f;
	shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
	playerVelocity = VGet(0, 0, 0);
	playerRotate = VGet(0, 0, 0);
	playerScale = VGet(3, 3, 3);
	HP = 3;
	Score = 0;
	alpha = 1.0f;
	isJumping = false;
	isTakeDamage = false;
	isHit = false;
	isHitting = false;
	isInvincible = false;
	isGameOver = false;

	moveSpeed = 0.0001f;
	fallSpeed = 0.0f;

	invincibleTimer = new Timer(3000);
	alpha = 1.0;
	alphaTime = 0;
	padInputX = 0;
	padInputY = 0;
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	isInputB = false;


	animNum = MV1GetAnimNum(playerHandle);
	MV1DetachAnim(playerHandle, 0);
	animIndex = 1;
	attachAnimIndex = MV1AttachAnim(playerHandle, animIndex, -1, FALSE);
	animTimer = 0;
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);

	MV1SetScale(playerHandle, VGet(0.01, 0.01, 0.01));

	// �t�H���g���ꎞ�I�ɃV�X�e���ɓo�^
	fontFilePath = "data/font/ZenMaruGothic-Black.ttf";
	fontCount = AddFontResourceEx(fontFilePath, FR_PRIVATE, NULL);
	fontHandle_Score= CreateFontToHandle("ZenMaruGothic-Black", 24, 3, DX_FONTTYPE_ANTIALIASING);
}

void Player::Update()
{
	//�R���g���[���[����
	GetJoypadAnalogInput(&padInputX, &padInputY, DX_INPUT_PAD1);
	padState = GetJoypadInputState(DX_INPUT_PAD1);

	playerVelocity = VGet(0, 0, 0);


	//�_���[�W���󂯂Ă��Ȃ������瑀��\
	if (!isTakeDamage)
	{
		if (HP > 0)
		{
			playerMove();	// �ړ�����
			playerJump();	// �W�����v����
			playerAlpha();	// �_���[�W�œ����؂�ւ�
		}
	}
	else
	{
		takeDamage();
	}
	if (isHit)
	{
		if (!isHitting)
		{
			isTakeDamage = true;
		}
	}


	if (animTime == totalAnimTime&&playerState==DIE)
	{
		isGameOver = true;
	}


	if (invincibleTimer->hasElapsed())
	{
		isHitting = false;
		isInvincible = false;
	}

	Animation();	// �A�j������

	//���W�A��]�A�T�C�Y�Ȃǂ��Z�b�g
	playerPos = VAdd(playerPos, playerVelocity);
	MV1SetPosition(playerHandle, playerPos);
	MV1SetRotationXYZ(playerHandle, playerRotate);
	MV1SetScale(playerHandle, playerScale);
	MV1SetOpacityRate(playerHandle, alpha);

	//�v���C���[�̏�Ԃ�ۑ�
	lastState = playerState;
}

void Player::Draw()
{
	//�ۉe�̕`��
	shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
	shadowWidth = (-0.05 * playerPos.y) + 1.5f;
	DrawCone3D(shadowPos, VGet(shadowPos.x, shadowPos.y - 0.3, shadowPos.z), shadowWidth, 5, 0xAA000000, 0xAA000000, TRUE);

	MV1DrawModel(playerHandle);
}

void Player::playerMove()
{


	//���X�e�B�b�N�����͂���Ă���ꍇ
	if ((padInputX != 0 || padInputY != 0))
	{
		// A�{�^����������Ă���ꍇ
		if ((padState & PAD_INPUT_A) != 0)
		{
			if (!isJumping)
			{
				playerState = RUNNING;  // ����
			}
			moveSpeed = 0.0004f;
		}
		else
		{
			if (!isJumping)
			{
				playerState = WALK;      // ����
			}
			moveSpeed = 0.0002f;
		}

		// ���X�e�B�b�N�̓��͂ɉ����Ĉړ��ʂ�ǉ�
		playerVelocity.x += padInputX * moveSpeed;
		playerVelocity.z -= padInputY * moveSpeed;
		// �X�e�B�b�N�̓��͂�����΁A�L�����N�^�[�̌�����ύX����
		playerRotate.y = atan2(padInputX * -1, padInputY); // �i�s�����ɉ�]
	}
	else
	{
		if (!isJumping)
		{
			playerState = STOP;		// ��~��
		}
	}
}


void Player::playerJump()
{

	//�W�����v���̂ݏd�͂�K��
	if (playerPos.y>0)
	{
		fallSpeed -= Gravity;
	}

	//���n����A�{�^���������ꂽ��W�����v
	if (!isJumping)
	{
		if (padState & PAD_INPUT_B)
		{
				fallSpeed += JumpPower;
				isJumping = true;
				playerState = JUMP_START;
		}
	}

	if (playerState == JUMP_START && animTimer == totalAnimTime)
	{
		playerState = JUMP_IDLE;
	}

	//Y���W���O�����������O�ɖ߂�
	if (playerPos.y < 0)
	{
		fallSpeed = 0;
		playerPos.y = 0;
		animTimer = 0;
		playerState = JUMP_END;
		isJumping = false;
	}

	//�������x���ړ��ʂɉ�����
	playerVelocity.y += fallSpeed;
}

void Player::Animation()
{
	animTimer ++;
	animTime = animTimer;
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);
	// �A�j���[�V���������[�v������
	if (animTime > totalAnimTime) {
		animTimer = 0.0;  // �A�j���[�V�����̐i�s���Ԃ����Z�b�g
	}


	switch (playerState)
	{
	case STOP:			ChangeAnim(Anim_IDLE);			break;
	case WALK:			ChangeAnim(Anim_WALK);			break;
	case RUNNING:		ChangeAnim(Anim_RUNNING);		break;
	case JUMPING:		ChangeAnim(Anim_JUMPING);		break;
	case JUMP_START:	ChangeAnim(Anim_JUMP_START);	break;
	case JUMP_IDLE:		ChangeAnim(Anim_JUMP_IDLE);		break;
	case JUMP_END:		ChangeAnim(Anim_JUMP_END);		break;
	case DAMAGE:		ChangeAnim(Anim_DAMAGE);		break;
	case DIE:			ChangeAnim(Anim_DIE);			break;

	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(playerHandle, attachAnimIndex, animTime);
}

void Player::ChangeAnim(int motionNum)
{
	MV1DetachAnim(playerHandle,attachAnimIndex);

	if (lastState != playerState)
	{
		animTimer = 0;
	}

	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	attachAnimIndex = MV1AttachAnim(playerHandle, motionNum, -1, FALSE);
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);
}

void Player::takeDamage()
{

	if (playerPos.y > 0)
	{
		fallSpeed -= Gravity;
		// �v���C���[�̌����ɑ΂��Ĕ��Ε����փm�b�N�o�b�N���邽�߁A��]�p����ړ��ʂ��v�Z
		playerVelocity.x += sin(playerRotate.y) * KnockBackPower;
		playerVelocity.z += cos(playerRotate.y) * KnockBackPower;
	}

	//�_���[�W���󂯂��ꍇ
	if (isTakeDamage)
	{
		//�q�b�g������Ȃ����
		if (!isHitting)
		{
			playerState = DAMAGE;
			fallSpeed = 0;
			fallSpeed += JumpPower / 1.5;
			isHitting = true;
			HP--;
			if (!invincibleTimer->isActive())
			{
				invincibleTimer->start();
				isInvincible = true;
			}
		}
	}
	

	if (playerState == DAMAGE && animTime == totalAnimTime)
	{
		playerState = JUMP_IDLE;
	}

	if (playerPos.y < 0)
	{
		fallSpeed = 0;
		playerPos.y = 0;
		isJumping = false;
		isTakeDamage = false;

		if (HP <= 0)
		{
			playerState = DIE;
		}
		//isHitting = false;

		//if (!invincibleTimer->isActive())
		//{
		//	invincibleTimer->start();
		//	isInvincible = true;
		//}
	}


	//�������x���ړ��ʂɉ�����
	playerVelocity.y += fallSpeed;
}

void Player::DrawDebug()
{
	// �f�o�b�O�����蔻��.
	DrawSphere3D(playerPos, playerWidth, 5, 0x00ffff, 0x00ffff, false);

	if (CheckHitKey(KEY_INPUT_D))
	{
		isTakeDamage = true;
	}

}

void Player::playerAlpha()
{
	if (isHitting)
	{
		alphaTime++;
		if (alphaTime == 5)
		{
			alphaTime = 0;
			if (alpha == 0.0f)
			{
				alpha = 1.0;
			}
			else
			{
				alpha = 0.0;
			}
		}
	}
}

void Player::Title(float waveScale)
{
	playerVelocity = VGet(0, 0, 0);

	if (!isJumping)
	{
		playerState = STOP;
	}
		//�W�����v���̂ݏd�͂�K��
	if (playerPos.y > 0)
	{
		fallSpeed -= Gravity;
	}

	//���n����A�{�^���������ꂽ��W�����v
	if (!isJumping)
	{
		if (waveScale>=0.04f&&waveScale<=0.045f)
		{
			fallSpeed += JumpPower;
			isJumping = true;
			playerState = JUMP_START;
		}
	}

	if (playerState == JUMP_START && animTimer == totalAnimTime)
	{
		playerState = JUMP_IDLE;
	}

	//Y���W���O�����������O�ɖ߂�
	if (playerPos.y < 0)
	{
		fallSpeed = 0;
		playerPos.y = 0;
		animTimer = 0;
		playerState = JUMP_END;
		isJumping = false;
	}

	//�������x���ړ��ʂɉ�����
	playerVelocity.y += fallSpeed;

	Animation();	// �A�j������

	//���W�A��]�A�T�C�Y�Ȃǂ��Z�b�g
	playerRotate = VGet(0, -135 * DX_PI_F / 180.0f, 0);
	playerPos = VAdd(playerPos, playerVelocity);
	MV1SetPosition(playerHandle, playerPos);
	MV1SetRotationXYZ(playerHandle, playerRotate);
	MV1SetScale(playerHandle, playerScale);

	//�v���C���[�̏�Ԃ�ۑ�
	lastState = playerState;

}