#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "EulerOperation.h"


CHAR appName[] = "OpenGL";
HWND  g_hWnd;
HDC   g_hDC;
HGLRC g_hGLRC;

#define SWAPBUFFERS SwapBuffers(g_hDC)
#define APP_WIDTH 800 
#define APP_HEIGHT 600

#define FACEMODE 0
#define LINEMODE 1
INT renderMode = FACEMODE;

GLfloat eyeX = 0;
GLfloat eyeY = 0;
GLfloat eyeZ = -4;
GLfloat yaw = 3.14159265359f / 2;
GLfloat pitch = 0;
bool LDown = false;
INT mouseX;
INT mouseY;

LONG WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL bSetupPixelFormat(HDC);

/* OpenGL Code */
GLvoid gl_resize_cb(GLsizei, GLsizei);
GLvoid gl_init_cb(GLsizei, GLsizei);
GLvoid gl_draw_cb(GLvoid);

void normalizeVec3(GLfloat& x, GLfloat& y, GLfloat& z);
EulerOperation* EOsolid = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;

    /* Register the frame class */
    wndclass.style = 0;
    wndclass.lpfnWndProc = (WNDPROC)MainWndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, appName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndclass.lpszMenuName = appName;
    wndclass.lpszClassName = appName;

    if (!RegisterClass(&wndclass))
        return FALSE;

    /* Create the frame */
    g_hWnd = CreateWindow(appName,
        "EulerOperation",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        APP_WIDTH,
        APP_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL);

    /* make sure window was created */
    if (!g_hWnd)
        return FALSE;

    /* show and update main window */
    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    while (1) {
        /* Message */
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
        {
            if (GetMessage(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                return TRUE;
            }
        }
        /* Render */
        gl_draw_cb();
    }
}



/* OpenGL code */
GLvoid gl_resize_cb(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLdouble)width / height, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

GLvoid gl_init_cb(GLsizei width, GLsizei height)
{
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLdouble)width / height, 1, 100);
    glMatrixMode(GL_MODELVIEW);

    GLfloat lightAmb[] = { 0.3f, 0.3f, 0.3f, 0.1f };
    GLfloat lightDiff[] = { 0.45f, 0.45f, 0.45f, 1.f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);

    EOsolid = new EulerOperation();


    //choose model
    int selectCase = 1;
    if (selectCase == 0) {
        // Cube
        EOsolid->mvfs(1, -1, 1);
        Solid* solid1 = EOsolid->Solids.front();
        // face0 
        EOsolid->mev(solid1->GetFace(0)->OutLoop, solid1->GetVertex(0), 1, 1, 1);
        EOsolid->mev(solid1->GetFace(0)->OutLoop, solid1->GetVertex(1), -1, 1, 1);
        EOsolid->mev(solid1->GetFace(0)->OutLoop, solid1->GetVertex(2), -1, -1, 1);
        EOsolid->mef(solid1->GetFace(0)->OutLoop, solid1->GetVertex(3), solid1->GetVertex(0));
        // face1
        EOsolid->mev(solid1->GetFace(1)->OutLoop, solid1->GetVertex(0), 1, -1, -1);
        EOsolid->mev(solid1->GetFace(1)->OutLoop, solid1->GetVertex(4), 1, 1, -1);
        EOsolid->mef(solid1->GetFace(1)->OutLoop, solid1->GetVertex(5), solid1->GetVertex(1));
        // face2
        EOsolid->mev(solid1->GetFace(2)->OutLoop, solid1->GetVertex(5), -1, 1, -1);
        EOsolid->mef(solid1->GetFace(2)->OutLoop, solid1->GetVertex(6), solid1->GetVertex(2));
        // face3
        EOsolid->mev(solid1->GetFace(3)->OutLoop, solid1->GetVertex(6), -1, -1, -1);
        EOsolid->mef(solid1->GetFace(3)->OutLoop, solid1->GetVertex(7), solid1->GetVertex(3));
        // face4
        EOsolid->mef(solid1->GetFace(4)->OutLoop, solid1->GetVertex(7), solid1->GetVertex(4));
    }  
    else if (selectCase == 1) {
        // 2 through holes
        EOsolid->mvfs(0.9, 0.5, 0.5);
        Solid* solid3 = EOsolid->Solids.back();
        //outloop
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(0), -0.9, 0.5, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(1), -0.9, -0.5, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(2), 0.9, -0.5, 0.5);
        EOsolid->mef(solid3->GetFace(0)->OutLoop, solid3->GetVertex(3), solid3->GetVertex(0));
                            
        //innerloop1     
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(0), 0.75, 0.25, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(4), 0.75, -0.25, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(5), 0.25, -0.25, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(6), 0.25, 0.25, 0.5);
        EOsolid->mef(solid3->GetFace(0)->OutLoop, solid3->GetVertex(7), solid3->GetVertex(6), solid3->GetVertex(4), solid3->GetVertex(0));
        EOsolid->kemr(solid3->GetFace(0)->OutLoop, solid3->GetFace(0)->OutLoop->GetHalfEdge(solid3->GetVertex(0), solid3->GetVertex(4))->Hedge, solid3->GetVertex(0));

        //innerloop2
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(1), -0.8, 0.2, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(8), -0.2, 0.2, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(9), -0.2, -0.2, 0.5);
        EOsolid->mev(solid3->GetFace(0)->OutLoop, solid3->GetVertex(10), -0.8, -0.2, 0.5);
        EOsolid->mef(solid3->GetFace(0)->OutLoop, solid3->GetVertex(11), solid3->GetVertex(10), solid3->GetVertex(8), solid3->GetVertex(1));
        EOsolid->kemr(solid3->GetFace(0)->OutLoop, solid3->GetFace(0)->OutLoop->GetHalfEdge(solid3->GetVertex(1), solid3->GetVertex(8))->Hedge, solid3->GetVertex(1));
        //sweep
        EOsolid->sweep(solid3, solid3->GetFace(0), 0.5, 0, -2);
    }

    
}

