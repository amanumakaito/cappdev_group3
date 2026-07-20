#include "common.h"

Bowl balls[MAX_BALLS];  //ボール構造体定義
Pin pin;  //ピン構造体定義

double launcherX = 0.0; //発射位置の初期値
double remainingTime = GAME_TIME; //残り時間

unsigned int keyFlags = 0;  //状態管理
unsigned int gameFlags = 0; //ゲーム状態管理

//ラウンド制関連
int currentRound = 1;
int maxRound = 3;
int ballScore = 0;
int pinScore = 0;
int matchFinished = 0;

//特殊ボール関連
int snakeAmmo = SPECIAL_AMMO;
int zigzagAmmo = SPECIAL_AMMO;

void initGame(void) //ゲームリセット
{
  resetGame();
}

void resetGame(void)  //スタート画面に戻る
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    balls[i].active = 0;
    balls[i].x = 0.0;
    balls[i].y = 0.0;
    balls[i].z = BALL_RADIUS;
    balls[i].vx = 0.0;
    balls[i].vy = BALL_SPEED_Y;
    balls[i].vz = 0.0;
    balls[i].r = BALL_RADIUS;
    balls[i].color = i;
    balls[i].type = BALL_NORMAL;
  }

  launcherX = 0.0;

  pin.x = 0.0;
  pin.y = -10.0;
  pin.z = 0.0;
  pin.h = PIN_HEIGHT;
  pin.r = PIN_RADIUS;
  pin.vx = 0.0;

  remainingTime = GAME_TIME;

  keyFlags = 0;
  gameFlags = 0;  //MASK_GAMEを消す

  //ラウンド制関連
  currentRound = 1;
  ballScore = 0;
  pinScore = 0;
  matchFinished = 0;

  //残弾数関連
  snakeAmmo = SPECIAL_AMMO;
  zigzagAmmo = SPECIAL_AMMO;
}

void resetRound(void) //次のラウンドへ移行
{
    int i;

    for (i = 0; i < MAX_BALLS; i++) {
        balls[i].active = 0;
        balls[i].x = 0.0;
        balls[i].y = 0.0;
        balls[i].z = BALL_RADIUS;
        balls[i].vx = 0.0;
        balls[i].vy = BALL_SPEED_Y;
        balls[i].vz = 0.0;
        balls[i].type = BALL_NORMAL;
    }

    launcherX = 0.0;

    pin.x = 0.0;
    pin.y = -10.0;
    pin.z = 0.0;
    pin.vx = 0.0;

    remainingTime = GAME_TIME;
    keyFlags = 0;

    snakeAmmo = SPECIAL_AMMO;
    zigzagAmmo = SPECIAL_AMMO;

    gameFlags &= ~(MASK_BALL_WIN | MASK_PIN_WIN); //MASK_GAMEは消さない

    startWait();  //待機へ移行
}

void spawnBall(void)  //通常ボール発射
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    if (!balls[i].active) {//アクティブでない=未使用ボールを探す
      balls[i].active = 1;  //アクティブにする

      //発射位置にセット
      balls[i].x = launcherX;
      balls[i].y = 10.0;
      balls[i].z = BALL_RADIUS;
      balls[i].vx = 0.0;
      balls[i].vy = BALL_SPEED_Y;
      balls[i].vz = 0.0;
      balls[i].type = BALL_NORMAL;  //通常ボール
      return;
    }
  }
}

//特殊ボール発射
int spawnSpecialBall(int type)
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    if (!balls[i].active) {
      balls[i].active = 1;

      balls[i].x = launcherX;
      balls[i].y = 10.0;
      balls[i].z = BALL_RADIUS;
      balls[i].vx = 0.0;
      balls[i].vy = BALL_SPEED_Y;
      balls[i].vz = 0.0;

      balls[i].type = type;

      return 1;
    }
  }

  return 0;
}

void updateGame(void) //状態更新
{
  if (gameFlags & MASK_BALL_WIN) return;
  if (gameFlags & MASK_PIN_WIN) return;

  //発射位置の状態更新
  if (keyFlags & MASK_BALL_LEFT) {
    launcherX -= LAUNCHER_SPEED;
  }

  if (keyFlags & MASK_BALL_RIGHT) {
    launcherX += LAUNCHER_SPEED;
  }

    //ピンの行動範囲制限
  if (launcherX < -LANE_WIDTH / 2.0 + BALL_RADIUS) {
    launcherX = -LANE_WIDTH / 2.0 + BALL_RADIUS;
  }

  //ピンの行動範囲制限
  if (launcherX > LANE_WIDTH / 2.0 - BALL_RADIUS) {
    launcherX = LANE_WIDTH / 2.0 - BALL_RADIUS;
  }

  //ピンとボールの状態更新
  updateBalls();
  updatePin();

  //ピンに衝突
  if (collision()) {
    gameFlags |= MASK_BALL_WIN;
  }

  //時間切れ
  judgeGame();
}

void updateBalls(void)  //ボールの挙動
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    if (!balls[i].active) continue;

    // //アクティブだけボールが動く
    // balls[i].x += balls[i].vx;
    // balls[i].y += balls[i].vy;

    switch (balls[i].type) {

      case BALL_SNAKE:
        // スネーク処理
        balls[i].x += sin(balls[i].y*0.3) * 0.3;
        balls[i].y += balls[i].vy;
        break;

      case BALL_ZIGZAG:
        // 進んだ距離に応じて左右方向を切り替える
        if (((int)((10.0 - balls[i].y) / 1.6)) % 2 == 0) {
          balls[i].x += 0.3;
        }
        else {
          balls[i].x -= 0.3;
        }

        balls[i].y += balls[i].vy;
        break;

      case BALL_NORMAL:
      default:
        balls[i].x += balls[i].vx;
        balls[i].y += balls[i].vy;
        break;
    }

    //レーン端に到達したらアクティブでなくなる
    if (balls[i].y < -12.0) {
      balls[i].active = 0;
    }
  }
}

void updatePin(void)  //ピンの挙動の詳細
{
  pin.vx = 0.0;

  if (keyFlags & MASK_PIN_LEFT) {
    pin.vx -= PIN_SPEED;
  }

  if (keyFlags & MASK_PIN_RIGHT) {
    pin.vx += PIN_SPEED;
  }

  pin.x += pin.vx;

  //ピンの行動範囲制限
  if (pin.x < -LANE_WIDTH / 2.0 + pin.r) {
    pin.x = -LANE_WIDTH / 2.0 + pin.r;
  }

  //ピンの行動範囲制限
  if (pin.x > LANE_WIDTH / 2.0 - pin.r) {
    pin.x = LANE_WIDTH / 2.0 - pin.r;
  }
}

int collision(void) //衝突
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    if (!balls[i].active) continue;

    //中心間距離
    double dx = balls[i].x - pin.x;
    double dy = balls[i].y - pin.y;
    double rr = balls[i].r + pin.r;

    if (dx * dx + dy * dy < rr * rr) {
      return 1;
    }
  }

  return 0;
}

void judgeGame(void)  //時間切れ
{
  if (remainingTime <= 0.0) {
    gameFlags |= MASK_PIN_WIN;
  }
}