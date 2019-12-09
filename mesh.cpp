#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include "mesh.h"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <fstream>
#include <math.h>
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
    calculateNormals();
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
    bool otro;
	for(auto point: puntos) {
        otro=false;
		E* e = point.second->edge;
		//cout<<point.second->id<<endl;
		do{
           //cout<<point.second->edge->id<<" first "<<e->id<<endl;
           point.second->neighbors.push_back(e->head);
           if(e->twin)
                e=e->twin->next;
           else{
                e=point.second->edge;
                otro=true;
           }
		}while(e!=point.second->edge);
		//cout<<otro<<"----------------------------------------------------------"<<endl;
		if(otro){
            e = point.second->edge->next;
            do{
                //cout<<"second "<<e->id<<endl;
                point.second->neighbors.push_back(e->head);
                if(e->next->twin)
                    e=e->next->twin->next;
            }while(e->next->twin);
		}
	}
}

void Mesh::showTweens(){
    ofstream myfile;
    myfile.open ("tweens.txt");
    for (auto arista:aristas){
        if(arista.second->twin){
            cout<<arista.second->id<<" "<<arista.second->twin->id<<endl;
            myfile<<arista.second->id<<" "<<arista.second->twin->id<<endl;
        }
        else{
            cout<<arista.second->id<<" NULL"<<endl;
            myfile<<arista.second->id<<" NULL"<<endl;
        }
    }
    myfile.close();
}

void Mesh::showCaras(){
    for (map<int, F* >::iterator it = caras.begin(); it != caras.end(); it++)
        cout<<"cara "<<(*it).first<<": "<<(*it).second->edge->head->id<<"/"<<(*it).second->edge->next->head->id<<"/"<<(*it).second->edge->next->next->head->id<<endl;
}

void Mesh::calculateNormals(){
    for(auto point: puntos){
        float sx=0,sy=0,sz=0;
        for(unsigned int i=0;i<point.second->neighbors.size();++i){
            sx+=point.second->neighbors[i]->x;
            sy+=point.second->neighbors[i]->y;
            sz+=point.second->neighbors[i]->z;
        }
        point.second->normal[0]=sx/point.second->neighbors.size();
        point.second->normal[1]=sy/point.second->neighbors.size();
        point.second->normal[2]=sz/point.second->neighbors.size();
    }
}

void Mesh::edgeCollapse(float umbralD, float umbralA){
    list<E*> cercanos;
    V* uno;
    V* dos;
    for(auto arista:aristas){
        uno=arista.second->head;
        dos=arista.second->next->next->head;
        if(sqrt((dos->x-uno->x)*(dos->x-uno->x)+(dos->y-uno->y)*(dos->y-uno->y)+(dos->z-uno->z)*(dos->z-uno->z))<umbralD)
            cercanos.push_back(arista.second);
    }
    V* cola;
    float xv,yv,zv,xv1,yv1,zv1,xv2,yv2,zv2,alfa1,alfa2;
    for(auto cerca:cercanos){
        for(int i=0;i<cerca->head->neighbors.size();++i){
            xv1+=cerca->head->neighbors[i]->x;
            yv1+=cerca->head->neighbors[i]->y;
            zv1+=cerca->head->neighbors[i]->z;
        }
        xv1=xv1/cerca->head->neighbors.size()-cerca->head->x;
        yv1=yv1/cerca->head->neighbors.size()-cerca->head->y;
        zv1=zv1/cerca->head->neighbors.size()-cerca->head->z;
        cola=cerca->next->next->head;
        for(int i=0;i<cola->neighbors.size();++i){
            xv2+=cola->neighbors[i]->x;
            yv2+=cola->neighbors[i]->y;
            zv2+=cola->neighbors[i]->z;
        }
        xv2=xv2/cola->neighbors.size()-cola->x;
        yv2=yv2/cola->neighbors.size()-cola->y;
        zv2=zv2/cola->neighbors.size()-cola->z;
        xv=cerca->head->x-cola->x;
        yv=cerca->head->y-cola->y;
        zv=cerca->head->z-cola->z;
        alfa1=acos((xv1*xv+yv1*yv+zv1*zv)/(sqrt(xv1*xv1+yv1*yv1+zv1*zv1)*sqrt(xv*xv+yv*yv+zv*zv)));
        alfa2=acos((xv2*xv+yv2*yv+zv2*zv)/(sqrt(xv2*xv2+yv2*yv2+zv2*zv2)*sqrt(xv*xv+yv*yv+zv*zv)));
        if(alfa1<umbralA&&alfa2<umbralA){
            for(int i=0;i<cola->neighbors.size();++i){
                if(cola->neighbors[i]!=cerca->head){

                }
            }
        }
    }
}

