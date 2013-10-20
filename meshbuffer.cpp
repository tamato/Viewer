#include "meshbuffer.h"
#include <assert.h>
#include <stdlib.h>
#include <iostream>

using namespace svl;

TextureCoordinate::TextureCoordinate() : u(0.0f), v(0.0f)
{

}

TextureCoordinate::TextureCoordinate(float _u, float _v)
    : u(_u)
    , v(_v)
{

}

float& TextureCoordinate::operator[](unsigned int idx)
{
    assert(idx < 2);
    if (idx == 0)
        return u;

    return v;
}

MeshBuffer::MeshBuffer()
    : UsesNormals(false)
    , UsesUVs(false)
    , UsesIndices(false)
    , VertCnt(0)
    , IdxCnt(0)
    , Indices(0)
{

}

MeshBuffer::~MeshBuffer()
{
    cleanUp();
}

MeshBuffer::MeshBuffer(const MeshBuffer & ref)
    : UsesNormals(false)
    , UsesUVs(false)
    , UsesIndices(false)
    , VertCnt(0)
    , IdxCnt(0)
    , Indices(0)
{
    this->operator =(ref);
}

MeshBuffer& MeshBuffer::operator=(const MeshBuffer & ref)
{
    if (this == &ref) return *this;
    cleanUp();

    setVerts(ref.VertCnt, (float*)&ref.Verts[0], 4);
    setNorms(ref.VertCnt, (float*)&ref.Norms[0], 4);
    setTexCoords(0, ref.VertCnt, (float*)&ref.TexCoords[0], 2);
    setIndices(ref.IdxCnt, ref.Indices);
    return *this;
}

void MeshBuffer::loadFile(const char * fileName)
{

}

void MeshBuffer::setVerts(unsigned int count, const float* verts, int src_num_componets)
{
    if (!verts) return;

    if (VertCnt == 0)
    {
        VertCnt = count;
        Verts.reserve(VertCnt);
    }

    for (unsigned int i=0; i<VertCnt; ++i)
    {
        svl::Vector3 vec;
        vec[0] = verts[i * src_num_componets + 0];
        vec[1] = verts[i * src_num_componets + 1];
        vec[2] = verts[i * src_num_componets + 2];

        Verts.push_back(vec);
    }
}

void MeshBuffer::setNorms(unsigned int count, const float* normals, int src_num_componets)
{
    if (count != VertCnt)
    {
        std::cout << "Vert count does not match the number normals to create" << std::endl;
        exit(1);
    }

    if (!normals) return;
    UsesNormals = true;

    Norms.clear();
    Norms.reserve(count);

    for (unsigned int i=0; i<VertCnt; ++i)
    {
        svl::Vector3 vec;
        vec[0] = normals[i * src_num_componets + 0];
        vec[1] = normals[i * src_num_componets + 1];
        vec[2] = normals[i * src_num_componets + 2];

        Norms.push_back(vec);
    }
}

void MeshBuffer::setTexCoords(unsigned int layer, unsigned int count, const float* coords, int src_num_componets)
{
    if (count != VertCnt)
    {
        std::cout << "Vert count does not match the number uvs to create" << std::endl;
        exit(1);
    }

    if (!coords) return;
    UsesUVs = true;

    TexCoords.clear();
    TexCoords.reserve(count);

    for (unsigned int i=0; i<VertCnt; ++i)
    {
        TextureCoordinate vec;
        vec[0] = coords[i * src_num_componets + 0];
        vec[1] = coords[i * src_num_componets + 1];

        TexCoords.push_back(vec);
    }
}

void MeshBuffer::setIndices(unsigned int count, const unsigned int * indices)
{
    if (!indices) return;
    UsesIndices = true;

    IdxCnt = count;
    delete [] Indices;
    Indices = new unsigned int[IdxCnt];

    for (unsigned int i=0; i<IdxCnt; ++i)
    {
        Indices[i] = indices[i];
    }
}

const std::vector<svl::Vector3>& MeshBuffer::getVerts()
{
    return Verts;
}

const std::vector<svl::Vector3>& MeshBuffer::getNorms()
{
    return Norms;
}

const std::vector<TextureCoordinate>& MeshBuffer::getTexCoords(unsigned int layer)
{
    return TexCoords;
}

const unsigned int * MeshBuffer::getIndices() const
{
    return Indices;
}

unsigned int MeshBuffer::getVertCnt() const
{
    return VertCnt;
}

unsigned int MeshBuffer::getIdxCnt() const
{
    return IdxCnt;
}

void MeshBuffer::cleanUp()
{
    UsesNormals = false;
    UsesUVs     = false;
    UsesIndices = false;
    VertCnt     = 0;
    IdxCnt      = 0;

    Verts.clear();
    Norms.clear();
    TexCoords.clear();

    delete [] Indices;
}

