#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include <map>
#include <string>
#include <math.h>
#include <vector>
#include <list>
#define PI 3.1415926535897

using namespace std;
template<class G>
struct Vertex
{
   typedef typename G::E Edge;
   typedef typename G::V V;
   float x;
   float y;
   float z;
   int id;
   Edge* edge;
   vector<V* > neighbors;
   list<Edge* > edgeVertex;
};

template <class G>
int cuadrante(const Vertex<G> &v) {
	if (v.x >= 0 && v.y >= 0)
		return 1;
	if (v.x < 0 && v.y >= 0)
		return 2;
	if (v.x < 0 && v.y < 0)
		return 3;
	if (v.x >= 0 && v.y < 0)
		return 4;
}

template<class G>
bool operator <(const Vertex<G> &v1, const Vertex<G> &v2) {
    if(cuadrante(v1)<cuadrante(v2))
        return 1;
    if(cuadrante(v1)>cuadrante(v2))
        return 0;
    return (atan(v1.y/v1.x)*180.0/PI < atan(v2.y/v2.x)*180.0/PI);
}

template<class G>
class Edge
{
    public:
    typedef typename G::F Face;
    typedef typename G::E E;
    typedef typename G::V Vertex;
    Edge(){twin=NULL;};
    int id;
    Face * face;
    E * twin;
    E * next;
    Vertex * head;//tail
    Vertex * pmiddle;//point middle
};

template<class G>
class Face
{
    public:
    typedef typename G::E Edge;
    int id;
    Edge *edge;
};

class Mesh
{
    public:
    // typedef Mesh<V,E, F> self;
    typedef Vertex<Mesh> V;
    typedef Edge<Mesh> E;
    typedef Face<Mesh> F;
    // typedef V v_;
    //typedef E E_;
    // typedef F f_;

    map<int,V* > puntos;
    map<int,E* > aristas;
    map<int,F* > caras;
    void loadObject(string);
    void getPoints();
    void drawEdges();
    void searchTween(V*,E*);
    void vertFace();
    void addVertexNeighbors();

	void loopSubdivision(Mesh*& );
};

#endif // MESH_H_INCLUDED
