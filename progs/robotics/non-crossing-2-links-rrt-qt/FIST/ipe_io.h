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

#ifndef DOUBLE_OVERRIDE
#define machine_double double
#define double_global double
#endif

#include "defs.h"

FILE *InitIpe(ipe_iodef *ipeio, const char *file_name,
              machine_double xl, machine_double xr, 
              machine_double yl, machine_double yr);

void CloseIpeFile(FILE *ipe_file);

void WriteBeginGroup(FILE *ipe_file);

void WriteEndGroup(FILE *ipe_file);

void WriteEnd(FILE *ipe_file);

void WriteLineBegin(FILE *ipe_file, int dashed, machine_double width, 
                    int no_vertices);

void WriteSegmentBegin(FILE *ipe_file, int dashed, machine_double width, 
                        int no_vertices);

void WriteArcBegin(FILE *ipe_file, int dashed, machine_double width);

void WriteObjStroke(FILE *ipe_file, 
                    machine_double r, machine_double g, machine_double b);

void WriteObjFill(FILE *ipe_file, 
                  machine_double r, machine_double g, machine_double b);

void WriteArcCCW(ipe_iodef *ipeio, FILE *ipe_file, machine_double xc, machine_double yc,
                 machine_double r, 
                 machine_double alpha, machine_double beta);

void WriteArcCW(ipe_iodef *ipeio, FILE *ipe_file, machine_double xc, machine_double yc,
                machine_double r, 
                machine_double alpha, machine_double beta);

void WriteMark(ipe_iodef *ipeio, FILE *ipe_file,
               machine_double r, machine_double g, machine_double b, int type, 
               machine_double size, machine_double x, machine_double y);

void WriteLineFirstPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteLineNextPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSegmentFirstPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSegmentNextPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSegmentEndPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSegmentClosePnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSplineBegin(FILE *ipe_file, int dashed, machine_double width, int
                      no_vertices); 

void WriteSplineFirstPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSplineLastPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSplineSecondPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void WriteSplineNextPnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void SetIpeDimensions(ipe_iodef *ipeio,
		              machine_double xmin, machine_double xmax,
                      machine_double ymin, machine_double ymax);

void SetWorldDimensions(ipe_iodef *ipeio,
		                machine_double xmin, machine_double xmax,
                        machine_double ymin, machine_double ymax);

void SetScaleFactor(ipe_iodef *ipeio);

void WritePnt(ipe_iodef *ipeio, FILE *ipe_file, machine_double x, machine_double y);

void InitIpeDimensions(ipe_iodef *ipeio,
		               machine_double xmin, machine_double ymin,
                       machine_double xmax, machine_double ymax,
                       machine_double ixmin, machine_double iymin, 
                       machine_double ixmax, machine_double iymax);

void WriteLineSegment(ipe_iodef *ipeio, FILE *ipe_file,
                      machine_double r, machine_double g, machine_double b, 
                      machine_double width, int dashed, 
                      machine_double x1, machine_double y1, 
                      machine_double x2, machine_double y2);

void WriteCircularArc(ipe_iodef *ipeio, FILE *ipe_file,
                      machine_double r, machine_double g, machine_double b, 
                      machine_double width, int dashed, 
                      machine_double xc, machine_double yc, 
                      machine_double x1, machine_double y1, 
                      machine_double x2, machine_double y2, boolean ccw);

void WriteDiscretizedCircularArc(ipe_iodef *ipeio, FILE *ipe_file,
                                 machine_double xc, machine_double yc, 
                                 machine_double x1, machine_double y1, 
                                 machine_double x2, machine_double y2, 
                                 boolean ccw, machine_double delta, 
                                 int *num_segs);
