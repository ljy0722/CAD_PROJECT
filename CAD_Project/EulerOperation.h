#pragma once
#include "StructDefine.h"

class EulerOperation
{
public:
	EulerOperation(){}

	list<Solid*> Solids;

	void mvfs(float x, float y, float z);
	HalfEdge* mev(Loop* loop, Vertex* vertex1, float x, float y, float z);
	//2situations of mef
	void mef(Loop* outLoop, Vertex* vertex1, Vertex* vertex2);
	void mef(Loop* outLoop, Vertex* vertex1, Vertex* vertex2, Vertex* vertex3, Vertex* vertex4);
	void kemr(Loop* outLoop, Edge* edge, Vertex* vertex);
	void kfmhr(Face* outFace, Face* innerFace);

	void sweep(Solid* solid, Face* face, float x, float y, float z);
};