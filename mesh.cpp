#define GLUT_DISABLE_ATEXIT_HACK
// #include <windows.h>
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

void Mesh::label_boundary() {
	for (auto edge : aristas) {
		if ( !edge.second->twin) {
			edge.second->head->boundary = true;
			edge.second->next->next->head->boundary = true;
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
	V* odd_v = new V; //new
	V* even_v = new V; //original
	// map<int, V* > old_vertex;
	map<int, V*> new_vertex;
	// map<int, E*> old_edge = mesh->aristas;
	
	this->aristas = mesh->aristas;

	int n;
	int idd = 1, ide = 1, idf = 1;

	//new vertex 
	for (auto edge : mesh->aristas) {
		if (!edge.second->twin) {
			odd_v->x = (1.0/2.0)*(edge.second->head->x + edge.second->next->next->head->x);
			odd_v->y = (1.0/2.0)*(edge.second->head->y + edge.second->next->next->head->y);
			odd_v->z = (1.0/2.0)*(edge.second->head->z + edge.second->next->next->head->z);
			cout<<"ODD_v-----> "<<odd_v->x<<" "<<odd_v->y<<" "<<odd_v->z<<"\n";
		}
		else {
			odd_v->x = ((3.0/8.0)*(edge.second->head->x + edge.second->next->next->head->x)) + ((1.0/8.0)*(edge.second->next->head->x + edge.second->twin->next->head->x));
			odd_v->y = ((3.0/8.0)*(edge.second->head->y + edge.second->next->next->head->y)) + ((1.0/8.0)*(edge.second->next->head->y + edge.second->twin->next->head->y));
			odd_v->z = ((3.0/8.0)*(edge.second->head->z + edge.second->next->next->head->z)) + ((1.0/8.0)*(edge.second->next->head->z + edge.second->twin->next->head->z));
			// cout<<"ODD_v 2-----> "<<odd_v->x<<" "<<odd_v->y<<" "<<odd_v->z<<"\n";
		}
		odd_v->id = idd;
		edge.second->pmiddle->x = odd_v->x;	edge.second->pmiddle->y = odd_v->y;	edge.second->pmiddle->z = odd_v->z;
		//new_vertex.insert(pair<int, V* >(idd, edge.second->pmiddle));
		
		//this->puntos.insert(pair<int, V* >(idd, edge.second->pmiddle));
		idd++;
	}

	float BETA;
	double sx,sy,sz;
	//original vertex - new position
	for (auto point : mesh->puntos) {
		V* v = point.second;
		sx = sy = sz = 0;
		n=0;
		vector<V*>::iterator it=v->neighbors.begin();
		if ( v-> boundary ) {
			for ( n=0; it!=v->neighbors.end(); it++) {
				if ( (*it)->boundary) {
					sx += (*it)->x;
					sy += (*it)->y;
					sz += (*it)->z;
					n++;
				}
			}
			even_v->x = ((1.0/8.0)*sx) + ((3.0/4.0)*v->x);
			even_v->y = ((1.0/8.0)*sy) + ((3.0/4.0)*v->y);
			even_v->z = ((1.0/8.0)*sz) + ((3.0/4.0)*v->z);
		    cout<<"EVEN_v-----> "<<even_v->x<<" "<<even_v->y<<" "<<even_v->z<<"\n";
		}
		else{
			sx = sy = sz = 0;
			for ( n=0; it!=v->neighbors.end(); it++) {
				sx += (*it)->x;
				sy += (*it)->y;
				sz += (*it)->z;
				n++;
			}
			if (n>3)
				BETA = 1.0/n + 5.0/8.0- pow(3.0/8.0 + 1.0/4.0*cos((2.0* PI)/n), 2);
			else if (n==3)
				BETA = 3/16;
			even_v->x = v->x*(1-n*BETA) + BETA*sx;
			even_v->y = v->y*(1-n*BETA) + BETA*sy;
			even_v->z = v->z*(1-n*BETA) + BETA+sz;
		}
		even_v->id = idd;
		v->x = even_v->x;	v->y = even_v->y;	v->z = even_v->z;
		//edge.second->pmiddle = even_v;
		// cout<<"even-----> "<<edge.second->pmiddle->id<<"\n";
		//new_vertex.insert(pair<int, V* >(idd, v));
		//this->puntos.insert(pair<int, V* >(idd, even_v));
		idd++;
	}
	mesh->puntos = new_vertex;

	//create new faces
	for (auto face : mesh->caras ) {
		F* f = face.second;
		E * fe[3];
		fe[0] = f->edge;
		fe[1] = fe[0]->next;
		fe[2] = fe[1]->next;

		V* v[3];
		int vindex[3];
		for ( int i=0; i<3; i++) {
			vindex[i] = v[i]->id;
			v[i] = fe[i]->pmiddle;
		} 
		f->vindex[0] = vindex;
		this->caras.insert(pair<int, F* >(idf, f)); idf++;
		
		for ( int i=0; i<3; i++) {
			v[0] = fe[i]->next->next->head;	vindex[0]=v[0]->id;
			v[1] = fe[i]->pmiddle; vindex[1]=v[1]->id; 	
			v[2] = fe[(i+2)%3]->pmiddle; vindex[2]=v[2]->id;
			f->vindex[0] = vindex;
			this->caras.insert(pair<int, F* >(idf, f)); idf++;
		}
		this->aristas.insert(pair<int, E* >(idf, fe[0])); ide++;
		this->aristas.insert(pair<int, E* >(idf, fe[1])); ide++;
		this->aristas.insert(pair<int, E* >(idf, fe[2])); ide++;
	}

	//new mesh
	std::ofstream os('2'+mesh->nameOBj);
	int vObjID = 1;
	std::map<int, int> vidToObjID;

	for(auto point : this->puntos) {
		V *v = point.second;
		os << "v " << v->x << " " << v->y << " " << v->z << std::endl;
		vidToObjID[v->id] = vObjID++;
	}			
	os << "#" << puntos.size() << "vertex" << std::endl;

	for(auto face : this->caras) {
		F* f=face.second;
		os << "f ";
		for (int i=0; i<3; i++) {
			os << f->vindex[i] << " ";
		}
		os << std::endl;
	}
	os.close();
	std::cout<<"complete new mesh \n";
}