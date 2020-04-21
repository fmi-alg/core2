/* **************************************
 File: QuadTree.h

 Description:

 The main routines here are:
 expand()  -- this keeps expanding mixed boxes
 until the priority queue is empty
 or a path is found

 HISTORY: March, 2012: Cong Wang, Chee Yap and Yi-Jen Chiang

 Since Core Library  Version 2.1
 $Id: $
 ************************************** */

#pragma once
#include <iostream>
#include "Box.h"
#include "UnionFind.h"
#include "PriorityQueue.h"

//#ifdef __CYGWIN32__
//#include "GL/glui.h"
//#endif
//#ifdef __linux__
//#include <GL/glut.h>
//#include "GL/glui.h"
//#endif
//#ifdef _WIN32
//#include <gl/glui.h>
//#endif
//#ifdef __APPLE__
//#include <GLUI/glui.h>
//#endif

extern vector<Box*> allLeaf;
extern vector<Set*> allSet;

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;

//extern void glutPostRedisplay(void);

enum Status {
    FREE, STUCK, MIXED, UNKNOWN
};

/* We defined this in Box.h (Box::NON, etc)
enum Order {
    NON, ALL, LT, GT
};
*/

class QuadTree {
private:
    void insertNode(Box* b) {
        switch (b->getStatus()) {
        case Box::FREE: {
            if (b->pSet == 0) {
                Set* st = new Set(b);
                allSet.push_back(st);
            }

//			cout << "QuadTree.h-58\n";
            unionAdjacent(b);
//			cout << "QuadTree.h-60\n";
            ++freeCount;
        }
            break;
        case Box::STUCK:
            ++stuckCount;
            break;
        case Box::MIXED:
            ++mixCount;
            if (b->height / 2 < epsilon || b->width / 2 < epsilon)
                ++mixSmallCount;
            PQ->push(b);
            break;
        case Box::UNKNOWN:
            std::cout << "UNKNOWN not handled?" << std::endl;
        }
    }

public:
    UnionFind* pSets;
    BoxQueue* PQ;
    Box* pRoot;
    double epsilon;
    int QType;

    QuadTree(Box* root, double e, int qType) :
            pRoot(root), epsilon(e), QType(qType) {
        switch (QType) {
        case 1:
            PQ = new seqQueue();
            break;
        case 0:
            PQ = new randQueue();
            break;
        case 2:
            PQ = new dijkstraQueue<DistCmp>();
            break;
        case 3:
            PQ = new dijkstraQueue<DistPlusSizeCmp>();
            break;
        case 4:
            PQ = new dijkstraQueue<VorCmp>();
            break;
        }

        //PQ = new randQueue();

        pRoot->updateStatus();
        insertNode(pRoot);
    }

