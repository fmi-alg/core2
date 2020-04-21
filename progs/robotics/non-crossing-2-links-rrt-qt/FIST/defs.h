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

#ifndef FIST_DEFS_H
#define FIST_DEFS_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "ext_appl_defs.h"

#define PROG_NAME    "F I S T"
#define PROG_VERSION "6.4"
#define PROG_YEAR    "1997-2016"

#define NIL  -1

#ifndef DOUBLE_OVERRIDE
#define machine_double double
#define double_global double
#endif

#define machine_long long
#define NEW_TRIANGLE_MIN     5

typedef enum {SEG,   /* (input) line segment                                 */
              ARC,   /* (input) circular arc                                 */
              PNT,   /* (input) point/vertex                                 */
              VDN,   /* node of Voronoi diagram                              */
              VDE,   /* edge of Voronoi diagram                              */
              DTE,   /* edge of Delaunay triangulation                       */
              CCW,   /* ccw offset arc                                       */
              CW,    /* cw offset arc                                        */
              UNKNOWN} t_site;

typedef struct {
   int imin;           /* lexicographically smallest point, determines min-x */
   int imax;           /* lexicographically largest point, determines max-x  */
   double ymin;        /* minimum y-coordinate                               */
   double ymax;        /* maximum y-coordinate                               */
} bounding_box;        /* bounding box                                       */

typedef struct {
   int imin;           /* lexicographically smallest point, determines min-x */
   int imax;           /* lexicographically largest point, determines max-x  */
   double xmin;        /* minimum x-coordinate                               */
   double xmax;        /* maximum x-coordinate                               */
   double ymin;        /* minimum y-coordinate                               */
   double ymax;        /* maximum y-coordinate                               */
} bounding_box_extended;

typedef struct {
   double x;
   double y;
   double z;
#ifdef EXT_APPL_SITES
   eas_type ext_appl;  /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} vertex;                                  

typedef struct {
   double x;
   double y;
#ifdef EXT_APPL_SITES
   eas_type ext_appl;  /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} point;

/* This struct is needed for the PntPntDistSqd-Macro.                        */
typedef struct {
   machine_double x;
   machine_double y;
} md_point;

typedef int list_ind;

typedef struct {
   int v1;
   int v2;
   int v3;
   int color;
#ifdef EXT_APPL_TRI
   eat_type ext_appl;  /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
#ifdef PARTITION_FIST
   list_ind ind1, ind2, ind3;
   boolean disabled;
#endif
} triangle;

#ifdef NORMALS
typedef struct {
   int i1;
   int i2;
   int i3;
} i_triangle;
#endif

typedef struct {
   int v1;
   int v2;
   int v3;
   int v4;
#ifdef EXT_APPL_TRI
   eat_type ext_appl;  /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} quadrangle;

typedef struct {
   list_ind ind;
   boolean ccw;
} loop_list;


typedef struct {
   list_ind ind;
   machine_double dist;
} distance_;

typedef struct {
   int index;
   list_ind prev;
   list_ind next;
   int convex;
   int original;
   boolean bridge;
#ifdef NORMALS
   int t_index;
   int n_index;
#endif
#ifdef STAGES
   int stage;
#endif
#ifdef PARTITION_FIST
   boolean delete_reflex;
#endif
} list_node;

typedef struct {
   boolean color_graphics;
   boolean graphics; 
   int step_size; 
   char input_file[256]; 
   boolean read_input; 
   boolean read_poly;
   boolean read_lines; 
   boolean read_obj; 
   boolean read_dxf; 
   boolean save_poly; 
   char output_file[256];
   boolean write_dxf;
   boolean verbose; 
   boolean quiet;
   boolean scale_data; 
   boolean ears_sorted; 
   boolean ears_random; 
   boolean ears_fancy; 
   boolean ears_top; 
   boolean do_quads; 
   boolean keep_quads;
   boolean keep_convex;
   boolean write_geom; 
   boolean write_tri; 
   char tri_file[256];
   char ipe_file[256];
   boolean write_ipe; 
   boolean write_ipe7;
   boolean c2p;             /* convert to triangle's .poly format */
   char c2p_file[256];
   boolean help;
   boolean time; 
   boolean statistics; 
   boolean copy; 
   boolean use_colors; 
   boolean draw_concave; 
   boolean sgi_output; 
   boolean draw_groups; 
   boolean full_screen;
   int inputprec; 
   int mpfr_prec;
} rt_options;


