#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include<GL/glut.h>
#include "mesh.h"
#define KEY_ESC 27

Mesh* mesh;
float rotarx=0, rotary=0;
float w=8.0;

void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glRotatef(rotarx,1.0,0.0,0.0);
	glRotatef(rotary,0.0,1.0,0.0);
	glOrtho(-w,  w,-w, w, -w, w);
	//gluPerspective(65, 1, 1, 20);
    //glFrustum(-20,20,-20,20,5,9);
	//dibujar quadTree (qt->draw())
    mesh->drawEdges();
	//dibuja el gizmo
	//displayGizmo();
    glFlush();
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tama�o
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}
}
void specialKeys(int key,int x,int y){
    switch(key){
        case GLUT_KEY_RIGHT:
            rotary+=5;break;
        case GLUT_KEY_LEFT:
            rotary-=5;break;
        case GLUT_KEY_UP:
            rotarx+=5;break;
        case GLUT_KEY_DOWN:
            rotarx-=5;break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600); //tama�o de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Puntos"); //titulo de la ventana
	GLfloat light_ambient0[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat light_diffuse0[] = {0.4, 0.6, 0.4, 1.0};
	GLfloat light_specular0[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position0[] = {-0.5, 0.5, 0.0, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    GLfloat light_ambient1[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat light_diffuse1[] = {0.6, 0.6, 0.4, 1.0};
	GLfloat light_specular1[] = {1.0, 1.0, 1.0, 0.2};
	GLfloat light_position1[] = {0.5, -0.5, 0.0, 1.0};
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	//glutKeyboardFunc(&window_key);
	//glutMouseFunc(&OnMouseClick);
	//glutMotionFunc(&OnMouseMotion);
	glutSpecialFunc(&specialKeys);
	glutIdleFunc(&idle);

    mesh=new Mesh;
    //mesh->loadObject("source/girafa_testes.obj");
    //mesh->loadObject("source/silla.obj");
    //mesh->loadObject("source/FF1.obj");
    //mesh->loadObject("source/icosfera.obj");
    //mesh->loadObject("source/perrito2.obj");
    mesh->loadObject("source/cubo.obj");
    glutMainLoop(); //bucle de rendering
    return 0;
}
