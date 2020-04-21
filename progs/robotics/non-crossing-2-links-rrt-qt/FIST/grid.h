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

#ifndef FIST_GRID_H
#define FIST_GRID_H

boolean InGrid3(griddef *grid, int cell);

#ifdef EXPR_WRAPPER
const ExprMD FUDGE = 1.0e-12;
#else
#define FUDGE 1.0e-12                 /* purely for convenience purposes ;-) */
#endif
#define BLOCK_SIZE 32768

#define N_BUCKET     CD_1_5
#define N_GRID       CD_2_5
#define N_REFLEX     30

#define STR(x) #x
#define pragma_omp_critical_start(x) \
_Pragma( STR( omp critical (x) ) )

/*                                                                           */
/* computes the bounding box of  segments[low,..,high].                      */
/*                                                                           */
#define BBox_Compute(grid, tmp, low, high, bb)  \
 { assert(InSegments(grid,low)); \
   assert(InSegments(grid,high)); \
   \
   tmp.seg_addr = &(grid->segments[low]); \
   BBox_Copy(*tmp.seg_addr, bb); \
   for (tmp.seg_i = low+1;  tmp.seg_i <= high;  ++tmp.seg_i) { \
      seg_addr = &(grid->segments[seg_i]); \
      BBox_Enlarge(*tmp.seg_addr, bb); \
   } \
 }

#define ResetSet(grid, tmp) \
 { for (tmp.seg_i = 0;  tmp.seg_i < grid->num_set;  ++tmp.seg_i) { \
	   grid->segments[grid->set[tmp.seg_i]].checked = false; \
   } \
   grid->num_set = 0; \
 }                                              



#define AddToSet(grid, ind) \
 { if (grid->num_set >= grid->max_num_set) { \
	  grid->max_num_set += BLOCK_SIZE; \
	  grid->set = (int*) ReallocateArray(grid->memptr, grid->set, grid->max_num_set, sizeof(int), \
                                   "grid:set"); \
   }\
   grid->set[grid->num_set] = ind; \
   grid->segments[ind].checked = true; \
   ++grid->num_set; \
}



#define StoreSegment(data, grid, tmp, i1, i2) \
 { assert(InPointsList(data, i1)); \
   assert(InPointsList(data, i2)); \
   if (grid->num_segments >= grid->max_num_segments) { \
	  grid->max_num_segments += BLOCK_SIZE; \
	  grid->segments = CORE_ReallocateArray(grid->memptr, grid->segments, grid->max_num_segments - BLOCK_SIZE, \
			  grid->max_num_segments, segment, \
                                      "grid:segments"); \
   } \
   tmp.seg_addr = &(grid->segments[grid->num_segments]); \
   BBox(data, i1, i2, *tmp.seg_addr); \
   tmp.seg_addr->checked = false; \
   ++grid->num_segments; \
 }


#define InsertAfterSeg(grid, k, iseg) \
 { assert(InGrid(grid, k)); \
   assert(InSegments(grid, iseg)); \
   \
   if (grid->num_seg_list >= grid->max_num_seg_list) { \
	  grid->max_num_seg_list += BLOCK_SIZE; \
	  grid->seg_list = (segment_node*) ReallocateArray(grid->memptr, grid->seg_list, grid->max_num_seg_list, \
                                                 sizeof(segment_node), \
                                                 "grid:seg_list"); \
   } \
   grid->seg_list[grid->num_seg_list].seg  = iseg; \
   grid->seg_list[grid->num_seg_list].next = grid->grid[k]; \
   grid->grid[k]                           = grid->num_seg_list; \
   ++grid->num_seg_list; \
 }



#define InsertAfterPnt(list, grid, data, k, ipnt) \
 { assert(InGrid2(grid, k)); \
   assert(InPointsList(data, ipnt)); \
   \
   if (grid->num_pnt_list >= grid->max_num_pnt_list) { \
	   grid->max_num_pnt_list += BLOCK_SIZE; \
	   grid->pnt_list = (pnt_node*) ReallocateArray(grid->memptr, grid->pnt_list, grid->max_num_pnt_list, \
                                             sizeof(pnt_node), \
                                             "grid:pnt_list"); \
   } \
   \
   grid->pnt_list[grid->num_pnt_list].pnt  = ipnt; \
   grid->pnt_list[grid->num_pnt_list].next = grid->grid2[k]; \
   grid->grid2[k]                          = grid->num_pnt_list; \
   ++grid->num_pnt_list; \
 }



