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


/* compute.c: */
#ifdef __cplusplus
extern "C" {
#endif
   void Compute(global_struct *all);
#ifdef __cplusplus
}
#endif

void StatisticsFIST(global_struct *all);

#ifdef __cplusplus
extern "C" {
#endif
void ResetAll(global_struct *all);
#ifdef __cplusplus
}
#endif




/* memory.c: */

void *ReallocateArray(debug_memdef *mem, void *old_ptr, int number, size_t size,
                      const char var_name[]);
void FreeMemory(debug_memdef *mem, void **ptr, const char var_name[]);

void InitDebugMemDefaults(debug_memdef *mem);

unsigned machine_long ReportMaxNumberBytes(debug_memdef *mem);

unsigned machine_long ReportCurrNumberBytes(debug_memdef *mem);

boolean AllMemoryFreed(debug_memdef *mem);

void FreeMemory(debug_memdef *mem, void **ptr, const char var_name[]);

#ifdef DEBUG_MEMORY
boolean IndexOutOfBounds(debug_memdef *mem, void *array_ptr,
		                 char var_name[], size_t size, int index);
#endif


/* data.c: */

void InitGlobalStruct(global_struct *all, rt_options *rt_opt, 
                      boolean stand_alone);

void InitDataDefaults(datadef *data);

void ScaleData(global_struct *all);

#ifdef EXT_APPL_SITES
int StorePnt(datadef *data, double x, double y, eas_type ext_appl);
#else
int StorePnt(datadef *data, double x, double y);
#endif

void InitPnts(datadef *data, int number);

void FreePnts(datadef *data);

void DecrementPoints(datadef *data);

void SetBoundingBox(datadef *data, point *bb_min, point *bb_max);

void ComputeBoundingBox(datadef *data, listdef *list, list_ind ind, point *bb_min, point *bb_max);

boolean InPointsList(datadef *data, int index);

void InitStoragePnts(datadef *data, int number);



/* redraw.c: */

#ifdef GRAPHICS

void FreeGraphics(void);

void InitRedrawDefaults(redrawdef *redraw);

void ResetBufferData(redrawdef *redraw);

void Redraw(global_struct *all);

void AddPntToBuffer(redrawdef *redraw, int index, int color);

void AddEdgeToBuffer(redrawdef *redraw, int index1, int index2, int color);

void AddTriToBuffer(redrawdef *redraw, int index1, int index2, int index3, 
                    int color1, int color2);

void DecrementPntBuffer(redrawdef *redraw);

void DecrementEdgeBuffer(redrawdef *redraw);

void FreeDrawingBuffer(redrawdef *redraw);

void ResetTriBuffer(redrawdef *redraw);

void UpdatePntEdgeBuffers(global_struct *all);

#ifdef PARTITION_FIST
void UpdateTriBuffer(global_struct *all);
#endif
#endif


/* graphics.c: */

#ifdef GRAPHICS

void UpdateScaleData(void);

void InitializeGraphics(int argc, char *argv[], global_struct *all);

void ProcessGraphicsEvents(void);

void DrawTri(point pnt1, point pnt2, point pnt3, int color1, int color);

void DrawSeg(point pnt1, point pnt2, int color);

void DrawPnt(point pnt, int color);

void ResetGraphicsData(void);

void DrawText(point pnt, int color, char* text);

#endif



/* io_parse.c: */

boolean ReadOptionalNumber(FILE *input, int *data);

boolean ReadOptionalCoord(FILE *input, double *xy);

boolean ReadNumber(FILE *input, int *data);

boolean ReadVectorData(FILE *input, double *xc, double *yc);

#ifdef EXT_APPL_SITES
boolean ReadPntData(FILE *input, double *xc, double *yc, eas_type *eas_data);
#else
boolean ReadPntData(FILE *input, double *xc, double *yc);
#endif

#ifdef EXT_APPL_SITES
void WritePntData(FILE *output, machine_double xc, machine_double yc, 
                  eas_type *eas_data);
#else
void WritePntData(FILE *output, machine_double xc, machine_double yc);
#endif

void WriteNumber(FILE *output, int number);

void WriteVectorData(FILE *output, machine_double xc, machine_double yc);



/* io_basic.c: */

void InitIOListDefaults(iolistdef* iolist);

void ResetIOListData(iolistdef* iolist);

void Read2DInputData(global_struct *all);

void StartPoly(global_struct *all);

#ifdef EXT_APPL_SITES
void AddPolyVertex(global_struct *all, double xc1, double yc1, eas_type eas_data);
#else
void AddPolyVertex(global_struct *all, double xc1, double yc1);
#endif

void CloseCurrentPoly(global_struct *all);

void DXFStartPoly(global_struct *all);

void DXFAddPolyVertex(global_struct *all, double xc1, double yc1, double bulge);

