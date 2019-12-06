#define GLUT_DISABLE_ATEXIT_HACK
// #include <windows.h>
#include<GL/glut.h>
#include <iostream>
#include "mesh.h"
#define KEY_ESC 27

//g++ main.cpp mesh.cpp -o m -lglut -lGLU -lGL
Mesh* mesh;
Mesh* mesh_loop;
float rotarx=0, rotary=0;
float w=5.0;

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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

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
    //glEnable(GL_DEPTH_TEST);
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
    mesh->loadObject("source/icosfera.obj");
    //mesh->loadObject("source/cubo.obj");
	mesh_loop = new Mesh;
	mesh_loop->loopSubdivision(mesh);
	cout << "loop: "<<mesh_loop->caras.size()<<endl;
    glutMainLoop(); //bucle de rendering
    return 0;
}
