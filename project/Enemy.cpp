#include"Enemy.h"

Enemy::Enemy(VECTOR groundPos)
{
	wave_Normal = MV1LoadModel("data/model/Wave/ShockWave_Normal.mv1");
	wave_Slow = MV1LoadModel("data/model/Wave/ShockWave_Slow.mv1");
	wave_High = MV1LoadModel("data/model/Wave/ShockWave_High.mv1");
	enemyHandle = MV1LoadModel("data/model/Character/Skeleton_Mage.mv1");

	waveSpeed = 0.0015f;

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
	waveState = NORMAL;
	isAttack = false;
	waveRunning = false;
	isRunning = false;
	titleInit = false;
	waitTimer = new Timer(4000);	//待機時間を5000ミリ秒に設定

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);
	MV1SetScale(wave_Normal, waveScale);
}

Enemy::~Enemy()
{
	MV1DeleteModel(wave_Normal);
	MV1DeleteModel(enemyHandle);
}

void Enemy::Init(VECTOR groundPos)
{
	hitRadius = 5;
	hitRadiusWidth = 1.5;
	safeRadius = (waveScale.x * hitRadiusScale) - hitRadiusWidth-3.0;

	waveSpeed = 0.0015f;

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
	waveState = NORMAL;
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

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);
	MV1SetScale(wave_Normal, waveScale);
}

void Enemy::Update(VECTOR playerPos,int score)
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
			AttackManager(score);
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

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);

	MV1SetPosition(wave_Slow, wavePos);
	MV1SetRotationXYZ(wave_Slow, waveRotate);

	MV1SetPosition(wave_High, wavePos);
	MV1SetRotationXYZ(wave_High, waveRotate);


	switch (waveState)
	{
	case NORMAL:	
		MV1SetPosition(wave_Normal, wavePos);
		MV1SetRotationXYZ(wave_Normal, waveRotate);
		MV1SetScale(wave_Normal, waveScale);
		break;
	case SLOW:
		MV1SetPosition(wave_Slow, wavePos);
		MV1SetRotationXYZ(wave_Slow, waveRotate);
		MV1SetScale(wave_Slow, waveScale);
		break;
	case HIGH:
		MV1SetPosition(wave_High, wavePos);
		MV1SetRotationXYZ(wave_High, waveRotate);
		MV1SetScale(wave_High, waveScale);
		break;
	}


	lastState = enemyState;
}

void Enemy::Draw()
{
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	DrawCone3D(shadowPos, VGet(shadowPos.x, shadowPos.y - 0.3, shadowPos.z), shadowWidth, 5, 0xAA000000, 0xAA000000, TRUE);
	MV1DrawModel(enemyHandle);
	if (waveRunning)
	{
		switch (waveState)
		{
		case NORMAL:	MV1DrawModel(wave_Normal);	break;
		case SLOW:		MV1DrawModel(wave_Slow);	break;
		case HIGH:		MV1DrawModel(wave_High);	break;
		}
	}
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

void Enemy::AttackManager(int score)
{
	if (!isAttack)
	{
		wavePos = enemyPos;
		if (!waitTimer->isActive())
		{
			//待機時間を4～6でランダムに設定
			switch (waveState)
			{
			case NORMAL:	waitTimer->setDuration((GetRand(2000) + 3000));		break;
			case SLOW:		waitTimer->setDuration((GetRand(2000) + 6000));		break;
			case HIGH:		waitTimer->setDuration((GetRand(2000) + 2000));		break;
			}
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
			waveRotate.y += 0.05;
		}
		else if(waveScale.x > maxWaveScale)//リセット
		{
			hitRadiusWidth = 1.5;
			waveScale.x = 0.001;
			waveScale.z = 0.001;
			waveRotate.y = 0;
			waveRunning = false;

			//次の攻撃選択
			int random = rand() % 100;

			if (score < 50) {
				waveState = NORMAL;  // スコアが50未満ならノーマル攻撃
			}
			else if (score < 100) {
				waveState = (random <= 70) ? NORMAL : SLOW;  // スコアが50以上100未満なら、70%でノーマル、30%でスロー攻撃
			}
			else if (score < 200) {
				if (random <= 70) {
					waveState = NORMAL;  // 70%でノーマル
				}
				else if (random <= 85) {
					waveState = SLOW;    // 15%でスロー
				}
				else {
					waveState = HIGH;    // 15%でハイ
				}
			}

			switch (waveState)	//波速度変更
			{
			case NORMAL:	waveSpeed = 0.0015;	break;
			case SLOW:		waveSpeed = 0.001;	break;
			case HIGH:		waveSpeed = 0.002;	break;
			}

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
			waveRotate.y += 0.05;
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

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);
	MV1SetScale(wave_Normal, waveScale);

	lastState = enemyState;

}