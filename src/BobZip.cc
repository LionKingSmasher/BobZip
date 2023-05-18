#include "BobZip.h"

#include <memory>
#include <string.h>

static size_t
getFileSize(
    std::ifstream& _fs
)
{
    size_t fileSize = 0;
    size_t recentPos = _fs.tellg();
    
    _fs.seekg(0, std::ios::end);
    fileSize = _fs.tellg();
    _fs.seekg(recentPos, std::ios::beg);

    return fileSize;
}

namespace bob::compress
{

ZipLocalHeader*
Zip::AllocateZipFLocalHeader(
)
{
    bool ret = false;
    size_t recentPos = 0;
    size_t headerSize = 0;

    ZipLocalHeader* header = nullptr;

    header = reinterpret_cast<decltype(header)>(AllocateMemory(
        sizeof(*header)
    ));

    recentPos = _infile.tellg();
    readFileStream(
        reinterpret_cast<char*>(header),
        sizeof(*header)
    );

    if(header->magicNum == localHeaderMagicNumber)
    {
        headerSize = sizeof(*header) + 
                        header->fileNameLen +
                        header->extraFieldLen;
        
        FreeMemory(header);

        header = reinterpret_cast<decltype(header)>(AllocateMemory(
            headerSize
        ));

        _infile.seekg(recentPos, std::ios::beg);
        readFileStream(
            reinterpret_cast<char*>(header),
            headerSize
        );

        ret = true;
    }

    if(!ret)
    {
        FreeMemory(header);
        header = nullptr;
    }

    return header;
}

void
Zip::FreeZipLocalHeader(
    ZipLocalHeader* header
)
{
    FreeMemory(header);
}


void*
Zip::AllocateMemory(
    size_t size
)
{
    return (new char[size]{0,});
}


void
Zip::GetFileNameInZip(
    std::string& refFilename,
    const ZipLocalHeader* header
)
{
    size_t filenameLen = header->fileNameLen;
    char* fileName = new char[filenameLen];

    memcpy(
        fileName,
        header->fileNameExtraField,
        filenameLen
    );

    refFilename = fileName;

    delete[] fileName;
}


void Zip::Initialize(size_t bufSize)
{
    inBuf.reserve(bufSize);
    outBuf.reserve(bufSize);

    zipStream.zalloc = nullptr;
    zipStream.zfree = nullptr;
    zipStream.opaque = nullptr;

    zipStream.next_in = inBuf.data();
    zipStream.avail_in = inBuf.capacity(); // * sizeof(uint8_t);
    zipStream.next_out = outBuf.data();
    zipStream.avail_out = outBuf.capacity();
}

void
Zip::FreeMemory(
    void* mem
)
{
    delete[] reinterpret_cast<char*>(mem);
}

void 
Zip::Uninitialize()
{
    // TODO
}

bool 
Zip::Compress()
{
    bool status = false;
    size_t fileSize = 0;

    fileSize = getFileSize(_infile);
    if(fileSize > 0)
    {
        status = readFileStream(
            reinterpret_cast<char*>(inBuf.data()),
            inBuf.capacity()
        );
        if(status)
        {
            deflateInit(&zipStream, Z_BEST_COMPRESSION);
            deflate(&zipStream, Z_FINISH);
            deflateEnd(&zipStream);
            
            status = writeFileStream(
                reinterpret_cast<char*>(outBuf.data()),
                zipStream.total_out
            );
        }
    }

    return status;
}

bool Zip::Extract()
{
    bool status = false;
    ZipLocalHeader* header = nullptr;

    std::string extractFilename = "";

    while((header = AllocateZipFLocalHeader()) != nullptr)
    {
        zipStream.next_in = inBuf.data();
        zipStream.next_out = outBuf.data();
        
        GetFileNameInZip(
            extractFilename,
            header
        );

        OpenExtractFile(
            extractFilename
        );

        readFileStream(
            reinterpret_cast<char*>(inBuf.data()),
            header->compressSize
        );

        switch(header->compressionMethod)
        {
        // STORE (Normal)
        case CompressionMethod::STORE:
            writeFileStream(
                inBuf.data(),
                header->uncompressSize
            );
            break;

        // Deflate
        case CompressionMethod::DEFLATE:
            int err;
            err = inflateInit2(&zipStream, -MAX_WBITS);
            if(err == Z_OK)
                err = inflate(&zipStream, Z_NO_FLUSH);
            else
                std::cout << "Error code: " << err << std::endl;
                
            inflateEnd(&zipStream);
            
            if(err == Z_STREAM_END || 
               err == Z_OK)
            {
                writeFileStream(
                    outBuf.data(),
                    header->uncompressSize
                );
            }

            break;
        }
        
        CloseExtractFile();

        FreeZipLocalHeader(header);

        status = true;
    }

    return status;
}

}