#include <vgl.h>
#include <vec.h>
#include <mat.h>

#include "BVHReader.h"

// ----------------------------------------------------------------
// 전역 상태
// ----------------------------------------------------------------
int      mainWindow;
Skeleton skeleton;
Motion   motion;

int   currentFrame = 0;
float elapsedTime = 0.0f;
bool bplay = false;

// ----------------------------------------------------------------
// X(빨강) Y(초록) Z(파랑) 축 렌더링
// ----------------------------------------------------------------
void drawAxis(float length)
{
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f); // X - 빨강
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    glColor3f(0.0f, 1.0f, 0.0f); // Y - 초록
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f); // Z - 파랑
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);

    glEnd();
    glLineWidth(1.0f);
}

// ----------------------------------------------------------------
// XZ 평면 그리드 렌더링
// ----------------------------------------------------------------
void drawFloor(float size, int divisions)
{
    float step = size / divisions;
    float half = size * 0.5f;

    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    for (int i = 0; i <= divisions; ++i)
    {
        float pos = -half + i * step;

        // Z 방향 선
        glVertex3f(pos, 0.0f, -half);
        glVertex3f(pos, 0.0f, half);

        // X 방향 선
        glVertex3f(-half, 0.0f, pos);
        glVertex3f(half, 0.0f, pos);
    }
    glEnd();
}

// ----------------------------------------------------------------
// T-pose + root translation 적용해서 Joint를 점으로 렌더링
// ----------------------------------------------------------------
void collectTPosePositions(Joint* joint,
    const vec3& parentWorldPos,
    std::vector<vec3>& outPositions)
{
    vec3 worldPos = parentWorldPos + joint->getOffset();

    if (!joint->isEndSite())
        outPositions.push_back(worldPos);

    for (Joint* child : joint->getChildren())
        collectTPosePositions(child, worldPos, outPositions);
}

vec3 getRootTranslation(int frame)
{
    Joint* root = skeleton.getRoot();
    const auto& rootChannels = root->getChannels();
    const auto& frameData = motion.getFrame(frame);

    vec3 translation(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < static_cast<int>(rootChannels.size()); ++i)
    {
        switch (rootChannels[i])
        {
        case Channel::Xposition: translation.x = frameData[i]; break;
        case Channel::Yposition: translation.y = frameData[i]; break;
        case Channel::Zposition: translation.z = frameData[i]; break;
        default: break;
        }
    }
    return translation;
}

void drawSkeleton(int frame)
{
    std::vector<vec3> positions;
    collectTPosePositions(skeleton.getRoot(), vec3(0.0f, 0.0f, 0.0f), positions);

    vec3 rootTrans = getRootTranslation(frame);

    glPointSize(5.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    for (const vec3& pos : positions)
    {
        vec3 finalPos = pos + rootTrans;
        glVertex3f(finalPos.x, finalPos.y, finalPos.z);
    }
    glEnd();
}

// ----------------------------------------------------------------
void myInit()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    BVHReader reader;
    if (!reader.read("test.bvh", skeleton, motion))
    {
        std::cerr << "Failed to load test.bvh" << std::endl;
        exit(1);
    }
}

// ----------------------------------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0, 10.0, 100.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 10000.0);

    drawAxis(50.0f);
    drawFloor(500.0f, 20);
    drawSkeleton(currentFrame);

    glutSwapBuffers();
}

// ----------------------------------------------------------------
void reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(w) / h, 0.1, 10000.0);
}

// ----------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // ESC
        exit(0);
    if (key == ' ')
        bplay = !bplay;
}

// ----------------------------------------------------------------
void idle()
{
    if (bplay == false)
        return;
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int        now = glutGet(GLUT_ELAPSED_TIME);
    float      delta = (now - lastTime) / 1000.0f;
    lastTime = now;

    elapsedTime += delta;

    float frameTime = static_cast<float>(motion.getFrameTime());
    if (frameTime > 0.0f && elapsedTime >= frameTime)
    {
        elapsedTime -= frameTime;
        currentFrame = (currentFrame + 1) % motion.getFrameCount();
    }

    glutPostRedisplay();
}

// ----------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    mainWindow = glutCreateWindow("BVH viewer");

    myInit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}