/* needed by memory.x moved globals to this struct                      *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */

#define MAX_MEMORY_CURSOR 1024

typedef struct {
   void *array_ptr;
   size_t size;
   unsigned int high;
} memory_dbg;

typedef struct {
   unsigned long curr_memory;
   unsigned long max_memory;
   memory_dbg memory_history[MAX_MEMORY_CURSOR];
   int memory_dbg_cursor;
} debug_memdef;

/* needed by list.x moved globals to this struct listdef                *
 * call InitListDefaults(*list) to get the default initialization       */
typedef struct {
	list_node *list;
	int num_list;
	int max_num_list;

	list_ind *loops;
	int num_loops;
	int max_num_loops;

	int *faces;
	int num_faces;
	int max_num_faces;

	list_ind first_node;

	list_ind *chains;
	int num_chains;
	int max_num_chains;
   
	debug_memdef *memptr;
} listdef;


/* needed by bridge.x moved globals to this struct                      *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
   list_ind ind;
   int index;
} left_;

typedef struct {
    distance_ *distances;
	int max_num_dist;

    left_ *left_most;
	int max_num_left_most;

	debug_memdef *memptr;
} bridgedef;


/* needed by vertex.x moved globals to this struct                      *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
#ifdef NORMALS
	vertex *t_vertices;
	int num_t_vertices;
	int max_num_t_vertices;

	vertex *v_normals;
	int num_v_normals;
	int max_num_v_normals;

	i_triangle *i_triangles;
	int num_i_triangles;
	int max_num_i_triangles;
#endif

	vertex *vertices;
	int num_vertices;
	int max_num_vertices;

	triangle *triangles;
	int num_triangles;
	int max_num_triangles;

	quadrangle *quads;
	int num_quads;
	int max_num_quads;

	int *groups;
	int num_groups;
	int max_num_groups;
	int num_group_tris;
	int *group_tris;
	int *group_quads;
	int *group_loops;

	loop_list *convex_loops;
	int num_convex_loops;
	int num_concave_loops;
	int max_num_convex_loops;

	debug_memdef *memptr;
} vertexdef;

/* needed by data.x moved globals to this struct                        *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
	point *points;
	int num_pnts;
	int max_num_pnts;

	boolean data_scaled;
	double scale_factor;
	point shift;
	machine_double bbox_diagonal_sqd;

	debug_memdef *memptr;
} datadef;

/* needed by clean_data.x moved globals to this struct                  *
 * call InitXXXXDef(*ptr) to get the default initialization             */
typedef struct {
   point p;
   list_ind ind;
} sort_record;

typedef struct {
   sort_record  *p_unsorted;
   int max_num_p_unsorted;
   debug_memdef *memptr;
} cleandef;

/***************************** grid.x structs ***************************/
typedef int seg_ind;
typedef int pnt_ind;

typedef struct {
   int imin;           /* lexicographically smaller point, determines min-x  */
   int imax;           /* lexicographically larger point, determines max-x   */
   double ymin;        /* minimum y-coordinate                               */
   double ymax;        /* maximum y-coordinate                               */
   boolean checked;    /* has this segment already been checked?             */
} segment;             /* boundary segment  i1, i2,  with  i1 < i2           */

typedef struct {
   int seg;
   seg_ind next;
} segment_node;

typedef struct {
   int pnt;
   pnt_ind next;
} pnt_node;