GLvoid gl_draw_cb(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, \
        eyeX + cosf(pitch) * cosf(yaw), \
        eyeY + sinf(pitch), \
        eyeZ + cosf(pitch) * sinf(yaw), \
        0, 1, 0);

    glPushMatrix();
    static GLfloat lightPos[4];
    lightPos[0] = 0;
    lightPos[1] = 1;
    lightPos[2] = 1;
    lightPos[3] = 1;
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    if (renderMode == FACEMODE)
    {
        for (std::list<Solid*>::iterator solidIt = EOsolid->Solids.begin(); solidIt != EOsolid->Solids.end(); ++solidIt)
        {
            Solid* solid = *solidIt;
            GLuint id = glGenLists(solid->Sfaces.size());
            if (id)
            {
                GLuint faceId = id;
                for (std::list<Face*>::iterator faceIt = solid->Sfaces.begin(); faceIt != solid->Sfaces.end(); ++faceIt)
                {
                    Face* face = *faceIt;
                    GLUtesselator* faceTess = gluNewTess();
                    if (faceTess != nullptr)
                    {
                        gluTessCallback(faceTess, GLU_TESS_BEGIN, (void(__stdcall*)(void))glBegin);
                        gluTessCallback(faceTess, GLU_TESS_END, (void(__stdcall*)(void))glEnd);
                        gluTessCallback(faceTess, GLU_TESS_VERTEX, (void(__stdcall*)())glVertex3dv);
                        glNewList(faceId, GL_COMPILE);
                        std::vector<GLdouble*> data;
                        gluTessBeginPolygon(faceTess, nullptr);
                        for (Loop* loop : face->Floops)
                        {
                            gluTessBeginContour(faceTess);
                            HalfEdge* he = loop->firstHalfEdge;
                            do
                            {
                                GLdouble* d = new GLdouble[3];
                                d[0] = he->Hvertex->vx;
                                d[1] = he->Hvertex->vy;
                                d[2] = he->Hvertex->vz;
                                gluTessVertex(faceTess, d, d);
                                data.push_back(d);
                                he = he->next;
                            } while (he != loop->firstHalfEdge);
                            gluTessEndContour(faceTess);
                        }
                        gluTessEndPolygon(faceTess);
                        for (GLdouble* d : data)
                        {
                            delete[] d;
                        }
                        glEndList();
                        gluDeleteTess(faceTess);
                    }
                    glCallList(faceId);
                    faceId++;
                }
                glDeleteLists(id, solid->Sfaces.size());
            }
        }
    }
    else if (renderMode == LINEMODE)
    {
        for (std::list<Solid*>::iterator solidIt = EOsolid->Solids.begin(); solidIt != EOsolid->Solids.end(); ++solidIt)
        {
            Solid* solid = *solidIt;
            glPushMatrix();
            for (std::list<Face*>::iterator faceIt = solid->Sfaces.begin(); faceIt != solid->Sfaces.end(); ++faceIt)
            {
                Face* face = *faceIt;
                for (std::list<Loop*>::iterator loopIt = face->Floops.begin(); loopIt != face->Floops.end(); ++loopIt)
                {
                    Loop* loop = *loopIt;
                    glBegin(GL_LINE_LOOP);
                    HalfEdge* he = loop->firstHalfEdge;
                    do
                    {
                        glVertex3f(he->Hvertex->vx, he->Hvertex->vy, he->Hvertex->vz);
                        he = he->next;
                    } while (he != loop->firstHalfEdge);
                    glVertex3f(he->Hvertex->vx, he->Hvertex->vy, he->Hvertex->vz);
                    glEnd();
                }
                //glTranslatef(0.1, 0.1, 0);
            }
            glPopMatrix();
        }
    }
    else
    {
        //Nothing
    }

    glPopMatrix();

    SWAPBUFFERS;
}

