#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#include "glut.h"
#include <chrono>
#include <thread>

using namespace std;

const double M_PI_VAL = atan(1.0) * 4.0;
const double M_TAU_VAL = atan(1.0) * 8.0;

double increment = 0.05;
int windowW = 1000, windowH = 1000;
unsigned char activeKey;

void Display1() {
    double mxX, mxY, mnX, mnY;
    double kA = 1.0, kB = 2.0;
    double localStep = 0.05;

    mxX = kA - kB - 1.0;
    mnX = kA + kB + 1.0;
    mxY = mnY = 0.0;

    for (double t = -M_PI_VAL / 2.0 + localStep; t < M_PI_VAL / 2.0; t += localStep) {
        double vx1, vy1, vx2, vy2;
        vx1 = kA + kB * cos(t);
        mxX = max(mxX, vx1);
        mnX = min(mnX, vx1);

        vx2 = kA - kB * cos(t);
        mxX = max(mxX, vx2);
        mnX = min(mnX, vx2);

        vy1 = kA * tan(t) + kB * sin(t);
        mxY = max(mxY, vy1);
        mnY = min(mnY, vy1);

        vy2 = kA * tan(t) - kB * sin(t);
        mxY = max(mxY, vy2);
        mnY = min(mnY, vy2);
    }

    mxX = max(fabs(mxX), fabs(mnX));
    mxY = max(fabs(mxY), fabs(mnY));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_STRIP);
    for (double t = -M_PI_VAL / 2.0 + localStep; t < M_PI_VAL / 2.0; t += localStep) {
        glVertex2d((kA + kB * cos(t)) / mxX, (kA * tan(t) + kB * sin(t)) / mxY);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (double t = -M_PI_VAL / 2.0 + localStep; t < M_PI_VAL / 2.0; t += localStep) {
        glVertex2d((kA - kB * cos(t)) / mxX, (kA * tan(t) - kB * sin(t)) / mxY);
    }
    glEnd();
}

void Display2() {
    double xLimit = 8.0 * M_PI_VAL;
    double yLimit = exp(1.1);

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_STRIP);
    for (double x = 0.0; x < xLimit; x += increment) {
        glVertex2d(x / xLimit, (fabs(sin(x)) * exp(-sin(x))) / yLimit);
    }
    glEnd();
}

void Display3() {
    double xLimit = 20.0, yMaxVal = 0.0, yMinVal = 1.0;
    double localStep = 0.05;

    for (double x = 0.0; x <= xLimit; x += localStep) {
        double dist = fabs(x - round(x));
        double y = (x == 0) ? 1.0 : dist / x;
        yMaxVal = max(yMaxVal, y);
        yMinVal = min(yMinVal, y);
    }

    yMaxVal = max(fabs(yMaxVal), fabs(yMinVal));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_STRIP);
    for (double x = 0.0; x <= xLimit; x += localStep) {
        double yVal = (x == 0) ? 1.0 : fabs(x - round(x)) / x;
        glVertex2d((x / xLimit) * 0.95, (yVal / yMaxVal) * 0.95);
    }
    glEnd();
}

void plot(double (*fx)(double, double, double), double (*fy)(double, double, double), double p1, double p2, double startT, double endT, double s = 0.01, double scX = 1.0, double scY = 1.0, GLint mode = GL_LINE_STRIP) {
    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(mode);
    for (double t = startT; t <= endT; t += s) {
        glVertex2d((fx(p1, p2, t) / scX) * 0.95, (fy(p1, p2, t) / scY) * 0.95);
    }
    glEnd();
}

double circleConcoidX(double a, double b, double t) { return 2.0 * (a * cos(t) + b) * cos(t); }
double circleConcoidY(double a, double b, double t) { return 2.0 * (a * cos(t) + b) * sin(t); }
double cycloidX(double a, double b, double t) { return a * t - b * sin(t); }
double cycloidY(double a, double b, double t) { return a - b * cos(t); }
double epicycloidX(double a, double b, double t) { double ratio = b / a; return (a + b) * cos(ratio * t) - b * cos(t + ratio * t); }
double epicycloidY(double a, double b, double t) { double ratio = b / a; return (a + b) * sin(ratio * t) - b * sin(t + ratio * t); }
double hypocycloidX(double a, double b, double t) { double ratio = b / a; return (a - b) * cos(ratio * t) - b * cos(t - ratio * t); }
double hypocycloidY(double a, double b, double t) { double ratio = b / a; return (a - b) * sin(ratio * t) - b * sin(t - ratio * t); }

