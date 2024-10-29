#pragma once
#include "DxLib.h"
#include "Fishing.h"
#include "BackGround.h"
#include "Camera.h"
#include"Player.h"
#include"Enemy.h"
#include"HitCheck.h"

enum GameState
{
    TITLE,      // �^�C�g�����
    PLAYING,    // �Q�[����
    RESULT      // ���U���g���
};

class Game
{
public:
    Game();
    void Update();
    void Draw();

private:
    GameState currentState;
    int titleHandle;     // �^�C�g���摜
    bool isDebugMode;    // �f�o�b�O���[�h�̃t���O
    bool inputF1;        // F1�L�[�������ꂽ��
    bool inputF2;        // F2�L�[�������ꂽ��
    bool inputF3;        // F3�������ꂽ��
    bool enemyStop;      //�@�G��~��
    bool inputA;         //A�L�[�������ꂽ��
    bool hitStop;        // �q�b�g�X�g�b�v
    int ObjCode;         // �f�o�b�O�����삷�镨�̔ԍ�
    int enemyNum;        // �G�ԍ�
    int frameCount;      // �t���[���J�E���g
    int startTime;       // FPS�v�Z�p�̊J�n����
    int currentTime;     // FPS�v�Z�p�̎���
    int fps;             // FPS
    int padState;           //�R���g���[��[�̏��
    int padInputY;			// ���X�e�B�b�N��Y��



    BackGround background;
    Camera camera;
    Fishing ply1;
    Player player;
    Enemy* enemy[maxGroundNum];
    Timer* enemyAwakenTime;
    Timer* hitStopTimer;
    HitChecker hitChecker;

    void UpdateTitle();  // �^�C�g����ʂ̍X�V����
    void UpdatePlaying(); // �Q�[�����̍X�V����
    void UpdateResult();  // ���U���g��ʂ̍X�V����

    void DrawTitle();   // �^�C�g����ʂ̕`�揈��
    void DrawPlaying();  // �Q�[�����̕`�揈��
    void DrawResult();   // ���U���g��ʂ̕`�揈��

    void enemyAwaken();

    void ToggleDebugMode();  // �f�o�b�O���[�h�̃I��/�I�t�؂�ւ�
    void DisplayDebugInfo(); // �f�o�b�O���̕\��
    void DebugMove(VECTOR& pos, VECTOR& rotate, VECTOR& scale);    //�@�f�o�b�O�ł̈ړ�
    void SelectDebugObject();// �f�o�b�O�ňړ�������̂�I��
    void DisplayFPS();       // FPS�\��
    void HitStopManager();
};
