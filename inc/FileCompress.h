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
public:
    FileCompress() : _infile(std::ifstream()) {};
    ~FileCompress() 
    {
        if(_infile.is_open())
        {
            _infile.close();
        }
    }

    bool OpenFile(const std::string& filename)
    {
        if(_infile.is_open() ||
           _outfile.is_open())
            return false;

        _infile.open(filename);
        _outfile.open(filename + "_test");
        return true;
    }

    bool CloseFile()
    {
        if(!_infile.is_open() ||
           !_outfile.is_open())
            return false;

        _infile.close();
        _outfile.close();
        return true;
    }

    virtual bool Compress() = 0;
    virtual bool Extract() = 0;
};

#endif