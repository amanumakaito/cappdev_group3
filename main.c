#include "common.h"

int timer_interval = 50;
int elapsed_time = 0;

void timer(int value)
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

    glutPostRedisplay();

    glutTimerFunc(timer_interval, timer, 0);
}

void initGL(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);

// 背面カリングを有効化
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    glutSpecialFunc(onSpecialKeyDown);
    glutSpecialUpFunc(onSpecialKeyUp);

    remainingTime = GAME_TIME;

}

void onReshape(int w, int h)
{
    winW = w;
    winH = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

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