typedef struct {
	int ident_cntr;
#ifdef STATISTICS
	unsigned machine_long pnt_tri_cntr;
#endif
	boolean no_grid_yet;
	seg_ind *grid;
	int max_num_grid;
	pnt_ind *grid2;
	int max_num_grid2;
	pnt_ind *grid3;
	int max_num_grid3;
	int N, N_x, N_y;
	int N2, N_x2, N_y2;
	int N3, N_x3, N_y3;
	double_global grid_x, grid_y;
	double_global grid_min_x, grid_min_y, grid_max_x, grid_max_y;
	double *raster_x;
	double *raster_y;
	double_global grid_x2, grid_y2;
	double_global grid_min_x2, grid_min_y2, grid_max_x2, grid_max_y2;
	double *raster_x2;
	double *raster_y2;
	double_global grid_x3, grid_y3;
	double_global grid_min_x3, grid_min_y3, grid_max_x3, grid_max_y3;
	double *raster_x3;
	double *raster_y3;
	int max_num_raster_x;
	int max_num_raster_y;
	int max_num_raster_x2;
	int max_num_raster_y2;
	int max_num_raster_x3;
	int max_num_raster_y3;
	segment *segments;
	int num_segments;
	int max_num_segments;
	segment_node *seg_list;
	int num_seg_list;
	int max_num_seg_list;
	pnt_node *pnt_list;
	int num_pnt_list;
	int max_num_pnt_list;
	pnt_node *vtx_list;
	int num_vtx_list;
	int max_num_vtx_list;
	int *set;
	int max_num_set;
	int num_set;
	double_global step_x, step_y;
	double_global delta_x, delta_y;
	int num_reflex;
	int num_original_reflex;
	boolean buckets_initialized;

	debug_memdef *memptr;
} griddef;
/************************* END grid.x structs ***************************/

typedef struct {
	segment *seg_addr;
	int seg_i;
	seg_ind ind_seg;
} gridtmp;

/* needed by quads.x moved globals to this struct                       *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
   int v1;
   int v2;
   int tri;
} edge;

typedef struct {
   int v1;
   int v2;
   int tri;
   int next;
} hash_table;

typedef struct {
   int num_grouped;
   int *heads;
   int max_num_heads;
   hash_table *table;
   hash_table *element;
   int num_table;
   int max_num_table;
} quaddef;

typedef struct {
   machine_double x;
   machine_double y;
} coord;


/* needed by io_basic.x moved globals to this struct                       *
 * call InitXXXXDefaults(*ptr) to get the default initialization           */
typedef struct {
   int first_pnt, last_pnt, new_pnts, curr_loop, poly_vertex_number;
   list_ind last_ind;
} iolistdef;


/* needed by io_3D.x moved globals to this struct                       *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
   int r;
   int g;
   int b;
} rgb;

typedef struct {
	boolean draw_concave;
	boolean sgi_output;
	boolean draw_groups;

	rgb color;
#ifdef NORMALS
	boolean tex_norm_data_exists;
	boolean vertex_normals;
	boolean texture_vertices;
#endif
} io_3ddef;


/* needed by redraw.x moved globals to this struct                      *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
   int index;
   int color;
} pnt_buffer;

typedef struct {
   int index1;
   int index2;
   int color;
} edge_buffer;

typedef struct {
   int index1;
   int index2;
   int index3;
   int color1;
   int color2;
} tri_buffer;

typedef struct {
	pnt_buffer  *pnt_buf;
	edge_buffer *edge_buf;
	tri_buffer  *tri_buf;

	int num_pnt_buf, max_num_pnt_buf;
	int num_edge_buf, max_num_edge_buf;
	int num_tri_buf, max_num_tri_buf;

	debug_memdef *memptr;
} redrawdef;


/* needed by ipe_io.x moved globals to this struct                      *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
	machine_double scale;  
	machine_double ipe_x_min, ipe_x_max, ipe_y_min, ipe_y_max;
	machine_double x_min, x_max, y_min, y_max;
   
   boolean ipe7;
} ipe_iodef;


/* needed by predicates.x moved globals to this struct                  */
#ifdef JRC_PREDICATE
typedef struct {
	          /* = 2^ceiling(p / 2) + 1.  Used to split floats in half. */
	double splitter;
	double epsilon;     /* = 2^(-p).  Used to estimate roundoff errors. */
	/* A set of coefficients used to calculate maximum roundoff errors. */
	double resulterrbound;
	double ccwerrboundA, ccwerrboundB, ccwerrboundC;
	double o3derrboundA, o3derrboundB, o3derrboundC;
	double iccerrboundA, iccerrboundB, iccerrboundC;
	double isperrboundA, isperrboundB, isperrboundC;
} predicatesdef;
#endif


