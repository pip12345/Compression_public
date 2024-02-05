#include <iostream>
#include <string>
#include "compression_handler.h"

int main() {
    compression::Compression_handler handler{};

    std::string file = "../test_txt/test";

    handler.huff_compress(file);
    handler.huff_decompress(file);

    handler.lzw_compress(file);
    handler.lzw_decompress(file);

    std::cout << "\nDone!" << std::endl;

    return 0;
}