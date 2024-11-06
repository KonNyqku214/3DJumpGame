#include "Game.h"

// コンストラクタ
Game::Game()
    : currentState(TITLE), isDebugMode(false), inputF1(false) , frameCount(0), startTime(GetNowCount()),
    ply1(VGet(-24,0,0)),camera(player.getPlayerPos())// 初期状態はタイトル画面
{
    // 必要な初期化処理を行う
    Title_Start = LoadGraph("data/picture/Title_Start.png");
    Title_Settings = LoadGraph("data/picture/Title_Settings.png");
    Score_UI = LoadGraph("data/picture/Score_UI.png");
    Score_Effect = LoadGraph("data/picture/+1.png");
    HP_UI = LoadGraph("data/picture/HP.png");
    GameOver_Die = LoadGraph("data/picture/GameOver_Die.png");
    GameOver_Fall = LoadGraph("data/picture/GameOver_Fall.png");
    Results_UI = LoadGraph("data/picture/results.png");
    Title_isStart = true;
    isInit = false;
    isDrawingScoreEffect = false;
    LPCSTR ScoreFontpath = "ZenMaruGothic-Black.ttf";
    ChangeFont("Zen Maru Gothic Black", DX_CHARSET_DEFAULT);
    FontHandle = CreateFontToHandle("Zen Maru Gothic Black", 40, 4);

    fps = 0;
    enemyNum = 1;
    currentTime = 0;
    ObjCode = 0;
    inputA = false;
    hitStop = false;
    img_Y = -700.0f;

    for (int i = 0; i < maxGroundNum; i++)
    {
        enemy[i] = new Enemy(background.getS_groundPos(i));
        addScore[i] = false;
        effectPos[i] = VGet(0, 0, 0);
    }

    TitleEnemy = new Enemy(background.getTitle_groundPos());
    enemyAwakenTime = new Timer(7000);  //7000ミリ秒ごとに敵が起きる
    hitStopTimer = new Timer(300);      //ダメージ時300ミリ秒のヒットストップ
    waitChangeTimer = new Timer(2000);  //シーン切り替え待機時間(2秒)
}

// 更新処理
void Game::Update()
{
    int frameStartTime = GetNowCount(); // フレームの開始時刻を取得

    // デバッグモードの切り替え
    ToggleDebugMode();
    if (isDebugMode)
    {
        SelectDebugObject();

        switch (ObjCode)
        {
        case 0:
            // オブジェクトの移動処理（デバッグモード中のみ）
            DebugMove(player.getPlayerPos(), player.getPlayerRotate(), player.getPlayerScale());
            break;
        case 1:
            DebugMove(background.getGroundPos(), background.getGroundRotate(), background.getGroundScale());
            break;
        case 2:
            DebugMove(enemy[0]->getEnemyPos(), enemy[0]->getEnemyRotate(), enemy[0]->getEnemyScale());
            break;
        case 3:
            camera.DebugMode();
            break;
        }
    }


    switch (currentState)
    {
    case TITLE:
        UpdateTitle();
        break;
    case PLAYING:
        UpdatePlaying();
        break;
    case RESULT:
        UpdateResult();
        break;
    }

    
    // FPSを60に固定するための待機処理
    int frameEndTime = GetNowCount(); // フレームの終了時刻を取得
    int elapsedTime = frameEndTime - frameStartTime; // 経過時間を計算

    int waitTime = 1000 / 60 - elapsedTime; // 1フレームあたりの時間から経過時間を引く
    if (waitTime > 0) {
        WaitTimer(waitTime); // 残りの時間だけ待機
    }
}

// 描画処理
void Game::Draw()
{
    switch (currentState)
    {
    case TITLE:
        DrawTitle();
        break;
    case PLAYING:
        DrawPlaying();
        break;
    case RESULT:
        DrawResult();
        break;
    }

    // デバッグ情報の描画
    if (isDebugMode) {
        DisplayDebugInfo();
        DisplayFPS();
    }
}

