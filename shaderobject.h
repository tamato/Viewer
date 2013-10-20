#ifndef SHADER_OBJECT_H_
#define SHADER_OBJECT_H_

class ShaderObject
{
public:
    ShaderObject();
    virtual ~ShaderObject();

    void loadVertFile(const char * vertFileName);
    void loadPixelFile(const char * pixelFileName);
    void loadShaderPair(const char * vert, const char * pixel);

    void attachVert();
    void attachPixel();

    void bindAttribLoc(unsigned int index, const char * variable);
    void setTexture(unsigned int textureStage, const char * name);
    void setFloat(float val, const char * name);
    void setVec4(float * vec, const char * name);
    void setMatrix44(float * mat, const char * name);

    void linkProgram();
    void useProgram();

private:
    const char * loadShaderSource(const char * shaderFile);
    unsigned int loadShader(const char * source, unsigned int shaderType);

    bool NeedsRelinked;

    unsigned int ShaderProgram;
    unsigned int VertShader;
    unsigned int PixelShader;

    const char * VertFile;
    const char * PixelFile;
  
};


#endif //SHADER_OBJECT_H_
