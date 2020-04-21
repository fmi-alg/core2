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

/* api_functions.cpp: */

#include "martin.h"
#include "defs.h"

boolean FIST_HandlePolyhedron(rt_options *rt_opt);

boolean FIST_HandlePolygon(rt_options *rt_opt);

boolean FIST_2D_PolyArray(int num_contours, int num_vertices[],
                            double (*input_vtx)[2], int *num_triangles,
                            int (*output_tri)[3]);

boolean FIST_3D_FaceArray(int num_contours, int num_vtx[], 
                          double (*input_vtx)[3], int *num_tri, 
                          int (*output_tri)[3]);

void FIST_TerminateProgram(global_struct *all);

void FIST_HandleError(errordef FistErrorCode);

const char* FIST_GetProgName(); 

const char* FIST_GetProgVersion(); 

const char* FIST_GetProgYear();

