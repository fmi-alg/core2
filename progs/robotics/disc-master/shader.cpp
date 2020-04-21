//==============================================================================
// shader.cpp
// =================
// This class generates and saves the shaders and shader program, which are
// necessary for drawing objects using OpenGL.
//
// AUTHOR: Bryant Curto (bryantcurto@gmail.com)
// CREATED: 2015-07-28
//
// Copyright (c) 2015 Bryant Curto
//==============================================================================

// Header
#include "shader.h"

// Custom
#include "ShaderException.h"

// Qt
#include <QFile>


/*
 * CONSTRUCTOR
 *
 * Store vertex and fragment shader source code paths
 */
shader::shader(const GLchar* vertex, const GLchar* fragment):
    vertSrcPath(vertex), fragSrcPath(fragment) {}

/*
 * DESTRUCTOR
 *
 * Delete shaders and shader program.
 */
shader::~shader() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    glDeleteProgram(program);
}

/*
 * CREATE SHADER PROGRAM
 *
 * Use 'vertexShader' and 'fragShader' to create
 * 'program'.
 * ShaderException thrown if error occurs during linking.
 */
void shader::createShaderProgram() {
    program = glCreateProgram();
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    
    // Check linking
    GLint success;
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        
        ShaderException exc(infoLog);
        throw exc;
    }
}

/*
 * CREATE SHADER
 *
 * Compile the source code into a shader.
 * ShaderException is thrown if ShaderType is not recognized
 * or if error occurs during compilation.
 */
void shader::createShader (ShaderType type, GLuint& shader, const char* source) {
    switch (type) {
        case Vertex_Shader:
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
        case Fragment_Shader:
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            ShaderException exc("Unknown Shader Type Detected");
            throw exc;
    }
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    // Check compilation
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        
        ShaderException exc(infoLog);
        throw exc;
    }
}

/*
 * INITIALIZE Shader Program
 *
 * This function reads in the source code for shaders, 
 * compiles each shader, and links the shaders into
 * a shader program.
 * ShaderException is thrown if an error occurs in any
 * of the previous steps.
 */
void shader::init() {
    QFile vertFile(vertSrcPath);
    QFile fragFile(fragSrcPath);
    
    // Test if Vertex File can be Opened
    if (!vertFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ShaderException exc("Failed to Open Vertex Shader File");
        throw exc;
    }
    // Test if Fragment File can be Opened
    if (!fragFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ShaderException exc("Failed to Open Fragment Shader File");
        throw exc;
    }
    
    // Get data from Vertex file and close
    QByteArray vertFileData = vertFile.readAll();
    vertFile.close();
    // Get data from Fragment file and close
    QByteArray fragFileData = fragFile.readAll();
    fragFile.close();
    
    // Nothing Read from Vertex Shader
    if (!vertFileData.data()) {
        ShaderException exc("Failed to Read Vertex Shader File");
        throw exc;
    }
    // Nothing Read from Fragment Shader
    if (!fragFileData.data()) {
        ShaderException exc("Failed to Read Fragment Shader File");
        throw exc;
    }

    // Otherwise, create shaders! //
    
    // Create Shaders //
    createShader(Vertex_Shader, vertexShader, vertFileData.data());
    createShader(Fragment_Shader, fragShader, fragFileData.data());
    
    // Create Program //
    createShaderProgram();
}

/*
 * USE Shader Program
 *
 * Enables the shaders linked in the
 * shader program to be used.
 */
void shader::use() {
    glUseProgram(program);
}
