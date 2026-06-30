#include "common.h"

int timer_interval = 50;
int ball_interval = 3000;

int remaining_time = 60;
int erapsed_time = 0;

void timer(int value){
    updateGame();
    glutPostRedisplay();
    glutTimerFunc(timer_interval, timer, 0);
}