/* needed by heap.x moved globals to this struct                        *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct hnode {
   machine_double ratio;
   list_ind index;
   list_ind prev;
   list_ind next;
} heap_node;

typedef struct {
	boolean ears_sorted;
	boolean ears_random;

	heap_node *heap;
	int num_heap;
	int max_num_heap;
	int num_zero;

	boolean deleted;
	boolean not_updated;
	boolean sorted;
	boolean ear_right;
   
#ifdef PARTITION_FIST
   int heap_idx;
#endif
   debug_memdef *memptr;
} heapdef;

/* needed by ear_clip.x moved globals to this struct                    *
 * call InitXXXXDefaults(*ptr) to get the default initialization        */
typedef struct {
	boolean ears_fancy;
	boolean ears_top;
	boolean use_colors;
	int tri_color; /* 0 ... output triangle equals input triangle   */
					   /* 1 ... output triangle belongs to input quad   */
					   /* 2 ... output triangle belongs to convex face  */
					   /* 3 ... output triangle belongs to concave face */
	boolean is_convex_polygon;
	boolean ears_may_have_changed;
} eardef;

/* needed by macros in numerics.h                                       */
typedef struct {
	point numerics_h_p, numerics_h_q, numerics_h_r;
	double numerics_h_det, numerics_h_dot;
	int numerics_h_ori1, numerics_h_ori2;
} tmp_data_def;

typedef struct {
	pnt_ind ind_pnt, ind_pnt1;
	list_ind ind_vtx;
} tmp_grid_def;

typedef struct {
#ifdef CUDA
    /* not jet used */
#endif
} cudadef;


typedef enum {
   SUCCESS,
   MEM_ALLOC_FAILED,
   FILE_ACCESS_FAILED,
   INSUFFICENT_INPUT,
   EOF_ENCOUNTERED,
   WRONG_INPUT_OPTION,
   WRONG_OBJ_FORMAT,
   MEM_TRACKING_EXHAUSTED,
   MEM_REALLOC_MISMATCH,
   MEM_TRACKING_MESSED_UP,
   MEM_TYPE_MISMATCH,
   MEM_NULL_POINTER,
   IPE_FILE_NOT_INITIALIZED,
   IPE_FILE_INIT_FAILED,
   UNKNOWN_ERROR
} errordef;


/* global_struct for ALL globals                                        */
/* all former globals are packed into this struct                       */
typedef struct {
   rt_options    rt_opt;         /* run-time options                   */
   iolistdef     c_iolist;       /* globals from basic.x               */
   listdef       c_list;         /* globals from list.x                */
   bridgedef     c_bridge;       /* globals from bridge.x              */
   vertexdef     c_vertex;       /* globals from vertex.x              */
   datadef       c_data;         /* globals from data.x                */
   cleandef      c_clean;        /* globals from clean_data.x          */
   griddef       c_grid;         /* globals from grid.x                */
   quaddef       c_quad;         /* globals from quads.x               */
   io_3ddef      c_io3d;         /* globals from io_3D.x               */
#ifdef GRAPHICS
   redrawdef     c_redraw;       /* globals from redraw.x              */
#endif
   ipe_iodef     c_ipe_io;       /* globals from ipe_io.x              */
   cudadef       c_cuda;         /* globals for ./gpu/.x               */
#ifdef JRC_PREDICATE
   predicatesdef c_pred;         /* globals from predicates.x          */
#endif
#ifdef DEBUG_MEMORY
   debug_memdef  c_mem;          /* globals from memory.x              */
#else
   void         *c_mem;
#endif
   eardef        c_ear;          /* globals from ear_clip.x            */
#ifdef PARTITION_FIST
   boolean       partition_mode;
   int           number_of_heaps;
   list_ind     *corner_nodes;
   heapdef      *c_heap;         /* globals from heap.x                */
#else
   heapdef       c_heap;         /* globals from heap.x                */
#endif
   
   point bb_min;
   point bb_max;
   
#ifdef GRAPHICS
   boolean draw_pnts;
   boolean draw_point_idx;
#endif
   boolean is_convex_face;

   int num_contours;
   
   unsigned machine_long bb_cntr;
   unsigned machine_long cell_cntr;
   
   boolean ccw_loop;
   boolean isolated_pnts;
   
   int io_flag;
   machine_double x_delta, y_delta;
   
   boolean new_input;
   boolean done;
   boolean reset;
   boolean troubles;
   boolean written;

   machine_double  cpu_time;
   timeval start, end;
   
   FILE *dxf_file;
   
   int curr_loop_main;                       /* main.c's separate curr_loop */
   
   /* from orientation.c */
   double *poly_area;
   int max_num_poly_area;
   
#ifdef CPUTIME_IN_MILLISECONDS 
   unsigned long total_secs;
   unsigned long total_usecs;
#else
#ifdef CPUTIME_VIA_CLOCK
   clock_t total_cpu_time;
   boolean cpu_time_initialized;
#else 
   long total_cpu_time;
#endif
#endif

   /* from write_ipe.c */
   boolean ipe_initialized;
#ifdef EXPR_WRAPPER
   boolean returncore;
#endif
} global_struct;
/* ------------------------------ END global_struct ------------------------ */

