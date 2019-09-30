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
    int idd=1,ide=1,a,b,c;
    while(getline(obj, line)){
        istringstream iss(line);
        iss>>method;
        if(method=="v"){
            v=new V;
            iss>>v->x>>v->y>>v->z;
            v->id=idd;
            puntos.insert(pair<int,V* >(idd,v));
            idd++;
        }
        if(method=="f"){
            iss>>a>>b>>c;
            E* e1=new E;
            E* e2=new E;
            E* e3=new E;
            e1->id=ide;
            e1->head=puntos[b];
            puntos[a]->edge=e1;
            e1->next=e2;
            e2->id=ide+1;
            puntos[b]->edge=e2;
            e2->head=puntos[c];
            e2->next=e3;
            e3->id=ide+2;
            puntos[c]->edge=e3;
            e3->head=puntos[a];
            e3->next=e1;
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

//template <class E>
void Mesh::drawEdges(){
    V* v=puntos[1];
    glBegin(GL_LINE_LOOP);
    glVertex3f(v->x,v->y,v->z);
    v=v->edge->head;
    glVertex3f(v->x,v->y,v->z);
    v=v->edge->head;
    glVertex3f(v->x,v->y,v->z);
    glEnd();
    //for(auto val:puntos)
    //    cout<<val.first<<" "<<val.second->id<<endl;
}