    //bfs search for alpha/beta return the box which has the specific configuration
    Box* getBox(double x, double y, double a1, double a2, int& ct) {
        std::queue<Box*> q;
//		std::cout << "getBox 113" << endl;
        Box::Order tempOrder;
        if (a1 > a2) {
            tempOrder = Box::GT;
        } else {
            tempOrder = Box::LT;
        }
        for (int i = 0; i < (int) allLeaf.size(); ++i) {
            if (allLeaf[i]->contains(x, y, a1, a2)) {
                //TODO added by Zhongdi 7/22/2013
                if (allLeaf[i]->isFree()) {
                    if (crossingOption) {
                        if (allLeaf[i]->order == Box::ALL
                                || allLeaf[i]->order == tempOrder) {
                            return allLeaf[i];
                        } else {
                            continue;
                        }
                    } else {
                        return allLeaf[i];
                    }

                }
                if (allLeaf[i]->status == Box::STUCK) {
                    std::cout << "getBox 121" << endl;
                    return 0;
                }
                q.push(allLeaf[i]);
            }
        }
//		std::cout << "getBox 127" << endl;

        while (q.size()) {
            Box* b = q.front();

            q.pop();

            //TODO could be deleted
            if (!b->isLeaf) {
//				cout << "This is Wrong" << endl;
//				cout << "getbox 134: x = " << b->x << " y = " << b->y
//						<< " width = " << b->width << endl;
//				std::cout << "getBox 139" << endl;
                continue;
            }

            vector<Box*> cldrn;
//			cout << "QuadTree.h-144\n";
            if (!expand(b, cldrn)) {
//				std::cout<<"getBox 142"<< endl;
//				return 0;
//				std::cout << "getBox 147" << endl;
                // Since the smarter strategy added, only knowing return code true/false is not enough to determine next move
                if (!b->shouldSplit2D) {
                    continue;
                }
            }
//			cout << "QuadTree.h-154\n";

//			std::cout << "getBox 149" << endl;
            if (b->shouldSplit2D) {
                q.push(b);
//				std::cout << q.size() << endl;
//				if (q.size() > 0) {
//					std::cout << q.front()->x << " " << q.front()->y << endl;
//				}
                continue;
            }
            ++ct;

            for (int i = 0; i < (int) cldrn.size(); ++i) {
                if (cldrn[i]->contains(x, y, a1, a2)) {
                    if (cldrn[i]->isFree()) {
//						if(b->shouldSplit2D){
//							q.push(b);
//							break;
//						}
//						std::cout<<"getBox 153"<< endl;
                        if (crossingOption) {
                            if (cldrn[i]->order == Box::ALL
                                    || cldrn[i]->order == tempOrder) {
                                return cldrn[i];
                            } else {
                                continue;
                            }
                        } else {
                            return cldrn[i];
                        }

                    } else if (cldrn[i]->status == Box::STUCK) {
//						if (b->shouldSplit2D) {
//							std::cout << q.size() << endl;
//							q.push(b);
//							std::cout << "after push stuck" << q.size() << endl;
//							break;
//						} else {
                        return 0;
//						}
                    } else {
//						if (b->shouldSplit2D) {
//							std::cout << q.size() << endl;
//							q.push(b);
//							std::cout << "after push mixed" << q.size() << endl;
//							break;
//						} else {
                        q.push(cldrn[i]);
//						}

                    }
                }
            }
//			std::cout << "getBox 188" << endl;

        }
//		std::cout<<"getBox 160"<< endl;
        return 0;
    }

    bool expand(Box* b) {
        vector<Box*> cldrn;
        return expand(b, cldrn);
    }

    //expand and put children ptr in cldrn
    bool expand(Box* b, vector<Box*>& cldrn) {
        if (!b->split(epsilon, cldrn)) {
//			std::cout<<"expand 207"<<endl;
            return false;
        }

        for (int i = 0; i < (int) cldrn.size(); ++i) {
            //TODO determine whether it is fine to delete this
//			cldrn[i]->updateStatus();
//			cout << "QuadTree.h-220\n";
            insertNode(cldrn[i]);
//			cout << "QuadTree.h-222\n";
        }

        return true;
    }

    bool expand() {
        while (!PQ->empty()) {
            Box* b = PQ->extract();
            if (!b->isLeaf) {
                continue;
            }

            vector<Box*> cldrn;

            //b might not be a leaf since it could already be split in expand(Box* b), and PQ is not updated there
            if (b->split(epsilon, cldrn)) {
//				cout<< "expand() 231"<<endl;
                assert(b->status == Box::MIXED);

                for (int i = 0; i < (int) cldrn.size(); ++i) {

//					cldrn[i]->updateStatus();
                    insertNode(cldrn[i]);
                }
                return true;
            }
        }
        return false;
    }

    bool isConnected(Box* a, Box* b) {
        if (pSets->Find(a) == pSets->Find(b)) {
            return true;
        }
        return false;
    }

    void unionAdjacent(Box* b) {
        for (int i = 0; i < 5; ++i) {
            for (vector<Box*>::iterator it = b->Nhbrs[i].begin();
                    it != b->Nhbrs[i].end(); ++it) {
                Box* neighbor = *it;
                if (neighbor->status == Box::FREE) {
//					cout << "QuadTree.h-268\n";
//					cout << i << "\n";
//					cout << b->xi[0] << " " << b->xi[1] << " " << b->xi[2]
//							<< " " << b->xi[3] << "\n";
//					cout << neighbor->xi[0] << " " << neighbor->xi[1] << " "
//							<< neighbor->xi[2] << " " << neighbor->xi[3]
//							<< "\n";
                    if (neighbor->pSet != 0) {
                        pSets->Union(b, neighbor);
                    }
//					else {
//						Set* st = new Set(neighbor);
//						allSet.push_back(st);
//						pSets->Union(b, neighbor);
//					}

//					cout << "QuadTree.h-273\n";
                }
            }
        }
    }

    ~QuadTree(void) {
    }
};
