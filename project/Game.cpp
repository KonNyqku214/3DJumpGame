#include "Game.h"

// �R���X�g���N�^
Game::Game()
    : currentState(TITLE), isDebugMode(false), inputF1(false) , frameCount(0), startTime(GetNowCount()),
    ply1(VGet(-24,0,0)),camera(player.getPlayerPos())// ������Ԃ̓^�C�g�����
{
    // �K�v�ȏ������������s��
    Title_Start = LoadGraph("data/picture/Title_Start.png");
    Title_Settings = LoadGraph("data/picture/Title_Settings.png");
    Score_UI = LoadGraph("data/picture/Score_UI.png");
    Score_Effect = LoadGraph("data/picture/+1.png");
    Title_isStart = true;
    isInit = false;
    isDrawingScoreEffect = false;

    fps = 0;
    enemyNum = 1;
    currentTime = 0;
    ObjCode = 0;
    inputA = false;
    hitStop = false;
    for (int i = 0; i < maxGroundNum; i++)
    {
        enemy[i] = new Enemy(background.getS_groundPos(i));
        addScore[i] = false;
    }

    TitleEnemy = new Enemy(background.getTitle_groundPos());
    enemyAwakenTime = new Timer(7000);  //7000�~���b���ƂɓG���N����
    hitStopTimer = new Timer(300);      //�_���[�W��300�~���b�̃q�b�g�X�g�b�v
}

// �X�V����
void Game::Update()
{
    int frameStartTime = GetNowCount(); // �t���[���̊J�n�������擾

    // �f�o�b�O���[�h�̐؂�ւ�
    ToggleDebugMode();
    if (isDebugMode)
    {
        SelectDebugObject();

        switch (ObjCode)
        {
        case 0:
            // �I�u�W�F�N�g�̈ړ������i�f�o�b�O���[�h���̂݁j
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

    
    // FPS��60�ɌŒ肷�邽�߂̑ҋ@����
    int frameEndTime = GetNowCount(); // �t���[���̏I���������擾
    int elapsedTime = frameEndTime - frameStartTime; // �o�ߎ��Ԃ��v�Z

    int waitTime = 1000 / 60 - elapsedTime; // 1�t���[��������̎��Ԃ���o�ߎ��Ԃ�����
    if (waitTime > 0) {
        WaitTimer(waitTime); // �c��̎��Ԃ����ҋ@
    }
}

// �`�揈��
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

    // �f�o�b�O���̕`��
    if (isDebugMode) {
        DisplayDebugInfo();
        DisplayFPS();
    }
}

// �^�C�g����ʂ̍X�V����
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
            currentState = PLAYING; // �X�y�[�X�L�[�������ꂽ��Q�[���J�n
            isInit = false;
        }
        inputA = true;
    }
    else
    {
        inputA = false;
    }
}

// �Q�[�����̍X�V����
void Game::UpdatePlaying()
{
    HitStopManager();
    camera.Update(player.getPlayerPos());
    background.Update();
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

    if (!enemyStop&&!hitStop)
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
                player.setIsHit() = hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(),player.setScore(),addScore[i],player.getIsHitting());
            }
        }
    }

    if (isDebugMode)
    {
        if(CheckHitKey(KEY_INPUT_RETURN))
        {
            currentState = TITLE;       //�f�o�b�O���ɃG���^�[�Ń^�C�g��
        }
    }

    // �v���C���I�������烊�U���g��ʂɑJ�ڂ��鏈��
    if (player.getIsGameOver())
    {
        currentState = RESULT;
    }

}

// ���U���g��ʂ̍X�V����
void Game::UpdateResult()
{
    padState = GetJoypadInputState(DX_INPUT_PAD1);
    if ((padState & PAD_INPUT_A) != 0)
    {
        if (!inputA)
        {
            currentState = TITLE; // �X�y�[�X�L�[�������ꂽ��Q�[���J�n
        }
        inputA = true;
    }
    else
    {
        inputA = false;
    }
}

// �^�C�g����ʂ̕`�揈��
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

// �Q�[�����̕`�揈��
void Game::DrawPlaying()
{
    background.Draw();
    for (int i = 0; i < maxGroundNum; i++)
    {
        enemy[i]->Draw();
    }
    //enemy2->Draw();
    player.Draw();

    if (isDebugMode)
    {
        player.DrawDebug();
        for (int i = 0; i < maxGroundNum; i++)
        {
            enemy[i]->DrawDebug();
        }
    }

    DrawGraph(0, 0, Score_UI, TRUE);
}

// ���U���g��ʂ̕`�揈��
void Game::DrawResult()
{
    DrawString(100, 100, "Result Screen - Press ENTER to Return to Title", GetColor(255, 255, 255));
}

// �f�o�b�O���[�h�̃I��/�I�t�؂�ւ�
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

// �f�o�b�O���̕\��
void Game::DisplayDebugInfo()
{
    //box
    // �`�惂�[�h��ݒ�
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(5, 5, 450, 600, GetColor(0, 0, 0), TRUE);
    // �u�����h���[�h�����ɖ߂�
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    // ���W����\��
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
    // S�L�[�����Ȃ���㉺���ő傫���ύX
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
        return; // �X�P�[���ύX���D�悳�ꂽ�ꍇ�A���̏��������s���Ȃ�
    }

    // R�L�[�����Ȃ��獶�E����Y����]�ύX
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
        return; // ��]�ύX���D�悳�ꂽ�ꍇ�A���̏��������s���Ȃ�
    }

    // ���V�t�g�L�[�����Ȃ���㉺����Y���W�ύX
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
        return; // Y���̍��W�ύX���D�悳�ꂽ�ꍇ�A���̏��������s���Ȃ�
    }

    // ���W�ύX�i�ʏ�̈ړ��j
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

// FPS�̕\��
void Game::DisplayFPS()
{
    frameCount++;
    if (frameCount % 60 == 0) {
        currentTime = GetNowCount();
        fps = 60000 / (currentTime - startTime);  // FPS���v�Z
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
        if (hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(),player.setScore(),addScore[i],player.getIsHitting()))
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

void Game::DrawScoreEffect(VECTOR playerPos)
{

    if (!isDrawingScoreEffect)
    {
        VECTOR EffectPos = playerPos;



        DrawBillboard3D(playerPos, 0.5f, 0.5f, 1.0f, 0.0f, Score_Effect, TRUE);
    }
}