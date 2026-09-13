#include "DDS.h"
#include <stdio.h>

inline int GetDDSSize(int w, int h)
{
	return (((w + 3) >> 2) * ((h + 3) >> 2)) << 4;
}

int LoadDDSFile(const std::string& name, int& width, int& height, int& bpp, uint8** data, int& numMipMaps)
{
	if (name.length() < 5 || data == NULL )
		return 0;

	FILE* file = fopen(name.c_str(), "rb");

	if (NULL == file)
		return 0;

	fseek(file, 0, SEEK_SET);
	size_t fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8* fileData = new uint8[fileLen];
	if (!fileData)
		return 0;

	return LoadDDSFile(fileData, width, height, bpp, data, numMipMaps);
}

int LoadDDSFile(uint8* fileData, int& width, int& height, int& bpp, uint8** data, int& numMipMaps)
{
	uint32 ddsMagic = *reinterpret_cast<uint32 *>(fileData);
	fileData += sizeof(ddsMagic);

	if (MAGIC_DDS != ddsMagic)
		return 0;

	DDS_HEADER ddshdr;
	memcpy(&ddshdr, fileData, sizeof(ddshdr));
	fileData += sizeof(ddshdr);

	int numComponents;
	if (ddshdr.ddspf.dwFlags & DDS_ALPHA_PIXELS)
		numComponents = 4;
	else
		numComponents = 3;

	bool is3DTexture = ((ddshdr.dwFlags & DDS_DEPTH) != 0);	// check for 3D texture
	if (is3DTexture)
		return 0;

	width  = (int)ddshdr.dwWidth;
	height = (int)ddshdr.dwHeight;
	bpp    = numComponents << 3;

	numMipMaps = (int)ddshdr.dwMipMapCount;

	if ((ddshdr.ddspf.dwFlags & DDS_FOURCC) == 0)	// not compressed
		return DDS_LoadUncompressed2D(fileData, data, ddshdr, numComponents, numMipMaps);

	bool bError = (ddshdr.ddspf.dwFourCC != FOURCC_DXT1 && ddshdr.ddspf.dwFourCC != FOURCC_DXT5);
	if (bError)
		return FALSE;

	// DTX3 & DTX5 always have alpha channel
	numComponents = 4;
	bpp = numComponents << 3;

	uint8* pixbuf;
	int sizeToRead;
	if (numMipMaps <= 1)
	{
		sizeToRead = GetDDSSize(width, height);
		numMipMaps = 1;
	}
	else
	{
		int w = width;
		int h = height;
		sizeToRead = 0;
		for (int i = 0; i < numMipMaps; ++i, w >>= 1, h >>= 1)
		{
			if (w < 1) w = 1;
			if (h < 1) h = 1;
			sizeToRead += GetDDSSize(w, h);
		}
	}

	pixbuf = fileData;

	uint8* uncompressed = new uint8[width * height * numComponents];
	DDS_DecodeCompressedImage(uncompressed, pixbuf, width, height, ddshdr.ddspf.dwFourCC);
	*data = uncompressed;

	return sizeToRead;
}


// internal functions

int DDS_LoadUncompressed2D(uint8* fileData, uint8** data, const DDS_HEADER& ddshdr, int numComponents, int& numMipMaps)
{
	int w            = (int)ddshdr.dwWidth;
	int h            = (int)ddshdr.dwHeight;
	int uint8sPerLine = w * numComponents;

	if (( uint8sPerLine & 3) != 0)	// do uint32 alignment
		uint8sPerLine += 4 - (uint8sPerLine & 3);

	const int fullSize = w * h * numComponents;
	uint8* buf;
	uint8* dest = new uint8[fullSize];

	for (int i = 0; i < h; ++i)
	{
		buf = fileData;
		fileData += uint8sPerLine;

		uint8* d = dest + (i * w * numComponents);
		uint8* s = buf;

		// rearrange components
		for (int j = 0; j < w; ++j)
		{
			d[0] = s[2];	// red
			d[1] = s[1];	// green
			d[2] = s[0];	// blue

			if (4 == numComponents)
				d[3] = s[3];	// alpha

			d += numComponents;
			s += numComponents;
		}
	}

	*data = dest;

	numMipMaps = 1;
	return TRUE;
}



/********************************************************************
*
*               iOrange - software DDS decompress
*
*********************************************************************/

