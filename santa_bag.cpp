#define _CRT_SECURE_NO_WARNINGS
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif
#include <stdio.h>
#include <gl/glut.h>
#include <gl/glu.h>
#include "glaux.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Camera.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define TEXTURE_NUM 2
GLuint texture[TEXTURE_NUM];
char texture_name[TEXTURE_NUM][30] = { "Data/blue.bmp", "Data/rudo.bmp" };
int selectedBoxIndex = 0;
float scenePositionX = 0.0f;
float scenePositionY = 0.0f;
bool bMousing = false;
bool renderBoxesInFront = true;
static POINT ptLastMousePosit;
std::vector<glm::vec3> boxPositions(10, glm::vec3(0.0f, 4.0f, 0.0f));
GLuint backgroundTexture;



void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    for (int i = 0; i < std::min(10, static_cast<int>(boxPositions.size())); i++) {
        boxPositions[i] = glm::vec3(-2.0f + i * 1.5f, 4.0f, 0.0f);
    }
    // 이미지 비율을 유지하면서 크기 조정
    float aspectRatio = (float)w / (float)h;
    glLoadIdentity();
    gluOrtho2D(-aspectRatio, aspectRatio, -1.0, 1.0);
}


void DrawSantaAndGiftSack() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);  // 깊이 테스트 비활성화
    glPushMatrix();

    // Drawing a sphere
    glPushMatrix();
    glTranslatef(4.8f, -3.2f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f);  // Set color for Santa
    glScalef(1.5f, 1.0f, 1.0f);
    glutSolidSphere(4.0, 20, 20);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color
    glPopMatrix();

    // Drawing a triangle right next to the sphere
    glPushMatrix();
    glTranslatef(4.8f - 3.0f * 1.5f, -4.2f, 1.0f);  // Adjust the x-coordinate based on the sphere's size
    glRotatef(120.0f, 0.0f, 1.0f, 0.0f);  // Rotate the triangle (90 degrees around y-axis)

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);  // Set color for Santa

    // Define the vertices of the triangle
    glVertex3f(0.0f, 0.0f, 0.0f);  // Vertex 1
    glVertex3f(6.0f, 0.0f, 0.0f);  // Vertex 2
    glVertex3f(6.0f, 6.0f, 0.0f);  // Vertex 3

    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color
    glPopMatrix();









    glColor3f(237.0f / 255.0f, 181.0f / 255.0f, 33.0f / 255.0f);  // Green color (r:45, g:135, b:0)

    glPushMatrix();
    glTranslatef(1.0f, 13.0f, 1.0f);
    glRotatef(70.0f, 1.0f, 0.0f, 0.0f);
    glScalef(39.0f, 17.5f, 0.0f);

    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();

    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color



    // Draw the cylinder
    float radius = 0.37f;
    float height = 6.5f;
    int numSides = 20;

    // Rotate the cylinder to stand upright
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(10.1f, 5.7f, 1.0f);
    glRotatef(27.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (int i = 0; i < numSides; ++i) {
        float angle = 2.0f * M_PI * i / numSides;

        // Vertices on the side of the cylinder
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        float z = 0.0f;

        if (i % 2 == 0) {
            glColor3f(1.0f, 0.0f, 0.0f);  // Red color
        }
        else {
            glColor3f(45.0f / 255.0f, 135.0f / 255.0f, 0.0f);  // Green color (r:45, g:135, b:0)
        }

        glVertex3f(x, y, z);
        glVertex3f(x, y, z + height);
        glVertex3f(radius * cos(angle + 2.0f * M_PI / numSides), radius * sin(angle + 2.0f * M_PI / numSides), z + height);
        glVertex3f(radius * cos(angle + 2.0f * M_PI / numSides), radius * sin(angle + 2.0f * M_PI / numSides), z);
    }

    glEnd();
    // Draw a circle at the top of the cylinder
    glColor3f(188.0f / 255.0f, 236.0f / 255.0f, 245.0f / 255.0f);  // Green color (r:45, g:135, b:0)
    glPushMatrix();
    glTranslatef(2.8f, -4.0f, 8.3f);
    //glScalef(1.0f, 1.0f, 1.2f);
    glutSolidSphere(1.0, 15, 15);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color
    glPopMatrix();


    // Reset color
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();


    // Draw the cylinder
    float radius1 = 0.40f;
    float height1 = 6.0f;
    int numSides1 = 20;

    // Rotate the cylinder to stand upright
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(9.4f, 7.8f, 1.0f);
    glRotatef(27.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (int i = 0; i < numSides1; ++i) {
        float angle = 2.0f * M_PI * i / numSides1;

        // Vertices on the side of the cylinder
        float x = radius1 * cos(angle);
        float y = radius1 * sin(angle);
        float z = 0.0f;
        if (i % 2 == 0) {
            glColor3f(1.0f, 0.0f, 0.0f);  // Red color
        }
        else {
            glColor3f(45.0f / 255.0f, 135.0f / 255.0f, 0.0f);  // Green color (r:45, g:135, b:0)
        }
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + height);
        glVertex3f(radius1 * cos(angle + 2.0f * M_PI / numSides1), radius1 * sin(angle + 2.0f * M_PI / numSides1), z + height);
        glVertex3f(radius1 * cos(angle + 2.0f * M_PI / numSides1), radius1 * sin(angle + 2.0f * M_PI / numSides1), z);
    }

    glEnd();



    // Draw a circle at the top of the cylinder
    glColor3f(255.0f / 255.0f, 206.0f / 255.0f, 226.0f / 255.0f);  // Green color (r:45, g:135, b:0)
    glPushMatrix();
    glTranslatef(2.0f, -2.5f, 7.8f);
    //glScalef(1.0f, 1.0f, 1.2f);
    glutSolidSphere(1.0, 15, 15);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color
    glPopMatrix();


    // Reset color
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();






    glEnable(GL_TEXTURE_2D);
    if (renderBoxesInFront) {

        for (int i = 0; i < 5; i++) {
            glPushMatrix();
            glm::vec3 boxPosition = boxPositions[i % 10];
            glTranslatef(boxPosition.x - 12.0f, boxPosition.y - 4.5f, boxPosition.z);
            glScalef(1.5f, 1.5f, 1.0f);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, texture[i % TEXTURE_NUM]);

            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.7f, -0.7f, 0.7f);   // 앞면
            glTexCoord2f(0.33f, 0.0f); glVertex3f(0.7f, -0.7f, 0.7f);
            glTexCoord2f(0.33f, 0.5f); glVertex3f(0.7f, 0.7f, 0.7f);
            glTexCoord2f(0.0f, 0.5f); glVertex3f(-0.7f, 0.7f, 0.7f);

            glTexCoord2f(0.34f, 0.0f); glVertex3f(0.7f, -0.7f, -0.7f);
            glTexCoord2f(0.684f, 0.0f); glVertex3f(-0.7f, -0.7f, -0.7f);   // 뒷면
            glTexCoord2f(0.684f, 0.5f); glVertex3f(-0.7f, 0.7f, -0.7f);
            glTexCoord2f(0.34f, 0.5f); glVertex3f(0.7f, 0.7f, -0.7f);

            glTexCoord2f(0.685f, 0.0f); glVertex3f(-0.7f, 0.7f, 0.7f);   // 윗면
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7f, 0.7f, 0.7f);
            glTexCoord2f(1.0f, 0.51f); glVertex3f(0.7f, 0.7f, -0.7f);
            glTexCoord2f(0.685f, 0.51f); glVertex3f(-0.7f, 0.7f, -0.7f);

            glTexCoord2f(0.0f, 0.51f); glVertex3f(-0.7f, -0.7f, -0.7f);   // 아랫면
            glTexCoord2f(0.336f, 0.51f); glVertex3f(0.7f, -0.7f, -0.7f);
            glTexCoord2f(0.336f, 1.0f); glVertex3f(0.7f, -0.7f, 0.7f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.7f, -0.7f, 0.7f);

            glTexCoord2f(0.34f, 0.51f); glVertex3f(0.7f, -0.7f, 0.7f);   // 우측면
            glTexCoord2f(0.683f, 0.51f); glVertex3f(0.7f, -0.7f, -0.7f);
            glTexCoord2f(0.683f, 1.0f); glVertex3f(0.7f, 0.7f, -0.7f);
            glTexCoord2f(0.34f, 1.0f); glVertex3f(0.7f, 0.7f, 0.7f);

            glTexCoord2f(0.685f, 0.51f); glVertex3f(-0.7f, -0.7f, -0.7f);   // 좌측면
            glTexCoord2f(1.0f, 0.51f); glVertex3f(-0.7f, -0.7f, 0.7f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.7f, 0.7f, 0.7f);
            glTexCoord2f(0.685f, 1.0f); glVertex3f(-0.7f, 0.7f, -0.7f);
            glEnd();
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glPopMatrix();

            glTranslatef(1.5f, 0.0f, 0.0f);
        }
    }


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);  // 깊이 테스트 다시 활성화
    glPopMatrix();



    // Draw the cylinder
    float radius2 = 0.40f;
    float height2 = 6.0f;
    int numSides2 = 20;

    // Rotate the cylinder to stand upright
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(-52.5f, 10.0f, 15.0f);
    glScalef(2.8f, 5.0f, 0.0f);
    glRotatef(27.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (int i = 0; i < numSides2; ++i) {
        float angle = 2.0f * M_PI * i / numSides2;

        // Vertices on the side of the cylinder
        float x = radius2 * cos(angle);
        float y = radius2 * sin(angle);
        float z = 0.0f;
        if (i % 2 == 0) {
            glColor3f(255.0f / 255.0f, 244.0f / 255.0f, 49.0f / 255.0f);  // Green color (r:45, g:135, b:0)
        }
        else {
            glColor3f(0.0f / 255.0f, 122.0f / 255.0f, 249.0f / 255.0f);  // Green color (r:45, g:135, b:0)
        }
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + height2);
        glVertex3f(radius2 * cos(angle + 2.0f * M_PI / numSides2), radius2 * sin(angle + 2.0f * M_PI / numSides2), z + height2);
        glVertex3f(radius2 * cos(angle + 2.0f * M_PI / numSides2), radius2 * sin(angle + 2.0f * M_PI / numSides2), z);
    }

    glEnd();



    // Draw a circle at the top of the cylinder
    glColor3f(188.0f / 255.0f, 236.0f / 255.0f, 245.0f / 255.0f);  // Green color (r:45, g:135, b:0)
    glPushMatrix();
    glTranslatef(0.1f, -2.5f, -5.2f);
    glScalef(0.9f, 0.9f, 0.9f);
    glutSolidSphere(0.6, 8, 8);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color
    glPopMatrix();


    // Reset color
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();


    // Draw the second cylinder
    float radius3 = 0.40f;
    float height3 = 6.0f;
    int numSides3 = 20;

    // Rotate the cylinder to stand upright
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(-42.5f, 10.0f, 8.0f);
    glScalef(2.8f, 5.0f, 0.0f);
    //glRotatef(27.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    for (int i = 0; i < numSides3; ++i) {
        float angle = 2.0f * M_PI * i / numSides3;

        // Vertices on the side of the cylinder
        float x = radius3 * cos(angle);
        float y = radius3 * sin(angle);
        float z = 0.0f;
        if (i % 2 == 0) {
            glColor3f(255.0f / 255.0f, 244.0f / 255.0f, 49.0f / 255.0f);  // Green color (r:45, g:135, b:0)
        }
        else {
            glColor3f(0.0f / 255.0f, 122.0f / 255.0f, 249.0f / 255.0f);  // Blue color
        }
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + height3);
        glVertex3f(radius3 * cos(angle + 2.0f * M_PI / numSides3), radius3 * sin(angle + 2.0f * M_PI / numSides3), z + height3);
        glVertex3f(radius3 * cos(angle + 2.0f * M_PI / numSides3), radius3 * sin(angle + 2.0f * M_PI / numSides3), z);
    }

    glEnd();

    // Draw a circle at the top of the second cylinder
    glColor3f(188.0f / 255.0f, 236.0f / 255.0f, 245.0f / 255.0f);  // Light Blue color
    glPushMatrix();
    glTranslatef(0.1f, -2.5f, -5.2f);
    glScalef(0.9f, 0.9f, 0.9f);
    glutSolidSphere(0.6, 8, 8);
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color
    glPopMatrix();

    // Reset color and transformation for the second cylinder
    glColor3f(1.0f, 1.0f, 1.0f);
    glPopMatrix();



}