void Display4() {
    double mxX = 0, mnX, mxY, mnY;
    double pA = 0.3, pB = 0.2;
    double s = 0.05;

    mnX = 2.0 * (pA + pB);
    mxY = -2.0 * pB;
    mnY = 2.0 * pB;

    for (double t = -M_PI_VAL; t <= M_PI_VAL; t += s) {
        double curX = 2.0 * (pA * cos(t) + pB) * cos(t);
        double curY = 2.0 * (pA * cos(t) + pB) * sin(t);
        mxX = max(mxX, curX); mnX = min(mnX, curX);
        mxY = max(mxY, curY); mnY = min(mnY, curY);
    }
    mxX = max(fabs(mxX), fabs(mnX));
    mxY = max(fabs(mxY), fabs(mnY));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
    for (double t = -M_PI_VAL; t <= M_PI_VAL; t += s) {
        glVertex2d(((2.0 * (pA * cos(t) + pB) * cos(t)) / mxX) * 0.95, ((2.0 * (pA * cos(t) + pB) * sin(t)) / mxY) * 0.95);
    }
    glEnd();
}

void Display5() {
    double s = 0.05, pA = 0.1, pB = 0.2;
    double tMin = -3.0 * M_PI_VAL, tMax = 3.0 * M_PI_VAL;
    double mxX = pA * tMax - pB, mnX = pA * tMin + pB;

    for (double t = tMin; t <= tMax; t += s) {
        double curX = pA * t - pB * sin(t);
        mxX = max(mxX, curX);
        mnX = min(mnX, curX);
    }
    mxX = max(fabs(mxX), fabs(mnX));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_STRIP);
    for (double t = tMin; t <= tMax; t += s) {
        glVertex2d(((pA * t - pB * sin(t)) / mxX) * 0.95, (pA - pB * cos(t)) * 0.95);
    }
    glEnd();
}

void Display6() {
    double pA = 0.1, pB = 0.3, s = 0.05;
    const double r = pB / pA;
    double mxX = -pA - 2.0 * pB, mnX = pA + 2.0 * pB;
    double mxY = mxX, mnY = mnX;

    for (double t = 0.0; t <= 2.0 * M_PI_VAL; t += s) {
        double curX = (pA + pB) * cos(r * t) - pB * cos(t + r * t);
        double curY = (pA + pB) * sin(r * t) - pB * sin(t + r * t);
        mxX = max(mxX, curX); mnX = min(mnX, curX);
        mxY = max(mxY, curY); mnY = min(mnY, curY);
    }
    mxX = max(fabs(mxX), fabs(mnX));
    mxY = max(fabs(mxY), fabs(mnY));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
    for (double t = 0.0; t < 2.0 * M_PI_VAL; t += s) {
        glVertex2d((((pA + pB) * cos(r * t) - pB * cos(t + r * t)) / mxX) * 0.95, (((pA + pB) * sin(r * t) - pB * sin(t + r * t)) / mxY) * 0.95);
    }
    glEnd();
}

void Display7() {
    double pA = 0.1, pB = 0.3, s = 0.05;
    const double r = pB / pA;
    double mxX = pA - 2.0 * pB, mnX = 2.0 * pB - pA;
    double mxY = mxX, mnY = mnX;

    for (double t = 0.0; t <= 2.0 * M_PI_VAL; t += s) {
        double curX = (pA - pB) * cos(r * t) - pB * cos(t - r * t);
        double curY = (pA - pB) * sin(r * t) - pB * sin(t - r * t);
        mxX = max(mxX, curX); mnX = min(mnX, curX);
        mxY = max(mxY, curY); mnY = min(mnY, curY);
    }
    mxX = max(fabs(mxX), fabs(mnX));
    mxY = max(fabs(mxY), fabs(mnY));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
    for (double t = 0.0; t <= 2.0 * M_PI_VAL; t += s) {
        glVertex2d((((pA - pB) * cos(r * t) - pB * cos(t - r * t)) / mxX) * 0.95, (((pA - pB) * sin(r * t) - pB * sin(t - r * t)) / mxY) * 0.95);
    }
    glEnd();
}

