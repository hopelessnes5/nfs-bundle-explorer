#ifndef __NFSModel_h__
#define __NFSModel_h__

#include <QString>
#include <vector>
#include "common.h"

struct NFSVertex
{
    vec3 pos;
    vec3 normal;
    vec2 uv;
};

struct NFSFace
{
    uint16 a, b, c;
};

using NFSVertexesVec = std::vector<NFSVertex>;
using NFSFacesVec    = std::vector<NFSFace>;

struct NFSMesh
{
    NFSVertexesVec  vertexes;
    NFSFacesVec     faces;
    uint32          vertSize = 0;
};

using NFSMeshesVec = std::vector<NFSMesh>;

class CNFSModel
{
public:
    CNFSModel();
    virtual ~CNFSModel() = default;

    int InitFromData(const Uint8Vector& headerData, const Uint8Vector& modelData);
    [[nodiscard]] int SaveAsOBJ(const QString& fileName) const;

private:
    [[nodiscard]] NFSFacesVec    TriStripToTriList(const uint16* ib, uint32 numIndexes) const;
    [[nodiscard]] NFSVertexesVec MakeVertexBuffer(const uint8* vb, uint32 numVertexes, uint32 vertexSize) const;

private:
    NFSMeshesVec m_Meshes;
};

#endif // __NFSModel_h__