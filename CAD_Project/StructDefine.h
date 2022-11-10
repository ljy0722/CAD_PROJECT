#pragma once
#include <iostream>
#include <list>

using namespace std;

class Solid;
class Face;
class Loop;
class Edge;
class HalfEdge;
class Vertex;

class Solid
{
public:
	Solid() {};
	Face* GetFace(int idx);
	Edge* GetEdge(int idx);
	Vertex* GetVertex(int idx);

	list<Face*> Sfaces;
	list<Edge*> Sedges;
	list<Vertex*> Svertices;
};

class Face
{
public:
	Face() {};
	Loop* GetLoop(int idx);

	list<Loop*> Floops;
	Loop* OutLoop = nullptr;
	Solid* Fsolid = nullptr;
};

class Loop
{
public:
	Loop() {};
	HalfEdge* GetHalfEdge(Vertex* startV);
	HalfEdge* GetHalfEdge(Vertex* startV, Vertex* endV);

	HalfEdge* firstHalfEdge = nullptr;
	Face* Lface = nullptr;
};

class Edge
{
public:
	Edge() {};

	HalfEdge* HalfA = nullptr;
	HalfEdge* HalfB = nullptr;
};

class HalfEdge
{
public:
	HalfEdge() {};
	HalfEdge* adjacent();

	HalfEdge* prev = nullptr;
	HalfEdge* next = nullptr;
	Vertex* Hvertex = nullptr;
	Edge* Hedge = nullptr;
	Loop* Hloop = nullptr;
};

class Vertex
{
public:
	Vertex() {};
	Vertex(float x, float y, float z) :vx(x), vy(y), vz(z) {};

	float vx;
	float vy;
	float vz;
	HalfEdge* VhalfEdge = nullptr;
};


