#include "EulerOperation.h"
#include "StructDefine.h"

using namespace std;

void EulerOperation::mvfs(float x, float y, float z) {
	Solid* solid = new Solid();
	Face* face = new Face();
	Loop* loop = new Loop();
	HalfEdge* halfEdge = new HalfEdge();
	Vertex* vertex = new Vertex(x, y, z);

	Solids.push_back(solid);

	solid->Sfaces.push_back(face);
	face->Fsolid = solid;

	face->OutLoop = loop;
	face->Floops.push_back(loop);
	loop->Lface = face;

	loop->firstHalfEdge = halfEdge;
	halfEdge->Hloop = loop;
	halfEdge->prev = halfEdge;
	halfEdge->next = halfEdge;

	halfEdge->Hvertex = vertex;
	halfEdge->Hedge = nullptr;
	vertex->VhalfEdge = halfEdge;
	solid->Svertices.push_back(vertex);
}

HalfEdge* EulerOperation::mev(Loop* loop, Vertex* vertex1, float x, float y, float z) {
	Edge* e = new Edge();
	HalfEdge* he = nullptr;
	HalfEdge* he1 = nullptr;
	HalfEdge* he2 = new HalfEdge();
	Vertex* v = new Vertex(x, y, z);

	he = loop->firstHalfEdge;
	do {
		if (he->Hvertex == vertex1)
			break;
		he = he->next;
	} while (he != loop->firstHalfEdge);

	loop->Lface->Fsolid->Svertices.push_back(v);
	if (he->Hedge == nullptr) {
		he1 = he;
		he1->next = he2;
		he1->prev = he2;
		he2->next = he1;
		he2->prev = he1;

		he2->Hloop = loop;
		he2->Hvertex = v;
		he1->Hedge = e;
		he2->Hedge = e;
	}
	else {
		he1 = new HalfEdge();
		he1->next = he2;
		he1->prev = he->prev;
		he2->next = he;
		he2->prev = he1;
		he->prev->next = he1;
		he->prev = he2;

		he1->Hloop = loop;
		he2->Hloop = loop;
		he1->Hvertex = vertex1;
		he2->Hvertex = v;
		he1->Hedge = e;
		he2->Hedge = e;
	}
	e->HalfA = he1;
	e->HalfB = he2;
	loop->Lface->Fsolid->Sedges.push_back(e);
	return he1;

}

void EulerOperation::mef(Loop* outLoop, Vertex* vertex1, Vertex* vertex2) {
	HalfEdge* he1 = outLoop->GetHalfEdge(vertex1);
	HalfEdge* he2 = outLoop->GetHalfEdge(vertex2);
	Face* face = new Face();
	Loop* loop = new Loop();
	Edge* edge = new Edge();

	Solid* solid = outLoop->Lface->Fsolid;
	solid->Sfaces.push_back(face);
	face->Fsolid = solid;
	face->Floops.push_back(loop);
	face->OutLoop = loop;
	loop->Lface = face;

	HalfEdge* tmphe1 = new HalfEdge();
	HalfEdge* tmphe2 = new HalfEdge();
	tmphe1->prev = he1->prev;
	tmphe1->next = he2;
	tmphe2->prev = he2->prev;
	tmphe2->next = he1;
	tmphe1->Hedge = edge;
	tmphe2->Hedge = edge;
	tmphe1->Hvertex = vertex1;
	tmphe2->Hvertex = vertex2;
	tmphe1->Hloop = outLoop;
	tmphe2->Hloop = loop;

	outLoop->firstHalfEdge = tmphe1;
	loop->firstHalfEdge = tmphe2;

	HalfEdge* he = new HalfEdge();
	for (he = he1; he != he2; he = he->next)
		he->Hloop = loop;

	edge->HalfA = tmphe1;
	edge->HalfB = tmphe2;
	solid->Sedges.push_back(edge);
	he1->prev->next = tmphe1;
	he1->prev = tmphe2;
	he2->prev->next = tmphe2;
	he2->prev = tmphe1;
}

void EulerOperation::mef(Loop* outLoop, Vertex* vertex1, Vertex* vertex2, Vertex* vertex3, Vertex* vertex4) {
	HalfEdge* he1 = outLoop->GetHalfEdge(vertex1, vertex2);
	HalfEdge* he2 = outLoop->GetHalfEdge(vertex3, vertex4);

	Face* face = new Face();
	Loop* loop = new Loop();
	Edge* edge = new Edge();
	HalfEdge* tmphe1 = new HalfEdge();
	HalfEdge* tmphe2 = new HalfEdge();

	Solid* solid = outLoop->Lface->Fsolid;
	solid->Sfaces.push_back(face);
	face->Fsolid = solid;

	face->Floops.push_back(loop);
	face->OutLoop = loop;
	loop->Lface = face;

	outLoop->firstHalfEdge = tmphe1;
	loop->firstHalfEdge = tmphe2;

	HalfEdge *he = new HalfEdge();
	for (he = he1; he != he2; he = he->next) {
		he->Hloop = loop;
	}

	tmphe1->prev = he1->prev;
	tmphe1->next = he2;
	tmphe2->prev = he2->prev;
	tmphe2->next = he1;
	tmphe1->Hedge = edge;
	tmphe2->Hedge = edge;
	tmphe1->Hvertex = vertex1;
	tmphe2->Hvertex = vertex3;
	tmphe1->Hloop = outLoop;
	tmphe2->Hloop = loop;

	edge->HalfA = tmphe1;
	edge->HalfB = tmphe2;
	solid->Sedges.push_back(edge);

	he1->prev->next = tmphe1;
	he1->prev = tmphe2;
	he->prev->next = tmphe2;
	he->prev = tmphe1;
}

