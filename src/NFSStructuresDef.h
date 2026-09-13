/*
 * This code is written by iOrange, 2010
 * Modernized for C++17 / x64 compatibility
 */

#ifndef __NFSStructuresDef_h__
#define __NFSStructuresDef_h__

#include "NFSTypes.h"
#include "NFSMath.h"

enum E_NFS_FILE_TYPE : uint32
{
    NFS_FILE_UNKNOWN_TYPE   = 0,
    NFS_FILE_TEXTURE        = 1,      // texture
    NFS_FILE_MATERIAL       = 2,      // material/shader definition
    NFS_FILE_MODEL          = 5,      // model geometry
    NFS_FILE_UNKNOWN7       = 7,      // ?
    NFS_FILE_DEFS_TABLE     = 18,     // definitions table
    NFS_FILE_UNKNOWN19      = 19,     // ?
    NFS_FILE_UNKNOWN81      = 81,     // ? small file
    NFS_FILE_UNKNOWN129     = 129,    // ? sound file or sound reference (SPS) ?
    NFS_FILE_UNKNOWN176     = 176,    // ?
    NFS_FILE_UNKNOWN178     = 178,    // ?
    NFS_FILE_COLLISION      = 262,    // ? collision model?
    NFS_FILE_STRINGS_TABLE  = 513,    // strings table / localization database
    NFS_FILE_UNKNOWN527     = 527,    // ?
    NFS_FILE_UNKNOWN532     = 532,    // ?
    NFS_FILE_DEFORMATION    = 771,    // ? collision/deform model?

    LAST_NFS_FILE_TYPE
};

inline constexpr uint32 k_RegularAllocationAlign = 16;
inline constexpr uint32 k_MidAllocationAlign     = 32;
inline constexpr uint32 k_BigAllocationAlign     = 128;

inline constexpr uint32 k_EntryUncompSizeMask    = 0x00ffffff;

// Фиксируем побайтовое выравнивание структур для точного соответствия файлам игры
#pragma pack(push, 1)

// Заголовок бандла (0x30 = 48 байт)
struct S_NFSBndlHeader
{
    uint32  magic;              // "bnd2"
    uint32  version;            // 2 - Burnout Paradise, 3 - Hot Pursuit
    uint32  u2;                 // always 1
    uint32  bundleSize;         // size of bundle
    uint32  numFiles;           // num of files
    uint32  tableOffset;        // files table
    uint32  headersOffset;      // begin of files headers
    uint32  bodysOffset;        // begin of files bodys
    uint32  dataEnd;            // could match bodysOffset if no bodys data stored
    uint32  bundleSize2;        // == bundleSize ???
    uint32  bundleFlags;        // 0x07, 0x27
    uint32  zero;               // 0x00000000
};
static_assert(sizeof(S_NFSBndlHeader) == 48, "S_NFSBndlHeader size mismatch!");

// Элемент таблицы файлов (0x50 = 80 байт)
struct S_NFSTableEntry
{
    uint32  hash;
    uint32  zero1;
    uint32  linkID;
    uint32  u1;
    uint32  hdrUncompSize;
    uint32  dataUncompSize;
    uint32  zero2[2];
    uint32  hdrCompSize;
    uint32  dataCompSize;
    uint32  zero3[2];
    uint32  hdrOffset;
    uint32  bodyOffset;
    uint32  zero4[2];
    uint32  dataEnd;
    uint32  fileType;
    uint32  numLinks;
    uint32  zero5;
};
static_assert(sizeof(S_NFSTableEntry) == 80, "S_NFSTableEntry size mismatch!");

struct S_NFSCrossFileLink
{
    uint32  linkID;
    uint32  zero1;
    uint32  targetVar;
    uint32  zero2;
};

struct S_NFSDefsTableHeader
{
    static constexpr int k_DefNameMaxLen = 51;

    uint32  stuff0;
    uint32  stuff1;
    uint32  stuff2;
    uint32  defsCount;
    uint8   zero0;
    char    defName[k_DefNameMaxLen];
};

struct S_NFSDefsTableEntry
{
    uint32  defID;
    uint32  type;
    uint32  data2;
    uint32  offset;
    uint32  data4;
};

struct S_NFSStringsTableHeader
{
    uint32  version;
    uint32  stringsCount;
    uint32  charSize;
    uint32  stringInfosOffset;
};

struct S_NFSStringsTableInfo
{
    uint32  offset;
    uint32  stringLength;
};

struct S_NFSTextureHeader
{
    uint32  stuff0;
    uint32  stuff1;
    uint32  stuff2;
    uint32  type;
    uint16  width;
    uint16  height;
    uint32  stuff3;
    uint8   unknown;
    uint8   numMipmaps;
    uint16  stuff4;
    uint32  stuff5;
};

struct S_NFSModelHeader
{
    uint32  stuff0[4];
    uint16  unknown;
    uint16  numMeshes;
    uint32  stuff1[3];
};

struct S_NFSMeshHeader
{
    uint32  stuff0[7];
    uint32  numVertexes;
    uint32  stuff1[12];
    uint32  numIndexes;
    uint32  beginMeshOffset;
    uint32  stuff2[2];
    uint32  VBStartOffset;
    uint32  stuff3;
    uint32  VBSize;
    uint32  stuff4;
};

struct mesh_vertex_48
{
    int16   x, y, z, w;
    vec3    normal;
    vec3    tangent;
    uint8   bones[4];
    uint8   weights[4];
    float16 u0, v0;
    float16 u1, v1;
};

struct mesh_vertex_52
{
    int16   x, y, z, w;
    vec3    normal;
    vec3    tangent;
    uint8   bones[4];
    uint8   weights[4];
    float16 u0, v0;
    float16 u1, v1;
    float16 u2, v2;
};

struct mesh_vertex_60
{
    int16   x, y, z, w;
    vec3    normal;
    vec3    tangent;
    vec3    binormal;
    uint8   bones[4];
    uint8   weights[4];
    float16 u0, v0;
    float16 u1, v1;
};

#pragma pack(pop)

#endif // __NFSStructuresDef_h__