#define InsertAfterVtx(list, grid, k, ivtx) \
 { assert(InGrid3(grid, k)); \
   assert(InPolyList(list, ivtx)); \
   \
   if (grid->num_vtx_list >= grid->max_num_vtx_list) { \
	   grid->max_num_vtx_list += BLOCK_SIZE; \
	   grid->vtx_list = (pnt_node*) ReallocateArray(grid->memptr, grid->vtx_list, grid->max_num_vtx_list, \
                                             sizeof(pnt_node), \
                                             "grid:vtx_list"); \
   } \
   \
   grid->vtx_list[grid->num_vtx_list].pnt  = ivtx; \
   grid->vtx_list[grid->num_vtx_list].next = grid->grid3[k]; \
   grid->grid3[k]                          = grid->num_vtx_list; \
   ++grid->num_vtx_list; \
   ++grid->num_reflex; \
 }



#define InsertAfterVtxCells(list, grid, k, ivtx, full_cells) \
 { assert(InGrid3(grid,k)); \
   assert(InPolyList(list,ivtx)); \
   \
   if (grid->num_vtx_list >= grid->max_num_vtx_list) { \
	  grid->max_num_vtx_list += BLOCK_SIZE; \
	  grid->vtx_list = (pnt_node*) ReallocateArray(grid->memptr, grid->vtx_list, grid->max_num_vtx_list, \
                                             sizeof(pnt_node), \
                                             "grid:vtx_list"); \
   } \
   \
   if (grid->grid3[k] == NIL)  ++full_cells; \
   grid->vtx_list[grid->num_vtx_list].pnt  = ivtx; \
   grid->vtx_list[grid->num_vtx_list].next = grid->grid3[k]; \
   grid->grid3[k]                          = grid->num_vtx_list; \
   ++grid->num_vtx_list; \
   ++grid->num_reflex; \
 }



#define DetermineCell(grid, x, y, i, j, ii, jj) \
 { \
   assert(x > grid->grid_min_x); \
   i = REAL_TO_INT(((x - grid->grid_min_x) / grid->grid_x)); \
   if (x < grid->raster_x[i]) { \
      ii = i; \
      --i; \
   } \
   else if (x > grid->raster_x[i+1]) { \
      ii = i; \
      ++i; \
      } \
   else { \
      if (le(x - grid->raster_x[i], EPS))         ii = i - 1; \
      else if (le(grid->raster_x[i+1] - x, EPS))  ii = i + 1; \
      else                                  ii = i; \
   } \
   assert((i >= 0)  &&  (i < grid->N_x)); \
   assert((ii >= 0)  &&  (ii < grid->N_x)); \
   \
   assert(y > grid->grid_min_y); \
   j = REAL_TO_INT(((y - grid->grid_min_y) / grid->grid_y)); \
   if (y < grid->raster_y[j]) { \
      jj = j; \
      --j; \
   } \
   else if (y > grid->raster_y[j+1]) { \
      jj = j; \
      ++j; \
      } \
   else { \
      if (le(y - grid->raster_y[j], EPS))         jj = j - 1; \
      else if (le(grid->raster_y[j+1] - y, EPS))  jj = j + 1; \
      else                                  jj = j; \
   } \
   assert((j >= 0)  &&  (j < grid->N_y)); \
   assert((jj >= 0)  &&  (jj < grid->N_y)); \
 }



#define DetermineCellStrict(grid, x, y, i, j) \
 { \
   assert(x > grid->grid_min_x); \
   i = REAL_TO_INT(((x - grid->grid_min_x) / grid->grid_x)); \
   if (x < grid->raster_x[i]) { \
      --i; \
   } \
   else if (x > grid->raster_x[i+1]) { \
      ++i; \
      } \
   assert((i >= 0)  &&  (i < grid->N_x)); \
   \
   assert(y > grid->grid_min_y); \
   j = REAL_TO_INT(((y - grid->grid_min_y) / grid->grid_y)); \
   if (y < grid->raster_y[j]) { \
      --j; \
   } \
   else if (y > grid->raster_y[j+1]) { \
      ++j; \
      } \
   assert((j >= 0)  &&  (j < grid->N_y)); \
 }



#define Convert(grid, i, j, k) \
 { k = i * grid->N_y + j; }



#define Convert2(grid, i, j, k) \
 { k = i * grid->N_y2 + j; }



#define Convert3(grid, i, j, k) \
 { k = i * grid->N_y3 + j; }



