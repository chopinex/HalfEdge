#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include "mesh.h"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <fstream>
#include<GL/glut.h>

//template <class E>
void Mesh::loadObject(string nameObj){
    ifstream obj;
    try {
    // Enter the path to the example.obj file
    obj.open(nameObj, ios::in);
    }
    catch (ifstream::failure::exception e) {
        cout << "Failed to open file";
        return;
    }
    string line;
    string method;
    V *v;
    int idd=1,ide=1,idf=1,a,b,c;
    while(getline(obj, line)){
        istringstream iss(line);
        iss>>method;
        if(method=="v"){
            v=new V;
            iss>>v->x>>v->y>>v->z;
            v->id=idd;
            //cout<<idd<<": "<<v->x<<" "<<v->y<<" "<<v->z<<endl;
            puntos.insert(pair<int,V* >(idd,v));
            idd++;
        }
        if(method=="f"){
            iss>>a;
            iss.ignore(14,' ');
            iss>>b;
            iss.ignore(14,' ');
            iss>>c;
            E* e1=new E;
            E* e2=new E;
            E* e3=new E;
            F* cara= new F;
            cara->id=idf;
            cara->edge= e1;
            caras.insert(pair<int,F* >(idf,cara));
            idf++;
            //---------e1----------------------------
            e1->id=ide;
            e1->head=puntos[b];
            puntos[a]->edge=e1;
            e1->next=e2;
            e1->face=cara;
            searchTween(puntos[a],e1);
            //---------e2----------------------------
            e2->id=ide+1;
            puntos[b]->edge=e2;
            e2->head=puntos[c];
            e2->next=e3;
            e2->face=cara;
            searchTween(puntos[b],e2);
            //---------e3----------------------------
            e3->id=ide+2;
            puntos[c]->edge=e3;
            e3->head=puntos[a];
            e3->next=e1;
            e3->face=cara;
            searchTween(puntos[c],e3);
            aristas.insert(pair<int,E* >(ide,e1));
            aristas.insert(pair<int,E* >(ide+1,e2));
            aristas.insert(pair<int,E* >(ide+2,e3));
            ide+=3;
        }
    }
    obj.close();
}

//template <class E>
void Mesh::getPoints(){
    for (auto val : puntos)
        cout<<val.first<<" "<<val.second->x<<" "<<val.second->y<<" "<<val.second->z<<endl;
}
void Mesh::vertFace(){
    V* v;
	E* e;
	for (map<int, F* >::iterator it = caras.begin(); it != caras.end(); it++) {
		e = (*it).second->edge;
		v = e->head;
		cout<<(*it).first<<" "<<v->id<<endl;
	}
}

void Mesh::drawEdges() {
	V* v;
	E* e;
	for (map<int, F* >::iterator it = caras.begin(); it != caras.end(); it++) {
		e = (*it).second->edge;
		v = e->head;
		glBegin(GL_TRIANGLES);
		glColor3f(0.5, 0.4, 0.1);
		for (int i = 0; i < 3; ++i) {
			glVertex3f(v->x, v->y, v->z);
			e = e->next;
			v = e->head;
		}
		glEnd();
	}
	//for(auto val:puntos)
	//    cout<<val.first<<" "<<val.second->id<<endl;
}

void Mesh::searchTween(V* init, E* dir){
    V* h = dir->head;
    for (auto arista:aristas){
        if(arista.second->head==init&&arista.second->next->next->head==h){
            dir->twin=arista.second;
            arista.second->twin=dir;
            break;
        }
    }
}

