#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include "mesh.h"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <fstream>
#include"GL/glut.h"

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
    int idd=1,ide=1,idf=1,idn=1,a,b,c,an,bn,cn;
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
		if (method == "vn") {
			float nn[3];
			iss >> nn[0] >> nn[1] >> nn[2];
			normales.insert(pair<int, float* >(idn, nn));
			//cout << normales[idn][0]<<endl;
			idn++;
		}
        if(method=="f"){
            iss>>a;
            iss.ignore(20,'/');
            iss.ignore(20,'/');
            iss>>an;
            for(int i=0;i<3;++i)
                puntos[a]->normal[i]=normales[an][i];
            iss.ignore(20,' ');
            iss>>b;
            iss.ignore(20,'/');
            iss.ignore(20,'/');
            iss>>bn;
            for(int i=0;i<3;++i)
                puntos[b]->normal[i]=normales[bn][i];
            iss.ignore(20,' ');
            iss>>c;
            iss.ignore(20,'/');
            iss.ignore(20,'/');
            iss>>cn;
            for(int i=0;i<3;++i)
                puntos[c]->normal[i]=normales[cn][i];
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
            puntos[a]->edgeVertex.push_back(e1);
			puntos[b]->edgeVertex.push_back(e1);
            searchTween(puntos[a],e1);
            //---------e2----------------------------
            e2->id=ide+1;
            puntos[b]->edge=e2;
            e2->head=puntos[c];
            e2->next=e3;
            e2->face=cara;
            puntos[b]->edgeVertex.push_back(e2);
			puntos[c]->edgeVertex.push_back(e2);
            searchTween(puntos[b],e2);
            //---------e3----------------------------
            e3->id=ide+2;
            puntos[c]->edge=e3;
            e3->head=puntos[a];
            e3->next=e1;
            e3->face=cara;
            puntos[c]->edgeVertex.push_back(e3);
			puntos[a]->edgeVertex.push_back(e3);
            searchTween(puntos[c],e3);

            aristas.insert(pair<int,E* >(ide,e1));
            aristas.insert(pair<int,E* >(ide+1,e2));
            aristas.insert(pair<int,E* >(ide+2,e3));
            ide+=3;
        }
    }
    //showTweens();
    addVertexNeighbors();
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
		//glColor3f(0.5, 0.6, 0.5);
		for (int i = 0; i < 3; ++i) {
            glNormal3f(v->normal[0],v->normal[1],v->normal[2]);
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

void Mesh::addVertexNeighbors() {
	for(auto point: puntos) {
		E* e = point.second->edge;
		do{
           point.second->neighbors.push_back(e->head);
           e=e->twin->next;
		}while(e!=point.second->edge);
		for(int i=0;i<point.second->neighbors.size();++i)
            cout<<point.second->neighbors[i]->id<<" ";
        cout<<endl;
	}
}

void Mesh::showTweens(){
    for (auto arista:aristas){
        cout<<arista.second->id<<" "<<arista.second->twin->id<<endl;
    }
}
