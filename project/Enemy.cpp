#include"Enemy.h"

Enemy::Enemy(VECTOR groundPos)
{
	waveHandle = MV1LoadModel("data/model/Wave/ShockWave.mv1");
	enemyHandle = MV1LoadModel("data/model/Character/Skeleton_Mage.mv1");

	hitRadius = 5;
	hitRadiusWidth = 1.5;
	safeRadius = (waveScale.x * hitRadiusScale) - hitRadiusWidth;

	enemyPos = groundPos;
	enemyRotate = VGet(0, 0, 0);
	enemyScale = VGet(3, 3, 3);
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	shadowWidth = 1.5f;
	wavePos = enemyPos;
	waveRotate = VGet(0, 0, 0);
	waveScale = VGet(0.01, 0.01, 0.01);

	runningEnemyNum = 1;
	enemyState = INACTIVE;
	lastState = INACTIVE;
	isAttack = false;
	waveRunning = false;
	isRunning = false;
	titleInit = false;
	waitTimer = new Timer(4000);	//待機時間を5000ミリ秒に設定

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(waveHandle, wavePos);
	MV1SetRotationXYZ(waveHandle, waveRotate);
	MV1SetScale(waveHandle, waveScale);
}

Enemy::~Enemy()
{
	MV1DeleteModel(waveHandle);
	MV1DeleteModel(enemyHandle);
}

void Enemy::Init(VECTOR groundPos)
{
	hitRadius = 5;
	hitRadiusWidth = 1.5;
	safeRadius = (waveScale.x * hitRadiusScale) - hitRadiusWidth-3.0;

	enemyPos = groundPos;
	enemyRotate = VGet(0, 0, 0);
	enemyScale = VGet(3, 3, 3);
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	shadowWidth = 1.5f;
	wavePos = enemyPos;
	waveRotate = VGet(0, 0, 0);
	waveScale = VGet(0.01, 0.01, 0.01);

	runningEnemyNum = 1;
	enemyState = INACTIVE;
	lastState = INACTIVE;
	isAttack = false;
	waveRunning = false;
	isRunning = false;
	titleInit = false;

	wavePos = VGet(30, 0, 0);
	waveRotate = VGet(0, 0, 0);
	waveScale = VGet(0.001, 0.01, 0.001);
	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(waveHandle, wavePos);
	MV1SetRotationXYZ(waveHandle, waveRotate);
	MV1SetScale(waveHandle, waveScale);
}

void Enemy::Update(VECTOR playerPos)
{
	if (isRunning)
	{
		if (enemyState == INACTIVE)
		{
			enemyState = AWAKEN;
			animTime = 0;
		}
		if (enemyState==AWAKEN&&animTime == totalAnimTime)
		{
			enemyState = IDLE;
		}
		//起動中
		if (enemyState == IDLE || enemyState == ATTACK)
		{
			calcRotateY(playerPos);
			AttackManager();
		}
	}
	else
	{
		enemyState = INACTIVE;
	}

	Animation();	// アニメ処理

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(waveHandle, wavePos);
	MV1SetRotationXYZ(waveHandle, waveRotate);
	MV1SetScale(waveHandle, waveScale);

	lastState = enemyState;
}

void Enemy::Draw()
{
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	DrawCone3D(shadowPos, VGet(shadowPos.x, shadowPos.y - 0.3, shadowPos.z), shadowWidth, 5, 0xAA000000, 0xAA000000, TRUE);
	MV1DrawModel(enemyHandle);
	if(waveRunning){MV1DrawModel(waveHandle);}
}

void Enemy::DrawDebug()
{
	// 衝撃波のあたり判定.
	DrawSphere3D(wavePos, (waveScale.x * hitRadiusScale), 5, 0x00ffff, 0x00ffff, false);//外側
	DrawSphere3D(wavePos, (waveScale.x * hitRadiusScale) - hitRadiusWidth-3.0 , 5, 0x00ffff, 0x00ffff, false);//内側
}

