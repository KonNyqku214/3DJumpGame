#include "Game.h"

// �R���X�g���N�^
Game::Game()
    : currentState(TITLE), isDebugMode(false), inputF1(false) , frameCount(0), startTime(GetNowCount()),
    ply1(VGet(-24,0,0)),camera(player.getPlayerPos())// ������Ԃ̓^�C�g�����
{
    // �K�v�ȏ������������s��
    fps = 0;
    currentTime = 0;
    ObjCode = 0;
    isInit = false;
    for (int i = 0; i < 4; i++)
    {
        enemy[i] = new Enemy(background.getS_groundPos(i));
    }
    TitleEnemy = new Enemy(background.getTitle_groundPos());
    enemyAwakenTime = new Timer(7000);  //7000�~���b���ƂɓG���N����
    Title_START = LoadGraph("data/picture/Title_Start.png");
    Title_SETTINGS = LoadGraph("data/picture/Title_Settings.png");
    Title_IsStart = TRUE;
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
        currentState = PLAYING; // �X�y�[�X�L�[�������ꂽ��Q�[���J�n
        isInit = false;
    }
}

// �Q�[�����̍X�V����
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
            currentState = TITLE;       //�f�o�b�O���ɃG���^�[�Ń^�C�g��
        }
    }

    // �v���C���I�������烊�U���g��ʂɑJ�ڂ��鏈��
}

// ���U���g��ʂ̍X�V����
void Game::UpdateResult()
{
    if (CheckHitKey(KEY_INPUT_RETURN))
    {
        currentState = TITLE; // �G���^�[�L�[�������ꂽ��^�C�g����ʂɖ߂�
    }
}

// �^�C�g����ʂ̕`�揈��
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

    DrawStringToHandle(1300, 20, "�J�X�^���t�H���g�P�Q�R�S", GetColor(255, 255, 255), player.fontHandle_Score);
    DrawString(100, 100, "Press SPACE to Start", GetColor(255, 255, 255));
}

// �Q�[�����̕`�揈��
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