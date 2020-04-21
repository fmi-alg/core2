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


#define BaseLength(u, v, base, delta) { \
   (base).x = (v).x - (u).x; \
   (base).y = (v).y - (u).y; \
   delta    = Abs(TO_MDOUBLE((base).x)) + Abs(TO_MDOUBLE((base).y)); }


#define SideLength(u, v, base, delta) { \
   (base).x = (v).x - (u).x; \
   (base).y = (v).y - (u).y; \
   delta    = (base).x * (base).x + (base).y * (base).y; }

#define PntPntDist(u, v, base, delta) { \
   (base).x = (v).x - (u).x; \
   (base).y = (v).y - (u).y; \
   delta    = sqrt((base).x * (base).x + (base).y * (base).y); }

#define PntPntDistSqd(u, v, base, delta) { \
   (base).x = TO_MDOUBLE((v).x) - TO_MDOUBLE((u).x); \
   (base).y = TO_MDOUBLE((v).y) - TO_MDOUBLE((u).y); \
   delta    = (base).x * (base).x + (base).y * (base).y; }


/*                                                                           */
/* this macro checks whether  i3,  which is collinear with  i1, i2,  is      */
/* between  i1, i2. note that we rely on the lexicographic sorting of the    */
/* points!                                                                   */
/*                                                                           */
#define InBetween(i1, i2, i3) \
   ((i1 <= i3)  &&  (i3 <= i2))


#define StrictlyInBetween(i1, i2, i3) \
   ((i1 < i3)  &&  (i3 < i2))


/*                                                                           */
/* this macro checks whether  i3,  which is collinear with  i1, i2,  is      */
/* between  i1, i2. note that we do _not_ rely on the lexicographic sorting  */
/* of the points!                                                            */
/*                                                                           */
#define InBetweenEps(data, i1, i2, i3) \
   (((Min(data->points[i1].x, data->points[i2].x) - EPS) <= data->points[i3].x)  && \
    ((Min(data->points[i1].y, data->points[i2].y) - EPS) <= data->points[i3].y)  && \
    ((Max(data->points[i1].x, data->points[i2].x) + EPS) >= data->points[i3].x)  && \
    ((Max(data->points[i1].y, data->points[i2].y) + EPS) >= data->points[i3].y))


/*                                                                           */
/* this macro computes the determinant  det(points[i],points[j],points[k])   */
/* in a consistent way.                                                      */
/*                                                                           */
#ifdef JRC_PREDICATE
double orient2dfast(point pa, point pb, point pc);
double orient2d(point pa, point pb, point pc);
#define StableDet2D(i, j, k, det) \
 { assert(InPointsList(i)); \
   assert(InPointsList(j)); \
   assert(InPointsList(k)); \
   \
   if ((i == j)  ||  (i == k)  ||   (j == k)) { \
      det = C_0_0; \
   } \
   else { \
      det = orient2d(points[i], points[j], points[k]); \
   } \
}
#else
#define StableDet2D(data, tmp, i, j, k, det) \
 { assert(InPointsList(data,i)); \
   assert(InPointsList(data,j)); \
   assert(InPointsList(data,k)); \
   \
   if ((i == j)  ||  (i == k)  ||   (j == k)) { \
      det = C_0_0; \
   } \
   else { \
	  tmp.numerics_h_p = data->points[i]; \
	  tmp.numerics_h_q = data->points[j]; \
	  tmp.numerics_h_r = data->points[k]; \
      \
      if (i < j) { \
         if (j < k)            /* i < j < k */ \
            det =  Det2D(tmp.numerics_h_p, tmp.numerics_h_q, tmp.numerics_h_r); \
         else if (i < k)       /* i < k < j */ \
            det = -Det2D(tmp.numerics_h_p, tmp.numerics_h_r, tmp.numerics_h_q); \
         else                  /* k < i < j */ \
            det =  Det2D(tmp.numerics_h_r, tmp.numerics_h_p, tmp.numerics_h_q); \
      } \
      else { \
         if (i < k)            /* j < i < k */ \
            det = -Det2D(tmp.numerics_h_q, tmp.numerics_h_p, tmp.numerics_h_r); \
         else if (j < k)       /* j < k < i */ \
            det =  Det2D(tmp.numerics_h_q, tmp.numerics_h_r, tmp.numerics_h_p); \
         else                  /* k < j < i */ \
            det = -Det2D(tmp.numerics_h_r, tmp.numerics_h_q, tmp.numerics_h_p); \
      } \
   } \
 }