void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0, -10.0, 15.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // Draw background
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-52.0f, -40.0f, -40.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(52.0f, -40.0f, -40.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(57.0f, 40.0f, -40.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-57.0f, 40.0f, -40.0f);
    glEnd();

    // Draw Santa and gift sack
    DrawSantaAndGiftSack();




    glPushMatrix();
    glTranslatef(scenePositionX, scenePositionY, 0.0f);
    glPopMatrix();

    glutSwapBuffers();
    glEnable(GL_LIGHTING);
}







AUX_RGBImageRec* LoadBMP(const char* Filename) {
    FILE* File = NULL;
    if (!Filename) return NULL;
    File = fopen(Filename, "r");
    if (File) {
        fclose(File);

        AUX_RGBImageRec* TextureImage = auxDIBImageLoad(Filename);

        // 이미지 데이터를 뒤집기
        int width = TextureImage->sizeX;
        int height = TextureImage->sizeY;
        int bytesPerPixel = 3;  // RGB 이미지이므로 3 바이트
        int imageSize = width * height * bytesPerPixel;
        unsigned char* flippedData = new unsigned char[imageSize];

        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                int originalIndex = (row * width + col) * bytesPerPixel;
                int flippedIndex = ((height - row - 1) * width + col) * bytesPerPixel;

                // 픽셀 데이터를 뒤집음
                flippedData[flippedIndex] = TextureImage->data[originalIndex];
                flippedData[flippedIndex + 1] = TextureImage->data[originalIndex + 1];
                flippedData[flippedIndex + 2] = TextureImage->data[originalIndex + 2];
            }
        }

        // 기존 데이터 대신 뒤집은 데이터로 교체
        delete[] TextureImage->data;
        TextureImage->data = flippedData;

        return TextureImage;
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
            MessageBox(NULL, "Image file has an error!", texture_name[i], MB_ICONERROR);
            Status = false;
        }

        if (TextureImage) {
            if (TextureImage->data) free(TextureImage->data);
            free(TextureImage);
        }
    }

    // Load background texture
    TextureImage = LoadBMP("Data/back6.bmp");
    if (TextureImage) {
        glGenTextures(1, &backgroundTexture);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    }
    else {
        MessageBox(NULL, "Background image file has an error!", "Data/back.bmp", MB_ICONERROR);
        Status = false;
    }

    if (TextureImage) {
        if (TextureImage->data) free(TextureImage->data);
        free(TextureImage);
    }

    return Status;
}

