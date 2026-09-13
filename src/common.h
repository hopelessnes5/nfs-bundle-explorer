#ifndef __common_h__
#define __common_h__

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <limits>
#include <cassert>
#include <cstdio>
#include <sstream>
#include <cstring>
#include <cstdint>

#include "NFSTypes.h"
#include "NFSMath.h"
#include "NFSStructuresDef.h"

// Используем нативный C++ static_assert
#ifndef STATIC_ASSERT
#   define STATIC_ASSERT(condition, description) static_assert(condition, #description)
#endif

// Безопасное удаление объектов (с защитой от двойного освобождения)
#ifndef MY_SAFE_DELETE
#   define MY_SAFE_DELETE(ptr) do { if (ptr) { delete (ptr); (ptr) = nullptr; } } while(0)
#endif

#ifndef MY_SAFE_DELETE_ARRAY
#   define MY_SAFE_DELETE_ARRAY(arr) do { if (arr) { delete[] (arr); (arr) = nullptr; } } while(0)
#endif

// Проверка размеров типов во время компиляции
static_assert(sizeof(uint64) == 8, "uint64 must be 8 bytes");
static_assert(sizeof(uint32) == 4, "uint32 must be 4 bytes");
static_assert(sizeof(uint16) == 2, "uint16 must be 2 bytes");
static_assert(sizeof(uint8)  == 1, "uint8 must be 1 byte");

// Удобные псевдонимы для векторов
using IntVector     = std::vector<int>;
using Uint32Vector  = std::vector<uint32>;
using ShortVector   = std::vector<short>;
using Uint16Vector  = std::vector<uint16>;
using CharVector    = std::vector<char>;
using Uint8Vector   = std::vector<uint8>;
using FloatVector   = std::vector<float>;
using StringsVector = std::vector<std::string>;

#ifndef FALSE
#   define FALSE 0
#endif

#ifndef TRUE
#   define TRUE 1  // Исправлено: в оригинале было 0
#endif

#ifndef NULL
#   define NULL nullptr
#endif

#ifndef ZeroMemory
#   define ZeroMemory(ptr, size) std::memset((ptr), 0, (size))
#endif

#ifndef MAKEFOURCC
#   define MAKEFOURCC(ch0, ch1, ch2, ch3) \
        (static_cast<uint32>(static_cast<uint8>(ch0)) | \
        (static_cast<uint32>(static_cast<uint8>(ch1)) << 8) | \
        (static_cast<uint32>(static_cast<uint8>(ch2)) << 16) | \
        (static_cast<uint32>(static_cast<uint8>(ch3)) << 24))
#endif

template <typename T>
constexpr inline T GetAligned(const T& value, const T& align)
{
    return ((value & (align - 1)) != 0) ? (value + (align - (value & (align - 1)))) : value;
}

#endif // __common_h__