// タイトル画面の更新処理
void Game::UpdateTitle()
{
    if (!isInit)
    {
        enemyNum = 1;
        enemyAwakenTime->reset();
        camera.Init(player.getPlayerPos());
        background.Init();
        player.Init();
        for (int i = 0; i < maxGroundNum; i++)
        {
            enemy[i]->Init(background.getS_groundPos(i));
        }

        isInit = true;
    }

    camera.Title_Update();
    background.Title_Update();
    player.Title(TitleEnemy->getWaveScale());
    TitleEnemy->Title_Update();

    padState = GetJoypadInputState(DX_INPUT_PAD1);
    if ((padState&PAD_INPUT_A)!=0)
    {
        if (!inputA)
        {
            currentState = PLAYING; // スペースキーが押されたらゲーム開始
            isInit = false;
        }
        inputA = true;
    }
    else
    {
        inputA = false;
    }
}

// ゲーム中の更新処理
void Game::UpdatePlaying()
{
    if (!player.getIsGameOver())
    {

        if (player.getPlayerPos().y > -0.5)
        {
            HitStopManager();
        }
        camera.Update(player.getPlayerPos());
        background.Update();
        item.Update();
        if (!hitStop)
        {
            player.Update();
        }

        if (!enemy[0]->setIsRunning())
        {
            enemy[0]->setIsRunning() = true;
        }
        enemyAwaken();

        if (CheckHitKey(KEY_INPUT_F3))
        {
            if (!inputF3)
            {
                enemyStop = !enemyStop;
            }
            inputF3 = true;
        }
        else
        {
            inputF3 = false;
        }

        if (!enemyStop && !hitStop)
        {
            for (int i = 0; i < maxGroundNum; i++)
            {
                enemy[i]->Update(player.getPlayerPos());
            }
        }

        if (player.getPlayerPos().y >= -0.5)
        {
            for (int i = 0; i < maxGroundNum; i++)
            {
                if (!player.setIsHit())
                {
                    player.setIsHit() = hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(), player.setScore(), addScore[i], player.getIsHitting(), effectPos[i]);

                }
            }
        }


        if (isDebugMode)
        {
            if (CheckHitKey(KEY_INPUT_RETURN))
            {
                currentState = TITLE;       //デバッグ中にエンターでタイトル
            }
        }

    }
    // プレイが終了したらリザルト画面に遷移する処理
    else
    {
        if (!waitChangeTimer->isActive())
        {
            waitChangeTimer->start();
        }

        if (waitChangeTimer->hasElapsed())
        {
            currentState = RESULT;
        }

    }

}

// リザルト画面の更新処理
void Game::UpdateResult()
{
    padState = GetJoypadInputState(DX_INPUT_PAD1);

    if (!isInit)
    {
        enemyNum = 1;
        enemyAwakenTime->reset();
        player.Init();
        camera.Init(player.getPlayerPos());
        background.Init();
        for (int i = 0; i < maxGroundNum; i++)
        {
            enemy[i]->Init(background.getS_groundPos(i));
        }

        isInit = true;
    }



    if ((padState & PAD_INPUT_A) != 0)
    {
        if (!inputA)
        {
            currentState = TITLE; 
        }
        inputA = true;
        isInit = false;
    }
    else
    {
        inputA = false;
    }
}

// タイトル画面の描画処理
void Game::DrawTitle()
{
    background.Title_Draw();
    player.Draw();
    TitleEnemy->Draw();
    if (Title_isStart)
    {
        DrawGraph(0, 0, Title_Start, TRUE);
    }
    else
    {
        DrawGraph(0, 0, Title_Settings, TRUE);
    }
}

// ゲーム中の描画処理
void Game::DrawPlaying()
{
    background.Draw();
    for (int i = 0; i < maxGroundNum; i++)
    {
        enemy[i]->Draw();
    }
    //enemy2->Draw();
    player.Draw();
    item.Draw();

    if (isDebugMode)
    {
        player.DrawDebug();
        for (int i = 0; i < maxGroundNum; i++)
        {
            enemy[i]->DrawDebug();
        }
    }
    for (int i = 0; i < maxGroundNum; i++)
    {
        isDrawingScoreEffect = addScore[i];
        if (addScore[i])
        {
            DrawScoreEffect(i);
        }
    }

    if (!player.getIsGameOver())
    {
        DrawGraph(0, 0, Score_UI, TRUE);
        DrawFormatStringToHandle(1300, 700, GetColor(255, 255, 255), FontHandle, "%d", player.setScore());
        DrawHP(player.getPlayerHP());
        img_Y = -700;
    }
    else
    {
        if (player.getPlayerPos().y > -0.5f)
        {
            if (img_Y >= 0)
            {
                img_Y = 0;
            }
            else
            {
                img_Y += 100;
            }
            DrawGraph(0, img_Y, GameOver_Die, TRUE);
        }
        else
        {
            if (img_Y >= 0)
            {
                img_Y = 0;
            }
            else
            {
                img_Y += 100;
            }
            DrawGraph(0, img_Y, GameOver_Fall, TRUE);
        }
    }

}

