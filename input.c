#include "common.h"


void onKeyDown(unsigned char key, int mx, int my)
{
    // デバッグ表示
    printf("KeyDown : %c (%d)\n", key, key);
    fflush(stdout);

    switch (key)
    {
        // ボール側プレイヤー
        case 'a':
        case 'A':
            keyFlags |= MASK_BALL_LEFT;
            break;

        case 'd':
        case 'D':
            keyFlags |= MASK_BALL_RIGHT;
            break;

        // ゲームリセット

        case 'r':
        case 'R':
            resetGame();
            break;


        // 終了

        case 'q':
        case 'Q':
        case 27: 
            exit(0);
            break;
    }
}


void onKeyUp(unsigned char key, int mx, int my)
{
    printf("KeyUp : %c (%d)\n", key, key);
    fflush(stdout);

    switch (key)
    {
        case 'a':
        case 'A':
            keyFlags &= ~MASK_BALL_LEFT;
            break;

        case 'd':
        case 'D':
            keyFlags &= ~MASK_BALL_RIGHT;
            break;
    }
}


void onSpecialKeyDown(int key, int mx, int my)
{
    printf("SpecialKeyDown : %d\n", key);
    fflush(stdout);

    switch (key)
    {

        // ピン側プレイヤー

        case GLUT_KEY_LEFT:
            keyFlags |= MASK_PIN_LEFT;
            break;

        case GLUT_KEY_RIGHT:
            keyFlags |= MASK_PIN_RIGHT;
            break;
    }
}


void onSpecialKeyUp(int key, int mx, int my)
{
    printf("SpecialKeyUp : %d\n", key);
    fflush(stdout);

    switch (key)
    {
        case GLUT_KEY_LEFT:
            keyFlags &= ~MASK_PIN_LEFT;
            break;

        case GLUT_KEY_RIGHT:
            keyFlags &= ~MASK_PIN_RIGHT;
            break;
    }
}