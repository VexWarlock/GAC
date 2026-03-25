#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <string>
#include <complex>
#include <vector>
#include "glut.h" 

using namespace std::complex_literals;

// Constante matematice originale
double circle = atan(1) * 8;
double halfCircle = atan(1) * 4;
double tau = circle;
double pi = halfCircle;

// Variabile globale
double r = 0, g = 0, b = 0;
int contor = 0;
int g_w = 1000, g_h = 1000;
unsigned char g_prevKey;

int g_recursionMax = 8, g_recursionCurrent = 2;
double g_jfa = -0.82, g_jfb = -0.17;

// --- Utilitare pentru Text ---
void bitmapString(void* font, const char* str) {
    for (const char* c = str; *c != 0; ++c) {
        glutBitmapCharacter(font, *c);
    }
}

void drawBitmapString(const char* str, float x = -2, float y = -2) {
    if ((-2 != x) || (-2 != y)) glRasterPos2f(x, y);
    bitmapString(GLUT_BITMAP_8_BY_13, str);
}

template <typename Numtype>
void drawBitmapNumber(Numtype number, float x = -2, float y = -2) {
    if ((-2 != x) || (-2 != y)) glRasterPos2f(x, y);
    bitmapString(GLUT_BITMAP_8_BY_13, std::to_string(number).c_str());
}

// --- Clasa Turtle ---
class Turtle {
protected:
    double m_x, m_y, m_angle;
    double pixelSize;
    int thickness;

public:
    Turtle() : m_x(0), m_y(0), m_angle(0), pixelSize(0), thickness(1) {}
    double offsetX = 0.0, offsetY = 0.0;

    void setOffsets(double ox, double oy) { offsetX = ox; offsetY = oy; }
    void setPixelSize(double size) { pixelSize = size; }

    void rotate(double angle) { m_angle += angle; }
    void move(double distance) {
        m_x += distance * cos(m_angle);
        m_y += distance * sin(m_angle);
    }

    void draw(double distance) {
        glBegin(GL_LINES);
        glVertex2d(m_x, m_y);
        move(distance);
        glVertex2d(m_x, m_y);
        glEnd();
    }

    void drawGrid(int windowWidth, int windowHeight, int numCellsX, int numCellsY) {
        double marginX = windowWidth * 0.05;
        double marginY = windowHeight * 0.05;
        double usableWidth = windowWidth - 2 * marginX;
        double usableHeight = windowHeight - 2 * marginY;
        double pSize = std::min(usableWidth / numCellsX, usableHeight / numCellsY);
        double gridWidth = pSize * numCellsX;
        double gridHeight = pSize * numCellsY;
        double ox = (windowWidth - gridWidth) / 2.0;
        double oy = (windowHeight - gridHeight) / 2.0;

        for (int i = 0; i <= numCellsX; ++i) {
            m_x = ox + i * pSize; m_y = oy; m_angle = pi / 2;
            draw(gridHeight);
        }
        for (int j = 0; j <= numCellsY; ++j) {
            m_x = ox; m_y = oy + j * pSize; m_angle = 0;
            draw(gridWidth);
        }
    }

    void drawBlueCircleWithTurtle(int cx, int cy, int r, double pSize, double ox, double oy) {
        glColor3f(0.1, 0.1, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 200; ++i) {
            double angle = tau * i / 200.0;
            double x = ox + (cx + r * cos(angle) + 0.5) * pSize + 0.5 * pSize;
            double y = oy + (cy + r * sin(angle) + 0.5) * pSize + 0.5 * pSize;
            glVertex2d(x, y);
        }
        glEnd();
    }

    void drawPixel(int x, int y) {
        double radius = pixelSize * 0.3;
        double cx = offsetX + (x + 0.5) * pixelSize + 0.5 * pixelSize;
        double cy = offsetY + (y + 0.5) * pixelSize + 0.5 * pixelSize;
        glColor3f(0, 0, 0);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int angle = 0; angle <= 360; angle += 20) {
            double rad = angle * pi / 180.0;
            glVertex2f(cx + cos(rad) * radius, cy + sin(rad) * radius);
        }
        glEnd();
    }

    void drawOctagonOnGrid(int numCellsX, int numCellsY, double pSize, double ox, double oy) {
        glColor3f(0.1, 0.1, 0.4);
        const int octX[8] = { 13,  7, -3, -12, -14, -8,  2, 11 };
        const int octY[8] = { 2, 11, 13,   7,  -3, -12, -14, -8 };
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 8; ++i) {
            glVertex2d(ox + (octX[i] + numCellsX / 2) * pSize + pSize,
                oy + (octY[i] + numCellsY / 2) * pSize + pSize);
        }
        glEnd();
    }

    void drawThicknessPixels(int x, int y) {
        drawPixel(x, y);
        for (int i = -thickness; i < thickness; ++i) {
            if (i == 0) continue;
            drawPixel(x + i, y); drawPixel(x, y + i);
        }
    }
};

