#include "NFSModel.h"
#include <QFile>
#include <sstream>

uint32 ChooseVertexSize(uint32 numVertexes, uint32 vbAlignedSize)
{
    static constexpr uint32 knownVertexSizes[] = {36, 48, 52, 56, 60, 64};

    for (uint32 vertexSize : knownVertexSizes)
    {
        uint32 vbUnalignedSize = vertexSize * numVertexes;
        if ((vbUnalignedSize & (k_MidAllocationAlign - 1)) != 0) {
            vbUnalignedSize += k_MidAllocationAlign - (vbUnalignedSize & (k_MidAllocationAlign - 1));
        }

        if (vbUnalignedSize == vbAlignedSize) {
            return vertexSize;
        }
    }

    return 0;
}

template <typename T>
inline void MakeNFSVertex(const T& source, NFSVertex& newVert)
{
    newVert.pos.x  = (static_cast<float>(source.x) / 32767.0f) * 8.0f;
    newVert.pos.y  = (static_cast<float>(source.y) / 32767.0f) * 8.0f;
    newVert.pos.z  = (static_cast<float>(source.z) / 32767.0f) * 8.0f;
    newVert.normal = source.normal;
}

template <>
inline void MakeNFSVertex<mesh_vertex_48>(const mesh_vertex_48& source, NFSVertex& newVert)
{
    newVert.pos.x  = (static_cast<float>(source.x) / 32767.0f) * 8.0f;
    newVert.pos.y  = (static_cast<float>(source.y) / 32767.0f) * 8.0f;
    newVert.pos.z  = (static_cast<float>(source.z) / 32767.0f) * 8.0f;
    newVert.normal = source.normal;
    newVert.uv.x   = static_cast<float>(source.u0);
    newVert.uv.y   = static_cast<float>(source.v0);
}

template <>
inline void MakeNFSVertex<mesh_vertex_60>(const mesh_vertex_60& source, NFSVertex& newVert)
{
    newVert.pos.x  = (static_cast<float>(source.x) / 32767.0f) * 8.0f;
    newVert.pos.y  = (static_cast<float>(source.y) / 32767.0f) * 8.0f;
    newVert.pos.z  = (static_cast<float>(source.z) / 32767.0f) * 8.0f;
    newVert.normal = source.normal;
    newVert.uv.x   = static_cast<float>(source.u0);
    newVert.uv.y   = static_cast<float>(source.v0);
}

CNFSModel::CNFSModel() = default;

int CNFSModel::InitFromData(const Uint8Vector& headerData, const Uint8Vector& modelData)
{
    if (headerData.size() < sizeof(S_NFSModelHeader) || modelData.empty()) {
        return 0;
    }

    const auto* mdlHdr = reinterpret_cast<const S_NFSModelHeader*>(headerData.data());
    const auto* meshHeadersOffsets = reinterpret_cast<const uint32*>(headerData.data() + sizeof(S_NFSModelHeader));

    m_Meshes.clear();
    m_Meshes.reserve(mdlHdr->numMeshes);

    for (uint32 i = 0; i < mdlHdr->numMeshes; ++i)
    {
        NFSMesh mesh;
        const uint32 meshHeaderOffset = meshHeadersOffsets[i];
        if (meshHeaderOffset + sizeof(S_NFSMeshHeader) > headerData.size()) {
            continue;
        }

        const auto* meshHdr = reinterpret_cast<const S_NFSMeshHeader*>(headerData.data() + meshHeaderOffset);
        const uint32 meshNumIndexes  = meshHdr->numIndexes;
        const uint32 meshNumVertexes = meshHdr->numVertexes;

        if (meshHdr->beginMeshOffset < modelData.size()) {
            const auto* indexBuffer = reinterpret_cast<const uint16*>(modelData.data() + meshHdr->beginMeshOffset);
            mesh.faces = TriStripToTriList(indexBuffer, meshNumIndexes);
        }

        uint32 vertexSize = ChooseVertexSize(meshNumVertexes, meshHdr->VBSize);
        if (meshHdr->VBStartOffset < modelData.size()) {
            const uint8* vertexBuffer = modelData.data() + meshHdr->VBStartOffset;
            mesh.vertexes = MakeVertexBuffer(vertexBuffer, meshNumVertexes, vertexSize);
        }

        mesh.vertSize = vertexSize;
        m_Meshes.push_back(std::move(mesh));
    }

    return 1;
}