void normalizeVec3(GLfloat& x, GLfloat& y, GLfloat& z)
{
    GLfloat len = sqrtf(x * x + y * y + z * z);
    if (abs(len) <= 0.000001)
    {
        x = 0;
        y = 0;
        z = 0;
    }
    else
    {
        x /= len;
        y /= len;
        z /= len;
    }
}

BOOL bSetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd, * ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 8;
    ppfd->cDepthBits = 16;
    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    pixelformat = ChoosePixelFormat(hdc, ppfd);

    if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0)
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
    {
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;
}

/* Main window proc */
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LONG lRet = 1;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg) {
    case WM_CREATE:
        g_hDC = GetDC(hWnd);
        if (!bSetupPixelFormat(g_hDC))
            PostQuitMessage(0);

        g_hGLRC = wglCreateContext(g_hDC);
        wglMakeCurrent(g_hDC, g_hGLRC);
        GetClientRect(hWnd, &rect);
        gl_init_cb(rect.right, rect.bottom);
        break;

    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        GetClientRect(hWnd, &rect);
        gl_resize_cb(rect.right, rect.bottom);
        break;

    case WM_CLOSE:
        if (g_hGLRC)
            wglDeleteContext(g_hGLRC);
        if (g_hDC)
            ReleaseDC(hWnd, g_hDC);
        g_hGLRC = 0;
        g_hDC = 0;

        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        if (g_hGLRC)
            wglDeleteContext(g_hGLRC);
        if (g_hDC)
            ReleaseDC(hWnd, g_hDC);

        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
        LDown = true;
        mouseX = lParam & 0xFFFF;
        mouseY = lParam >> 16;
        break;

    case WM_LBUTTONUP:
        LDown = false;
        break;

    case WM_MOUSEMOVE:
        if (LDown)
        {
            INT x = (int)(short)LOWORD(lParam);
            INT y = (int)(short)HIWORD(lParam);
            yaw += 3.14159265359f * (x - mouseX) / 360;
            pitch -= 3.14159265359f * (y - mouseY) / 360;
            mouseX = x;
            mouseY = y;
        }
        break;

    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            break;
        case VK_RIGHT:
            break;
        case VK_UP:
            break;
        case VK_DOWN:
            break;
        case 'W':
            eyeX += 0.2f * cosf(pitch) * cosf(yaw);
            eyeY += 0.2f * sinf(pitch);
            eyeZ += 0.2f * cosf(pitch) * sinf(yaw);
            break;
        case 'S':
            eyeX -= 0.2f * cosf(pitch) * cosf(yaw);
            eyeY -= 0.2f * sinf(pitch);
            eyeZ -= 0.2f * cosf(pitch) * sinf(yaw);
            break;
        case 'A':
        {
            GLfloat tmpX = -cosf(pitch) * sinf(yaw), tmpY = 0, tmpZ = cosf(pitch) * cosf(yaw);
            normalizeVec3(tmpX, tmpY, tmpZ);
            eyeX -= 0.2f * tmpX;
            eyeZ -= 0.2f * tmpZ;
        }
        break;
        case 'D':
        {
            GLfloat tmpX = -cosf(pitch) * sinf(yaw), tmpY = 0, tmpZ = cosf(pitch) * cosf(yaw);
            normalizeVec3(tmpX, tmpY, tmpZ);
            eyeX += 0.2f * tmpX;
            eyeZ += 0.2f * tmpZ;
        }
        break;
        case 'Z':
            renderMode = !renderMode;
            break;
        }
        break;

    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

