// 重複宣言を防ぐ（1回目のみ読み込む）ための仕組み
#ifndef COMMON_H // COMMON_Hが定義されていなければ、以下を読み込む
#define COMMON_H // 空定数COMMON_Hの定義

// 0. 標準ヘッダファイルの読み込み
#include <stdio.h>
#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>

// 1. マクロによる定数の定義
#define PI 3.14159

#define MAX_BALLS 5
#define LANE_WIDTH 5.0
#define LANE_LENGTH 25.0

#define BALL_RADIUS 0.35
#define PIN_RADIUS 0.4
#define PIN_HEIGHT 1.5

#define BALL_SPEED_Y -0.20
#define PIN_SPEED 0.15
#define LAUNCHER_SPEED 0.15

#define GAME_TIME 30.0
#define SPAWN_INTERVAL 1000

// 2. グローバル変数のextern宣言
// （「他のソースファイルで定義されている」ことを宣言）
extern Bowl balls[MAX_BALLS];
extern Pin pin;

extern double launcherX; //ボール発射時の座標
extern double remainingTime; //残り時間

extern unsigned int keyFlags; //入力状態フラグ(3)を管理
extern unsigned int gameFlags; //ゲーム状態フラグ(4)を管理

extern int winW, winH;  //ウィンドウサイズ

// 3. 入力状態を表すフラグ
#define MASK_BALL_LEFT   (1 << 0)
#define MASK_BALL_RIGHT  (1 << 1)
#define MASK_PIN_LEFT    (1 << 2)
#define MASK_PIN_RIGHT   (1 << 3)

// 4. ゲーム状態を表すフラグ
#define MASK_BALL_WIN    (1 << 4)
#define MASK_PIN_WIN     (1 << 5)

// 5.構造体
typedef struct {
  double x, y, z;
  double vx, vy, vz;
  double r;
  int color;
  int active;
} Bowl;

typedef struct {
  double x, y, z;
  double h;
  double r;
  double vx;
} Pin;

// 6. 関数のプロトタイプ宣言
// logic.c
void initGame(void);
void resetGame(void);
void updateGame(void);
void spawnBall(void);
void updateBalls(void);
void updatePin(void);
int collision(void);
void judgeGame(void);

// main.c
void timer(int value);
void initGL(void);
void onReshape(int w, int h);

// input.c
void onKeyDown(unsigned char key, int mx, int my);
void onKeyUp(unsigned char key, int mx, int my);
void onSpecialKeyDown(int key, int mx, int my);
void onSpecialKeyUp(int key, int mx, int my);

// draw.c
void display(void);
void drawGame(void);
void drawLane(void);
void drawBalls(void);
void drawPin(void);
void drawUI(void); //時間表示，勝敗リザルト表示

#endif // #ifndef COMMON_H ここまで

