#include "common.h"

//連射防止用関数
int isActive(int type)
{
    int i;

    //今アクティブなボールの内, 特定のtypeの物があるか
    for (i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active && balls[i].type == type) {
            return 1;
        }
    }

    return 0;
}

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

        //特殊ボール
        case 'f':
        case 'F':
            if ((gameFlags & MASK_GAME) && !(gameFlags & MASK_WAIT) && !(gameFlags & (MASK_BALL_WIN | MASK_PIN_WIN)) 
                && (snakeAmmo > 0) && !isActive(BALL_SNAKE)) {
                if (spawnSpecialBall(BALL_SNAKE)) {
                    snakeAmmo--;
                }
            }
            break;

        case 'g':
        case 'G':
            if ((gameFlags & MASK_GAME) && !(gameFlags & MASK_WAIT) && !(gameFlags & (MASK_BALL_WIN | MASK_PIN_WIN)) 
                 && (zigzagAmmo > 0) && !isActive(BALL_ZIGZAG)) {

                if (spawnSpecialBall(BALL_ZIGZAG)) {
                    zigzagAmmo--;
                }
            }
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

        //以下はスタート画面関連
        case '1':
            if (!(gameFlags & MASK_GAME) &&
                !(gameFlags & MASK_WAIT)) {
                gameFlags &= ~MASK_CHAR_PIKA;
                gameFlags |= MASK_CHAR_PIN;
            }
            break;

        case '2':
            if (!(gameFlags & MASK_GAME) &&
                !(gameFlags & MASK_WAIT)) {
                gameFlags &= ~MASK_CHAR_PIN;
                gameFlags |= MASK_CHAR_PIKA;
            }
            break;

        case 13:
            if (!(gameFlags & MASK_GAME) &&
                !(gameFlags & MASK_WAIT) &&
                (gameFlags & (MASK_CHAR_PIN | MASK_CHAR_PIKA))) {
                gameFlags |= MASK_GAME;
                startWait();
            }
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