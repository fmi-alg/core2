//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include "Interval.h"

namespace itree
{

template< class Pt >
ostream & operator << (ostream & out, const Interval<Pt>& interval) {
    out << "<<"<< interval.getFrom() << "->" << interval.getTo() << ">>";
    return out;
}

}//end namespace itree

