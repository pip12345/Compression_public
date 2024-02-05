#ifndef COMPRESSION_COMPRESSION_HANDLER_H
#define COMPRESSION_COMPRESSION_HANDLER_H

#include <string>
#include "lzw.h"
#include "huff.h"
#include "file_handler.h"

// Number of coded LZW bits can be set in lzw.h

namespace compression {

    class Compression_handler {
    public:
        // file_name is original filename with or without .txt
        // Example: "alice_in_wonderland.txt" or "alice_in_wonderland"

        void huff_compress(const std::string& file_name);
        void huff_decompress(const std::string& file_name);
        void lzw_compress(const std::string& file_name);
        void lzw_decompress(const std::string& file_name);

    private:
        std::string remove_txt_extension(const std::string& file_name);

        files::File_handler file{};
        huff::Huffman_coder hcoder{};
        lzw::LZW_coder lzwcoder{};

        // Extensions huffman
        std::string h_extension_compressed{".piph"};
        std::string h_extension_freqtable{".piphf"};
        std::string h_extension_decompressed{"_decompressed_h.txt"};

        // Extensions LZW
        std::string lzw_extension_compressed{".pipl"};
        std::string lzw_extension_decompressed{"_decompressed_l.txt"};
    };


}

#endif //COMPRESSION_COMPRESSION_HANDLER_H
