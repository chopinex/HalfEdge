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
   float normal[3];
   vector<V* > neighbors;
   list<Edge* > edgeVertex;
   Edge* edge;
};


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
    Vertex * middle;
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

    typedef Vertex<Mesh> V;
    typedef Edge<Mesh> E;
    typedef Face<Mesh> F;


    map<int,V* > puntos;
    map<int,E* > aristas;
    map<int,F* > caras;
	map<int, float* > normales;
    void loadObject(string);
    void getPoints();
    void drawEdges();
    void searchTween(V*,E*);
    void vertFace();
    void addVertexNeighbors();
    //	void loopSubdivision(Mesh*& );
    void showTweens();
};

#endif // MESH_H_INCLUDED
