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
	Score = initialScore;
	alpha = 1.0f;
	isJumping = false;
	onGround = true;
	isTakeDamage = false;
	isHit = false;
	isHitting = false;
	isInvincible = false;
	isGameOver = false;
	isInit_Result = false;

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
	Score = initialScore;
	alpha = 1.0f;
	isJumping = false;
	onGround = true;
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
}

void Player::Update()
{
	//�R���g���[���[����
	GetJoypadAnalogInput(&padInputX, &padInputY, DX_INPUT_PAD1);
	padState = GetJoypadInputState(DX_INPUT_PAD1);

	playerVelocity = VGet(0, 0, 0);
	onGround = CheckOnGround();

	if (isHit)
	{
		if (!isHitting)
		{
			isTakeDamage = true;
		}
	}

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


	if (animTime == totalAnimTime&&playerState==DIE||playerPos.y<-100.0f)
	{
		isGameOver = true;
	}


	if (invincibleTimer->hasElapsed())
	{
		isHit = false;
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
	if (onGround)
	{
		shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
		shadowWidth = (-0.05 * playerPos.y) + 1.5f;
		DrawCone3D(shadowPos, VGet(shadowPos.x, shadowPos.y - 0.3, shadowPos.z), shadowWidth, 5, 0xAA000000, 0xAA000000, TRUE);
	}

	MV1DrawModel(playerHandle);
}

void Player::playerMove()
{

	if (!onGround && !isJumping)
	{
		fallSpeed -= Gravity;
	}

	if (playerPos.y > -0.5f)
	{

		//���X�e�B�b�N�����͂���Ă���ꍇ
		if ((padInputX != 0 || padInputY != 0))
		{
			// A�{�^����������Ă���ꍇ
			if ((padState & PAD_INPUT_A) != 0)
			{
				if (!isJumping)
				{
					if (onGround)
					{
						playerState = RUNNING;  // ����
					}
					else
					{
						playerState = JUMP_IDLE;
					}
				}
				moveSpeed = 0.0004f;
			}
			else
			{
				if (!isJumping)
				{
					if (onGround)
					{
						playerState = WALK;  // ����
					}
					else
					{
						playerState = JUMP_IDLE;
					}
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
				if (onGround)
				{
					playerState = STOP;  // IDLE
				}
				else
				{
					playerState = JUMP_IDLE;
				}
			}
		}
	}
}



void Player::playerJump()
{

	//�W�����v���̂ݏd�͂�K��
	if (isJumping)
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
		if (onGround)
		{
			fallSpeed = 0;
			playerPos.y = 0;
			animTimer = 0;
			playerState = JUMP_END;
			isJumping = false;
		}
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
	case RESULTS:		ChangeAnim(Anim_RESULTS);		break;

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

	if (isTakeDamage)
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
		if (onGround)
		{
			fallSpeed = 0;
			playerPos.y = 0;
			isJumping = false;
			isTakeDamage = false;
		}

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
	else
	{
		alpha = 1.0;
	}
}

bool Player::CheckOnGround()
{
	float absX = fabs(playerPos.x);
	float absZ = fabs(playerPos.z);

	// �Ђ��`�̏���
	if (absX + absZ <= 40) {
		return true; // �Ђ��`�̓���
	}
	else
	{
		return false;
	}
}

void Player::Title(float waveScale)
{
	playerVelocity = VGet(0, 0, 0);
	playerRotate = VGet(0, -135 * DX_PI_F / 180.0f, 0);

	//�W�����v���̂ݏd�͂�K��
	if (isJumping)
	{
		fallSpeed -= Gravity;
	}

	if (!isJumping)
	{
		playerState = STOP;
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
		if (onGround)
		{
			fallSpeed = 0;
			playerPos.y = 0;
			animTimer = 0;
			playerState = JUMP_END;
			isJumping = false;
		}
	}

	//�������x���ړ��ʂɉ�����
	playerVelocity.y += fallSpeed;


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

void Player::Results_Update()
{
	if (!isInit_Result)
	{
		playerPos = VGet(-5, -3, 0);
		playerRotate = VGet(0, -25 * DX_PI_F / 180.0f, 0);
		playerState = RESULTS;
	}


	MV1SetPosition(playerHandle, playerPos);
	MV1SetRotationXYZ(playerHandle, playerRotate);
	Animation();	// �A�j������

	//�v���C���[�̏�Ԃ�ۑ�
	lastState = playerState;
}