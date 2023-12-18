#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <glaux.h>

using namespace std;
using namespace glm;

vec2 preMouse, currentMouse;
int windowHeight, windowWidth;

#define PI 3.14159265358979323846

//텍스처
GLuint textureBackground;

AUX_RGBImageRec* LoadBMP(char* Filename) {
    FILE* File = NULL;
    if (!Filename) return NULL;
    File = fopen(Filename, "r");
    if (File) {
        fclose(File);
        return auxDIBImageLoad(Filename);
    }
    return NULL;
}

//define camera class
class camera {
public:
    vec3 eye; //center position
    vec3 at; //point where this camera is pointing out
    vec3 up; // up direction (normalized)

    vec3 forward; //vector pointing forward direction
    float distance = 1; //distance from eye to at
    vec3 right; //vector indicating right direction from the point of camera
    mat4 viewMat = mat4(1); //set initial view matrix as identity

    void InitCamera(vec3 _center, vec3 _at, vec3 _up)
    {
        //set three variables and update others
        eye = _center;
        at = _at;
        up = _up;
        UpdateCamera();
    }

    void MoveCamera(vec3 move)
    {
        eye += move;
        at += move;
    }

    void RotateCamera(vec3 axis, float angle)
    {
        //rotate direc and up vector
        vec4 tempForward = vec4(forward[0], forward[1], forward[2], 0);
        vec4 tempUp = vec4(up[0], up[1], up[2], 0);
        mat4 rot = rotate(mat4(1), radians(angle), axis);
        forward = rot * tempForward;
        up = rot * tempUp;

        //update at vector accordingly
        at = eye + forward * distance;

        UpdateCamera();
    }

    //update variables accordingly
    void UpdateCamera()
    {
        up = normalize(up);
        distance = length(at - eye);
        forward = normalize(at - eye);
        right = normalize(cross(forward, up));
    }

    void ComputeViewMatrix()
    {
        viewMat = mat4(1);
        for (int i = 0; i < 3; i++)
        {
            viewMat[0][i] = right[i];
            viewMat[1][i] = up[i];
            viewMat[2][i] = -forward[i];
            viewMat[i][3] = -eye[i];
        }
    }

};

camera myCamera;

//float x0 = 0.0f;


//텍스처
void LoadGLTextures() {
    AUX_RGBImageRec* TextureImage;


    TextureImage = LoadBMP("Data/shootingBackground.bmp");
    glGenTextures(1, &textureBackground);
    glBindTexture(GL_TEXTURE_2D, textureBackground);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    free(TextureImage->data);
    free(TextureImage);
}

void Init() {
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    LoadGLTextures();

}

void SetMaterial(float ambient[4], float diffuse[4], float specular[4], float shininess) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
bool isFiring = false;
float cannonHeight = 0.0f;

void DrawCannon() {
    // 대포 그리기
    GLUquadric* quadric = gluNewQuadric();
    glColor3f(0.5f, 0.5f, 0.5f);  // 회색 대포

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float lightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    float pocketAmbient[] = { 0.8f, 0.0f, 0.0f, 1.0f };
    float pocketDiffuse[] = { 0.0f, 0.9f, 0.5f, 1.0f };
    float pocketSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float pocketShininess = 100.0f;
    SetMaterial(pocketAmbient, pocketDiffuse, pocketSpecular, pocketShininess);
    glPushMatrix();
    //glColor3f(0.596f, 0.0f, 0.0f);
    glTranslatef(0.0f, -0.25f + cannonHeight, 0.0f);
    glutSolidSphere(0.1, 100, 100);
    glPopMatrix();

    float cylinderAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float cylinderDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float cylinderSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float cylinderShininess = 0.5f;
    SetMaterial(cylinderAmbient, cylinderDiffuse, cylinderSpecular, cylinderShininess);
    glColor3f(0.7f, 0.3f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, -0.25f, 0.0f);
    glTranslatef(0.0f, 0.45f, 0.2f);
    gluCylinder(quadric, 0.12, 0.12, 0.2, 20, 20);
    glPopMatrix();

    float cannonAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float cannonDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float cannonSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float cannonShininess = 0.5f;
    SetMaterial(cannonAmbient, cannonDiffuse, cannonSpecular, cannonShininess);
    glPushMatrix();
    glTranslatef(0.0f, -0.25f, 0.0f);  // 대포의 위치 조절
    glutSolidSphere(0.2, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.17f, -0.3f, 0.01f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    gluCylinder(quadric, 0.12, 0.12, 0.07, 20, 20);
    glTranslatef(0.0f, 0.0f, -0.4f);
    gluCylinder(quadric, 0.12, 0.12, 0.07, 20, 20);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}
void DrawBackground() {

    //get camera variables from camera class
    vec3 eye = myCamera.eye;
    vec3 at = myCamera.at;
    vec3 up = myCamera.up;


    glMatrixMode(GL_MODELVIEW); //set matrix mode
    glLoadIdentity(); //set Identity 
    //set view transform matrix
    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);

    //배경 텍스처 매핑
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureBackground);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    glFlush();

}


void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    //Rotate camera
    float x_move = -30.f * (currentMouse[0] - preMouse[0]) / windowWidth;
    float y_move = -30.f * (currentMouse[1] - preMouse[1]) / windowHeight;

    myCamera.RotateCamera(myCamera.right, (float)y_move);
    myCamera.RotateCamera(myCamera.up, (float)x_move);
    preMouse = currentMouse;

    //Set projection;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, (float)windowWidth / (float)windowHeight, 0, 10);

    //Draw Scene
    DrawBackground();
    if (isFiring) {
        cannonHeight += 0.1f;
        if (cannonHeight > 2.0f) {
            isFiring = false;
            cannonHeight = 0.0f;
        }
    }
    DrawCannon();

    //Send it to buffer
    glFlush();
}


void MyReshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;

    //Set viewport
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void MyKeyboard(unsigned char key, int x, int y) {
    float scale = 0.1;
    switch (key) {
    case 'w':
        //myCamera.RotateCamera(myCamera.right, 1);
        myCamera.MoveCamera(myCamera.forward * scale);
        glutPostRedisplay();
        break;
    case 's':
        //myCamera.RotateCamera(myCamera.right, -1);
        myCamera.MoveCamera(myCamera.forward * -scale);
        glutPostRedisplay();
        break;
    case 'a':
        myCamera.MoveCamera(myCamera.right * -scale);
        glutPostRedisplay();
        break;
    case 'd':
        myCamera.MoveCamera(myCamera.right * scale);
        glutPostRedisplay();
        break;
    case 'f':
        isFiring = true;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void MyTimer(int Value) {

    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);
}


void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
        preMouse = vec2(X, Y);
        currentMouse = preMouse;
    }



}

void MyMouseMove(GLint X, GLint Y) {
    currentMouse = vec2(X, Y);
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    //Create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(700, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Shooting");

    //Clear and set background color;
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //get window variables
    windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT);



    //Init camera
    vec3 center(0, 2, 2);
    vec3 at(0, 0, 0);
    vec3 up = cross(vec3(1, 0, 0), at - center);
    up = normalize(up);
    myCamera.InitCamera(center, at, up);

    Init();
    //set callback functions
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutTimerFunc(40, MyTimer, 1);
    glutMouseFunc(MyMouseClick);
    glutMotionFunc(MyMouseMove);
    glutReshapeFunc(MyReshape);

    //start mainloop
    glutMainLoop();
    return 0;
}