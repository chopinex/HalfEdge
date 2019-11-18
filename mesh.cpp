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

void Mesh::addVertexNeighbors() {
	for(auto point: puntos) {		
		for (list<E* >::iterator it=point.second->edgeVertex.begin(); it!=point.second->edgeVertex.end(); it++) {
			if( (*it)->head != point.second ) 
				point.second->neighbors.push_back(point.second);
		}
	} 
}

void Mesh::loopSubdivision(Mesh* &mesh) {
	V* odd_v = new V;
	V* even_v = new V;
	map<int, E*> new_edge;
	map<int, E*> old_edge = mesh->aristas;
	
	this->aristas = mesh->aristas;

	int n;	double sum, sx=0.0, sy=0.0, sz=0.0;
	int idd = 1, ide = 1,idf=1, a, b, c;

	//new vertex 
	for (auto edge : old_edge) {
		if (!edge.second->twin) {
			odd_v->x = (1.0/2.0)*(edge.second->head->x + edge.second->next->next->head->x);
			odd_v->y = (1.0/2.0)*(edge.second->head->y + edge.second->next->next->head->y);
			odd_v->z = (1.0/2.0)*(edge.second->head->z + edge.second->next->next->head->z);
		}
		else {
			odd_v->x = ((3.0/8.0)*(edge.second->head->x + edge.second->next->next->head->x)) + ((1.0/8.0)*(edge.second->next->head->x + edge.second->twin->next->head->x));
			odd_v->y = ((3.0/8.0)*(edge.second->head->y + edge.second->next->next->head->y)) + ((1.0/8.0)*(edge.second->next->head->y + edge.second->twin->next->head->y));
			odd_v->z = ((3.0/8.0)*(edge.second->head->z + edge.second->next->next->head->z)) + ((1.0/8.0)*(edge.second->next->head->z + edge.second->twin->next->head->z));
		}
		odd_v->id = idd;
		edge.second->pmiddle = odd_v;
		this->puntos.insert(pair<int, V* >(idd, edge.second->pmiddle));
		idd++;
	}

	float BETA;
	for (auto edge : old_edge) {
		n = edge.second->head->neighbors.size();
		BETA = 1.0/n + 5.0/8.0- pow(3.0/8.0 + 1.0/4.0*cos((2.0* PI)/n), 2);
		
		if (!edge.second->twin) {
			even_v->x = ((1.0/8.0)*(edge.second->head->x + edge.second->next->next->head->x)) + ((3.0/4.0)*even_v->x);
			even_v->y = ((1.0/8.0)*(edge.second->head->y + edge.second->next->next->head->y)) + ((3.0/4.0)*even_v->y);
			even_v->z = ((1.0/8.0)*(edge.second->head->z + edge.second->next->next->head->z)) + ((3.0/4.0)*even_v->z);
		}
		else {
			for(int k=0; k<n; k++) {
				sx += BETA * edge.second->head->neighbors[k]->x;
				sy += BETA * edge.second->head->neighbors[k]->y;
				sz += BETA * edge.second->head->neighbors[k]->z;
			}
			sx=sx/n;	sy=sy/n;	sz=sz/n;
			even_v->x = edge.second->head->x*(1-n*BETA) + sx;
			even_v->y = edge.second->head->y*(1-n*BETA) + sy;
			even_v->z = edge.second->head->z*(1-n*BETA) + sz;
		}
		// cout<<"EVEN_v 2-----> "<<even_v->x<<" "<<even_v->y<<" "<<even_v->z<<"\n";
		even_v->id = idd;
		edge.second->pmiddle = even_v;
		// cout<<"even-----> "<<edge.second->pmiddle->id<<"\n";
		this->puntos.insert(pair<int, V* >(idd, edge.second->pmiddle));
		idd++;
	}
	
	getPoints();
	E* e1 = new E;
	E* e2 = new E;
	E* e3 = new E;
	F* cara = new F;
	cara->id = idf;
	cara->edge = e1;
	caras.insert(pair<int, F* >(idf, cara));
	idf++;
	for (auto edge : old_edge) {
		a = edge.second->pmiddle->id;
		b = edge.second->next->pmiddle->id;
		c = edge.second->next->next->pmiddle->id;
        // cout<<"-----> "<<a<<" "<<b<<" "<<c<<"\n";
		//---------e1----------------------------
		e1->id = ide;
		e1->head = puntos[b];
		puntos[a]->edge = e1;
		e1->next = e2;
		e1->face = cara;
		searchTween(puntos[a], e1);
		//---------e2----------------------------
		e2->id = ide + 1;
		puntos[b]->edge = e2;
		e2->head = puntos[c];
		e2->next = e3;
		e2->face = cara;
		searchTween(puntos[b], e2);
		//---------e3----------------------------
		e3->id = ide + 2;
		puntos[c]->edge = e3;
		e3->head = puntos[a];
		e3->next = e1;
		e3->face = cara;
		searchTween(puntos[c], e3);
		aristas.insert(pair<int, E* >(ide, e1));
		aristas.insert(pair<int, E* >(ide + 1, e2));
		aristas.insert(pair<int, E* >(ide + 2, e3));
		ide += 3;
	}			
}