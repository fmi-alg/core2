//==============================================================================
// ShaderException.h
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

#ifndef __disc__ShaderException__
#define __disc__ShaderException__

// Standard Library
#include <exception>

class ShaderException : public std::exception
{
public:
    // Constructor
    ShaderException(const char*);

    // Retrieve Information
    virtual const char* what() const throw();

private:
    const char* error_;
};

#endif /* defined(__disc__ShaderException__) */
