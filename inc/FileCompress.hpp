#ifndef __BOB_FILE_COMPRESS_H__
#define __BOB_FILE_COMPRESS_H__

#include <iostream>
#include <fstream>
#include <string>

class FileCompress
{
protected:
    std::ifstream _infile;
    std::ofstream _outfile;
private:
    template<class FS>
    void OpenFileStream(
        FS& stream,
        const std::string& filename
    )
    {
        if constexpr (std::is_same_v<FS, std::ifstream> ||
                     std::is_same_v<FS, std::ofstream>)
        {
            if(stream.is_open())
                stream.close();

            stream.open(filename);
        }
    }

    template<class FS>
    void CloseFileStream(
        FS& stream
    )
    {
        if constexpr (std::is_same_v<FS, std::ifstream> ||
                     std::is_same_v<FS, std::ofstream>)
        {
            if(stream.is_open())
                stream.close();
        }
    }

protected:
    void OpenExtractFile(
        const std::string& filename
    )
    {
        OpenFileStream(
            _outfile,
            filename
        );
    }

    void CloseExtractFile(void)
    {
        CloseFileStream(_outfile);
    }

public:
    FileCompress() : _infile(std::ifstream()) {};
    ~FileCompress() 
    {
        if(_infile.is_open())
        {
            CloseFileStream(_infile);
        }
    }

    void OpenFile(const std::string& filename)
    {
        OpenFileStream(
            _infile,
            filename
        );
    }

    bool CloseFile()
    {
        if(!_infile.is_open())
            return false;

        CloseFileStream(_infile);
        return true;
    }

    virtual bool Compress() = 0;
    virtual bool Extract() = 0;
};

#endif