// リザルト画面の描画処理
void Game::DrawResult()
{
    background.Draw();
    DrawExtendGraph(0, 0,1600,900, Results_UI, TRUE);
}

// デバッグモードのオン/オフ切り替え
void Game::ToggleDebugMode()
{
    if (CheckHitKey(KEY_INPUT_F1))
    {
        if (!inputF1)
        {
            isDebugMode = !isDebugMode;
        }
        inputF1 = true;
    }
    else
    {
        inputF1 = false;
    }
}

void Game::SelectDebugObject()
{
    if (CheckHitKey(KEY_INPUT_F2))
    {
        if (!inputF2)
        {
            ObjCode++;
            if (ObjCode > 3)
            {
                ObjCode = 0;
            }
            inputF2 = true;
        }
    }
    else
    {
        inputF2 = false;
    }
}

// デバッグ情報の表示
void Game::DisplayDebugInfo()
{
    //box
    // 描画モードを設定
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(5, 5, 450, 600, GetColor(0, 0, 0), TRUE);
    // ブレンドモードを元に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    // 座標情報を表示
    DrawFormatString(10, 70, GetColor(255, 255, 255), "Camera Pos: (%f, %f, %f)", camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);
    DrawFormatString(10, 90, GetColor(255, 255, 255), "PlayerPos: (%f,%f,%f)", player.getPlayerPos().x, player.getPlayerPos().y, player.getPlayerPos().z);
    DrawFormatString(10, 110, GetColor(255, 255, 255), "PlayerRotate: (%f,%f,%f)", player.getPlayerRotate().x, player.getPlayerRotate().y, player.getPlayerRotate().z);
    DrawFormatString(10, 130, GetColor(255, 255, 255), "PlayerScale: (%f,%f,%f)", player.getPlayerScale().x, player.getPlayerScale().y, player.getPlayerScale().z);
    DrawFormatString(10, 150, GetColor(255, 255, 255), "animTime : %f", player.getAnimTime());
    DrawFormatString(10, 170, GetColor(255, 255, 255), "groundPos: (%f,%f,%f)", background.getGroundPos().x, background.getGroundPos().y, background.getGroundPos().z);
    DrawFormatString(10, 190, GetColor(255, 255, 255), "groundRotate: (%f,%f,%f)", background.getGroundRotate().x, background.getGroundRotate().y, background.getGroundRotate().z);
    DrawFormatString(10, 210, GetColor(255, 255, 255), "groundScale: (%f,%f,%f)", background.getGroundScale().x, background.getGroundScale().y, background.getGroundScale().z);
    DrawFormatString(10, 230, GetColor(255, 255, 255), "EnemyPos: (%f,%f,%f)", enemy[0]->getEnemyPos().x, enemy[0]->getEnemyPos().y, enemy[0]->getEnemyPos().z);
    DrawFormatString(10, 250, GetColor(255, 255, 255), "EnemyRotate: (%f,%f,%f)", enemy[0]->getEnemyRotate().x, enemy[0]->getEnemyRotate().y, enemy[0]->getEnemyRotate().z);
    DrawFormatString(10, 270, GetColor(255, 255, 255), "EnemyScale: (%f,%f,%f)", enemy[0]->getEnemyScale().x, enemy[0]->getEnemyScale().y, enemy[0]->getEnemyScale().z);
    DrawFormatString(10, 290, GetColor(255, 0, 0), "Score : %d", player.setScore());
    DrawFormatString(10, 410, GetColor(255, 255, 255), "enemyAwakenTimer : %f", enemyAwakenTime->getElapsedTime());


    DrawFormatString(10, 310, GetColor(255, 0, 0), "Selected Object : 0%d", ObjCode);
    DrawString(10, 330, "LShift:Y  S:size  R:Rotate", GetColor(255, 0, 0));
    if (player.setIsTakeDamage())
    {
        DrawString(10, 350, "isTakeDamage : true", GetColor(255, 0, 0));
    }
    else
    {
        DrawString(10, 350, "isTakeDamage : false", GetColor(255, 0, 0));
    }
    if (player.getIsHitting())
    {
        DrawString(10, 370, "isHitting : true", GetColor(255, 0, 0));
    }
    else
    {
        DrawString(10, 370, "isHittng : false", GetColor(255, 0, 0));
    }

}

