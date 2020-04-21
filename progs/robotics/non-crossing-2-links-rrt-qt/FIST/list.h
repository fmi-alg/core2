/*****************************************************************************/
/*                                                                           */
/*           F I S T :  Fast, Industrial-Strength Triangulation              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* (C)                            Martin Held                                */
/* (C)            Universitaet Salzburg, Salzburg, Austria                   */
/*                                                                           */
/* This code is not in the public domain. All rights reserved! Please make   */
/* sure to read the full copyright statement contained in api_functions.cpp. */
/*                                                                           */
/*****************************************************************************/

/*                                                                           */
/* obtains the interior angle associated with IND                            */
/*                                                                           */
#define GetAngle(LIST,IND) \
   (\
    assert(InPolyList(LIST,IND)),                    \
    LIST->list[IND].convex)


#define SetAngle(LIST,IND, ANGLE ) \
   {\
      assert(InPolyList(LIST,IND));   \
      LIST->list[IND].convex = ANGLE ; \
   }


/*                                                                           */
/* obtains the vertex index associated with IND                              */
/*                                                                           */
#define GetIndex(LIST,IND) \
   (\
    assert(InPolyList(LIST,IND)), \
    LIST->list[IND].index)


/*                                                                           */
/* returns pointer to the successor of IND in the circular list of nodes     */
/*                                                                           */
#define GetNextNode(LIST,IND) \
   (\
    assert(InPolyList(LIST,IND)), \
    LIST->list[IND].next)


/*                                                                           */
/* returns pointer to the predecessor of IND in the circular list of nodes   */
/*                                                                           */
#define GetPrevNode(LIST,IND) \
   (\
    assert(InPolyList(LIST,IND)), \
    LIST->list[IND].prev)



#ifdef STAGES
#define SetStage(LIST,IND, S) \
   {\
      assert(InPolyList(LIST,IND)); \
      LIST->list[IND].stage = S; \
   }


#define GetStage(LIST,IND) \
   (\
   assert(InPolyList(LIST,IND)), \
   LIST->list[IND].stage)
#endif
