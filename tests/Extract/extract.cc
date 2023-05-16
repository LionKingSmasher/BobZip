#include <iostream>
#include "Bob.h"

int main(int argc, char** argv)
{
    bob::compress::Zip zip = bob::compress::Zip();
    
    zip.Initialize();
    zip.OpenFile(argv[1]);

    std::cout << "=========== Test Program ===========\n";
    std::cout << "File Opened: " << argv[1] << "\n";
    std::cout << "Result: " << zip.Extract() << "\n";
    std::cout << "====================================\n";

    zip.Uninitialize();
}