void EulerOperation::kemr(Loop* outLoop, Edge* edge, Vertex* vertex) {
	Face* face = outLoop->Lface;
	Loop* loop = new Loop();
	face->Floops.push_back(loop);
	loop->Lface = face;

	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	if (edge->HalfA->Hvertex == vertex) {
		he1 = edge->HalfA;
		he2 = edge->HalfB;
	}
	else {
		he1 = edge->HalfB;
		he2 = edge->HalfA;
	}

	HalfEdge* he = he1;
	int count = 0;
	while (he->next != he2) {
		count++;
		he = he->next;
		he->Hloop = loop;
	}

	if (count == 0) {
		face->Floops.pop_back();
		delete loop;
	}
	else {
		outLoop->firstHalfEdge = he1->prev;
		loop->firstHalfEdge = he1->next;
		he2->prev->next = he1->next;
		he1->next->prev = he2->prev;
	}

	he1->prev->next = he2->next;
	he2->next->prev = he1->prev;

	delete he1;
	delete he2;

	for (list<Edge*>::iterator it = face->Fsolid->Sedges.begin(); it != face->Fsolid->Sedges.end(); ++it) {
		if (*it == edge) {
			it = face->Fsolid->Sedges.erase(it);
			break;
		}
	}
	delete edge;
}

void EulerOperation::kfmhr(Face* outFace, Face* innerFace) {
	Solid* solid1 = outFace->Fsolid;
	Solid* solid2 = innerFace->Fsolid;

	if (solid1 == solid2) {
		outFace->Floops.push_back(innerFace->OutLoop);
		for (list<Face*>::iterator it = solid1->Sfaces.begin(); it != solid1->Sfaces.end(); ++it) {
			if (*it == innerFace) {
				it = solid1->Sfaces.erase(it);
				break;
			}
		}
		delete innerFace;
	}
	else {
		outFace->Floops.push_back(innerFace->OutLoop);
		for (list<Face*>::iterator it = solid2->Sfaces.begin(); it != solid2->Sfaces.end(); ++it) {
			if (*it != innerFace) {
				(*it)->Fsolid = solid1;
				solid1->Sfaces.push_back(*it);
			}
		}
		for (list<Edge*>::iterator it = solid2->Sedges.begin(); it != solid2->Sedges.end(); ++it) {
			solid1->Sedges.push_back(*it);
		}
		for (list<Vertex*>::iterator it = solid2->Svertices.begin(); it != solid2->Svertices.end(); ++it) {
			solid1->Svertices.push_back(*it);
		}
		for (list<Solid*>::iterator it = Solids.begin(); it != Solids.end(); ++it) {
			if (*it == solid2) {
				it = Solids.erase(it);
				break;
			}
		}
		delete innerFace;
		delete solid2;
	}
}

void EulerOperation::sweep(Solid* solid, Face* face, float x, float y, float z) {
	Loop* loop = face->OutLoop->firstHalfEdge->adjacent()->Hloop;
	Vertex* vertex = face->OutLoop->firstHalfEdge->adjacent()->Hvertex;
	mev(loop, vertex, vertex->vx + x, vertex->vy + y, vertex->vz + z);
	Vertex* firstnvertex = solid->Svertices.back();
	Vertex* nvertex = firstnvertex;

	HalfEdge* he = face->OutLoop->firstHalfEdge->prev;
	while (he != face->OutLoop->firstHalfEdge) {
		mev(loop, he->adjacent()->Hvertex, he->adjacent()->Hvertex->vx + x, he->adjacent()->Hvertex->vy + y, he->adjacent()->Hvertex->vz + z);
		mef(loop, nvertex, solid->Svertices.back());
		nvertex = solid->Svertices.back();
		he = he->prev;
	}

	mef(loop, nvertex, firstnvertex);

	for (Loop* out : face->Floops) {
		if (out != face->OutLoop) {
			Loop* ring = out->firstHalfEdge->adjacent()->Hloop;
			Vertex* vtx = out->firstHalfEdge->adjacent()->Hvertex;
			mev(ring, vtx, vtx->vx + x, vtx->vy + y, vtx->vz + z);
			Vertex* fnvtx = solid->Svertices.back();
			Vertex* nvtx = fnvtx;
			HalfEdge* he = out->firstHalfEdge->prev;
			while (he != out->firstHalfEdge) {
				mev(ring, he->adjacent()->Hvertex, he->adjacent()->Hvertex->vx + x, he->adjacent()->Hvertex->vy + y, he->adjacent()->Hvertex->vz + z);
				mef(ring, nvtx, solid->Svertices.back());
				nvtx = solid->Svertices.back();
				he = he->prev;
			}

			mef(ring, nvtx, fnvtx);
			kfmhr(loop->Lface, ring->Lface);
		}
	}
}