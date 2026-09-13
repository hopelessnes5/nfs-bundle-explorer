#ifndef	__NFSTexture_h__
#define	__NFSTexture_h__

#include <QString>
#include "common.h"
#include "DDS.h"

class CNFSTexture
{
public:
	static bool IsNFSTextureSignature(const Uint8Vector& headerData);
	static bool ConvertDDS2NFS(const QString& fileName);

public:
	CNFSTexture();

	int InitFromData(const Uint8Vector& headerData, const Uint8Vector& imageData);
	int SaveAsDDS(const QString& fileName);

	int GetNumComponents(void) const;
	int GetWidth(void) const;
	int GetHeight(void) const;

	int GetDecompressedSize(void) const;
	int Decompress(uint8* data);

private:
	void ConstructHeader(const S_NFSTextureHeader* hdr);

private:
	DDS_HEADER		m_DDSHeader;
	Uint8Vector		m_Data;
};

#endif	// __NFSTexture_h__