void DXFCloseCurrentPoly(global_struct *all, double bulge);

void EnsureClosedPolygon(global_struct *all);

#ifdef EXT_APPL_SITES
int HandlePnt(global_struct *all,
		      double xc1, double yc1, eas_type ext_appl);

void HandleSeg(global_struct *all,
		       double xc1, double yc1, double xc2, double yc2,
               eas_type ext_appl);

void HandleArc(global_struct *all,
		       double xc1, double yc1, double xc2, double yc2,
               double xc3, double yc3, int attr, eas_type ext_appl);

#else
int HandlePnt(global_struct *all, double xc1, double yc1);

void HandleSeg(global_struct *all, double xc1, double yc1, double xc2, double yc2);

void HandleArc(global_struct *all,
		       double xc1, double yc1, double xc2, double yc2,
               double xc3, double yc3, int attr);
#endif



/* io_dxf.c: */

void ReadDXFFile(global_struct *all);

void WriteDXFLWPoly_Vertex(global_struct *all, machine_double xc1, machine_double yc1,
                           machine_double bulge);

void WriteDXFLWPoly_Start(global_struct *all, boolean closed, int color);

void WriteDXFPoly_Vertex(global_struct *all, machine_double xc1, machine_double yc1,
                         machine_double bulge);

void WriteDXFPoly_Start(global_struct *all, boolean closed, int color);

void WriteDXFArc(global_struct *all,
		         machine_double xc1, machine_double yc1,
                 machine_double xc2, machine_double yc2, 
                 machine_double xc3, machine_double yc3, 
                 boolean ccw_flag, int color);

void WriteDXFCircle(global_struct *all,
		            machine_double xc1, machine_double yc1,
                    machine_double radius, int color);

void WriteDXFPoint(global_struct *all,
		           machine_double xc1, machine_double yc1, int color);

void WriteDXFLine(global_struct *all,
		          machine_double xc1, machine_double yc1,
                  machine_double xc2, machine_double yc2, int color);

void StartDXFFile(global_struct *all, char *file_name);

void FinishDXFFile(global_struct *all);

void WriteDXFPoly_End(global_struct *all);




/* io_misc.c: */

void CopyOutputData(vertexdef *vert, int *num_tri, int (*output_tri)[3]);

void CopyInputData2D(global_struct *all, int num_contours, int *num_vtx, double (*input_vtx)[2]);

void CopyInputData3D(global_struct *all, int num_contours, int *num_vtx, double (*input_vtx)[3]);

#ifdef __cplusplus
extern "C" {
#endif
void Help(void);
#ifdef __cplusplus
}
#endif

FILE *OpenFile(const char *file_name, const char *access);

void FIST_Warning(const char string[]);

void Copyright(void);




/* io_2D.c: */

void ReadPolygon(global_struct *all);

void ReadPoly(global_struct *all);

void ReadPolyLines(global_struct *all);

void WritePolygon(global_struct *all, char output_file[]);

void WriteFacesDXF(global_struct *all, char output_file[]);

void WritePolyFormat(global_struct *all);


/* io_3D.c: */

void Init3dDefaults(io_3ddef *io3d);

void WriteGeomOutput(global_struct *all);

void WriteFaces(global_struct *all, char output_file[]);

void ReadPolyhedron(global_struct *all, char input_file[]);




/* arg_eval.c: */

void InitDefaults(rt_options *rt_opt);

boolean ArgEval(int argc, char *argv[], rt_options *rt_opt);

#ifdef __cplusplus
extern "C" {
#endif
void EvalError(void);
#ifdef __cplusplus
}
#endif



/* list.c: */
void InitListDefaults(listdef* list);

void RemoveZeroEdges(listdef* list, list_ind *ind);

boolean GetBridgeNode(listdef* list, list_ind ind);

void SetBridgeNodePair(listdef* list, list_ind ind1, list_ind ind2);

int GetNumList(listdef* list);

void InitStorageList(listdef* list, int number);

void InitStorageLoops(listdef* list, int number);

void StoreChain(listdef* list, list_ind ind);

list_ind GetNextChain(listdef* list, boolean *done);

void ResetListData(listdef* list);

void RotateLinks(listdef* list, list_ind ind1, list_ind ind2);

void SplitSplice(listdef* list, list_ind ind1, list_ind ind2, list_ind ind3, list_ind ind4);

int GetOriginal(listdef* list, list_ind ind);

#ifdef NORMALS
void SetTexNormData(listdef* list, list_ind ind, int index1, int index2);

int GetNormalIndex(listdef* list, list_ind ind);

int GetTextureIndex(listdef* list, list_ind ind);
#endif

void SetOriginal(listdef* list, list_ind ind, int original);

void DeleteLinks(listdef* list, list_ind ind);