/*                                                                           */
/* get the definitions for "exterior applications"                           */
/*                                                                           */
#include "ext_appl_defs.h"

#define TriTriColor  0 /* 0 ... output triangle equals input triangle        */
#define TriQuadColor 1 /* 1 ... output triangle belongs to input quad        */
#define TriCvxColor  2 /* 2 ... output triangle belongs to convex face       */
#define TriCveColor  3 /* 3 ... output triangle belongs to concave face      */

#ifdef DOUBLE_OVERRIDE                       /* if the CORE library is used */

const double C_0_0    = STRING_TO_REAL("0.0");
const double C_0_1    = STRING_TO_REAL("0.1");
const double C_0_3    = STRING_TO_REAL("0.3");
const double C_1_0    = STRING_TO_REAL("1.0");
const double C_1_1    = STRING_TO_REAL("1.1");
const double C_2_0    = STRING_TO_REAL("2.0");
const double C_m2_0   = STRING_TO_REAL("-2.0");
const double C_3_0    = STRING_TO_REAL("3.0");
const double C_4_0    = STRING_TO_REAL("4.0");
const double C_20_0   = STRING_TO_REAL("20.0");

#else

#define C_0_0         0.0
#define C_0_1         0.1
#define C_0_3         0.3
#define C_1_0         1.0
#define C_1_1         1.1
#define C_2_0         2.0
#define C_m2_0       -2.0
#define C_3_0         3.0
#define C_4_0         4.0
#define C_20_0        20.0

#endif

/*                                                                           */
/* Some constants are used in expressions that need pure machine_double      */
/* constants. So above constants cannot be used in conjunction with          */
/* defined(WITH_COREBACKEND). The following constants can be used instead.   */
/*                                                                           */
#define CD_0_0        0.0
#define CD_0_001      0.001
#define CD_0_01       0.01
#define CD_0_05       0.05
#define CD_0_3        0.3
#define CD_1_0        1.0
#define CD_m1_0      -1.0
#define CD_1_5        1.5
#define CD_2_0        2.0
#define CD_2_5        2.5
#define CD_3_0        3.0
#define CD_20_0       20.0
#define CD_1000_0     1000.0

/*
 * global precision thresholds
 * 
 * ZERO    ... "small" number used for checking whether a value equals zero 
 *              (in order to avoid a division by zero). if set to 0.0 then
 *              any number other than 0.0 is regarded as a legitimate 
 *              denumerator. please note that this may cause a numerical 
 *              overflow! thus, please think twice prior to changing the 
 *              default value of ZERO!!
 *                                                                           
 * EPS      ... used for comparisons with zero; if set to 0.0 then grazing    
 *              contacts are likely to be mishandled. however, the code
 *              should not crash, and a small speed-up can be expected. in
 *              particular, setting EPS to 0.0 causes the comparison macros
 *              (lt, le, ... see martin.h) to function like their conventional
 *              counterparts if such a macro is supplied with EPS as the
 *              precision threshold to be used. very likely, it is best not
 *              to change the default value of EPS! the main use of this
 *              threshold is for the evaluation of 3x3 determinants, i.e.,
 *              for the computation of the sign of the signed area of a 
 *              triangle defined by three (input) points. (virtually all
 *              predicates used by FIST boil down to the computation of 
 *              the signed area of a triangle.) three points are considered
 *              collinear if the absolute value of the area of the 
 *              corresponding triangle is less than or equal to EPS.
 *              depending on whether or not the run-time option "--scale"
 *              is used, those areas are computed relative to the original
 *              coordinates of the input points, or relative to scaled
 *              copies of the input points. (if requested by the user, 
 *              scaling is performed such that the input data ends up in 
 *              [-1,1]x[-1,1].)  note that setting  EPS  to a non-zero value
 *              is a must if the compile-time option  --DGRAZING  is used;
 *              see Imakefile for an explanation. (what is a good value for
 *              EPS  depends on your data and your intended application; the
 *              value defined below seems to work for a wide range of data.
 *              however, it is obvious that there may exist applications for
 *              which a user might want to be more liberal or more conservative
 *              in allowing FIST to classify items as degenerate. one way or
 *              the other, FIST should not crash!)
 */
