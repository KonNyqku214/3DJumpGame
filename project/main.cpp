//-----------------------------------------------------------------------------
// 10/18 製作開始
// ジャンプゲーム3D
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include"Game.h"

//-----------------------------------------------------------------------------
// @brief  メイン関数.
// 
// Testttttttttttttttttttttttttttttttttttttt
// 
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;	// エラーが起きたら直ちに終了
	}

	// 画面モードのセット
	SetGraphMode(1600, 900, 32);
	ChangeWindowMode(TRUE);


	Game game;

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		game.Update();

		// 画面を初期化する
		ClearDrawScreen();

		game.Draw();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}