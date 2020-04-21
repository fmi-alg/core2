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


void WriteFileMagic(FILE *fp);

void WriteFileEnd(FILE *outf);

void WriteObjType(FILE *outf, int num_tri);

void WriteSgiTri(FILE *fp, float sgi_tri[3][3]);
