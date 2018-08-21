#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "TPConst.h"
#include "TPgl.h"

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCRN_W, SCRN_H);
    glutInitWindowPosition(GetSystemMetrics(SM_CXSCREEN) / 2 - SCRN_W / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - SCRN_H / 2);
	glutCreateWindow(WINDOW_TITLE);

	TPInitGL();
    TPInitUI();

	glutDisplayFunc(TPRenderCallback);
	glutIdleFunc(TPRenderCallback);
	glutMouseFunc(TPMouseCallback);
    glutMotionFunc(TPMotionCallback);
	glutPassiveMotionFunc(TPPassiveMotionCallback);
	glutKeyboardFunc(TPKeyboardCallback);

	glutMainLoop();
	return 0;
}