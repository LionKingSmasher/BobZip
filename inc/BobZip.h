#ifndef __BOB_ZIP_H__
#define __BOB_ZIP_H__

#include <vector>
#include <zlib.h>

#include "FileCompress.h"

#define DEFAULT_BUF_SIZE 4096

namespace bob::compress
{

class Zip : public FileCompress
{
    z_stream zipStream;

    std::vector<uint8_t> inBuf;
    std::vector<uint8_t> outBuf;

public:

    void Initialize(size_t bufSize = DEFAULT_BUF_SIZE);
    void Uninitialize();
    bool Compress() override;
    bool Extract() override;
};

}

#endif