#include "meshobject.h"
#include <QOpenGLFunctions>

using namespace svl;

#define bufferOffest(x) ((char*)NULL+(x))

MeshObject::MeshObject()
    : IndiceCnt(0)
    , VertCnt(0)
    , EnabledArrays(0)
    , VAO(0)
    , VBO(0)
    , IBO(0)
    , Rotation(Matrix3::identity())
    , Translation(0,0,0)
    , PivotPoint(0,0,0)
    , AABBMin( 9e23f)
    , AABBMax(-9e23f)
    , IndexRangeStart(0)
    , IndexRangeEnd(0)
{
    /************************************************************************************
      According to:
      http://www.opengl.org/registry/specs/NV/vertex_program.txt

      vertex attribute indices are:

        Vertex
        Attribute  Conventional                                           Conventional
        Register   Per-vertex        Conventional                         Component
        Number     Parameter         Per-vertex Parameter Command         Mapping
        ---------  ---------------   -----------------------------------  ------------
         0         vertex position   Vertex                               x,y,z,w
         1         vertex weights    VertexWeightEXT                      w,0,0,1
         2         normal            Normal                               x,y,z,1
         3         primary color     Color                                r,g,b,a
         4         secondary color   SecondaryColorEXT                    r,g,b,1
         5         fog coordinate    FogCoordEXT                          fc,0,0,1
         6         -                 -                                    -
         7         -                 -                                    -
         8         texture coord 0   MultiTexCoord(GL_TEXTURE0_ARB, ...)  s,t,r,q
         9         texture coord 1   MultiTexCoord(GL_TEXTURE1_ARB, ...)  s,t,r,q
         10        texture coord 2   MultiTexCoord(GL_TEXTURE2_ARB, ...)  s,t,r,q
         11        texture coord 3   MultiTexCoord(GL_TEXTURE3_ARB, ...)  s,t,r,q
         12        texture coord 4   MultiTexCoord(GL_TEXTURE4_ARB, ...)  s,t,r,q
         13        texture coord 5   MultiTexCoord(GL_TEXTURE5_ARB, ...)  s,t,r,q
         14        texture coord 6   MultiTexCoord(GL_TEXTURE6_ARB, ...)  s,t,r,q
         15        texture coord 7   MultiTexCoord(GL_TEXTURE7_ARB, ...)  s,t,r,q

        Table X.2:  Aliasing of vertex attributes with conventional per-vertex
        parameters.
    /**************************************************************************************/
}

MeshObject::~MeshObject()
{
//    for (GLuint i=0; i<EnabledArrays; ++i)
//        glFuncs.glDisableVertexAttribArray(i);

    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
    glFuncs.glDeleteBuffers(1, &VBO);
    glFuncs.glDeleteBuffers(1, &IBO);
    glFuncs.glDeleteBuffers(1, &IBO);
    //glFuncs.glDeleteVertexArrays(1, &VAO);
}

void MeshObject::init(MeshBuffer * meshObj)
{
    // setup the mesh
    setMesh(meshObj);
}

void MeshObject::update()
{

}