#endif


/*                                                                           */
/* this macro sets ori to +1 if the points  i, j, k are given in CCW order,  */
/*                        -1 if the points  i, j, k are given in CW order,   */
/*                         0 if the points  i, j, k are collinear.           */
/*                                                                           */
#define Orientation(data, tmp, i, j, k, ori) \
 { StableDet2D(data, tmp, i, j, k, tmp.numerics_h_det); \
   \
   if (lt_(tmp.numerics_h_det, EPS))        ori = -1; \
   else if (gt(tmp.numerics_h_det, EPS))   ori =  1; \
   else                                    ori =  0; \
}


/*                                                                           */
/* this macro checks whether  l  is in the cone defined by  i, j  and  j, k  */
/*                                                                           */
#define IsInCone(data, tmp, i, j, k, l, convex, flag) \
 { assert(InPointsList(data, i)); \
   assert(InPointsList(data, j)); \
   assert(InPointsList(data, k)); \
   assert(InPointsList(data, l)); \
   \
   flag = true; \
   if (convex) { \
      if ((l != i)  &&  (i != j)) { \
         Orientation(data, tmp, i, j, l, tmp.numerics_h_ori1); \
         if (tmp.numerics_h_ori1 < 0)              flag = false; \
         else if (tmp.numerics_h_ori1 == 0) { \
            if (i < j) { \
               if (!InBetween(i, j, l))            flag = false; \
            } \
            else { \
               if (!InBetween(j, i, l))            flag = false; \
            } \
         } \
      } \
      if ((l != k)  &&  (j != k)  &&  (flag == true)) { \
         Orientation(data, tmp, j, k, l, tmp.numerics_h_ori2); \
         if (tmp.numerics_h_ori2 < 0)                flag = false; \
         else if (tmp.numerics_h_ori2 == 0) { \
            if (j < k) { \
               if (!InBetween(j, k, l))              flag = false; \
            } \
            else { \
               if (!InBetween(k, j, l))              flag = false; \
            } \
         } \
      } \
   } \
   else { \
      Orientation(data, tmp, i, j, l, tmp.numerics_h_ori1); \
      if (tmp.numerics_h_ori1 <= 0) { \
         Orientation(data, tmp, j, k, l, tmp.numerics_h_ori2); \
         if (tmp.numerics_h_ori2 <= 0) { \
            if (tmp.numerics_h_ori1 == 0) { \
               if (!InBetween(i, j, l))             flag = false; \
            } \
            else if (tmp.numerics_h_ori2 == 0) { \
               if (!InBetween(j, k, l))             flag = false; \
           } \
           else                                     flag = false; \
         } \
      } \
   } \
}



/*                                                                           */
/* this macro checks whether  l  is in the cone defined by  i, j  and  j, k  */
/*                                                                           */
#define IsInConeEps(data, tmp, i, j, k, l, convex, flag) \
 { assert(InPointsList(data, i)); \
   assert(InPointsList(data, j)); \
   assert(InPointsList(data, k)); \
   assert(InPointsList(data, l)); \
   \
   flag = true; \
   if (convex) { \
      if ((l != i)  &&  (i != j)) { \
         Orientation(data, tmp, i, j, l, tmp.numerics_h_ori1); \
         if (tmp.numerics_h_ori1 < 0)              flag = false; \
         else if (tmp.numerics_h_ori1 == 0) { \
            if (!InBetweenEps(data, i, j, l))      flag = false; \
         } \
      } \
      if ((l != k)  &&  (j != k)  &&  (flag == true)) { \
         Orientation(data, tmp, j, k, l, tmp.numerics_h_ori2); \
         if (tmp.numerics_h_ori2 < 0)              flag = false; \
         else if (tmp.numerics_h_ori2 == 0) { \
            if (!InBetweenEps(data, j, k, l))      flag = false; \
         } \
      } \
   } \
   else { \
      Orientation(data, tmp, i, j, l, tmp.numerics_h_ori1); \
      if (tmp.numerics_h_ori1 <= 0) { \
         Orientation(data, tmp, j, k, l, tmp.numerics_h_ori2); \
         if (tmp.numerics_h_ori2 <= 0) { \
            if (tmp.numerics_h_ori1 == 0) { \
               if (!InBetweenEps(data, i, j, l))    flag = false; \
            } \
            else if (tmp.numerics_h_ori2 == 0) { \
               if (!InBetweenEps(data, j, k, l))    flag = false; \
           } \
           else                                     flag = false; \
         } \
      } \
   } \
}




