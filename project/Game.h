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
    TITLE,      // タイトル画面
    PLAYING,    // ゲーム中
    RESULT      // リザルト画面
};

class Game
{
public:
    Game();
    void Update();
    void Draw();

private:
    GameState currentState;
    bool Title_isStart;     // スタート選択されているか
    int Title_Start;        // スタート選択画像
    int Title_Settings;     // 設定選択画像
    int Score_UI;           // スコアのUI
    int Score_Effect;       // "+1"
    float alpha = 1.0f;	//画像透明度
    float img_Y = 0.0f;	//画像Y座標
    void DrawScoreEffect(int i); // "+1"表示
    VECTOR effectPos[maxGroundNum];
    int FontHandle;
    bool isDrawingScoreEffect;  //"+1"表示中か


    bool isInit;
    bool isDebugMode;    // デバッグモードのフラグ
    bool inputF1;        // F1キーが押されたか
    bool inputF2;        // F2キーが押されたか
    bool inputF3;        // F3が押されたか
    bool enemyStop;      //　敵停止中
    bool inputA;         //Aキーが押されたか
    bool hitStop;        // ヒットストップ
    bool addScore[maxGroundNum];       //スコアが加算されたか
    int ObjCode;         // デバッグ中操作する物の番号
    int enemyNum;        // 敵番号
    int frameCount;      // フレームカウント
    int startTime;       // FPS計算用の開始時間
    int currentTime;     // FPS計算用の時間
    int fps;             // FPS
    int padState;           //コントおーらーの状態



    BackGround background;
    Camera camera;
    Fishing ply1;
    Player player;
    Enemy* enemy[maxGroundNum];
    Enemy* TitleEnemy;
    Timer* enemyAwakenTime;
    Timer* hitStopTimer;
    HitChecker hitChecker;

    void UpdateTitle();  // タイトル画面の更新処理
    void UpdatePlaying(); // ゲーム中の更新処理
    void UpdateResult();  // リザルト画面の更新処理

    void DrawTitle();   // タイトル画面の描画処理
    void DrawPlaying();  // ゲーム中の描画処理
    void DrawResult();   // リザルト画面の描画処理

    void enemyAwaken();

    void ToggleDebugMode();  // デバッグモードのオン/オフ切り替え
    void DisplayDebugInfo(); // デバッグ情報の表示
    void DebugMove(VECTOR& pos, VECTOR& rotate, VECTOR& scale);    //　デバッグでの移動
    void SelectDebugObject();// デバッグで移動するものを選択
    void DisplayFPS();       // FPS表示
    void HitStopManager();
};