void MeshObject::render()
{
    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
//    glBindVertexArray(VAO);

    if (IndiceCnt)
    {
        glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        //glDrawElements(GL_TRIANGLES, IndiceCnt/*-IndexRangeStart*/, GL_UNSIGNED_INT, (const void*)(IndexRangeStart * sizeof(unsigned int)));
//        glFuncs.glDrawElements(GL_TRIANGLES, IndexRangeEnd, GL_UNSIGNED_INT, (const void*)(IndexRangeStart * sizeof(unsigned int)));
        glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
//        glFuncs.glDrawArrays(GL_TRIANGLES, 0, VertCnt);
    }

    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshObject::setMesh(MeshBuffer * meshBuffer)
{
    Mesh = *meshBuffer;
    VertCnt = meshBuffer->getVertCnt();
    computeBoundingBox();

    unsigned int vertArrayCnt = VertCnt;
    Stride = 4; // will always have positions
    NormOffset = 0;
    UvOffset = 0;
    Normalidx = 0;
    UVidx = 0;
    EnabledArrays = 1; // 1 is for positions
    if (meshBuffer->UsesNormals)
    {
        Normalidx = EnabledArrays++;
        vertArrayCnt += VertCnt;
        NormOffset = Stride;
        Stride += 4;
    }
    if (meshBuffer->UsesUVs)
    {
        UVidx = EnabledArrays++;
        vertArrayCnt += VertCnt;
        UvOffset = Stride;
        Stride += 2;
    }
    if (meshBuffer->getIndices())
    {
        IndiceCnt = meshBuffer->getIdxCnt();
    }

    // interleave arrays (svl::vector3 are really 4 floats)
    const float* pos = (float*)&meshBuffer->getVerts()[0];
    const float* norm = (float*)&meshBuffer->getNorms()[0];
    const float* uv = (float*)&meshBuffer->getTexCoords(0)[0];
    float* vertArray = new float[vertArrayCnt*Stride];
    for (int i=0, idx=0, uvidx=0; i<VertCnt; i++, idx+=4, uvidx+=2)
    {
        int vi = i*Stride;
        vertArray[vi+0] = pos[idx+0];
        vertArray[vi+1] = pos[idx+1];
        vertArray[vi+2] = pos[idx+2];
        vertArray[vi+3] = pos[idx+3];

        if (NormOffset)
        {
            int ni = i*Stride+NormOffset;
            vertArray[ni+0] = norm[idx+0];
            vertArray[ni+1] = norm[idx+1];
            vertArray[ni+2] = norm[idx+2];
            vertArray[ni+3] = norm[idx+3];
        }

        if (UvOffset)
        {
            int ti = i*Stride+UvOffset;
            vertArray[ti+0] = uv[uvidx+0];
            vertArray[ti+1] = uv[uvidx+1];
        }
    }

    // make values go from number of componets to number of bytes
    Stride *= 4;
    NormOffset *= 4;
    UvOffset *= 4;

    QOpenGLFunctions glFuncs(QOpenGLContext::currentContext());
//    glFuncs.glGenVertexArrays(1, &VAO);
//    glFuncs.glBindVertexArray(VAO);

    glFuncs.glGenBuffers(1, &VBO);
    glFuncs.glGenBuffers(1, &IBO);

    for (GLuint i=0; i<EnabledArrays; ++i)
        glFuncs.glEnableVertexAttribArray(i);

    // set vert buffer    
    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFuncs.glBufferData(GL_ARRAY_BUFFER,
        vertArrayCnt*Stride,
        (GLvoid*)vertArray,
        GL_STATIC_DRAW);

    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFuncs.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride, bufferOffest(0));

    if (Normalidx)
        glFuncs.glVertexAttribPointer(Normalidx, 3, GL_FLOAT, GL_FALSE, Stride, bufferOffest(NormOffset));

    if (UVidx)
        glFuncs.glVertexAttribPointer(UVidx, 2, GL_FLOAT, GL_FALSE, Stride, bufferOffest(UvOffset));

    delete [] vertArray;
    if (IndiceCnt)
    {        
        glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glFuncs.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            IndiceCnt * sizeof(GLuint),
            (GLvoid*)meshBuffer->getIndices(),
            GL_STATIC_DRAW);
        glFuncs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        IndexRangeStart = 0;
        IndexRangeEnd = IndiceCnt;
    }

    glFuncs.glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glFuncs.glBindVertexArray(0);
}

void MeshObject::computeBoundingBox()
{
    const std::vector<svl::Vector3>& verts = Mesh.getVerts();
    for (int i=0; i<(int)verts.size(); ++i)
    {
        if (verts[i][0] < AABBMin[0]) AABBMin[0] = verts[i][0];
        if (verts[i][1] < AABBMin[1]) AABBMin[1] = verts[i][1];
        if (verts[i][2] < AABBMin[2]) AABBMin[2] = verts[i][2];

        if (verts[i][0] > AABBMax[0]) AABBMax[0] = verts[i][0];
        if (verts[i][1] > AABBMax[1]) AABBMax[1] = verts[i][1];
        if (verts[i][2] > AABBMax[2]) AABBMax[2] = verts[i][2];
    }

    svl::Vector3 diagonal = AABBMax - AABBMin;
    PivotPoint = AABBMin + (diagonal * 0.5f);
}

const MeshBuffer& MeshObject::getMesh()
{
    return Mesh;
}