#define DetermineCell2(grid, x, y, i, j, ii, jj) \
 { \
   assert(x > grid->grid_min_x2); \
   i = REAL_TO_INT(((x - grid->grid_min_x2) / grid->grid_x2)); \
   if (x < grid->raster_x2[i]) { \
      ii = i; \
      --i; \
   } \
   else if (x > grid->raster_x2[i+1]) { \
      ii = i; \
      ++i; \
      } \
   else { \
      if (x <= (grid->raster_x2[i] + grid->step_x))         ii = i - 1; \
      else if (x >= (grid->raster_x2[i+1] - grid->step_x))  ii = i + 1; \
      else                                      ii = i; \
   } \
   assert((i >= 0)  &&  (i < grid->N_x2)); \
   if (ii < 0)           ii = i; \
   else if (ii >= grid->N_x2)  ii = i; \
   \
   assert(y > grid->grid_min_y2); \
   j = REAL_TO_INT(((y - grid->grid_min_y2) / grid->grid_y2)); \
   if (y < grid->raster_y2[j]) { \
      jj = j; \
      --j; \
   } \
   else if (y > grid->raster_y2[j+1]) { \
      jj = j; \
      ++j; \
   } \
   else { \
      if (y <= (grid->raster_y2[j] + grid->step_y))         jj = j - 1; \
      else if (y >= (grid->raster_y2[j+1] - grid->step_y))  jj = j + 1; \
      else                                      jj = j; \
   } \
   assert((j >= 0)  &&  (j < grid->N_y2)); \
   if (jj < 0)           jj = j; \
   else if (jj >= grid->N_y2)  jj = j; \
 }




#define DetermineCell3(grid, x, y, i, j, ii, jj) \
 { \
   assert(x > grid->grid_min_x3); \
   i = REAL_TO_INT((x - grid->grid_min_x3) / grid->grid_x3); \
   if (x < grid->raster_x3[i]) { \
      ii = i; \
      --i; \
   } \
   else if (x > grid->raster_x3[i+1]) { \
      ii = i; \
      ++i; \
      } \
   else { \
      if (le(x - grid->raster_x3[i], EPS))         ii = i - 1; \
      else if (le(grid->raster_x3[i+1] - x, EPS))  ii = i + 1; \
      else                                   ii = i; \
   } \
   assert((i >= 0)  &&  (i < grid->N_x3)); \
   if (ii < 0)           ii = i; \
   else if (ii >= grid->N_x3)  ii = i; \
   \
   assert(y > grid->grid_min_y3); \
   j = REAL_TO_INT((y - grid->grid_min_y3) / grid->grid_y3); \
   if (y < grid->raster_y3[j]) { \
      jj = j; \
      --j; \
   } \
   else if (y > grid->raster_y3[j+1]) { \
      jj = j; \
      ++j; \
   } \
   else { \
      if (le(y - grid->raster_y3[j], EPS))         jj = j - 1; \
      else if (le(grid->raster_y3[j+1] - y, EPS))  jj = j + 1; \
      else                                   jj = j; \
   } \
   assert((j >= 0)  &&  (j < grid->N_y3)); \
   if (jj < 0)           jj = j; \
   else if (jj >= grid->N_y3)  jj = j; \
 }




#define DetermineCell4(grid, x, y, i, j) \
 { \
   assert(x > grid->grid_min_x3); \
   i = REAL_TO_INT((x - grid->grid_min_x3) / grid->grid_x3); \
   if (x < grid->raster_x3[i]) { \
      --i; \
   } \
   else if (x > grid->raster_x3[i+1]) { \
      ++i; \
   } \
   assert((i >= 0)  &&  (i < grid->N_x3)); \
   \
   assert(y > grid->grid_min_y3); \
   j = REAL_TO_INT((y - grid->grid_min_y3) / grid->grid_y3); \
   if (y < grid->raster_y3[j]) { \
      --j; \
   } \
   else if (y > grid->raster_y3[j+1]) { \
      ++j; \
   } \
   assert((j >= 0)  &&  (j < grid->N_y3)); \
 }




#define Invert(grid, k, i, j) \
 { \
   i = k / grid->N_y; \
   assert((0 <= i)  &&  (i < grid->N_x)); \
   j = k - i * grid->N_y; \
   assert((0 <= j)  &&  (j < grid->N_y)); \
 }