// --- Algoritmi Bresenham Octanti ---
void bresenhamOctant1_5(Turtle& t, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int d = 2 * dy - dx, dE = 2 * dy, dNE = 2 * (dy - dx);
    int x = x0, y = y0;
    for (int i = 0; i <= dx; ++i) {
        t.drawThicknessPixels(x, y);
        if (d <= 0) d += dE; else { d += dNE; y += sy; }
        x += sx;
    }
}

void bresenhamOctant2_6(Turtle& t, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int d = 2 * dx - dy, dE = 2 * dx, dNE = 2 * (dx - dy);
    int x = x0, y = y0;
    for (int i = 0; i <= dy; ++i) {
        t.drawThicknessPixels(x, y);
        if (d <= 0) d += dE; else { d += dNE; x += sx; }
        y += sy;
    }
}

void bresenhamOctant3_7(Turtle& t, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = (x0 > x1) ? -1 : 1, sy = (y0 < y1) ? 1 : -1;
    int d = 2 * dx - dy, dE = 2 * dx, dNE = 2 * (dx - dy);
    int x = x0, y = y0;
    for (int i = 0; i <= dy; ++i) {
        t.drawThicknessPixels(x, y);
        if (d <= 0) d += dE; else { d += dNE; x += sx; }
        y += sy;
    }
}

void bresenhamOctant4_8(Turtle& t, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    int sx = (x0 > x1) ? -1 : 1, sy = (y0 > y1) ? -1 : 1;
    int d = 2 * dy - dx, dE = 2 * dy, dNE = 2 * (dy - dx);
    int x = x0, y = y0;
    for (int i = 0; i <= dx; ++i) {
        t.drawThicknessPixels(x, y);
        if (d <= 0) d += dE; else { d += dNE; y += sy; }
        x += sx;
    }
}

void bresenhamSmart(Turtle& t, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), dy = abs(y1 - y0);
    if (dx >= dy) {
        if ((x0 <= x1 && y0 <= y1) || (x0 >= x1 && y0 >= y1)) bresenhamOctant1_5(t, x0, y0, x1, y1);
        else bresenhamOctant4_8(t, x0, y0, x1, y1);
    }
    else {
        if ((x0 <= x1 && y0 <= y1) || (x0 >= x1 && y0 >= y1)) bresenhamOctant2_6(t, x0, y0, x1, y1);
        else bresenhamOctant3_7(t, x0, y0, x1, y1);
    }
}

// --- Algoritmi Cerc Octanti ---
void drawCircleOctant1_5(Turtle& t, int cx, int cy, int r) {
    int x = 0, y = r, d = 1 - r, de = 3, dse = -2 * r + 5;
    auto draw = [&](int x, int y) { t.drawPixel(cx + x, cy + y); t.drawPixel(cx - x, cy - y); };
    draw(x, y);
    while (y > x) {
        if (d < 0) { d += de; de += 2; dse += 2; }
        else { d += dse; de += 2; dse += 4; --y; }
        ++x; draw(x, y);
    }
}

void drawCircleOctant2_6(Turtle& t, int cx, int cy, int r) {
    int x = 0, y = r, d = 1 - r, de = 3, dse = -2 * r + 5;
    auto draw = [&](int x, int y) { t.drawPixel(cx + y, cy + x); t.drawPixel(cx - y, cy - x); };
    draw(x, y);
    while (y > x) {
        if (d < 0) { d += de; de += 2; dse += 2; }
        else { d += dse; de += 2; dse += 4; --y; }
        ++x; draw(x, y);
    }
}

void drawCircleOctant3_7(Turtle& t, int cx, int cy, int r) {
    int x = 0, y = r, d = 1 - r, de = 3, dse = -2 * r + 5;
    auto draw = [&](int x, int y) { t.drawPixel(cx + y, cy - x); t.drawPixel(cx - y, cy + x); };
    draw(x, y);
    while (y > x) {
        if (d < 0) { d += de; de += 2; dse += 2; }
        else { d += dse; de += 2; dse += 4; --y; }
        ++x; draw(x, y);
    }
}

void drawCircleOctant4_8(Turtle& t, int cx, int cy, int r) {
    int x = 0, y = r, d = 1 - r, de = 3, dse = -2 * r + 5;
    auto draw = [&](int x, int y) { t.drawPixel(cx + x, cy - y); t.drawPixel(cx - x, cy + y); };
    draw(x, y);
    while (y > x) {
        if (d < 0) { d += de; de += 2; dse += 2; }
        else { d += dse; de += 2; dse += 4; --y; }
        ++x; draw(x, y);
    }
}

// --- Functii de Umplere Cerc ---
void fillCircleMidpoint(Turtle& t, int cx, int cy, int r) {
    int x = 0, y = r, d = 1 - r, de = 3, dse = -2 * r + 5;
    auto drawL = [&](int sx, int ex, int y) { for (int i = sx; i <= ex; ++i) t.drawPixel(i, y); };
    drawL(cx - r, cx + r, cy);
    while (y > x) {
        ++x;
        if (d < 0) { d += de; de += 2; dse += 2; }
        else { d += dse; de += 2; dse += 4; --y; }
        drawL(cx - x, cx + x, cy + y); drawL(cx - x, cx + x, cy - y);
        drawL(cx - y, cx + y, cy + x); drawL(cx - y, cx + y, cy - x);
    }
}