void InsertAfter(listdef* list, list_ind ind1, list_ind ind2);

list_ind MakeNode(listdef* list, int index);

#ifdef __cplusplus
extern "C" {
#endif
int MakeLoopHeader(listdef* list);
#ifdef __cplusplus
}
#endif

list_ind MakeHook(listdef* list);

void FreeList(listdef* list);

void DeleteHook(listdef* list, int curr_loop);

int CountListElements(listdef* list, list_ind ind1);

void DecrementLoops(listdef* list);

void UpdateIndex(listdef* list, list_ind ind, int index);

void SwapLinks(listdef* list, list_ind ind1);

boolean InPolyList(listdef* list, list_ind ind);

boolean InLoopList(listdef* list, int loop);

void ResetPolyList(listdef* list, list_ind ind);

list_ind GetNode(listdef* list);

void InitFace(listdef* list, int number);

void DecrementFaces(listdef* list);

#ifdef PARTITION_FIST
boolean isCorner(global_struct *all, int current_partition, list_ind ind);
#endif


/* clean_data.c: */
void InitCleanDefaults(cleandef *clean);

void CleanPolygon(global_struct *all, int *removed);

void CleanPolyhedralFace(global_struct *all, int loop_min, int loop_max, int *removed);

void FreeUnsorted(cleandef *clean);



/* numerics.c: */
#if defined(WITH_MPFRBACKEND) || defined(WITH_EXPR_WRAPPER)
void set_mpfrbackend_prec(mpfr_prec_t mpfr_prec, boolean verbose);
#endif

#ifdef JRC_PREDICATE
void exactinit();
#endif

boolean PntInTri(datadef *data, int i1, int i2, int i3, int i4, double *area);

boolean PntInTriClass(datadef *data, int i1, int i2, int i3, int i4, double *area,
                      int *pnt_on_edge);

double Angle(point p, point p1, point p2);

boolean InCone(int i, int j, int k, int l, boolean convex);

boolean SegIntersect(global_struct *all, int i1, int i2, int i3, int i4, int i5);

boolean SegIntersection(global_struct *all, int i1, int i2, int i3, int i4, int i5);

machine_double GetRatio(datadef *data, int i, int j, int k);

machine_double GetDoubleRatio(datadef *data, int i, int j, int k, int m);

double det2D(int i, int j, int k);

int orientation(int i, int j, int k);

void InitConstants(global_struct *all);

int SpikeAngle(listdef *list, datadef *data, int i, int j, int k, list_ind ind);

boolean PointInTriangle(int i1, int i2, int i3, int i4);


/* orientation.c: */

void AdjustOrientation(global_struct *all, int i1, int i2);

void DetermineOrientation(global_struct *all, list_ind ind);

double PolygonArea(global_struct *all, list_ind ind);

void FreeOrientation(global_struct *all);




/* ear_clip.c: */

void InitEarDefaults(eardef *ear);

boolean IsEar(global_struct *all, heapdef *hp, list_ind ind2, list_ind *ind1,
		      list_ind *ind3, machine_double *ratio);

void ClassifyEars(global_struct *all, list_ind ind);

void ClassifyAngles(global_struct *all, list_ind ind, int *num_reflex);

boolean ClipEar(global_struct *all, heapdef *hp, boolean *done);

void SetConvexityStatus(eardef *ear, boolean status);

void SetTangentNumber(int number);

void ResetEarStatus(eardef *ear);



/* heap.c: */

void InitHeapDefaults(heapdef *hp);

void FreeHeap(heapdef *hp);

void DumpOnHeap(heapdef *hp, machine_double ratio, list_ind ind, list_ind prev,
                list_ind next);

void InsertIntoHeap(heapdef *hp, machine_double ratio, list_ind ind, list_ind prev,
                    list_ind next);

boolean DeleteFromHeap(heapdef *hp, list_ind *ind, list_ind *prev, list_ind *next);

void InitHeap(heapdef *hp, datadef *data);

#ifdef PARTITION_FIST
void InitPartitionedHeap(global_struct *all);
#endif

void MakeHeap(heapdef *hp);



/* vertex.c: */

void InitVertexDef(vertexdef* vert);

boolean InVertexList(vertexdef *vertex, int index);

void FreeVertices(vertexdef *verte);

void InitVertices(vertexdef *vert, int number);

#ifdef EXT_APPL_SITES
int StoreVertex(vertexdef *vert, double x, double y, double z, eas_type eas_data);
#else
int StoreVertex(vertexdef *vert, double x, double y, double z);
#endif

void FreeTriangles(vertexdef *vertex);

void StoreTriangle(global_struct *all, int i, int j, int k, int color);

int GetNumTriangles(vertexdef* vert);

void InitTriangles(vertexdef *vertex, int number);