#ifdef PARTITION_FIST
#define DeleteFromCell(grid, tmp, k, i) \
 { \
   tmp.ind_pnt = grid->grid3[k]; \
   if(tmp.ind_pnt != NIL) {\
      assert(InVtxList(grid,tmp.ind_pnt)); \
      tmp.ind_vtx = grid->vtx_list[tmp.ind_pnt].pnt; \
      if (tmp.ind_vtx == i) { \
         pragma_omp_critical_start(DFC) {\
            grid->grid3[k] = grid->vtx_list[tmp.ind_pnt].next; \
            --grid->num_reflex; \
         } \
      } \
      else { \
      tmp.ind_pnt1 = grid->vtx_list[tmp.ind_pnt].next; \
         while (tmp.ind_pnt1 != NIL) { \
            assert(InVtxList(grid,tmp.ind_pnt1)); \
            tmp.ind_vtx = grid->vtx_list[tmp.ind_pnt1].pnt; \
            if (tmp.ind_vtx == i) { \
               pragma_omp_critical_start(DFC) {\
                  grid->vtx_list[tmp.ind_pnt].next = grid->vtx_list[tmp.ind_pnt1].next; \
                  --grid->num_reflex; \
               } \
               tmp.ind_pnt1 = NIL; \
            } \
            else { \
             tmp.ind_pnt  = tmp.ind_pnt1; \
             tmp.ind_pnt1 = grid->vtx_list[tmp.ind_pnt].next; \
            } \
         } \
      } \
   } \
}
#else
#define DeleteFromCell(grid, tmp, k, i) \
 { \
   tmp.ind_pnt = grid->grid3[k]; \
   if(tmp.ind_pnt != NIL) {\
      assert(InVtxList(grid,tmp.ind_pnt)); \
      tmp.ind_vtx = grid->vtx_list[tmp.ind_pnt].pnt; \
      if (tmp.ind_vtx == i) { \
         grid->grid3[k] = grid->vtx_list[tmp.ind_pnt].next; \
         --grid->num_reflex; \
      } \
      else { \
      tmp.ind_pnt1 = grid->vtx_list[tmp.ind_pnt].next; \
         while (tmp.ind_pnt1 != NIL) { \
            assert(InVtxList(grid,tmp.ind_pnt1)); \
            tmp.ind_vtx = grid->vtx_list[tmp.ind_pnt1].pnt; \
            if (tmp.ind_vtx == i) { \
               grid->vtx_list[tmp.ind_pnt].next = grid->vtx_list[tmp.ind_pnt1].next; \
               --grid->num_reflex; \
               tmp.ind_pnt1 = NIL; \
            } \
            else { \
             tmp.ind_pnt  = tmp.ind_pnt1; \
             tmp.ind_pnt1 = grid->vtx_list[tmp.ind_pnt].next; \
            } \
         } \
      } \
   } \
}
#endif


#define Deter2D(u, v, w) \
 ((u).x * ((v).y - (w).y) - (u).y * ((v).x - (w).x))




#ifdef GRAZING
#define ScanBridgeBucketCell(list, data, grid, k, i0, i1, ind1, ind_pnt, \
                             distances, num_dist, base, x_start)    \
{ \
   ind_pnt = grid->grid3[k]; \
   while (ind_pnt != NIL) { \
      assert(InVtxList(grid,ind_pnt)); \
      ind1 = grid->vtx_list[ind_pnt].pnt; \
      assert(InPolyList(list, ind1)); \
      i1 = GetIndex(list, ind1);     \
      if (data->points[i1].x <= x_start) { \
         PntPntDistSqd(data->points[i0], data->points[i1], base, \
                       distances[*num_dist].dist); \
         distances[*num_dist].ind = ind1; \
         ++(*num_dist); \
      } \
      ind_pnt = grid->vtx_list[ind_pnt].next; \
   } \
}
#else
#define ScanBridgeBucketCell(list, data, grid, k, i0, i1, ind1, ind_pnt, \
                             distances, num_dist, base)    \
{ \
   ind_pnt = grid->grid3[k]; \
   while (ind_pnt != NIL) { \
      assert(InVtxList(grid, ind_pnt)); \
      ind1 = grid->vtx_list[ind_pnt].pnt; \
      assert(InPolyList(list, ind1)); \
      i1 = GetIndex(list, ind1);     \
      if (i1 <= i0) { \
         if (i1 < i0) {                                                  \
            PntPntDistSqd(data->points[i0], data->points[i1], base, distances[*num_dist].dist); \
         } \
         else { \
            distances[*num_dist].dist = CD_0_0;  \
         } \
         distances[*num_dist].ind = ind1; \
         ++(*num_dist); \
      } \
      ind_pnt = grid->vtx_list[ind_pnt].next; \
   } \
}
#endif

#endif /* FIST_GRID_H */
