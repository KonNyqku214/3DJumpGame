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
    bool isDebugMode;    // デバッグモードのフラグ
    bool inputF1;        // F1キーが押されたか
    bool inputF2;        // F2キーが押されたか
    bool isInit;        // 初期化されたか
    int ObjCode;         // デバッグ中操作する物の番号
    int frameCount;      // フレームカウント
    int startTime;       // FPS計算用の開始時間
    int currentTime;     // FPS計算用の時間
    int fps;             // FPS

    //画像ハンドル
    int Title_START;        //スタートボタン
    int Title_SETTINGS;     //設定ボタン

    bool Title_IsStart;       //タイトル画面ボタンフラグ

    BackGround background;
    Camera camera;
    Fishing ply1;
    Player player;
    Enemy* enemy[4];
    Enemy* TitleEnemy;
    Timer* enemyAwakenTime;
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
};
