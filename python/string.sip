/****************************************************************************
 * string.sip -- Python Interface for std::string
 *
 * Core Library Version 2.0, March 2006
 * Copyright (c) 1995-2006 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of Core Library (http://cs.nyu.edu/exact/core); you 
 * may redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with Core Library.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * WWW URL: http://cs.nyu.edu/exact/core
 * Email: exact@cs.nyu.edu
 *
 * $Id: string.sip,v 1.1.1.1 2006/02/09 09:18:13 exact Exp $
 ***************************************************************************/

%MappedType std::string
{
%TypeHeaderCode
#include <string>
%End

%ConvertFromTypeCode
     const char *s = sipCpp->c_str();
     return PyString_FromString(s);
%End

%ConvertToTypeCode
         // Allow a Python string whenever a string is expected.
         if (sipIsErr == NULL)
             return PyString_Check(sipPy);
         if (sipPy == Py_None) {
             *sipCppPtr = NULL;
             return 0;
         }
         if (PyString_Check(sipPy)) {
             *sipCppPtr = new std::string(PyString_AS_STRING(sipPy));
             return 0;
         }
         *sipCppPtr = (std::string *)sipForceConvertTo_std_string(sipPy,sipIsErr);
         return 1;
%End
};
