#include "StructDefine.h"

//Solid
Face* Solid::GetFace(int idx) {
	list<Face*>::iterator it;
	int i;
	for ( it = Sfaces.begin(),i = 0; it != Sfaces.end(); it++, i++) {
		if (i == idx)
			return *it;
	}
	return nullptr;
}

Edge* Solid::GetEdge(int idx) {
	list<Edge*>::iterator it;
	int i;
	for (it = Sedges.begin(), i = 0; it != Sedges.end(); it++, i++) {
		if (i == idx)
			return *it;
	}
	return nullptr;
}

Vertex* Solid::GetVertex(int idx) {
	list<Vertex*>::iterator it;
	int i;
	for (it = Svertices.begin(), i = 0; it != Svertices.end(); it++, i++) {
		if (i == idx)
			return *it;
	}
	return nullptr;
}


//Face
Loop* Face::GetLoop(int idx) {
	list<Loop*>::iterator it;
	int i;
	for (it = Floops.begin(), i = 0; it != Floops.end(); it++, i++) {
		if (i == idx)
			return *it;
	}
	return nullptr;
}

//Loop
HalfEdge* Loop::GetHalfEdge(Vertex* startV) {
	HalfEdge* he = this->firstHalfEdge;
	do {
		if (he->Hvertex == startV)
			return he;
		he = he->next;
	} while (he != this->firstHalfEdge);
	return nullptr;
}

HalfEdge* Loop::GetHalfEdge(Vertex* startV,Vertex* endV) {
	HalfEdge* he = this->firstHalfEdge;
	do {
		if (he->Hvertex == startV&&he->next->Hvertex==endV)
			return he;
		he = he->next;
	} while (he != this->firstHalfEdge);
	return nullptr;
}

//HalfEdge
HalfEdge* HalfEdge::adjacent() {
	if (this->Hedge) {
		//return (this->Hedge->HalfA == this) ? this->Hedge->HalfA : this->Hedge->HalfB;
		if (this->Hedge->HalfA == this) {
			return this->Hedge->HalfB;
		}
		else {
			return this->Hedge->HalfA;
		}
	}
	return nullptr;
}

