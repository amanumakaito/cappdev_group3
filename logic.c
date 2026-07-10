#include "common.h"

Bowl balls[MAX_BALLS];
Pin pin;

double launcherX = 0.0;
double remainingTime = GAME_TIME;

unsigned int keyFlags = 0;
unsigned int gameFlags = 0;

void initGame(void)
{
  resetGame();
}

void resetGame(void)
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
  gameFlags = 0;
}

void spawnBall(void)
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
      return;
    }
  }
}

void updateGame(void)
{
  if (gameFlags & MASK_BALL_WIN) return;
  if (gameFlags & MASK_PIN_WIN) return;

  if (keyFlags & MASK_BALL_LEFT) {
    launcherX -= LAUNCHER_SPEED;
  }

  if (keyFlags & MASK_BALL_RIGHT) {
    launcherX += LAUNCHER_SPEED;
  }

  updateBalls();
  updatePin();

  if (collision()) {
    gameFlags |= MASK_BALL_WIN;
  }

  judgeGame();
}

void updateBalls(void)
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    if (!balls[i].active) continue;

    balls[i].x += balls[i].vx;
    balls[i].y += balls[i].vy;

    if (balls[i].y < -12.0) {
      balls[i].active = 0;
    }
  }
}

void updatePin(void)
{
  pin.vx = 0.0;

  if (keyFlags & MASK_PIN_LEFT) {
    pin.vx -= PIN_SPEED;
  }

  if (keyFlags & MASK_PIN_RIGHT) {
    pin.vx += PIN_SPEED;
  }

  pin.x += pin.vx;

  if (pin.x < -LANE_WIDTH / 2.0 + pin.r) {
    pin.x = -LANE_WIDTH / 2.0 + pin.r;
  }

  if (pin.x > LANE_WIDTH / 2.0 - pin.r) {
    pin.x = LANE_WIDTH / 2.0 - pin.r;
  }
}

int collision(void)
{
  int i;

  for (i = 0; i < MAX_BALLS; i++) {
    if (!balls[i].active) continue;

    double dx = balls[i].x - pin.x;
    double dy = balls[i].y - pin.y;
    double rr = balls[i].r + pin.r;

    if (dx * dx + dy * dy < rr * rr) {
      return 1;
    }
  }

  return 0;
}

void judgeGame(void)
{
  if (remainingTime <= 0.0) {
    gameFlags |= MASK_PIN_WIN;
  }
}