void fillCircleMidpoint2(Turtle& t, int cx, int cy, int r) {
    int x = 0, y = r, d = 1 - r, de = 3, dse = -2 * r + 5;
    auto drawLC = [&](int sx, int ex, int yL) {
        for (int i = sx; i <= ex; ++i)
            if ((i - cx) * (i - cx) + (yL - cy) * (yL - cy) <= r * r) t.drawPixel(i, yL);
        };
    drawLC(cx - r, cx + r, cy);
    while (y > x) {
        ++x;
        if (d < 0) { d += de; de += 2; dse += 2; }
        else { d += dse; de += 2; dse += 4; --y; }
        drawLC(cx - x, cx + x, cy + y); drawLC(cx - x, cx + x, cy - y);
        drawLC(cx - y, cx + y, cy + x); drawLC(cx - y, cx + y, cy - x);
    }
}

// --- Functii Display ---
void Display1() {
    Turtle t; int nx = 29, ny = 29;
    glColor3f(0.5, 0.5, 0.5); t.drawGrid(g_w, g_h, nx, ny);
    double pSize = std::min((g_w * 0.9) / nx, (g_h * 0.9) / ny);
    double ox = (g_w - pSize * nx) / 2.0, oy = (g_h - pSize * ny) / 2.0;
    t.setPixelSize(pSize); t.setOffsets(ox, oy);
    t.drawOctagonOnGrid(nx, ny, pSize, ox, oy);
    const int oX[8] = { 13, 7, -3, -12, -14, -8, 2, 11 }, oY[8] = { 2, 11, 13, 7, -3, -12, -14, -8 };
    for (int i = 0; i < 8; ++i) {
        int next = (i + 1) % 8;
        bresenhamSmart(t, oX[i] + nx / 2, oY[i] + ny / 2, oX[next] + nx / 2, oY[next] + ny / 2);
    }
}

void Display2() {
    Turtle t; int nx = 29, ny = 29;
    glColor3f(0.5, 0.5, 0.5); t.drawGrid(g_w, g_h, nx, ny);
    double pSize = std::min((g_w * 0.9) / nx, (g_h * 0.9) / ny);
    double ox = (g_w - pSize * nx) / 2.0, oy = (g_h - pSize * ny) / 2.0;
    t.setPixelSize(pSize); t.setOffsets(ox, oy);
    int cx = nx / 2, cy = ny / 2, r = 13;
    t.drawBlueCircleWithTurtle(cx, cy, r, pSize, ox, oy);
    drawCircleOctant1_5(t, cx, cy, r); drawCircleOctant2_6(t, cx, cy, r);
    drawCircleOctant3_7(t, cx, cy, r); drawCircleOctant4_8(t, cx, cy, r);
}

void Display3() {
    Turtle t; int nx = 29, ny = 29;
    glColor3f(0.5, 0.5, 0.5); t.drawGrid(g_w, g_h, nx, ny);
    double pSize = std::min((g_w * 0.9) / nx, (g_h * 0.9) / ny);
    double ox = (g_w - pSize * nx) / 2.0, oy = (g_h - pSize * ny) / 2.0;
    t.setPixelSize(pSize); t.setOffsets(ox, oy);
    int cx = nx / 2, cy = ny / 2, r = 13;
    t.drawBlueCircleWithTurtle(cx, cy, r, pSize, ox, oy);
    fillCircleMidpoint(t, cx, cy, r);
}

void Display4() {
    Turtle t; int nx = 29, ny = 29;
    glColor3f(0.5, 0.5, 0.5); t.drawGrid(g_w, g_h, nx, ny);
    double pSize = std::min((g_w * 0.9) / nx, (g_h * 0.9) / ny);
    double ox = (g_w - pSize * nx) / 2.0, oy = (g_h - pSize * ny) / 2.0;
    t.setPixelSize(pSize); t.setOffsets(ox, oy);
    int cx = nx / 2, cy = ny / 2, r = 13;
    t.drawBlueCircleWithTurtle(cx, cy, r, pSize, ox, oy);
    fillCircleMidpoint2(t, cx, cy, r);
}

// --- Callbacks GLUT ---
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glLineWidth(1); glPointSize(1);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (g_prevKey == '1') Display1();
    else if (g_prevKey == '2') Display2();
    else if (g_prevKey == '3') Display3();
    else if (g_prevKey == '4') Display4();
    glFlush();
}

void Reshape(int w, int h) {
    g_w = w; g_h = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

void KeyboardFunc(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (key == '+' && g_recursionCurrent < g_recursionMax) ++g_recursionCurrent;
    if (key == '-' && g_recursionCurrent > 0) --g_recursionCurrent;
    g_prevKey = key;
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
    std::cout << "Mouse button " << ((button == GLUT_LEFT_BUTTON) ? "left" : "right")
        << (state == GLUT_DOWN ? " pressed" : " released") << " at " << x << "x" << y << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(g_w, g_h);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("Rasterization Demo");
    init();
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyboardFunc);
    glutMouseFunc(MouseFunc);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}
