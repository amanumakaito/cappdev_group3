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

// カメラ側の斜め上に置く光源
float pos1[] = { 0.0, -15.0, 10.0, 1.0 };
float lightAmbient[]  = { 0.35, 0.35, 0.35, 1.0 };
float lightDiffuse[]  = { 0.90, 0.90, 0.90, 1.0 };
float lightSpecular[] = { 0.60, 0.60, 0.60, 1.0 };

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
            double rotation;

            glPushMatrix();

            glMaterialfv(GL_FRONT, GL_DIFFUSE, color[balls[i].color]);

            glTranslatef(balls[i].x, balls[i].y, balls[i].z);

            // 発射位置(y=10)からの移動距離を回転角に変換する
            // 移動距離 = 半径 × 回転角[rad]
            rotation = ((balls[i].y - 10.0) / balls[i].r) * 180.0 / PI;
            glRotated(rotation, 1.0, 0.0, 0.0);
            glutSolidSphere(balls[i].r, 20, 20);

            // 黒い小球をボール表面へ少し埋め込み、3つの指孔を表現する
            glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BLACK]);

            glPushMatrix();
            glTranslatef(-0.12, -0.46, 0.12);
            glutSolidSphere(0.075, 16, 16);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(0.12, -0.46, 0.12);
            glutSolidSphere(0.075, 16, 16);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(0.0, -0.46, -0.10);
            glutSolidSphere(0.090, 16, 16);
            glPopMatrix();

            glPopMatrix();
        }
    }
}

void drawPin(void)
{
    GLUquadric *quadric = gluNewQuadric();

    if (quadric == NULL) {
        return;
    }

    gluQuadricNormals(quadric, GLU_SMOOTH);

    glPushMatrix();
    glTranslatef(pin.x, pin.y, pin.z);

    // 半径の異なる円筒をつなぎ、細長いボウリングピンの輪郭を作る
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[WHITE]);

    // 底から胴体の最も太い部分まで
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.02);
    gluCylinder(quadric, 0.24, 0.34, 0.18, 36, 4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.20);
    gluCylinder(quadric, 0.34, 0.38, 0.30, 36, 4);
    glPopMatrix();

    // 太い胴体から肩へ向かって細くする
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.50);
    gluCylinder(quadric, 0.38, 0.24, 0.36, 36, 6);
    glPopMatrix();

    // 肩から細い首へつなぐ
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.86);
    gluCylinder(quadric, 0.24, 0.14, 0.22, 36, 5);
    glPopMatrix();

    // 頭の下の細い首
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.08);
    gluCylinder(quadric, 0.14, 0.12, 0.20, 36, 4);
    glPopMatrix();

    // 丸い頭
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.35);
    glScalef(0.19, 0.19, 0.20);
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();

    // 赤い2本線も短い円筒で描き、正面・側面の両方から見えるようにする
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color[RED]);

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.96);
    gluCylinder(quadric, 0.203, 0.174, 0.065, 36, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.11);
    gluCylinder(quadric, 0.145, 0.139, 0.065, 36, 2);
    glPopMatrix();

    glPopMatrix();

    gluDeleteQuadric(quadric);
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
    // 真っ黒よりも形を判別しやすい濃い青の背景
    glClearColor(0.03, 0.05, 0.10, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, -20.0, 4,
              0.0, -10.0, 1,
              0.0, 0.0, 1.0);

    // 手前側から照らし、影になる部分も環境光で見えるようにする
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    //MASK_GAMEが立っているならゲーム画面，いないならスタート画面
    if (gameFlags & MASK_GAME) {
        drawGame();
    } 
    else {
        drawStart();
    }

    glutSwapBuffers();
}
