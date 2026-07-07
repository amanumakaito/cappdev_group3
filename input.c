#include "common.h"

// ビットフィールドによるフラグ管理
unsigned int flags = 0;
#define MASK_UP    (1 << 0)
#define MASK_DOWN  (1 << 1)
#define MASK_LEFT  (1 << 2)
#define MASK_RIGHT (1 << 3)
#define MASK_JUMP  (1 << 4)

void onKeyDown(unsigned char key, int mx, int my)
{
  printf("KeyDown: ");
  // 修飾キーの検知
  if (glutGetModifiers() & GLUT_ACTIVE_ALT) printf("ALT ");
  if (glutGetModifiers() & GLUT_ACTIVE_CTRL) printf("CTRL ");
  if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) printf("SHIFT ");
  printf("%u\n",key); // デバッグ用にキーコードを出力
  fflush(stdout); // 標準出力をflush
  switch (key)
  {
    case 'q':
    case '\e': // ESCキー
      exit(0);
      break;
    case ' ':
      if (!(flags & MASK_JUMP)) {
        flags |= MASK_JUMP;
        v = 0.2; // 鉛直方向の初速度
      }
      break;
    case 'r':
      resetGame();
  }
}

void onKeyUp(unsigned char key, int mx, int my)
{
  printf("KeyUp: ");
  printf("%u\n",key); // デバッグ用にキーコードを出力
  fflush(stdout); // 標準出力をflush
}

void onSpecialKeyDown(int key, int mx, int my)
{
  printf("SpecialKeyDown: ");
  // 修飾キーの検知
  if (glutGetModifiers() & GLUT_ACTIVE_ALT) printf("ALT ");
  if (glutGetModifiers() & GLUT_ACTIVE_CTRL) printf("CTRL ");
  if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) printf("SHIFT ");
  printf("%d\n",key); // デバッグ用にコードを出力
  fflush(stdout); // 標準出力をflush
  switch (key)
  {
    case GLUT_KEY_UP:
      flags |= MASK_UP;
      break;
    case GLUT_KEY_DOWN:
      flags |= MASK_DOWN;
      break;
    case GLUT_KEY_LEFT:
      flags |= MASK_LEFT;
      break;
    case GLUT_KEY_RIGHT:
      flags |= MASK_RIGHT;
      break;
  }
}

void onSpecialKeyUp(int key, int mx, int my)
{
  printf("SpecialKeyUp: ");
  printf("%d\n",key); // デバッグ用にコードを出力
  fflush(stdout); // 標準出力をflush
  
  switch (key)
  {
    case GLUT_KEY_UP:
      flags &= ~(MASK_UP);
      break;
    case GLUT_KEY_DOWN:
      flags &= ~(MASK_DOWN);
      break;
    case GLUT_KEY_LEFT:
      flags &= ~(MASK_LEFT);
      break;
    case GLUT_KEY_RIGHT:
      flags &= ~(MASK_RIGHT);
      break;
  }
}

void timerKeyboard(void)
{
  if (flags & MASK_UP) {
    y += 0.1;
    if (collision()) y -= 0.1;
  }
  if (flags & MASK_LEFT) {
    x -= 0.1;
    if (collision()) x += 0.1;
  }
  if (flags & MASK_RIGHT) {
    x += 0.1;
    if (collision()) x -= 0.1;
  }
  if (flags & MASK_DOWN) {
    y -= 0.1;
    if (collision()) y += 0.1;
  }

  if (flags & MASK_JUMP) {
    z += v;
    v -= 0.01; // 鉛直方向の加速度
    if (z <= 0) {
      flags &= ~(MASK_JUMP);
      z = 0;
      v = 0;
    }
  }
}
