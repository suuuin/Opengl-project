#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>          
#include <stdio.h>    
#include <math.h>            
#include <glaux.h>
#include <gl/glut.h>                     
#include <gl/glu.h>

#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;
//Release 모드시 뜨는 오류 잡기 위함
#pragma comment(lib, "advapi32.lib")

#define MAX_PARTICLES   1500                  
float slowdown = 2.0f, xspeed, yspeed, zoom = -20.0f;
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;

static POINT ptLastMousePosit;
static POINT ptCurrentMousePosit;
static bool bMousing;

bool upflag, downflag, rightflag, leftflag, reset;


typedef struct {
    bool    active;
    float   life;
    float   fade;
    vec3 color;
    vec3 pos;
    vec3 vel;
    vec3 force;
}
particles;

particles particle[MAX_PARTICLES];

GLuint      texture[1];
bool		keys[256];

AUX_RGBImageRec* LoadBMP(CHAR* Filename) {
    FILE* File = NULL;
    if (!Filename) {
        return NULL;
    }
    File = fopen(Filename, "r");
    if (File) {
        fclose(File);
        return auxDIBImageLoadA(Filename);
    }
    return NULL;
}

void LoadGLTextures() {
    AUX_RGBImageRec* TextureImage[1];
    memset(TextureImage, 0, sizeof(void*) * 1);

    if (TextureImage[0] = LoadBMP("Data/Fire.bmp")) {
        glGenTextures(1, &texture[0]);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (TextureImage[0]) {
        if (TextureImage[0]->data) {
            free(TextureImage[0]->data);
        }

        free(TextureImage[0]);
    }
}


void SetParticle(int loop)
{
    particle[loop].active = true;
    //particle[loop].life=1.0f;
    particle[loop].life = 0.7f;
    particle[loop].fade = float(rand() % 80) / 1000.0f + 0.003f;
    particle[loop].color = vec3(1.0f, 0.3f, 0.2f);
    particle[loop].vel = vec3((rand() % 50) - 25.0f, (rand() % 50) - 25.0f, (rand() % 50) - 25.0f);
    particle[loop].force = vec3(0, 0.6, 0);

    if (loop % 2 == 0) {
        particle[loop].pos = vec3(0.0f, -1.0f, 0.0f);
    }
    if (loop % 2 == 1) {
        particle[loop].pos = vec3(0.0f, -3.0f, 0.0f);
    }

}
int InitGL(GLvoid) {
    upflag = false;
    downflag = false;
    leftflag = false;
    rightflag = false;
    reset = false;

    LoadGLTextures();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDisable(GL_DEPTH_TEST);

    for (int loop = 0;loop < MAX_PARTICLES;loop++) {
        SetParticle(loop);
    }

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    return TRUE;
}

void DrawGLScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-g_fSpinY, 1.0f, 0.0f, 0.0f);
    glRotatef(-g_fSpinX, 0.0f, 1.0f, 0.0f);

    for (int loop = 0;loop < MAX_PARTICLES;loop++) {
        if (particle[loop].active) {
            float x = particle[loop].pos[0];
            float y = particle[loop].pos[1];
            float z = particle[loop].pos[2] + zoom;

            //텍스쳐 이용해 particle 그리기
            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glColor4f(particle[loop].color[0], particle[loop].color[1], particle[loop].color[2], particle[loop].life);


            glBegin(GL_QUADS);
            glTexCoord2d(0, 0); glVertex3f(x - 0.15f, y - 1.15f, z);
            glTexCoord2d(1, 0); glVertex3f(x + 0.15f, y - 1.15f, z);
            glTexCoord2d(1, 1); glVertex3f(x + 0.15f, y + 0.15f, z);
            glTexCoord2d(0, 1); glVertex3f(x - 0.15f, y + 0.15f, z);
            glEnd();

            //속도와 가속도를 이용해 particle 위치 업데이트
            //life값 업데이트
            particle[loop].pos += particle[loop].vel / (slowdown * 1000);
            particle[loop].vel += particle[loop].force;
            particle[loop].life -= particle[loop].fade;

            //불꽃 생명이 다한 경우, 다시 불 시작 위치로 이동 및 생명 초기화
            if (particle[loop].life < 0.0f) {
                SetParticle(loop);
            }

        }
    }

    glutPostRedisplay();
    glutSwapBuffers();
}


void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Particle - Fire");
    InitGL();
    glutDisplayFunc(DrawGLScene);
    glutReshapeFunc(MyReshape);
    glutMainLoop();
}