void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            bMousing = true;
            ptLastMousePosit.x = x;
            ptLastMousePosit.y = y;

            // Calculate the index of the clicked box
            selectedBoxIndex = static_cast<int>((float)x / (float)glutGet(GLUT_WINDOW_WIDTH) * 10.0);
        }
        else {
            bMousing = false;
        }
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
    if (bMousing && selectedBoxIndex != -1) {
        float deltaX = static_cast<float>(x - ptLastMousePosit.x) * 0.01f;
        float deltaY = static_cast<float>(y - ptLastMousePosit.y) * 0.01f;

        // Update the position of the selected box with boundary checks
        float newX = boxPositions[selectedBoxIndex].x + deltaX;
        float newY = boxPositions[selectedBoxIndex].y + deltaY; // Invert deltaY to match the mouse motion

        // Define your boundary limits (adjust these values as needed)
        float minX = -40.0f;
        float maxX = 40.0f;
        float minY = -30.0f;
        float maxY = 40.0f;

        // Apply boundary checks
        if (newX >= minX && newX <= maxX)
            boxPositions[selectedBoxIndex].x = newX;
        if (newY >= minY && newY <= maxY)
            boxPositions[selectedBoxIndex].y = newY;

        ptLastMousePosit.x = x;
        ptLastMousePosit.y = y;
        glutPostRedisplay();
    }
}




int main(int argc, char** argv) {
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(700, 500);
    glutCreateWindow("Texture Mapping for One Source Multi Use");
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

    // Set the drawing function to DrawSantaAndGiftSack


    glutMainLoop();
    return 0;
}