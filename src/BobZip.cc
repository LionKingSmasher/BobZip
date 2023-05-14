#include "BobZip.h"
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

static bool
readFileStream(
    std::ifstream& _fs,
    std::vector<uint8_t>& refVec
)
{
    bool ret = false;

    if(_fs.is_open())
    {
        _fs.read(
            reinterpret_cast<char*>(refVec.data()),
            refVec.capacity()
        );
        ret = true;
    }

    return ret;
}

static bool
writeFileStream(
    std::ofstream& _fs,
    std::vector<uint8_t>& refVec
)
{
    bool ret = false;
    char* data = reinterpret_cast<char*>(refVec.data());

    if(_fs.is_open())
    {
        _fs.write(
            data,
            strlen(data)
        );
        ret = true;
    }

    return ret;
}

namespace bob::compress
{

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
Zip::Uninitialize()
{
    // TODO
}

bool 
Zip::Compress()
{
    bool status = true;
    size_t fileSize = 0;

    fileSize = getFileSize(_infile);
    if(fileSize > 0)
    {
        status = readFileStream(
            _infile,
            inBuf
        );
        if(status)
        {
            deflateInit(&zipStream, Z_BEST_COMPRESSION);
            deflate(&zipStream, Z_FINISH);
            deflateEnd(&zipStream);
            
            status = writeFileStream(
                _outfile,
                outBuf
            );
        }
    }

    return status;
}

bool Zip::Extract()
{
    return true;
}

}