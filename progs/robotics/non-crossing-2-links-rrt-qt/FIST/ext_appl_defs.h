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

#ifndef EXT_APPL_DEFS
#define EXT_APPL_DEFS


/*                                                                           */
/* the following data types are used for user-defined entities that are to   */
/* be stored as part of my data structures. you are welcome to redefine them */
/* according to your needs. however, please note that it is your task to     */
/* make sure that the functions and macros for I/O operations are modified   */
/* accordingly in io_parse.c !! also, you'd need to redefine the constants   */
/* eas_NIL, and eat_NIL in the file ext_appl_defs.c to whatever constant     */
/* integral value or structure that can be used for initializing the         */
/* exterior application structures. see the sample use for eas_... below.    */
/*                                                                           */

#ifdef EXT_APPL_SITES
#define EXT_APPL_PNTS
typedef int   eas_type;            /* ext. appl. hook for  point and  vertex */
typedef int   eap_type;               /* needed for compatibility with VRONI */
#endif
#ifdef EXT_APPL_TRI
typedef int   eat_type;    /* ext. appl. hook for  triangle  and  quadrangle */
#endif

/*
 * sample user-specific eas data:
 *
 * typedef struct {
 *   long id;
 *   char ch[10];
 * } eas_type;
 */

#ifdef EXT_APPL_SITES
extern const eas_type  eas_NIL;
extern const eap_type  eap_NIL;
#endif
#ifdef EXT_APPL_TRI
extern const eat_type  eat_NIL;
#endif



/*                                                                           */
/* macro definitions for application-specific macros                         */
/*                                                                           */

#define  ExtApplFuncStoreTri_1

#define  ExtApplFuncStoreTri_2

#define  ExtApplFuncStoreQuad

#define  ExtApplFuncStoreVNormal

#define  ExtApplFuncStoreTVertex

#define  ExtApplFuncStoreVertex

#define  ExtApplFuncStorePnt

#define  ExtApplFuncNewInput

#define  ExtApplFuncDesperate

#define  ExtApplFuncReligious

#define  ExtApplFuncRestart

#define  ExtApplFuncDoneOneChain

#define  ExtApplFuncNextChain

#define  ExtApplFuncFinished

#define  ExtApplFuncTerminateProg

#define  ExtApplFuncResetAll

#define  ExtApplFuncNewPoly

#define  ExtApplFuncNewFace

#define  ExtApplFuncBeforeTriangulation

#define  ExtApplFuncDesperate3D

#define  ExtApplFuncReligious3D

#define  ExtApplFuncRestart3D

#define  ExtApplFuncDoneOneChain3D

#define  ExtApplFuncNextChain3D

#define  ExtApplFuncDoneOneFace

#define  ExtApplFuncDoneOneGroup

#define  ExtApplFuncFinished3D

#define  ExtApplFuncStartPoly

#define  ExtApplFuncClosePoly_1

#define  ExtApplFuncClosePoly_2

#define  ExtApplFuncAddPolyVertex_1

#define  ExtApplFuncAddPolyVertex_2

#define  ExtApplFuncAddPolyVertex_3

#define  ExtApplFuncAddPolyVertex_4

#define  ExtApplFuncFISTWarning

#define  ExtApplFuncStoreIsoPnt

#define  ExtApplFuncHandlePolyhedronStart

#define  ExtApplFuncHandlePolyhedronEnd

#define  ExtApplFuncHandlePolygonEnd

#define  ExtApplFuncHandlePolygonStart

#define  ExtApplFunc2D_PolyArrayEnd

#define  ExtApplFunc2D_PolyArrayStart

#define  ExtApplFunc3D_FaceArrayEnd

#define  ExtApplFunc3D_FaceArrayStart

#define  ExtApplFuncTerminateTriangulation

#define  ExtApplFuncFIST_HandleError


#endif /* EXT_APPL_DEFS */

