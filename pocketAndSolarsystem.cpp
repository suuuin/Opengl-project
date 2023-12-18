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


//텍스처
GLuint textureEarth, textureMoon, textureStars;

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

void DrawPocket(float cx, float cy, float rx, float ry, int numSegments) {

    glPushMatrix();
    glTranslatef(cx, cy, 0.0f);
    glRotatef(-50, 0.0f, 0.0f, 0.1f);

    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // 초 단위로 구현
    if (time <= 3) {
        glTranslatef( cx - time / 3, cy, 0.0f);
    }
    
    else if (3 < time && time < 6 ) {
        glTranslatef(-1, cy, 0.0f);
        float rotationAngle = 0.0f;
        rotationAngle += 360.0f / 3.0f * time;  // 3초에 360도 회전
        glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    }
    else if (6 <=time && time < 9) {
        glTranslatef((time-9) / 3, cy, 0.0f);
    }
    

    
    glColor3f(0.596f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float x = rx * cosf(theta);
        float y = ry * sinf(theta);

        // 타원의 꼭지점을 생성
        glVertex2f(x, y);
    }
    glEnd();

    glBegin(GL_TRIANGLES);

    // 역삼각형 부분 (타원 중점 기준)
    glVertex3f(-1.3 * rx, 0.55 * ry, 0.0);
    glVertex3f(-0.7 * rx, 0.0, 0.0);
    glVertex3f(-1.3 * rx, -0.55 * ry, 0.0);

    glEnd();

    glPopMatrix();
    glFlush();
}

//텍스처
void LoadGLTextures() {
    AUX_RGBImageRec* TextureImage;

    // Load Earth texture
    TextureImage = LoadBMP("Data/Earth.bmp");
    glGenTextures(1, &textureEarth);
    glBindTexture(GL_TEXTURE_2D, textureEarth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    free(TextureImage->data);
    free(TextureImage);

    // Load Moon texture
    TextureImage = LoadBMP("Data/Moon.bmp");
    glGenTextures(1, &textureMoon);
    glBindTexture(GL_TEXTURE_2D, textureMoon);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    free(TextureImage->data);
    free(TextureImage);

    TextureImage = LoadBMP("Data/Stars.bmp");
    glGenTextures(1, &textureStars);
    glBindTexture(GL_TEXTURE_2D, textureStars);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    free(TextureImage->data);
    free(TextureImage);
}

void Init() {
    glEnable(GL_TEXTURE_2D);
    LoadGLTextures();
}

void texturedSphere(float radius, int slices, GLuint texture) {
    GLUquadric* obj = gluNewQuadric();
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricNormals(obj, GLU_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    gluQuadricTexture(obj, GL_TRUE);  // 텍스처 좌표 사용 활성화
    gluQuadricNormals(obj, GLU_SMOOTH);

    gluSphere(obj, radius, slices, slices);

    glDisable(GL_TEXTURE_2D);

    gluDeleteQuadric(obj);
}

void DrawSolarSystem() {

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
    glBindTexture(GL_TEXTURE_2D, textureStars);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -0.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-0.45f, 0.4f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    texturedSphere(0.13, 50, textureMoon);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75f, -0.75f, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    texturedSphere(0.4, 50, textureEarth);
    glPopMatrix();
  

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
    DrawSolarSystem();
    DrawPocket(0.4f, -0.1f, 0.15f, 0.1f, 100);

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


