//==============================================================================
// ShaderException.cpp
// =================
// This class is meant for use in the shader class when an error occurs
// that prevents the scene from being rendered. It stores a description
// of the error that the shader class's caller can access and print to
// the user.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-07-28
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

// Header
#include "display/ShaderException.h"

/*
 * CONSTRUCTOR
 *
 * Set error description.
 */
ShaderException::ShaderException(const char* desc):
    error_(desc)
{}

/*
 * WHAT?
 *
 * Returns string describing error
 */
 const char* ShaderException::what() const throw(){
    return error_;
}
