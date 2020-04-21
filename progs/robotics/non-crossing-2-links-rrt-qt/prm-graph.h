/*
 * prm-graph.h
 *
 *  Author: Chee Yap, Ching-Hsiang (Tom) Hsu
 *  Email: chhsu@nyu.edu
 *  Modified: Oct, 31, 2016
 */

#ifndef _SH_GRAPH_H_
#define _SH_GRAPH_H_

#ifndef _STL_PORT
#define _STL_PORT
#endif
#include "graph/Graph.h"
using namespace graph;

#include <mathtool/Point.h>
using namespace mathtool;

///////////////////////////////////////////////////////////////////////////////
class CNode 
{
    friend ostream & operator<<( ostream & out, const CNode & node );
    friend istream & operator>>( istream &  in, CNode & node );
public:
    ///////////////////////////////////////////////////////////////////////
    //  Constructor/Destructor
    CNode();
    ~CNode();

    bool operator==( const CNode & other ) const;

    //////////////////////////////////////////////////////////////////////
    //      Access Method
    static CNode & InvalidData(){return m_InvalidValue;}
    void setPos(Point2d & pos) { m_Pos=pos; }
    const Point2d & getPos() const { return m_Pos; }
    void setClearance(float clear) { m_Clear=clear; }
    float getClearance() const { return m_Clear; }
    void setID(VID id) { m_ID=id; }
    VID getID() const { return m_ID; }

//////////////////////////////////////////////////////////////////////
//      Private Method & Data
private:
    static CNode m_InvalidValue;
    VID m_ID;
    Point2d m_Pos;
    float m_Clear; //clearance
};

///////////////////////////////////////////////////////////////////////////////
class CEdge 
{
public:
    friend ostream & operator<<( ostream & out, const CEdge & node );
    friend istream & operator>>( istream &  in, CEdge & node );
public:
    ///////////////////////////////////////////////////////////////////////
    //  Constructor/Destructor
    CEdge();
    CEdge(float weight);
    ~CEdge();

    bool operator==( const CEdge & other ) const;

    //////////////////////////////////////////////////////////////////////
    //      Access Method
    static CEdge & InvalidWeight(){return m_InvalidValue;}
    float Weight(){ return m_Weight; }
    static float MaxWeight(){ return 1e10; }

//////////////////////////////////////////////////////////////////////
//      Private Method & Data
private:
    static CEdge m_InvalidValue;
    float m_Weight;
};

ostream & operator<<( ostream & out, const CNode & node );
istream & operator>>( istream &  in, CNode & node );
ostream & operator<<( ostream & out, const CEdge & node );
istream & operator>>( istream &  in, CEdge & node );

///////////////////////////////////////////////////////////////////////////////
//weighted directed graph
typedef Graph<DG<CNode,CEdge>,NMG<CNode,CEdge>,WG<CNode,CEdge>,CNode,CEdge> WDG;

#endif //_SH_GRAPH_H_