void Enemy::calcRotateY(VECTOR playerPos)
{
	float deltaX = playerPos.x - enemyPos.x;
	float deltaZ = playerPos.z - enemyPos.z;

	// atan2を使用して、Y軸の回転角度（ラジアン）を計算
	float angle = atan2(-deltaX, -deltaZ);

	// currentPos の Y軸回転（currentRotate.y）をターゲット方向の角度に設定
	enemyRotate.y = angle;
}

void Enemy::AttackManager()
{
	if (!isAttack)
	{
		wavePos = enemyPos;
		if (!waitTimer->isActive())
		{
			//待機時間を4～6でランダムに設定
			waitTimer->setDuration((GetRand(2000) + 3000));
			waitTimer->start();	//タイマースタート
		}
		enemyState = ATTACK;	//攻撃中に変更
		isAttack = true;
	}
	else
	{
		if (enemyState==ATTACK&&animTime == 18)
		{
			waveRunning = true;
		}
		if ((waveScale.x < maxWaveScale)&& waveRunning )//衝撃波を大きく
		{
			waveScale.x += waveSpeed;
			waveScale.z += waveSpeed;
			hitRadiusWidth += 0.05;
			waveRotate.y += 0.1;
		}
		else if(waveScale.x > maxWaveScale)//リセット
		{
			hitRadiusWidth = 1.5;
			waveScale.x = 0.001;
			waveScale.z = 0.001;
			waveRotate.y = 0;
			waveRunning = false;
		}

		if (animTime == totalAnimTime)
		{
			enemyState = IDLE;	//IDLEに変更
		}

		if (waitTimer->hasElapsed())
		{
			isAttack = false;
		}
	}
}

void Enemy::Animation()
{
	animTimer++;
	animTime = animTimer;
	totalAnimTime = MV1GetAttachAnimTotalTime(enemyHandle, attachAnimIndex);
	// アニメーションをループさせる
	if (animTime > totalAnimTime) {
		animTimer = 0.0;  // アニメーションの進行時間をリセット
	}


	switch (enemyState)
	{
	case IDLE:		ChangeAnim(Enemy_IDLE);		break;
	case ATTACK:	ChangeAnim(Enemy_Attack);	break;
	case INACTIVE:	ChangeAnim(Enemy_Inective);	break;
	case AWAKEN:	ChangeAnim(Enemy_Awaken);	break;

	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(enemyHandle, attachAnimIndex, animTime);
}

void Enemy::ChangeAnim(int motionNum)
{
	MV1DetachAnim(enemyHandle, attachAnimIndex);

	if (lastState != enemyState)
	{
		animTimer = 0;
	}

	// ３Ｄモデルの０番目のアニメーションをアタッチする
	attachAnimIndex = MV1AttachAnim(enemyHandle, motionNum, -1, FALSE);
	// アタッチしたアニメーションの総再生時間を取得する
	totalAnimTime = MV1GetAttachAnimTotalTime(enemyHandle, attachAnimIndex);
}

void Enemy::Title_Update()
{
	if (!titleInit)
	{
		enemyState = IDLE;
		waitTimer->setDuration(3000);
		titleInit = true;
	}

	calcRotateY(VGet(0, 0, 0));

	if (!isAttack)
	{
		wavePos = enemyPos;
		if (!waitTimer->isActive())
		{
			waitTimer->start();	//タイマースタート
		}
		enemyState = ATTACK;	//攻撃中に変更
		isAttack = true;
	}
	else
	{
		if (enemyState == ATTACK && animTime == 18)
		{
			waveRunning = true;
		}
		if ((waveScale.x < 0.1f) && waveRunning)//衝撃波を大きく
		{
			waveScale.x += waveSpeed;
			waveScale.z += waveSpeed;
			waveRotate.y += 0.1;
		}
		else if (waveScale.x > 0.1f)//リセット
		{
			waveScale.x = 0.001;
			waveScale.z = 0.001;
			waveRotate.y = 0;
			waveRunning = false;
		}

		if (animTime == totalAnimTime)
		{
			enemyState = IDLE;	//IDLEに変更
		}

		if (waitTimer->hasElapsed())
		{
			isAttack = false;
		}
	}
	Animation();	// アニメ処理

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(waveHandle, wavePos);
	MV1SetRotationXYZ(waveHandle, waveRotate);
	MV1SetScale(waveHandle, waveScale);

	lastState = enemyState;

}