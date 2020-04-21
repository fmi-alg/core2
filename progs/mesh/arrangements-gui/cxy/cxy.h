#pragma once

#include "cxy/cxy-defs.h"
#include "cxy/point-inl.h"
#include "cxy/box.h"

namespace cxy {

void cxy(const std::string& curve_str, Box * box, vector<pair<Point *, Point *> >& line_segs);

void cxy(Curve<double>& bi_poly,  Box * box, vector<pair<Point *, Point *> >& line_segs);

} //namespace cxy 