void StoreQuad(vertexdef *vertex, int i, int j, int k, int l);

void FreeQuads(vertexdef *vertex);

void InitQuads(vertexdef *vertex, int number);

void FreeGroups(vertexdef *vertex);

boolean InGroupList(vertexdef *vertex, int index);

void InitGroupTriangles(vertexdef *vertex);

void StoreGroupNumber(listdef *list, vertexdef *vertex);

void StoreGroupData(vertexdef *vertex);

void StoreConvexLoop(global_struct *all, list_ind ind);

void FreeConvexLoops(vertexdef *vertex);

#ifdef NORMALS
int StoreV_Normal(vertexdef *vert, double x, double y, double z);

void InitV_Normals(vertexdef *vert, int number);

void FreeV_Normals(vertexdef *vertex);

boolean InV_NormalList(vertexdef *vertex, int index);

int StoreT_Vertex(vertexdef *vert, double x, double y, double z);

void InitT_Vertices(vertexdef *vert, int number);

void FreeT_Vertices(vertexdef *vertex);

boolean InT_VertexList(vertexdef *vertex, int index);

void FreeI_Triangles(vertexdef *vertex);
#endif



/* misc.c: */

void Fake3D(global_struct *all);




/* desperate.c: */

boolean Desperate(global_struct *all, heapdef *hp, list_ind ind, int i, boolean *splitted);

boolean LetsHope(global_struct *all, heapdef *hp, list_ind ind);

void FreeDistances(bridgedef *bridge);

int WindingNumber(global_struct *all, list_ind ind, point p);




/* bridge.c: */

void InitBridgeDefaults(bridgedef* bridge);

void FindLeftMostVertex(listdef* list, list_ind ind, list_ind *left_ind, int *left_i);

void ConstructBridges(global_struct* all, int i1, int i2);

boolean FindBridge(global_struct *all, list_ind ind, int i, int start, list_ind *ind1, int *i1);

void InsertBridge(global_struct *all, list_ind ind1, int i1, list_ind ind3, int i3);

int d_comp();
 
void FreeBridges(bridgedef* bridge);




/* project.c: */

void ProjectFace(global_struct *all, int loop_min, int loop_max);




/* triangulate.c: */

void Triangulate(global_struct *all);



/* write_ipe.c: */

void WriteIpePolygon(global_struct *all);

void WriteIpeOutput(global_struct *all);




/* simple.c: */

boolean SimpleFace(global_struct *all, list_ind ind1, boolean ears_fancy);

boolean TrivialPolygon(global_struct *all, list_ind ind1);



/* elapsed.c: */

machine_double elapsed(global_struct *all);



/* grid.c: */

void InitGridDefaults(griddef *grid);

#ifdef GRAZING
void ScanBridgeBuckets(global_struct *all,
		               int i0, int grid_offset, distance *distances,
                       int *num_dist, boolean *grid_exhausted,
                       double x_start);
#else
void ScanBridgeBuckets(global_struct *all,
                       int i0, int grid_offset, distance_ *distances,
                       int *num_dist, boolean *grid_exhausted);
#endif

void ResetGrid(griddef *grid);

void InitBuckets(global_struct *all);

void InsertPntIntoBridgeBuckets(global_struct *all, list_ind ind1);

void BuildBridgeBuckets(global_struct *all, list_ind ind0, boolean init_grid);

boolean GridIntersectionExists(global_struct *all, bounding_box bb,
		                       int i1, int i2,list_ind ind5, int i5);

void InsertSegmentIntoGrid(datadef *data, griddef *grid,
		                   bounding_box bb);

void BuildGrid(global_struct *all, int loop_min, int loop_max);

void FreeGrid(griddef *grid);

machine_double TopQualityGrid(global_struct *all, int i1, int i2);

void BuildPntsGrid(global_struct *all, int loop_ind);

void BuildBuckets(global_struct *all, list_ind ind);

boolean BucketIntersectionExists(global_struct *all,
		                         int i1, list_ind ind1, int i2, int i3,
                                 bounding_box bb, int *i_close,
                                 boolean check_close);

void SetReflexNumber(global_struct *all,int number);

void DeleteReflexVertex(global_struct *all, list_ind ind);




/* bottleneck.c: */

boolean CheckBottleNeck(global_struct *all, int i1, int i2, int i3, list_ind ind4);




/* quads.c: */
void InitQuadsDefaults(quaddef *quad);

void DetermineQuads(global_struct *all);



/* degenerate.c: */

boolean HandleDegeneracies(global_struct *all, int i1, list_ind ind1, int i2,
		                   int i3, int i4,list_ind ind4);



/* thin.c: */

void Thin(char output_file[], boolean save_polygon, boolean new_input,
          int keep_pnts, boolean time, double *cpu_time);