void Mesh::loopSubdivision(Mesh* &mesh) {
	V* odd_v;
	V* even_v;
	//map<int, E*> new_edge;
	map<int, E*> old_edge = mesh->aristas;

	//this->aristas = mesh->aristas;

	int n;	double sx=0.0, sy=0.0, sz=0.0;
	int idd = 1, ide = 1,idf=1;

	//new vertex
	for (auto edge : old_edge) {
        odd_v=new V;
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
        even_v=new V;
		n = edge.second->head->neighbors.size();
		BETA = 1.0/n + 5.0/8.0- pow(3.0/8.0 + 1.0/4.0*cos((2.0* PI)/n), 2);

		if (!edge.second->twin) {
            //cout<<"twin\n";
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
		//edge.second->pmiddle = even_v;
		edge.second->head = even_v;
		// cout<<"even-----> "<<edge.second->pmiddle->id<<"\n";
		this->puntos.insert(pair<int, V* >(idd, edge.second->head));
		idd++;
	}

	// getPoints();
	E* e;
	E* e1;
	E* e2;
	E* e3;
	F* ncara;
	for (auto cara : mesh->caras){
        e=cara.second->edge;
        do{
            ncara=new F;
            e1=new E;
            e2=new E;
            e3=new E;
            e1->id=ide;
            e2->id=ide+1;
            e3->id=ide+2;
            e1->head=e->head;
            e2->head=e->next->pmiddle;
            e3->head=e->pmiddle;
            e1->next=e2;
            e2->next=e3;
            e3->next=e1;
            ncara->id = idf;
            ncara->edge = e1;
            caras.insert(pair<int, F* >(idf, ncara));
            idf++;
            e1->face=ncara;
            e2->face=ncara;
            e2->face=ncara;
            e1->head->edge=e2;
            e2->head->edge=e3;
            e3->head->edge=e1;
            searchTween(e1->head,e2);
            searchTween(e2->head,e3);
            searchTween(e3->head,e1);
            aristas.insert(pair<int, E* >(ide,e1));
            aristas.insert(pair<int, E* >(ide+1,e2));
            aristas.insert(pair<int, E* >(ide+2,e3));
            ide+=3;
            e=e->next;
        }while(e!=cara.second->edge);
        //cara interna
        e1=new E;
        e2=new E;
        e3=new E;
        ncara=new F;
        e1->id=ide;
        e2->id=ide+1;
        e3->id=ide+2;
        e1->head=e->pmiddle;
        e2->head=e->next->pmiddle;
        e3->head=e->next->next->pmiddle;
        e1->next=e2;
        e2->next=e3;
        e3->next=e1;
        ncara->id = idf;
        ncara->edge = e1;
        caras.insert(pair<int, F* >(idf, ncara));
        idf++;
        e1->face=ncara;
        e2->face=ncara;
        e2->face=ncara;
        e1->head->edge=e2;
        e2->head->edge=e3;
        e3->head->edge=e1;
        searchTween(e1->head,e2);
        searchTween(e2->head,e3);
        searchTween(e3->head,e1);
        aristas.insert(pair<int, E* >(ide,e1));
        aristas.insert(pair<int, E* >(ide+1,e2));
        aristas.insert(pair<int, E* >(ide+2,e3));
        ide+=3;
	}
	addVertexNeighbors();
    calculateNormals();
}
