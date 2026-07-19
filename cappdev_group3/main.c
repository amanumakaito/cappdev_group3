#include "common.h"

// 更新間隔[ms]
int timer_interval = 50;
// ボール生成用経過時間[ms]
int elapsed_time = 0;
int winW = 640, winH = 480;
int remainingWaitTime = WAIT_TIME;  //残り待機時間のデフォは5秒
int wait_counter = 0;   //timer関数一周ごとに50ms(timer_interval)だけ増加していき，1s(WAIT_INTERVAL)になったら分岐発生

//待機に必要なパラメータを初期化
void startWait(void) {
  remainingWaitTime = WAIT_TIME;
  wait_counter = 0;
  gameFlags |= MASK_WAIT;
}

//時間変化する発射間隔を返す
int getSpawnInterval(void)
{
    if (remainingTime > 20.0) {
        return 1000;
    }
    else if (remainingTime > 10.0) {
        return 750;
    }
    else {
        return 500;
    }
}

void timer(int value)
{   
    //MASK_WAITが立っている間はカウントダウンだけ進行
    if (gameFlags & MASK_WAIT) {
        wait_counter += timer_interval;//timer関数一周ごとに50ms(timer_interval)だけ増加

        if (wait_counter >= WAIT_INTERVAL) {
            remainingWaitTime -= 1;//1s(WAIT_INTERVAL)になったらカウント1ダウン
            wait_counter = 0;
        }

        if (remainingWaitTime <= 0) {
            remainingWaitTime = 0;
            gameFlags &= ~MASK_WAIT;   //残り待機時間が0になったらMASK_WAITから脱する
        }
    }

    //MASK_WAITが消えたら通常の進行に移る
    else if (gameFlags & MASK_GAME) {
        // 勝敗が決まっていない間だけゲームを実行
        if(!(gameFlags & (MASK_BALL_WIN | MASK_PIN_WIN)))
        {
            updateGame();

    //ボール生成タイマー
            elapsed_time += timer_interval;

            if (elapsed_time >= getSpawnInterval())
            {
                spawnBall();
                elapsed_time = 0;
            }

    //残り時間更新
            static int timer_counter = 0;

            timer_counter += timer_interval;

            if (timer_counter >= 1000)
            {
                if (remainingTime > 0.0)
                {
                    remainingTime -= 1.0;
                }

                timer_counter = 0;
            }
    }

    }    glutPostRedisplay();

    glutTimerFunc(timer_interval, timer, 0);

}

void initGL(void)
{
// 背景色指定 （黒）
    glClearColor(0.0, 0.0, 0.0, 1.0);

// Zバッファを有効化
    glEnable(GL_DEPTH_TEST);

// 背面カリングを有効化
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

// 光源設定
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

//glcolorによる色指定
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

// キーボード入力設定
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    glutSpecialFunc(onSpecialKeyDown);
    glutSpecialUpFunc(onSpecialKeyUp);
}

void onReshape(int w, int h)
{
    winW = w;
    winH = h;

// ウィンドウ全体を描画領域とする
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

// カメラ設定
    gluPerspective(
        30.0,
        (double)w / h,
        0.1,
        100.0);

    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_RGBA |
        GLUT_DOUBLE |
        GLUT_DEPTH);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bowling Game");

    glutDisplayFunc(display);
    glutReshapeFunc(onReshape);

    initGL();
    initGame();
    glutTimerFunc(timer_interval, timer, 0);
    glutMainLoop();

    return 0;
}