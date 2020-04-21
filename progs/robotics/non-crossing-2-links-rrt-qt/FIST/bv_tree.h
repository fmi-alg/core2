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


#define Pnt_In_BBox(data, bb1, i)  \
   ((((bb1).imax < i) ? false : \
     (((bb1).imin > i) ? false : \
      (((bb1).ymax < data->points[i].y) ? false : \
       (((bb1).ymin > data->points[i].y) ? false : true)))))



#define BBox_Overlap(bb1, bb2)  \
   ((((bb1).imax < (bb2).imin) ? false : \
     (((bb1).imin > (bb2).imax) ? false : \
      (((bb1).ymax < (bb2).ymin) ? false : \
       (((bb1).ymin > (bb2).ymax) ? false : true)))))


#define BBox_Contained(bb1, bb2)  \
   (((bb1).imin <= (bb2).imin)  &&  ((bb1).imax >= (bb2).imax)  && \
    ((bb1).ymin <= (bb2).ymin)  &&  ((bb1).ymax >= (bb2).ymax))



#define BBox_IdenticalLeaf(bb1, bb2)  \
   (((bb1).imin == (bb2).imin)  &&  ((bb1).imax == (bb2).imax))



#define BBox_Union(bb1, bb2, bb3)  \
 { (bb3).imin = Min((bb1).imin, (bb2).imin); \
   (bb3).imax = Max((bb1).imax, (bb2).imax); \
   (bb3).ymin = Min((bb1).ymin, (bb2).ymin); \
   (bb3).ymax = Max((bb1).ymax, (bb2).ymax); }



#define BBox_Area(data, bb, area) \
 { area = (data->points[(bb).imax].x - data->points[(bb).imin].x) * \
          ((bb).ymax - (bb).ymin); }



#define BBox_Copy(bb1, bb2) \
 { (bb2).imin = (bb1).imin; \
   (bb2).imax = (bb1).imax; \
   (bb2).ymin = (bb1).ymin; \
   (bb2).ymax = (bb1).ymax; }



#define BBox_CopyInverted(bb1, bb2) \
 { (bb2).imin = (bb1).imax; \
   (bb2).imax = (bb1).imin; \
   (bb2).ymin = (bb1).ymax; \
   (bb2).ymax = (bb1).ymin; }



#define BBox_Enlarge(bb1, bb2) \
 { if ((bb1).imin < (bb2).imin)    (bb2).imin = (bb1).imin; \
   if ((bb1).imax > (bb2).imax)    (bb2).imax = (bb1).imax; \
   if ((bb1).ymin < (bb2).ymin)    (bb2).ymin = (bb1).ymin; \
   if ((bb1).ymax > (bb2).ymax)    (bb2).ymax = (bb1).ymax; }




/*                                                                           */
/* this macro computes the bounding box of a line segment whose end          */
/* points  i, j  are sorted according to x-coordinates.                      */
/*                                                                           */
#define BBox(data, i, j, bb) \
 { assert(InPointsList(data,i)); \
   assert(InPointsList(data,j)); \
   \
   MinMax(i, j, (bb).imin, (bb).imax); \
   MinMax(data->points[(bb).imin].y, data->points[(bb).imax].y, (bb).ymin, (bb).ymax); }




#define BBox_Overlap_Extended(data, bb1, bb2)  \
   ((((bb1).ymax < (bb2).ymin) ? false : \
     (((bb1).ymin > (bb2).ymax) ? false : \
      (((bb1).xmax < data->points[(bb2).imin].x) ? false : \
       (((bb1).xmin > data->points[(bb2).imax].x) ? false : true)))))

