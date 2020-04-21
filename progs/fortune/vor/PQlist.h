
//	PQlist
class PQlistRep; 	//forward reference

class PQlist {
public:
	PQlistRep* r;

	PQlist() {r = (PQlistRep *)NULL; }
	PQlist(Halfedge& he, PQlist& next);
	PQlist(PQlist& p) { r = p.r;  }
	~PQlist() { }

	void operator = (PQlist& p) { r = p.r; }

	int isNull() { return (r == (PQlistRep *)NULL); }
	int notNull() { return (r != (PQlistRep *)NULL); }

	void PQinsert(Halfedge& he);
	void PQdelete(Halfedge& he);
	Halfedge extractmin();
	PQlist PQnext();
	Halfedge he();
};

class PQlistRep {
public:
	Halfedge he;
	PQlist PQnext;

	PQlistRep(Halfedge& he_in, PQlist next)
		: he(he_in), PQnext(next) {}
};