void Game::DebugMove(VECTOR& pos, VECTOR& rotate, VECTOR& scale)
{
    // Sキー押しながら上下矢印で大きさ変更
    if (CheckHitKey(KEY_INPUT_S))
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            scale.x += 0.1;
            scale.y += 0.1;
            scale.z += 0.1;
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            scale.x -= 0.1;
            scale.y -= 0.1;
            scale.z -= 0.1;
        }
        return; // スケール変更が優先された場合、他の処理を実行しない
    }

    // Rキー押しながら左右矢印でY軸回転変更
    if (CheckHitKey(KEY_INPUT_R))
    {
        if (CheckHitKey(KEY_INPUT_RIGHT))
        {
            rotate.y += 0.1;
        }
        else if (CheckHitKey(KEY_INPUT_LEFT))
        {
            rotate.y -= 0.1;
        }
        return; // 回転変更が優先された場合、他の処理を実行しない
    }

    // 左シフトキー押しながら上下矢印でY座標変更
    if (CheckHitKey(KEY_INPUT_LSHIFT))
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            pos.y++;
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            pos.y--;
        }
        return; // Y軸の座標変更が優先された場合、他の処理を実行しない
    }

    // 座標変更（通常の移動）
    if (CheckHitKey(KEY_INPUT_UP))
    {
        pos.z++;
    }
    else if (CheckHitKey(KEY_INPUT_DOWN))
    {
        pos.z--;
    }
    else if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        pos.x++;
    }
    else if (CheckHitKey(KEY_INPUT_LEFT))
    {
        pos.x--;
    }
}

// FPSの表示
void Game::DisplayFPS()
{
    frameCount++;
    if (frameCount % 60 == 0) {
        currentTime = GetNowCount();
        fps = 60000 / (currentTime - startTime);  // FPSを計算
        startTime = currentTime;
    }
    DrawFormatString(10, 10, GetColor(255, 255, 255), "FPS: %d", fps);
}

void Game::enemyAwaken()
{
    if (enemyNum < maxGroundNum)
    {
        if (!enemyAwakenTime->isActive())
        {
            enemyAwakenTime->start();
        }

        if (enemyAwakenTime->hasElapsed())
        {
            enemy[enemyNum]->setIsRunning() = true;
            enemyNum++;
        }
    }
}

void Game::HitStopManager()
{
    for (int i = 0; i < maxGroundNum; i++)
    {
        if (hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(),player.setScore(),addScore[i],player.getIsHitting(),effectPos[i]))
        {
            if (!player.getIsInvincible())
            {
                hitStop = true;
                if (!hitStopTimer->isActive())
                {
                    hitStopTimer->start();
                }
            }
        }
    }

    if (hitStopTimer->hasElapsed())
    {
        hitStop = false;
    }
}

void Game::DrawScoreEffect(int i)
{
    if (addScore[i - 1])
    {
        effectPos[i].y = effectPos[i-1].y + 0.3f;
    }
    DrawBillboard3D(effectPos[i], 0.5f, 0.5f, 5.0f, 0.0f, Score_Effect, TRUE);
    effectPos[i].y += 0.1f;
}

void Game::DrawHP(int HP)
{
    for (int i = 0; i < HP; i++)
    {
        DrawGraph(20 + (150 * i), 20, HP_UI, TRUE);
    }
}