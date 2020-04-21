//==============================================================================
// shader.h
// =================
// This class generates and saves the shaders and shader program, which are
// necessary for drawing objects using OpenGL.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-07-28
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

#ifndef _SHADER_H_
#define _SHADER_H_

// Qt
#include <QOpenGLFunctions>

enum ShaderType {
    Vertex_Shader = 0,
    Fragment_Shader = 1
};

class shader {

public:
    // Constructor stores paths
    shader(const GLchar*, const GLchar*);
    
    // Destructor
    ~shader();
    
    // Initialize shader program
    void init();
    
    // Use shader program
    void use();
    
private:
    void createShaderProgram ();
    void createShader (ShaderType, GLuint&, const char*);
    
    // shader program ID
    GLuint program;
    
    // shader ID
    GLuint vertexShader;
    GLuint fragShader;
    
    // Path to shader source code paths
    const char* vertSrcPath;
    const char* fragSrcPath;
};

#endif
