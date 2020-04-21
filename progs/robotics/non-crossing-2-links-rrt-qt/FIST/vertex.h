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

#ifdef EXT_APPL_SITES

#define SetExtApplVtx(V, X) \
{\
    assert(InVertexList(vert, V)); \
    vert->vertices[V].ext_appl = X; \
}


#define GetExtApplVtx(V) \
(\
    assert(InVertexList(vert, V)), \
    vert->vertices[V].ext_appl)

#endif