/*                                                                           */
/* this macro checks whether the diagonal  j, l  is in the cone defined by   */
/* i, j  and  j, k. if  left  then  j, k, l  forms the ear to be tested.     */
/* otherwise,  i, j, k  forms the ear to be tested.                          */
/*                                                                           */
#define DiagIsInConeEps(data, tmp, i, j, k, l, convex, left, flag) \
 { assert(InPointsList(data, i)); \
   assert(InPointsList(data, j)); \
   assert(InPointsList(data, k)); \
   assert(InPointsList(data, l)); \
   \
   flag = true; \
   if (convex) { \
      if (left) { \
         if ((l != i)  &&  (i != j)) { \
            Orientation(data, tmp, i, j, l,  tmp.numerics_h_ori1); \
            if (tmp.numerics_h_ori1 < 0)              flag = false; \
            else if (tmp.numerics_h_ori1 == 0) { \
               if (InBetweenEps(data, j, l, i))       flag = false; \
            } \
         } \
      } \
      else { \
         if ((l != k)  &&  (j != k)) { \
            Orientation(data, tmp, j, k, l, tmp.numerics_h_ori2); \
            if (tmp.numerics_h_ori2 < 0)              flag = false; \
            else if (tmp.numerics_h_ori2 == 0) { \
               if (InBetweenEps(data, j, l, k))       flag = false; \
            } \
         } \
      } \
   } \
}



#define DiagIsInCone(data, tmp, i, j, k, l, convex, left, flag) \
 { assert(InPointsList(data, i)); \
   assert(InPointsList(data, j)); \
   assert(InPointsList(data, k)); \
   assert(InPointsList(data, l)); \
   \
   flag = true; \
   if (convex) { \
      if (left) { \
         if ((l != i)  &&  (i != j)) { \
            Orientation(data, tmp, i, j, l, tmp.numerics_h_ori1); \
            if (tmp.numerics_h_ori1 < 0)              flag = false; \
            else if (tmp.numerics_h_ori1 == 0) { \
               if (InBetween(j, l, i))                flag = false; \
            } \
         } \
      } \
      else { \
         if ((l != k)  &&  (j != k)) { \
            Orientation(data, tmp, j, k, l, tmp.numerics_h_ori2); \
            if (tmp.numerics_h_ori2 < 0)              flag = false; \
            else if (tmp.numerics_h_ori2 == 0) { \
               if (InBetween(j, l, k))                flag = false; \
            } \
         } \
      } \
   } \
}

#ifdef PARALLEL_EXE
/* TODO: a problem would be if all 3 points are on the split line but none   */
/* of them is a Steiner point. We should carry the split value with us.      */
//inline boolean ThreePointsOnSplitLine(datadef *data, listdef *list,
//                                      list_ind ind) {
inline boolean ThreePointsOnSplitLine(listdef *list, datadef *data, int i, int j,
                                      int k, list_ind ind1) {
   assert(InPolyList(list,ind1));
   
   if(IsSteiner(list, ind1) &&
      data->points[i].x == data->points[j].x &&
      data->points[j].x == data->points[k].x)
      return true;
   else
      return false;
}
#endif