int CNFSModel::SaveAsOBJ(const QString& fileName) const
{
    std::ostringstream ss;
    ss << "# Generated from Need For Speed: Hot Pursuit model file\n";
    ss << "# Exported with NFS:HP Bundle Explorer (Modernized 2026)\n\n";

    int meshIndex = 0;
    uint32 lastIdx = 0;

    for (const auto& mesh : m_Meshes)
    {
        ss << "g Object_" << meshIndex++ << "\n";

        // Позиции вершин
        for (const auto& vert : mesh.vertexes) {
            ss << "v " << vert.pos.x << " " << vert.pos.y << " " << vert.pos.z << "\n";
        }
        ss << "# " << mesh.vertexes.size() << " vertexes\n\n";

        // Текстурные координаты
        for (const auto& vert : mesh.vertexes) {
            ss << "vt " << vert.uv.x << " " << vert.uv.y << "\n";
        }
        ss << "# " << mesh.vertexes.size() << " texcoords\n\n";

        // Нормали
        for (const auto& vert : mesh.vertexes) {
            ss << "vn " << vert.normal.x << " " << vert.normal.y << " " << vert.normal.z << "\n";
        }
        ss << "# " << mesh.vertexes.size() << " normals\n\n";

        // Полигоны (faces)
        for (const auto& face : mesh.faces)
        {
            const uint32 a = face.a + lastIdx + 1;
            const uint32 b = face.b + lastIdx + 1;
            const uint32 c = face.c + lastIdx + 1;

            ss << "f " << a << "/" << a << "/" << a << " "
                       << b << "/" << b << "/" << b << " "
                       << c << "/" << c << "/" << c << "\n";
        }
        ss << "# " << mesh.faces.size() << " faces\n\n";

        lastIdx += static_cast<uint32>(mesh.vertexes.size());
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return 0;
    }

    const std::string outStr = ss.str();
    file.write(outStr.data(), static_cast<qint64>(outStr.size()));
    return 1;
}

NFSFacesVec CNFSModel::TriStripToTriList(const uint16* ib, uint32 numIndexes) const
{
    NFSFacesVec newIndexes;
    if (!ib || numIndexes < 3) return newIndexes;

    for (uint32 i = 2; i < numIndexes; ++i)
    {
        const bool isEven = (i % 2 == 0);

        NFSFace face;
        face.a = ib[isEven ? i - 1 : i];
        face.b = ib[isEven ? i : i - 1];
        face.c = ib[i - 2];

        // Пропускаем вырожденные полигоны (degenerate triangles)
        if (face.a != face.b && face.b != face.c && face.c != face.a) {
            newIndexes.push_back(face);
        }
    }

    return newIndexes;
}

NFSVertexesVec CNFSModel::MakeVertexBuffer(const uint8* vb, uint32 numVertexes, uint32 vertexSize) const
{
    NFSVertexesVec newVertexes;
    if (!vb || numVertexes == 0 || vertexSize == 0) return newVertexes;

    newVertexes.reserve(numVertexes);

    for (uint32 i = 0; i < numVertexes; ++i)
    {
        const auto* vert48 = reinterpret_cast<const mesh_vertex_48*>(vb + (i * vertexSize));
        const auto* vert60 = reinterpret_cast<const mesh_vertex_60*>(vb + (i * vertexSize));

        NFSVertex newVert{};
        if (vertexSize == 60) {
            MakeNFSVertex(*vert60, newVert);
        } else if (vertexSize == 48) {
            MakeNFSVertex(*vert48, newVert);
        } else {
            newVert.pos.x  = (static_cast<float>(vert48->x) / 32767.0f) * 8.0f;
            newVert.pos.y  = (static_cast<float>(vert48->y) / 32767.0f) * 8.0f;
            newVert.pos.z  = (static_cast<float>(vert48->z) / 32767.0f) * 8.0f;
            newVert.normal = vert48->normal;
        }

        newVertexes.push_back(newVert);
    }

    return newVertexes;
}