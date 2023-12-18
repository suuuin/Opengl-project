﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <gl/glut.h>
#include <gl/glu.h>
#include "glaux.h"
#include <ctime>
#include <cmath>
#define TEXTURE_NUM 7
GLuint texture[TEXTURE_NUM];
char texture_name[TEXTURE_NUM][30] = {
    "Data/christmas.bmp",
    "Data/christmas2.bmp",
    "Data/blue.bmp",
    "Data/rudo.bmp",
    "Data/box1.bmp",
    "Data/santa7.bmp",
    "Data/dark2.bmp"
};

float g_fDistance = -4.5f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;
float boxYPosition = -5.0f;  // 초기 Y 위치 (양수로 설정)
float boxFallSpeed = -0.007f;  // 상자의 떨어지는 속도 (음수로 설정)

float boxSize[6] = { 0.5f, 0.8f, 0.8f, 1.0f, 0.4f, 0.8f };  // 각 사각형에 대한 크기 설정, 끝에서부터 결정됨
float boxPositions[6][3] = {
    {-5.5f, -1.8f, -1.0f},    // 첫 번째 상자 (높이 5.0)
    {-4.8f, -2.5f, -1.0f},    // 두 번째 상자 (높이 4.0)
    { -3.5f, -3.6f, -1.0f},    // 세 번째 상자 (높이 3.0)
    { -2.0f, -3.9f, -1.0f},    // 네 번째 상자 (높이 2.0)
    { -0.1f, -3.0f, -1.0f},    // 다섯 번째 상자 (높이 1.0)
    { 3.5f, -3.3f,  -1.0f}     // 여섯 번째 상자 (높이 0.0)
};





float stopHeight = 5.0f;    // 멈추는 높이
static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 흰색 배경

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // 현재 변환 매트릭스 초기화

    gluLookAt(1.0, -10.0, 15.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Draw background
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-20.0f, 10.2f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(20.0f, 10.2f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(20.0f, -15.0f, -8.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-20.0f, -15.0f, -8.0f);
    glEnd();




    // Update the elapsed time
    static clock_t startTime = clock();
    clock_t currentTime = clock();
    float elapsedTime = static_cast<float>(currentTime - startTime) / CLOCKS_PER_SEC;

    // Update the camera and viewpoint positions smoothly
    float smoothYPosition = boxYPosition + elapsedTime * boxFallSpeed;

    printf("boxYPosition: %f, stopHeight: %f\n", boxYPosition, stopHeight);
    // boxYPosition이 특정 높이 이하일 때만 상자를 이동시키도록 함
    if (boxYPosition < stopHeight) {
        boxYPosition -= boxFallSpeed;

        // 상자 그리기
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        for (int i = 0; i < 6; i++) {
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glPushMatrix();  // 현재 변환 매트릭스를 저장

            // 각 사각형의 위치로 이동
            glTranslatef(boxPositions[i][0], boxPositions[i][1], boxPositions[i][2]);

            glBegin(GL_QUADS);


            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.2f * boxSize[i], -1.2f * boxSize[i], 1.2f);		// �ո�
            glTexCoord2f(0.33f, 0.0f); glVertex3f(1.2f * boxSize[i], -1.2f * boxSize[i], 1.2f);
            glTexCoord2f(0.33f, 0.5f); glVertex3f(1.2f * boxSize[i], 1.2f * boxSize[i], 1.2f);
            glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.2f * boxSize[i], 1.2f * boxSize[i], 1.2f);

            glTexCoord2f(0.34f, 0.0f); glVertex3f(1.2f * boxSize[i], -1.2f * boxSize[i], -1.2f);
            glTexCoord2f(0.684f, 0.0f); glVertex3f(-1.2f * boxSize[i], -1.2f * boxSize[i], -1.2f);		// �޸�
            glTexCoord2f(0.684f, 0.5f); glVertex3f(-1.2f * boxSize[i], 1.2f * boxSize[i], -1.2f);
            glTexCoord2f(0.34f, 0.5f); glVertex3f(1.2f * boxSize[i], 1.2f * boxSize[i], -1.2f);

            glTexCoord2f(0.685f, 0.0f); glVertex3f(-1.2f * boxSize[i], 1.2f * boxSize[i], 1.2f);		// ����
            glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f * boxSize[i], 1.2f * boxSize[i], 1.2f);
            glTexCoord2f(1.0f, 0.51f); glVertex3f(1.2f * boxSize[i], 1.2f * boxSize[i], -1.2f);
            glTexCoord2f(0.685f, 0.51f); glVertex3f(-1.2f * boxSize[i], 1.2f * boxSize[i], -1.2f);

            glTexCoord2f(0.0f, 0.51f); glVertex3f(-1.2f * boxSize[i], -1.2f * boxSize[i], -1.2f);		// �Ʒ���
            glTexCoord2f(0.336f, 0.51f); glVertex3f(1.2f * boxSize[i], -1.2f * boxSize[i], -1.2f);
            glTexCoord2f(0.336f, 1.0f); glVertex3f(1.2f * boxSize[i], -1.2f * boxSize[i], 1.2f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.2f * boxSize[i], -1.2f * boxSize[i], 1.2f);

            glTexCoord2f(0.34f, 0.51f); glVertex3f(1.2f * boxSize[i], -1.2f * boxSize[i], 1.2f);		// ������
            glTexCoord2f(0.683f, 0.51f); glVertex3f(1.2f * boxSize[i], -1.2f * boxSize[i], -1.2f);
            glTexCoord2f(0.683f, 1.0f); glVertex3f(1.2f * boxSize[i], 1.2f * boxSize[i], -1.2f);
            glTexCoord2f(0.34f, 1.0f); glVertex3f(1.2f * boxSize[i], 1.2f * boxSize[i], 1.2f);

            glTexCoord2f(0.685f, 0.51f); glVertex3f(-1.2f * boxSize[i], -1.2f * boxSize[i], -1.2f);	// ������
            glTexCoord2f(1.0f, 0.51f); glVertex3f(-1.2f * boxSize[i], -1.2f * boxSize[i], 1.2f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.2f * boxSize[i], 1.2f * boxSize[i], 1.2f);
            glTexCoord2f(0.685f, 1.0f); glVertex3f(-1.2f * boxSize[i], 1.2f * boxSize[i], -1.2f);
            glEnd();
            glPopMatrix();
            glTranslatef(1.5f, 0.0f, 0.0f);

        }

        //glPopMatrix();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}



AUX_RGBImageRec* LoadBMP(char* Filename) {
    FILE* File = NULL;
    if (!Filename) return NULL;
    File = fopen(Filename, "r");
    if (File) {
        fclose(File);
        return auxDIBImageLoad(Filename);	     // ���Ϸκ��� �޸𸮷�
    }
    return NULL;
}
int LoadGLTextures() {
    int Status = TRUE, i;
    AUX_RGBImageRec* TextureImage;

    for (i = 0; i < TEXTURE_NUM; i++) {
        TextureImage = NULL;
        if (TextureImage = LoadBMP(texture_name[i])) {
            glGenTextures(1, &texture[i]);
            glBindTexture(GL_TEXTURE_2D, texture[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY,
                0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
        }
        else {
            MessageBox(NULL, "Image file has a error !", texture_name[i], MB_ICONERROR);
            Status = false;
        }

        if (TextureImage) {
            if (TextureImage->data) free(TextureImage->data);
            free(TextureImage);
        }
    }
    return Status;
}

void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;

    glutPostRedisplay();
}

int  main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Falling Box Game");
    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);
    LoadGLTextures();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutMainLoop();
}