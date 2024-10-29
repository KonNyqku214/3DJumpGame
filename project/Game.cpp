#include "Game.h"

// コンストラクタ
Game::Game()
    : currentState(TITLE), isDebugMode(false), inputF1(false) , frameCount(0), startTime(GetNowCount()),
    ply1(VGet(-24,0,0)),camera(player.getPlayerPos())// 初期状態はタイトル画面
{
    // 必要な初期化処理を行う
    fps = 0;
    currentTime = 0;
    ObjCode = 0;
    isInit = false;
    for (int i = 0; i < 4; i++)
    {
        enemy[i] = new Enemy(background.getS_groundPos(i));
    }
    TitleEnemy = new Enemy(background.getTitle_groundPos());
    enemyAwakenTime = new Timer(7000);  //7000ミリ秒ごとに敵が起きる
    Title_START = LoadGraph("data/picture/Title_Start.png");
    Title_SETTINGS = LoadGraph("data/picture/Title_Settings.png");
    Title_IsStart = TRUE;
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
        camera.Init(player.getPlayerPos());
        background.Init();
        player.Init();
        for (int i = 0; i < 4; i++)
        {
            enemy[i]->Init(background.getS_groundPos(i));
        }

        isInit = true;
    }

    camera.Title_Update();
    background.Title_Update();
    player.Title(TitleEnemy->getWaveScale());
    TitleEnemy->Title_Update();

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        currentState = PLAYING; // スペースキーが押されたらゲーム開始
        isInit = false;
    }
}

// ゲーム中の更新処理
void Game::UpdatePlaying()
{
    camera.Update(player.getPlayerPos());
    background.Update();
    player.Update();
    if (!enemy[0]->setIsRunning())
    {
        enemy[0]->setIsRunning() = true;
    }
    enemyAwaken();
    for (int i = 0; i < 4; i++)
    {
        enemy[i]->Update(player.getPlayerPos());
        player.setIsHit() = hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(),player.getScore());
    }

    if (isDebugMode)
    {
        if(CheckHitKey(KEY_INPUT_RETURN))
        {
            currentState = TITLE;       //デバッグ中にエンターでタイトル
        }
    }

    // プレイが終了したらリザルト画面に遷移する処理
}

// リザルト画面の更新処理
void Game::UpdateResult()
{
    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        currentState = TITLE; // エンターキーが押されたらタイトル画面に戻る
    }
}

// タイトル画面の描画処理
void Game::DrawTitle()
{
    background.Title_Draw();
    player.Draw();
    TitleEnemy->Draw();
    if (Title_IsStart)
    {
        DrawGraph(0, 0, Title_START, TRUE);
    }
    else
    {
        DrawGraph(0, 0, Title_SETTINGS, TRUE);
    }

    DrawStringToHandle(1300, 20, "カスタムフォント１２３４", GetColor(255, 255, 255), player.fontHandle_Score);
    DrawString(100, 100, "Press SPACE to Start", GetColor(255, 255, 255));
}

// ゲーム中の描画処理
void Game::DrawPlaying()
{
    background.Draw();
    for (int i = 0; i < 4; i++)
    {
        enemy[i]->Draw();
    }
    //enemy2->Draw();
    player.Draw();

    if (isDebugMode)
    {
        player.DrawDebug();
        for (int i = 0; i < 4; i++)
        {
            enemy[i]->DrawDebug();
        }
    }

    if (player.getIsGameOver())
    {
        currentState = RESULT;
    }
}

// リザルト画面の描画処理
void Game::DrawResult()
{
    DrawString(100, 100, "Result Screen - Press ENTER to Return to Title", GetColor(255, 255, 255));
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
    DrawFormatString(10, 290, GetColor(255, 0, 0), "PlayerHP : %d", player.getPlayerHP());
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
    int i = 1;
    if (i < 4)
    {
        if (!enemyAwakenTime->isActive())
        {
            enemyAwakenTime->start();
        }

        if (enemyAwakenTime->hasElapsed())
        {
            enemy[i]->setIsRunning() = true;
            i++;
        }
    }
}