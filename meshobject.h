#ifndef MESH_OBJECT_H_
#define MESH_OBJECT_H_
#include <svl/svl.h>
#include "meshbuffer.h"

class MeshObject
{
public:
    MeshObject();
    virtual ~MeshObject();

    void init(MeshBuffer * meshObj);

    void update();
    void render();

    void setMesh(MeshBuffer * meshBuffer);

    void computeBoundingBox();

    const MeshBuffer& getMesh();

    svl::Matrix3 Rotation;          // the 3x3 mat for this object
    svl::Vector3 Translation;
    svl::Vector3 PivotPoint;
    svl::Vector3 AABBMin;
    svl::Vector3 AABBMax;

    unsigned int IndexRangeStart;
    unsigned int IndexRangeEnd;
    unsigned int IndiceCnt;

private:
    bool Dirty;                     // If dirty re calculate the mesh buffer to be drawn
    unsigned int VertCnt;
    unsigned int EnabledArrays;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int IBO;

    MeshBuffer Mesh;

    unsigned int Normalidx;
    unsigned int UVidx;
    unsigned int Stride;
    unsigned int NormOffset;
    unsigned int UvOffset;
};

#endif // MESH_OBJECT_H_
