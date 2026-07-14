#include "common.h"

enum COLOR { WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, GRAY, BLACK, BROWN };

float color[][4] = {
    { 1.0, 1.0, 1.0, 1.0 },
    { 1.0, 0.0, 0.0, 1.0 },
    { 0.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 1.0 },
    { 1.0, 1.0, 0.0, 1.0 },
    { 1.0, 0.0, 1.0, 1.0 },
    { 0.0, 1.0, 1.0, 1.0 },
    { 0.7, 0.7, 0.7, 1.0 },
    { 0.0, 0.0, 0.0, 1.0 },
    { 0.6, 0.3, 0.1, 1.0 }
};

float pos1[] = { 0.0, 0.0, 5.0, 1.0 };

void drawString(float x, float y, char *s)
{
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 320, 0, 240);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);

    while (*s) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *s++);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
}

//MASK_GAME立っていないときだけスタート画面表示
void drawStart(void){
    char buf[64];
    glColor3f(1.0, 1.0, 1.0);
    drawString(150, 120, "bowling game");
    drawString(150, 90, "key 1: PIN  key2: PIKACHU");
    if(gameFlags & MASK_CHAR_PIN){
        drawString(150, 60, "PIN");
    }
    else if(gameFlags & MASK_CHAR_PIKA){
        drawString(150, 30, "PIKACHU");
    }
}

//MASK_GAMEかつMASK_WAITの時だけ待機時間表示
void drawWait(void){
    char buf[64];
    glColor3f(1.0, 1.0, 1.0);
    sprintf(buf, "Wait for %d s", remainingWaitTime);
    drawString(200, 220, buf);
}

void drawLane(void)
{
    int ix, iy;
    int xg = (int)LANE_WIDTH;
    int yg = (int)LANE_LENGTH;

    glNormal3d(0, 0, 1);
    glBegin(GL_QUADS);

    for (iy = 0; iy < yg; iy++) {
        for (ix = 0; ix < xg; ix++) {
            double x0 = ix - LANE_WIDTH / 2.0;
            double y0 = iy - LANE_LENGTH / 2.0;

            glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BROWN]);
            glVertex3d(x0,       y0,       0);
            glVertex3d(x0 + 1.0, y0,       0);
            glVertex3d(x0 + 1.0, y0 + 1.0, 0);
            glVertex3d(x0,       y0 + 1.0, 0);
        }
    }

    glEnd();
}

void drawBalls(void)
{
    int i;

    for (i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            glPushMatrix();

            glMaterialfv(GL_FRONT, GL_DIFFUSE, color[balls[i].color]);

            glTranslatef(balls[i].x, balls[i].y, balls[i].z);
            glutSolidSphere(balls[i].r, 20, 20);

            glPopMatrix();
        }
    }
}

void drawPin(void)
{
    glPushMatrix();
    glTranslatef(pin.x, pin.y, pin.z + 0.8);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BLACK]);

    glutSolidSphere(0.25, 20, 20);

    glTranslatef(0, 0, -0.3);
    glutSolidCone(0.25, 0.4, 20, 20);

    glTranslatef(0, 0, -0.4);
    glutSolidCone(0.35, 0.8, 20, 20);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BLACK]);
    glPushMatrix();
    glTranslatef(0, 0, 0.1);
    glutSolidTorus(0.02, 0.26, 10, 30);
    glTranslatef(0, 0, -0.05);
    glutSolidTorus(0.02, 0.26, 10, 30);
    glPopMatrix();

    glPopMatrix();
}

//ここにキャラクター描画部分が入る
void drawPikachu(void){
    glPushMatrix();
    glTranslatef(pin.x, 1.2*pin.y, pin.z + 0.8);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[WHITE]);
    glutSolidCone(0.35, 0.8, 20, 20);
    glPopMatrix();
}

void drawUI(void)
{
    char buf[64];

    glColor3f(1.0, 1.0, 1.0);

    sprintf(buf, "Time:%.0f", remainingTime);
    drawString(10, 220, buf);

    if (gameFlags & MASK_BALL_WIN) {
        drawString(100, 120, "BALL WIN!");
    }

    if (gameFlags & MASK_PIN_WIN) {
        drawString(100, 120, "PIN WIN!");
    }
}

//MASK_GAMEが立っているときだけ描画
void drawGame(void)
{
    drawLane();
    drawBalls();
    if (gameFlags & MASK_CHAR_PIN) {
        drawPin();
    }

    if (gameFlags & MASK_CHAR_PIKA) {
        drawPikachu();
    }

    if (gameFlags & MASK_WAIT) {
        drawWait();
    } 
    drawUI();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, -20.0, 4,
              0.0, -10.0, 1,
              0.0, 0.0, 1.0);

    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    
    //MASK_GAMEが立っているならゲーム画面，いないならスタート画面
    if (gameFlags & MASK_GAME) {
        drawGame();
    } 
    else {
        drawStart();
    }

    glutSwapBuffers();
}