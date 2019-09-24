#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include <map>
#include <string>

using namespace std;
template<class G>
struct Vertex
{
   typedef typename G::Edge Edge;
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
    typedef typename G::Face Face;
    //typedef typename G::E_ E;
    typedef typename G::Vertex V;
    int id;
    Face * face;
    Edge * twin;
    Edge * next;
    V * head;//tail
};

template<class G>
class Face
{
    public:
    typedef typename G::Edge Edge;
    int id;
    Edge *edge;
};

class Mesh
{
    public:
    // typedef Mesh<V,E, F> self;
    typedef Vertex<Mesh> Vertex;
    typedef Edge<Mesh> Edge;
    typedef Face<Mesh> Face;
    // typedef V v_;
    //typedef E E_;
    // typedef F f_;

    map<int,Vertex* > puntos;
    map<int,Edge* > aristas;
    void loadObject(string);
    void getPoints();
    void getEdges();
};

#endif // MESH_H_INCLUDED
