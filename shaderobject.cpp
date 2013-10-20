#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "shaderobject.h"
#include <QOpenGLFunctions>

ShaderObject::ShaderObject()
    : NeedsRelinked(true)
    , ShaderProgram(0)
    , VertShader(0)
    , PixelShader(0)
    , VertFile(0)
    , PixelFile(0)
{
}

ShaderObject::~ShaderObject()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glUseProgram(0);
    glDetachShader(ShaderProgram, VertShader);
    glDetachShader(ShaderProgram, PixelShader);
    glDeleteProgram(ShaderProgram);

    glDeleteShader(VertShader);
    glDeleteShader(PixelShader);
}

void ShaderObject::loadVertFile(const char * vertFileName)
{
    VertFile = vertFileName;
    const char * source = loadShaderSource(vertFileName);
    if (source == 0) assert(0); // failed to load shader
    VertShader = loadShader(source, GL_VERTEX_SHADER);
    delete [] source;

    NeedsRelinked = true;
}

void ShaderObject::loadPixelFile(const char * pixelFileName)
{
    PixelFile = pixelFileName;
    const char * source = loadShaderSource(pixelFileName);
    PixelShader = loadShader(source, GL_FRAGMENT_SHADER);
    NeedsRelinked = true;
}

void ShaderObject::loadShaderPair(const char * vert, const char * pixel)
{
    loadVertFile(vert);
    loadPixelFile(pixel);

    attachVert();
    attachPixel();
    linkProgram();
}

void ShaderObject::attachVert()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    if (ShaderProgram == 0)
        ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, VertShader);
    NeedsRelinked = true;
}

void ShaderObject::attachPixel()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    if (ShaderProgram == 0)
        ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, PixelShader);
    NeedsRelinked = true;
}

void ShaderObject::bindAttribLoc(GLuint index, const char * variable)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    NeedsRelinked = true;
    glBindAttribLocation(ShaderProgram, index, variable);
}

void ShaderObject::setTexture(unsigned int textureStage, const char * name)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    useProgram();
    int loc = glGetUniformLocation(ShaderProgram, name);
    glUniform1i(loc, textureStage);
}

void ShaderObject::setFloat(float val, const char * name)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    useProgram();
    int loc = glGetUniformLocation(ShaderProgram, name);
    glUniform1f(loc, val);
}

void ShaderObject::setVec4(float * vec, const char * name)
{
    useProgram();
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    int loc = glGetUniformLocation(ShaderProgram, name);
    glUniform4fv(loc, 1, vec);
}

void ShaderObject::setMatrix44(float * mat, const char * name)
{
    // in order to get a uniform loc, the program must be linked first
    // and to set a uniform, the shader must be in use.
    useProgram(); 
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    int loc = glGetUniformLocation(ShaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}

void ShaderObject::linkProgram()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    // Link Program MUST be done after and bindAttri's
    glLinkProgram(ShaderProgram);
    int status;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        const int buffSize = 1000;
        int length;
        char strBuffer[buffSize] = {0};
        glGetProgramInfoLog(ShaderProgram, buffSize, &length, strBuffer);
        printf("%s\n", strBuffer);
    }
    NeedsRelinked = false;
}

void ShaderObject::useProgram()
{
    if (NeedsRelinked) linkProgram();
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glUseProgram(ShaderProgram);
}

const char * ShaderObject::loadShaderSource(const char * shaderFile)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(shaderFile, "r");  /* Open file for reading */
    if (!fptr)
    {
        std::cout << "Failed to load shader source file: " << shaderFile << std::endl;
        return 0;
    }

    fseek(fptr, 0, SEEK_END);       /* Seek to the end of the file */
    length = ftell(fptr);           /* Find out how many bytes into the file we are */
    buf = new char[length+1];    /* Allocate a buffer for the entire length of the file and a null terminator */
    memset((void*)buf, 0, length+1);/* Ensure the memory is clean */
    fseek(fptr, 0, SEEK_SET);       /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr);    /* Read the contents of the file in to the buffer */
    fclose(fptr);                   /* Close the file */
    buf[length] = 0;                /* Null terminator */
    return buf;
}

GLuint ShaderObject::loadShader(const char * source, unsigned int shaderType)
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    GLuint shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const GLchar**)&source, 0);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        const int maxLen = 1000;
        int len;
        char errorBuffer[maxLen] = {0};
        glGetShaderInfoLog(shader, maxLen, &len, errorBuffer);
        printf("Shader Compile Errors:\n%s\n", errorBuffer);
    }
    return shader;
}
