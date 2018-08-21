#include "TPEnvironment.h"
#include "TPFont.h"
#include "TPTexture.h"
#include "TPUI.h"
#include "TPgl.h"

TPUI ui;

void TEST_Func1()
{
    GLuint count = 100;
    TPPoint *pointVector = (TPPoint *)malloc(sizeof(TPPoint)* count);

    for (unsigned i = 0; i < count; ++i)
    {
        pointVector[i].x = i / (float)count * 10;
        pointVector[i].y = sinf(pointVector[i].x);
    }

    ui.GetCoordinateByName("1")->SetDrawingPoints(TP_POINT, 4, pointVector, count);
    free(pointVector);
}

void TEST_Func2()
{
    GLuint count = 100;
    TPPoint *pointVector = (TPPoint *)malloc(sizeof(TPPoint)* count);

    for (unsigned i = 0; i < count; ++i)
    {
        pointVector[i].x = i / (float)count * 10;
        pointVector[i].y = log10(pointVector[i].x);
    }

    ui.GetCoordinateByName("2")->SetDrawingPoints(TP_CURVE, 4, pointVector, count);
    free(pointVector);
}

void ItemClick(void* p)
{
    TPMenuItem* itemx = (TPMenuItem*)p;
    char * name = itemx->GetName();
    ui.setCurrentCoordByName(name);
}

void TPInitGL()
{
    TPLoadFont();
    TPLoadTexture();

    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void TPInitUI()
{
    TPMenuItem* item1 = ui.AddMenuItem("1");
    item1->SetTexture(TPGetTextureByOrder(1));
    item1->SetClickFunc(ItemClick);

    TPMenuItem* item2= ui.AddMenuItem("2");
    item2->SetTexture(TPGetTextureByOrder(0));
    item2->SetClickFunc(ItemClick);

    ui.AddCoordinate("1")->SetAnchor(0, 10, -5, 5);
    TEST_Func1();

    ui.AddCoordinate("2")->SetAnchor(-1, 10, -1 ,2);
    TEST_Func2();
}

void TPRenderCallback()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ui.Render();
    glutSwapBuffers();
}

void TPMouseCallback(int button, int state, int x, int y)
{
    //std::cout << button << ' ' << state << ' ' << x << ' ' << y << ' ' << std::endl;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        std::cout << "x=" << x << "y=" << y << "state=" << state << std::endl;
        if (ui.InIllusionSection(x, y))
        {
            std::cout << "IN START" << std::endl;
            ui.StartTranslate(x, y);
        }
        
        TPMenuItem* menuItem = ui.InMenuItem(x, y);
        if (menuItem)
        {
            menuItem->ToggleChecked();
        }
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        std::cout << "IN END" << std::endl;
        std::cout << "x=" << x << "y=" << y << "state=" << state << std::endl;
        if (ui.InTranslating())
        {
            ui.StopTranslate();
        }

        if (ui.InIllusionSection(x, y) && ui.IsDoubleClick(x, y))
        {
            ui.StartScaleAnimation(x, y, true);
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        if (ui.InIllusionSection(x, y) && ui.IsDoubleClick(x, y))
        {
            ui.StartScaleAnimation(x, y, false);
        }
    }

    if (button == GLUT_WHEEL_UP && state == GLUT_UP)
    {
        std::cout << "IN START" << std::endl;
        std::cout << "x=" << x << "y=" << y << "state=" << state << std::endl;
        if (ui.InIllusionSection(x, y))
        {
            ui.StartScale(x, y, MOUSE_SCALE_FACTOR);
        }
    }

    if (button == GLUT_WHEEL_DOWN)
    {
        std::cout << "IN END" << std::endl;
        std::cout << "x=" << x << "y=" << y << "state=" << state << std::endl;
        if (ui.InIllusionSection(x, y))
        {
            ui.StartScale(x, y, 1 / MOUSE_SCALE_FACTOR);
        }
    }
}

void TPPassiveMotionCallback(int x, int y)
{
    // debug
    //std::cout << "x=" << x << "y=" << y << std::endl;

    ui.MousePassiveAction(x, y);
}

void TPMotionCallback(int x, int y)
{
    ui.MouseAction(x, y);
}

void TPKeyboardCallback(unsigned char key, int x, int y)
{

}