// iOrange - decodes a DXT color block
void DDS_DecodeColorBlock(uint8* dest, const int w, const int h, const int xOff, const int yOff, const uint32 format, uint8* src)
{
	uint8 colors[4][3];

	uint16 c0 = *(uint16*)src;
	uint16 c1 = *(uint16*)(src + 2);

	// Extract the two stored colors
	colors[0][0] = ((c0 >> 11) & 0x1F) << 3;
	colors[0][1] = ((c0 >>  5) & 0x3F) << 2;
	colors[0][2] =  (c0        & 0x1F) << 3;

	colors[1][0] = ((c1 >> 11) & 0x1F) << 3;
	colors[1][1] = ((c1 >>  5) & 0x3F) << 2;
	colors[1][2] =  (c1        & 0x1F) << 3;

	int i, x, y;

	// iOrange - compute the other two colors
	if (c0 > c1 || FOURCC_DXT5 == format)
	{
		for (i = 0; i < 3; ++i)
		{
			colors[2][i] = (2 * colors[0][i] +     colors[1][i] + 1) / 3;
			colors[3][i] = (    colors[0][i] + 2 * colors[1][i] + 1) / 3;
		}
	}
	else
	{
		for (i = 0; i < 3; ++i)
		{
			colors[2][i] = ( colors[0][i] + colors[1][i] + 1 ) >> 1;
			colors[3][i] = 0;
		}
	}

	src += 4;
	for (y = 0; y < h; ++y)
	{
		uint8* dst     = dest + yOff * y;
		uint32 indexes = src[y];
		for (x = 0; x < w; ++x)
		{
			uint32 index = indexes & 0x3;
			dst[0]      = colors[index][0];
			dst[1]      = colors[index][1];
			dst[2]      = colors[index][2];
			indexes   >>= 2;

			dst += xOff;
		}
	}
}

// iOrange - decode a DXT3 alpha block
void DDS_DecodeDXT3AlphaBlock(uint8* dest, const int w, const int h, const int xOff, const int yOff, uint8* src)
{
	int x, y;
	for (y = 0; y < h; ++y)
	{
		uint8* dst   = dest + yOff * y;
		uint32 alpha = ((uint32*)src)[y];
		for ( x = 0; x < w; x++ )
		{
			*dst    = (alpha & 0xF) * 17;
			alpha >>= 4;
			dst    += xOff;
		}
	}
}

// iOrange - decode a DXT5 alpha block / 3Dc channel block
void DDS_DecodeDXT5AlphaBlock(uint8* dest, const int w, const int h, const int xOff, const int yOff, uint8 *src)
{
	uint8  a0    = src[0];
	uint8  a1    = src[1];
	uint64 alpha = (*(uint64*) src) >> 16;

	int x, y;
	int k;

	for ( y = 0; y < h; y++ )
	{
		uint8* dst = dest + yOff * y;
		for ( x = 0; x < w; x++ )
		{
			k = ((uint32)alpha) & 0x7;
			if ( 0 == k )
				*dst = a0;
			else if ( 1 == k )
				*dst = a1;
			else if ( a0 > a1 )
				*dst = ((8 - k) * a0 + (k - 1) * a1) / 7;
			else if ( k >= 6 )
				*dst = (k == 6)? 0 : 255;
			else
				*dst = ((6 - k) * a0 + (k - 1) * a1) / 5;

			alpha >>= 3;
			dst    += xOff;
		}
		if ( w < 4 )
			alpha >>= (3 * (4 - w));
	}
}

// iOrange - decodes DXT and 3Dc formats
void DDS_DecodeCompressedImage(uint8* dest, uint8* src, const int width, const int height, const uint32 format)
{
	int sx = (width  < 4 ) ? width  : 4;
	int sy = (height < 4 ) ? height : 4;

	const int components = (format == FOURCC_DXT1) ? 3 : 4;

	for (int y = 0; y < height; y += 4)
		for (int x = 0; x < width; x += 4)
		{
			uint8* dst = dest + (y * width + x) * components;
			if (FOURCC_DXT3 == format)
			{
				DDS_DecodeDXT3AlphaBlock(dst + 3, sx, sy, 4, width * 4, src);
				src += 8;
			}
			else if (FOURCC_DXT5 == format)
			{
				DDS_DecodeDXT5AlphaBlock(dst + 3, sx, sy, 4, width * 4, src);
				src += 8;
			}
			DDS_DecodeColorBlock(dst, sx, sy, components, width * components, format, src);
			src += 8;
		}
}
