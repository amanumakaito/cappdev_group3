#include "common.h"

// 更新間隔[ms]
int timer_interval = 50;
// ボール生成用経過時間[ms]
int elapsed_time = 0;
int winW = 640, winH = 480;

void timer(int value)
{
// 勝敗が決まっていない間だけゲームを実行
    if(!(gameFlags & (MASK_BALL_WIN | MASK_PIN_WIN)))
    {
        updateGame();

//ボール生成タイマー
        elapsed_time += timer_interval;

        if (elapsed_time >= SPAWN_INTERVAL)
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
    glutPostRedisplay();

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

// キーボード入力設定
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    glutSpecialFunc(onSpecialKeyDown);
    glutSpecialUpFunc(onSpecialKeyUp);
}

void onReshape(int w, int h)
{
    w = winW;
    h = winH;

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