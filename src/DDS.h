#ifndef __DDS_h__
#define __DDS_h__

#include "common.h"
#include <string>

struct RGBA {
	union {
		struct { uint8 R, G, B, A; };
		uint32 dw;
	};
};

struct DDS_PIXELFORMAT
{
	uint32 dwSize;
	uint32 dwFlags;
	uint32 dwFourCC;
	uint32 dwRGBBitCount;
	uint32 dwRBitMask;
	uint32 dwGBitMask;
	uint32 dwBBitMask;
	uint32 dwABitMask;
};

struct DDS_HEADER
{
	uint32		  dwSize;
	uint32		  dwFlags;
	uint32		  dwHeight;
	uint32		  dwWidth;
	uint32		  dwPitchOrLinearSize;
	uint32		  dwDepth;
	uint32		  dwMipMapCount;
	uint32		  dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32		  dwCaps1;
	uint32		  dwCaps2;
	uint32		  dwReserved2[3];
};

enum : uint32
{
	// bit flags for header
	DDS_CAPS		= 0x00000001,
	DDS_HEIGHT	  = 0x00000002,
	DDS_WIDTH	   = 0x00000004,
	DDS_PITCH	   = 0x00000008,
	DDS_PIXELFORMAT = 0x00001000,
	DDS_MIPMAPCOUNT = 0x00020000,
	DDS_LINEARSIZE  = 0x00080000,
	DDS_DEPTH	   = 0x00800000,

	// flags for pixel formats
	DDS_ALPHA_PIXELS = 0x00000001,
	DDS_ALPHA		= 0x00000002,
	DDS_FOURCC	   = 0x00000004,
	DDS_RGB		  = 0x00000040,
	DDS_RGBA		 = 0x00000041,

	// flags for complex caps
	DDS_COMPLEX	  = 0x00000008,
	DDS_TEXTURE	  = 0x00001000,
	DDS_MIPMAP	   = 0x00400000,

	// flags for cubemaps
	DDS_CUBEMAP		   = 0x00000200,
	DDS_CUBEMAP_POSITIVEX = 0x00000400,
	DDS_CUBEMAP_NEGATIVEX = 0x00000800,
	DDS_CUBEMAP_POSITIVEY = 0x00001000,
	DDS_CUBEMAP_NEGATIVEY = 0x00002000,
	DDS_CUBEMAP_POSITIVEZ = 0x00004000,
	DDS_CUBEMAP_NEGATIVEZ = 0x00008000,
	DDS_VOLUME			= 0x00200000
};

inline constexpr uint32 MAGIC_DDS	= MAKEFOURCC('D', 'D', 'S', ' ');
inline constexpr uint32 FOURCC_DXT1  = MAKEFOURCC('D', 'X', 'T', '1');
inline constexpr uint32 FOURCC_DXT3  = MAKEFOURCC('D', 'X', 'T', '3');
inline constexpr uint32 FOURCC_DXT5  = MAKEFOURCC('D', 'X', 'T', '5');
inline constexpr uint32 FOURCC_ATI1N = MAKEFOURCC('A', 'T', 'I', '1');
inline constexpr uint32 FOURCC_ATI2N = MAKEFOURCC('A', 'T', 'I', '2');

int LoadDDSFile(const std::string& name, int& width, int& height, int& bpp, uint8** data, int& numMipMaps);
int LoadDDSFile(uint8* fileData, int& width, int& height, int& bpp, uint8** data, int& numMipMaps);

// internal functions
int DDS_LoadUncompressed2D(uint8* fileData, uint8** data, const DDS_HEADER& ddshdr, int numComponents, int& numMipMaps);

/********************************************************************
*			   iOrange - software DDS decompress
*********************************************************************/
void DDS_DecodeColorBlock(uint8* dest, int w, int h, int xOff, int yOff, uint32 format, uint8* src);
void DDS_DecodeDXT3AlphaBlock(uint8* dest, int w, int h, int xOff, int yOff, uint8* src);
void DDS_DecodeDXT5AlphaBlock(uint8* dest, int w, int h, int xOff, int yOff, uint8* src);
void DDS_DecodeCompressedImage(uint8* dest, uint8* src, int width, int height, uint32 format);

#endif // __DDS_h__