/*                                                                           */
/* returns                                                                   */
/*           -2 ... if angle is 360 degrees                                  */
/*           -1 ... if angle between 180 and 360 degrees                     */
/*            0 ... if angle is 180 degrees                                  */
/*            1 ... if angle between 0 and 180 degrees                       */
/*            2 ... if angle is 0 degrees                                    */
/*            3 ... if angle is about 180 degrees and triangle is very small */
/*                                                                           */
inline int IsConvexAngle(listdef *list, datadef *data, int i, int j, int k,
                         list_ind ind) {
   assert(InPointsList(data, i));
   assert(InPointsList(data, j));
   assert(InPointsList(data, k));
   tmp_data_def tmp;

   if (i == j) {
      if (j == k) {
         /*                                                                  */
         /* all three vertices are identical. we set the angle to -2.        */
         /* using -2 means to err on the safe side, as all the               */
         /* incarnations of this vertex will be clipped right at the         */
         /* start of the ear-clipping algorithm. thus, eventually there      */
         /* will be no other duplicates at this vertex position, and the     */
         /* regular classification of angles will yield the correct          */
         /* answer for j.                                                    */
         /*                                                                  */
         return -2;
      }
      else {
         /*                                                                  */
         /* two of the three vertices are identical; we set the angle to     */
         /* to 2 order to enable clipping of  j.                             */
         /*                                                                  */
         return 2;
      }
   }
   else if (j == k) {
      /*                                                                     */
      /* two vertices are identical. we could either determine the angle     */
      /* by means of yet another lengthy analysis, or simply set the         */
      /* angle to -1. using -1 means to err on the safe side, as all the     */
      /* incarnations of this vertex will be clipped right at the start      */
      /* of the ear-clipping algorithm. thus, eventually there will be no    */
      /* other duplicates at this vertex position, and the regular           */
      /* classification of angles will yield the correct answer for j.       */
      /*                                                                     */
      return -1;
   } else {
      StableDet2D(data, tmp, i, j, k, tmp.numerics_h_det);
      if (gt(tmp.numerics_h_det, EPS)) {
         return 1;
      }
      else if (lt_(tmp.numerics_h_det, EPS)) {
         return -1;
      }
      else {
         /*                                                                  */
         /* 0, 180, or 360 degrees.                                          */
         /*                                                                  */
         VectorSub2D(data->points[i], data->points[j], tmp.numerics_h_p);
         VectorSub2D(data->points[k], data->points[j], tmp.numerics_h_q);
         tmp.numerics_h_dot = DotProduct2D(tmp.numerics_h_p, tmp.numerics_h_q);
         if (tmp.numerics_h_dot < C_0_0) {
            /*                                                               */
            /* 180 degrees.                                                  */
            /*                                                               */
            /*data->numerics_h_dot  = sqrt(Length2D(data->numerics_h_p));    */
            /*data->numerics_h_dot += sqrt(Length2D(data->numerics_h_q));    */
            if (tmp.numerics_h_det > C_0_0)         return 1;
            else                                    return 0;
         }
         else {
            /*                                                               */
            /* 0 or 360 degrees? this cannot be judged locally, and more     */
            /* work is needed.                                               */
            /*                                                               */
            return SpikeAngle(list, data, i, j, k, ind);
         }
      }
   }
}


/*                                                                           */
/* this macro checks whether point  i4  is inside of or on the boundary      */
/* of the triangle  i1, i2, i3                                               */
/*                                                                           */
#define PntInTriangle(data, tmp, i1, i2, i3, i4, inside) \
 { \
   inside = false; \
   Orientation(data, tmp, i2, i3, i4, tmp.numerics_h_ori1); \
   if (tmp.numerics_h_ori1 >= 0) { \
      Orientation(data, tmp, i1, i2, i4, tmp.numerics_h_ori1); \
      if (tmp.numerics_h_ori1 >= 0) { \
         Orientation(data, tmp, i3, i1, i4, tmp.numerics_h_ori1); \
         if (tmp.numerics_h_ori1 >= 0)  inside = true; \
      } \
   } \
}


/*                                                                           */
/* this macro checks whether point  i4  is inside of or on the boundary      */
/* of the triangle  i1, i2, i3. it also returns a classification if  i4  is  */
/* on the boundary of the triangle (except for the edge  i2, i3).            */
/*                                                                           */
#define VtxInTriangle(data, tmp, i1, i2, i3, i4, inside, classifier) \
 { \
   inside = false; \
   Orientation(data, tmp, i2, i3, i4, tmp.numerics_h_ori1); \
   if (tmp.numerics_h_ori1 >= 0) { \
      Orientation(data, tmp, i1, i2, i4, tmp.numerics_h_ori1); \
      if (tmp.numerics_h_ori1 > 0) { \
         Orientation(data, tmp, i3, i1, i4, tmp.numerics_h_ori1); \
         if (tmp.numerics_h_ori1 > 0) { \
            inside = true; \
            classifier = 0; \
         } \
         else if (tmp.numerics_h_ori1 == 0) { \
            inside = true; \
            classifier = 1; \
         } \
      } \
      else if (tmp.numerics_h_ori1 == 0) { \
         Orientation(data, tmp, i3, i1, i4, tmp.numerics_h_ori1); \
         if (tmp.numerics_h_ori1 > 0) { \
            inside = true; \
            classifier = 2; \
         } \
         else if (tmp.numerics_h_ori1 == 0) { \
            inside = true; \
            classifier = 3; \
         } \
      } \
   } \
}


#define ComputeRatio(A, B, C, RATIO)           \
{ \
   if (Min(B, C) <= ZERO) {                                     \
      RATIO = Min(B, C);                                        \
   }                                                            \
   else {                                                       \
      RATIO = CD_1_0 + (A - B - C) / (CD_2_0 * sqrt(B * C));    \
   }                                                            \
}

