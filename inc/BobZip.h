#ifndef __BOB_ZIP_H__
#define __BOB_ZIP_H__

#include <vector>
#include <zlib.h>

#include "FileCompress.hpp"

#define DEFAULT_BUF_SIZE 4096

constexpr uint32_t localHeaderMagicNumber = 0x04034b50;

namespace bob::compress
{

enum class CompressionMethod : uint16_t
{
    STORE = 0x0000,
    DEFLATE = 0x00008
};

struct __attribute__((__packed__)) ZipLocalHeader
{
    uint32_t magicNum;
    uint16_t minVersion;
    uint16_t bitFlag;
    CompressionMethod compressionMethod;
    uint16_t lastModificationTime;
    uint16_t lastModificationDate;
    uint32_t checksum;
    uint32_t compressSize;
    uint32_t uncompressSize;
    uint16_t fileNameLen;
    uint16_t extraFieldLen;
    uint8_t fileNameExtraField[0];
};

class Zip : public FileCompress
{
    z_stream zipStream;
    size_t fileSize;

    std::vector<uint8_t> inBuf;
    std::vector<uint8_t> outBuf;

private:

    bool
    writeFileStream(
        const char* data,
        size_t size
    );
    bool
    readFileStream(
        char* data,
        size_t size
    );


    ZipLocalHeader*
    AllocateZipFLocalHeader(
    );
    void
    FreeZipLocalHeader(
        ZipLocalHeader* header
    );


    void*
    AllocateMemory(
        size_t size
    );

    void
    FreeMemory(
        void* mem
    );

    void
    GetFileNameInZip(
        std::string& refFilename,
        const ZipLocalHeader* header
    );

public:

    void Initialize(size_t bufSize = DEFAULT_BUF_SIZE);
    void Uninitialize();
    bool Compress() override;
    bool Extract() override;
};

}

#endif