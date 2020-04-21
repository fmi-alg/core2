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

#define SetExtApplPnt(DATA, P, X) \
{\
    assert(InPointsList(DATA,P)); \
    DATA->points[P].ext_appl = X; \
}


#define GetExtApplPnt(DATA, P) \
(\
    assert(InPointsList(DATA, P)), \
    DATA->points[P].ext_appl)
