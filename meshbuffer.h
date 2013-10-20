// A place for the bascis of what make a mesh

#ifndef MESH_BUFFER_H_
#define MESH_BUFFER_H_

#include <vector>
#include <svl/svl.h>

struct TextureCoordinate
{
    TextureCoordinate();
    TextureCoordinate(float _u, float _v);
    float u;
    float v;
    float& operator[](unsigned int idx);
};

class MeshBuffer
{
public:
    MeshBuffer();
    virtual ~MeshBuffer();

    MeshBuffer(const MeshBuffer & ref);
    MeshBuffer& operator=(const MeshBuffer & ref);

    void loadFile(const char * fileName);

    void setVerts(unsigned int count, const float* verts, int src_num_componets);
    void setNorms(unsigned int count, const float* normals, int src_num_componets);
    void setTexCoords(unsigned int layer, unsigned int count, const float* coords, int src_num_componets);
    void setIndices(unsigned int count, const unsigned int * indices);

    const std::vector<svl::Vector3>& getVerts();
    const std::vector<svl::Vector3>& getNorms();
    const std::vector<TextureCoordinate>& getTexCoords(unsigned int layer);
    const unsigned int * getIndices() const;

    unsigned int getVertCnt() const;
    unsigned int getIdxCnt() const;

    bool UsesNormals;
    bool UsesUVs;
    bool UsesIndices;

private:

    void cleanUp();

    unsigned int VertCnt;
    unsigned int IdxCnt;

    std::vector<svl::Vector3> Verts;
    std::vector<svl::Vector3> Norms;
    std::vector<TextureCoordinate> TexCoords;
    unsigned int * Indices;
};

#endif // MESH_BUFFER_H_
