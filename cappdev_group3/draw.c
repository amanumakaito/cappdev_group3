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
    // char buf[64];
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

    glColor3fv(color[BROWN]);
    glNormal3d(0, 0, 1);
    glBegin(GL_QUADS);

    for (iy = 0; iy < yg; iy++) {
        for (ix = 0; ix < xg; ix++) {
            double x0 = ix - LANE_WIDTH / 2.0;
            double y0 = iy - LANE_LENGTH / 2.0;

            // glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BROWN]);
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

            // glMaterialfv(GL_FRONT, GL_DIFFUSE, color[balls[i].color]);
            glColor3fv(color[balls[i].color]);

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
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BLACK]);
    glColor3fv(color[BLACK]);

    glutSolidSphere(0.25, 20, 20);

    glTranslatef(0, 0, -0.3);
    glutSolidCone(0.25, 0.4, 20, 20);

    glTranslatef(0, 0, -0.4);
    glutSolidCone(0.35, 0.8, 20, 20);

    // glMaterialfv(GL_FRONT, GL_DIFFUSE, color[BLACK]);
    glPushMatrix();
    glTranslatef(0, 0, 0.1);
    glutSolidTorus(0.02, 0.26, 10, 30);
    glTranslatef(0, 0, -0.05);
    glutSolidTorus(0.02, 0.26, 10, 30);
    glPopMatrix();

    glPopMatrix();
}