#ifdef DOUBLE_OVERRIDE

/* In case of using the MPFR backend, EPS and ZERO need to be set according the
 * MPFR precision, which is set at runtime. So EPS and ZERO need to be global
 * variables.
 */
#if defined(WITH_MPFRBACKEND) || defined (WITH_EXPR_WRAPPER)
   extern double EPS;
   extern double ZERO;
   #define ZERO_D   1.0e-16
#else /* so we are using an exact backend */
   const double EPS   = STRING_TO_REAL("0.0");
   const double ZERO  = STRING_TO_REAL("0.0");
   #define ZERO_D      0.0
#endif

#else                      /* DOUBLE OVERRIEDE                               */

#ifdef THRESHOLD
   #define EPS      1.0e-15
   #define ZERO     1.0e-16
   #define ZERO_D   1.0e-16
 #else                      /* don't even think about using this else branch! */
   #define EPS         0.0
   #define ZERO        0.0
   #define ZERO_D      0.0
#endif /* THRESHOLD */

#endif /* DOUBLE OVERRIDE                               */


#ifdef MEM_COPY

/*                                                                           */
/* unfortunately, the following stuff is needed in order to make FIST comply */
/* with CORE's memory re-allocation...                                       */
/*                                                                           */

#include <stdlib.h>
#include <stdio.h>

template <class DATATYPE>
DATATYPE* ReallocateArray_Copy(DATATYPE* old_mem, int old_size, int new_size, size_t size, const char var_name[])
{
	DATATYPE* new_mem = NULL;

	if (old_mem != NULL)
	{
		new_mem = new DATATYPE[new_size];
		if (new_mem != NULL)
		{
			for (int i = 0; i < old_size; i++)
				new_mem[i] = old_mem[i];
			delete []old_mem;
		}
		else
		{
         fprintf(stderr, "*** Array `%s' cannot be reallocated! ***\n", var_name);
         fprintf(stderr, "*** Cannot get %d elements of %lu bytes... ***\n", new_size, (unsigned long) size);
         exit(1);
		}
	}
	else
	{
		new_mem = new DATATYPE[new_size];
		if (new_mem == NULL)
		{
			fprintf(stderr, "*** Array `%s' cannot be reallocated! ***\n", var_name);
         fprintf(stderr, "*** Cannot get %d elements of %lu bytes... ***\n", new_size, (unsigned long) size);
         exit(1);
		}
	}

	return new_mem;	
}

template <class DATATYPE>
void FreeMemory_Copy(DATATYPE** ptr, const char *)
{
	if (*ptr == NULL) return;
	
	delete []*ptr;
	*ptr = NULL;
}

#define  CORE_ReallocateArray(mem, array, old_size, new_size, type, var_name) \
                              ReallocateArray_Copy((array), (old_size),  \
                                  (new_size), sizeof(type), (var_name))

#define  CORE_FreeMemory(mem, array, var_name) FreeMemory_Copy((array), (var_name))

#else  /* MEM_COPY */

#define  CORE_ReallocateArray(mem, array, old_size, new_size, type, var_name)  \
                             (type *) ReallocateArray((mem), (array), (new_size),\
                              sizeof(type), (var_name))
#define  CORE_FreeMemory(mem, array, var_name) \
                            FreeMemory((mem), (void**)(array), (var_name))


#endif /* MEM_COPY */

#endif /* FIST_DEFS_H */