void Display8() {
    double pA = 0.02, s = 0.05;
    double tMin = 0.0, tMax = 2.0 * M_PI_VAL;
    double mxVal = 0.0;

    for (double t = tMin; t <= tMax; t += s) {
        double radius = pA * exp(1.0 + t);
        double curX = radius * cos(t - 3.0 * M_PI_VAL / 4.0);
        double curY = radius * sin(t - 3.0 * M_PI_VAL / 4.0);
        mxVal = max(mxVal, max(fabs(curX), fabs(curY)));
    }

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_STRIP);
    for (double t = tMin; t <= tMax; t += s) {
        double radius = pA * exp(1.0 + t);
        glVertex2d((radius * cos(t - 3.0 * M_PI_VAL / 4.0) / mxVal) * 0.95, (radius * sin(t - 3.0 * M_PI_VAL / 4.0) / mxVal) * 0.95);
    }
    glEnd();
}

void Display9() {
    double pA = 10.0, s = 0.01;
    double mxX = -1.0, mnX = 1.0, mxY = -1.0, mnY = 1.0;

    for (double t = 0.0; t <= 2.0 * M_PI_VAL; t += s) {
        double curX = sin(pA * t) * cos(t);
        double curY = sin(pA * t) * sin(t);
        mxX = max(mxX, curX); mnX = min(mnX, curX);
        mxY = max(mxY, curY); mnY = min(mnY, curY);
    }
    mxX = max(fabs(mxX), fabs(mnX));
    mxY = max(fabs(mxY), fabs(mnY));

    glColor3f(1.0f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
    for (double t = 0.0; t <= 2.0 * M_PI_VAL; t += s) {
        glVertex2d((sin(pA * t) * cos(t) / mxX) * 0.95, (sin(pA * t) * sin(t) / mxY) * 0.95);
    }
    glEnd();
}

void Display10() {
    double s = 0.01, pA = 0.2;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.98f, 0.97f);
    for (double t = -M_PI_VAL / 2.0; t < -M_PI_VAL / 6.0 - s; t += s) {
        if (fabs(t) != M_PI_VAL / 6.0) {
            double den = 4.0 * cos(t) * cos(t) - 3.0;
            double vx = pA / den;
            double vy = pA * tan(t) / den;
            if (vx < 0 && vx > -1 && vy > 0 && vy < 1) {
                if (vy > 0 && vy < 0.19) glColor3f(1.0f, 0.0f, 0.0f);
                glVertex2f((float)vx, (float)vy);
            }
        }
    }
    glVertex2f(-0.98f, 0.97f);
    glEnd();

    s = 0.02;
    for (double t = -M_PI_VAL / 2.0; t < -M_PI_VAL / 6.0 - s; t += s) {
        double den = 4.0 * cos(t) * cos(t) - 3.0;
        double vx = pA / den;
        double vy = pA * tan(t) / den;
        if (vx < 0 && vx > -1 && vy > 0 && vy < 1) {
            double den2 = 4.0 * cos(t + s / 2.0) * cos(t + s / 2.0) - 3.0;
            double vx2 = pA / den2;
            double vy2 = pA * tan(t + s / 2.0) / den2;
            glBegin(GL_TRIANGLES);
            if (vy > 0 && vy < 0.18) glColor3f(1.0f, 0.1f, 0.1f);
            else glColor3f(0.1f, 0.1f, 0.1f);
            glVertex2f(-0.98f, 0.97f);
            glVertex2f((float)vx, (float)vy);
            glVertex2f((float)vx2, (float)vy2);
            glEnd();
        }
    }
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(2);
    glPointSize(1);
    glEnable(GL_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display(void) {
    cout << "Call Display" << endl;
    glClear(GL_COLOR_BUFFER_BIT);

    switch (activeKey) {
    case '1': Display1(); break;
    case '2': Display2(); break;
    case '3': Display3(); break;
    case '4': Display4(); break;
    case '5': Display5(); break;
    case '6': Display6(); break;
    case '7': Display7(); break;
    case '8': Display8(); break;
    case '9': Display9(); break;
    case '0': Display10(); break;
    default: break;
    }
    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    activeKey = key;
    if (key == 27) exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
    cout << "Mouse button " << ((button == GLUT_LEFT_BUTTON) ? "left" : ((button == GLUT_RIGHT_BUTTON) ? "right" : "middle")) << " ";
    cout << ((state == GLUT_DOWN) ? "pressed" : "released") << " at: " << x << " x " << y << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(windowW, windowH);
    glutInitWindowPosition(-1, -1);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Function Plotter");
    init();
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyboardFunc);
    glutMouseFunc(MouseFunc);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
