#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include <map>
#include <string>

using namespace std;
template<class G>
struct Vertex
{
   typedef typename G::E Edge;
   float x;
   float y;
   float z;
   int id;
   Edge* edge;
};

template<class G>
class Edge
{
    public:
    typedef typename G::F Face;
    typedef typename G::E E;
    typedef typename G::V Vertex;
    int id;
    Face * face;
    E * twin;
    E * next;
    Vertex * head;//tail
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
    void loadObject(string);
    void getPoints();
    void drawEdges();
};

#endif // MESH_H_INCLUDED
