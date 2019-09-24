#include "mesh.h"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <fstream>

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
    Vertex *v;
    int idd=1,ide=1,a,b,c;
    while(getline(obj, line)){
        istringstream iss(line);
        iss>>method;
        if(method=="v"){
            v=new Vertex;
            iss>>v->x>>v->y>>v->z;
            v->id=idd;
            puntos.insert(pair<int,Vertex* >(idd,v));
            idd++;
        }
        if(method=="f"){
            iss>>a>>b>>c;
            Edge* e1=new Edge;
            Edge* e2=new Edge;
            Edge* e3=new Edge;
            e1->id=ide;
            e1->head=puntos[b];
            e1->next=e2;
            e2->id=ide+1;
            e2->head=puntos[c];
            e2->next=e3;
            e3->id=ide+2;
            e3->head=puntos[a];
            e3->next=e1;
            aristas.insert(pair<int,Edge* >(ide,e1));
            aristas.insert(pair<int,Edge* >(ide+1,e2));
            aristas.insert(pair<int,Edge* >(ide+2,e3));
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
void Mesh::getEdges(){
    for(auto val:aristas)
        cout<<val.first<<" "<<val.second->next->id<<endl;
}