//キャラクター描画部分
void drawPikachu(void){
    glPushMatrix();

    // ピンと同じ位置に配置
    glTranslatef(pin.x, pin.y, pin.z + 0.55);

    // 元のモデルに軸をそろえる
    glRotatef(90.0, 1.0, 0.0, 0.0);

    // 通常ピンと同程度に縮小
    glScalef(0.40, 0.40, 0.40);

    // ===== 胴体 =====
    glPushMatrix();
    glColor3d(1.0, 0.9, 0.0);

    glTranslatef(0.0, -0.3, 0.0);
    glScalef(1.0, 1.3, 0.8);
    glutSolidSphere(0.8, 30, 30);
    glPopMatrix();

    // ===== 頭 =====
    glPushMatrix();
    glColor3d(1.0, 0.9, 0.0);
    glTranslatef(0.0, 0.9, 0.0);
    glutSolidSphere(0.7, 30, 30);
    glPopMatrix();

    // ===== 左目 =====
    glPushMatrix();

    glColor3f(0,0,0);
    glTranslatef(-0.22,1.05,0.62);
    glutSolidSphere(0.12,20,20);

    glColor3f(1,1,1);
    glTranslatef(0.04,0.04,0.08);
    glutSolidSphere(0.04,20,20);

    glPopMatrix();

    // ===== 右目 =====
    glPushMatrix();

    glColor3f(0,0,0);
    glTranslatef(0.22,1.05,0.62);
    glutSolidSphere(0.12,20,20);

    glColor3f(1,1,1);
    glTranslatef(-0.04,0.04,0.08);
    glutSolidSphere(0.04,20,20);

    glPopMatrix();

    // ===== 左頬 =====
    glPushMatrix();
    glColor3d(1.0, 0.0, 0.0);
    glTranslatef(-0.4, 0.8, 0.55);
    glutSolidSphere(0.12, 20, 20);
    glPopMatrix();

    // ===== 右頬 =====
    glPushMatrix();
    glColor3d(1.0, 0.0, 0.0);
    glTranslatef(0.4, 0.8, 0.55);
    glutSolidSphere(0.12, 20, 20);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    // ===== 口 =====
    glColor3f(0,0,0);

    glLineWidth(3);

    glBegin(GL_LINES);

    glVertex3f(0.0,0.75,0.66);
    glVertex3f(0.0,0.68,0.66);

    glEnd();

    glBegin(GL_LINE_STRIP);

    glVertex3f(-0.08,0.68,0.66);
    glVertex3f(0.0,0.62,0.66);
    glVertex3f(0.08,0.68,0.66);

    glEnd();

    glEnable(GL_LIGHTING);
    // 左手
    glPushMatrix();

    glColor3f(1.0,0.9,0.0);

    glTranslatef(-0.75,-0.05,0.25);
    glScalef(1.0,1.5,1.0);

    glutSolidSphere(0.18,20,20);

    glPopMatrix();
    
    // 右手
    glPushMatrix();

    glColor3f(1.0,0.9,0.0);

    glTranslatef(0.75,-0.05,0.25);
    glScalef(1.0,1.5,1.0);

    glutSolidSphere(0.18,20,20);

    glPopMatrix();
    //耳

    glColor3f(1.0,0.9,0.0);

    glBegin(GL_TRIANGLES);

    // 前
    glVertex3d(-0.2,1.2,0.1);
    glVertex3d(-0.8,2.3,0.1);
    glVertex3d( 0.0,1.7,0.1);

    // 後
    glVertex3d(-0.2,1.2,-0.1);
    glVertex3d( 0.0,1.7,-0.1);
    glVertex3d(-0.8,2.3,-0.1);

    glEnd();

    glBegin(GL_QUADS);

    glVertex3d(-0.2,1.2,0.1);
    glVertex3d(-0.8,2.3,0.1);
    glVertex3d(-0.8,2.3,-0.1);
    glVertex3d(-0.2,1.2,-0.1);

    glVertex3d(-0.8,2.3,0.1);
    glVertex3d(0.0,1.7,0.1);
    glVertex3d(0.0,1.7,-0.1);
    glVertex3d(-0.8,2.3,-0.1);

    glVertex3d(0.0,1.7,0.1);
    glVertex3d(-0.2,1.2,0.1);
    glVertex3d(-0.2,1.2,-0.1);
    glVertex3d(0.0,1.7,-0.1);

    glEnd();

    // 黒先端
    glColor3f(0,0,0);

    glBegin(GL_QUADS);

    glVertex3d(-0.55,1.85,0.11);
    glVertex3d(-0.8,2.3,0.11);
    glVertex3d(-0.8,2.3,-0.11);
    glVertex3d(-0.55,1.85,-0.11);

    glEnd();

    glColor3f(1.0,0.9,0.0);

    glBegin(GL_TRIANGLES);

    // 前面
    glNormal3f(0,0,1);
    glVertex3d(0.2,1.2,0.1);
    glVertex3d(0.8,2.3,0.1);
    glVertex3d(0.0,1.7,0.1);

    // 背面
    glNormal3f(0,0,-1);
    glVertex3d(0.2,1.2,-0.1);
    glVertex3d(0.0,1.7,-0.1);
    glVertex3d(0.8,2.3,-0.1);

    glEnd();

    // 側面
    glBegin(GL_QUADS);

    glNormal3f(1,0,0);
    glVertex3d(0.2,1.2,0.1);
    glVertex3d(0.8,2.3,0.1);
    glVertex3d(0.8,2.3,-0.1);
    glVertex3d(0.2,1.2,-0.1);


    glNormal3f(0.7,0.7,0);
    glVertex3d(0.8,2.3,0.1);
    glVertex3d(0.0,1.7,0.1);
    glVertex3d(0.0,1.7,-0.1);
    glVertex3d(0.8,2.3,-0.1);

    glNormal3f(-0.7,0.7,0);
    glVertex3d(0.0,1.7,0.1);
    glVertex3d(0.2,1.2,0.1);
    glVertex3d(0.2,1.2,-0.1);
    glVertex3d(0.0,1.7,-0.1);

    glEnd();

    // 黒い耳先
    glColor3f(0.0,0.0,0.0);

    glBegin(GL_QUADS);

    glNormal3f(1,0,0);

    glVertex3d(0.55,1.85,0.11);
    glVertex3d(0.80,2.30,0.11);
    glVertex3d(0.80,2.30,-0.11);
    glVertex3d(0.55,1.85,-0.11);

    glEnd();
    // 尻尾 
    glColor3d(0.7, 0.4, 0.0);

    glBegin(GL_TRIANGLES);
        glVertex3d(0.8, 0.2, -0.2);
        glVertex3d(1.8, 1.0, -0.2);
        glVertex3d(1.3, 0.0, -0.2);

        glVertex3d(1.3, 0.0, -0.2);
        glVertex3d(1.8, 1.0, -0.2);
        glVertex3d(2.0, 0.3, -0.2);
    glEnd();

    // glColorによる色指定
    glEnable(GL_COLOR_MATERIAL);

    glPopMatrix();
}

void drawUI(void)
{
    char buf[64];

    glColor3f(1.0, 1.0, 1.0);

    sprintf(buf, "Time:%.0f", remainingTime);
    drawString(10, 220, buf);

    //ラウンド関連
    sprintf(buf, "Round:%d/%d", currentRound, maxRound);
    drawString(10, 180, buf);

    sprintf(buf, "Ball:%d  Pin:%d", ballScore, pinScore);
    drawString(10, 160, buf);

    //ゲーム勝敗表示
    if (matchFinished) {
        if (ballScore > pinScore) {
            drawString(60, 120, "BALL PLAYER WIN!!");
        }
        else {
            drawString(60, 120, "PIN PLAYER WIN!!");
        }
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